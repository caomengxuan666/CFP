#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
license_header.py - 自动插入/更新版权声明
优化版：批量 Git 查询，极大提升速度
"""

import os
import sys
import subprocess
import threading
from datetime import datetime
import re
from collections import defaultdict
from concurrent.futures import ThreadPoolExecutor, as_completed
import time
import json

# ===== 配置 =====
DEBUG = False
USE_GIT_HISTORY = True
THREAD_COUNT = 8
DEFAULT_LICT_PATH = "license.lict"
DEFAULT_EXCLUDE_DIRS = {
    "third_party", "build", "bin", "obj",
    ".git", ".svn", ".hg", ".bzr"
}
SOURCE_EXTS = {".h", ".hpp", ".c", ".cpp", ".cc"}
# ===== 配置结束 =====

# 性能统计
class PerformanceStats:
    def __init__(self):
        self.timings = defaultdict(float)
        self.counts = defaultdict(int)
        self.lock = threading.Lock()
    
    def add_time(self, category, duration):
        with self.lock:
            self.timings[category] += duration
            self.counts[category] += 1
    
    def print_stats(self):
        print("\n" + "="*60)
        print("性能统计:")
        print("="*60)
        
        total_time = sum(self.timings.values())
        print(f"总处理时间: {total_time:.2f}秒")
        
        for category in sorted(self.timings.keys()):
            avg = self.timings[category] / max(self.counts[category], 1)
            percentage = (self.timings[category] / total_time) * 100 if total_time > 0 else 0
            print(f"  {category:20s}: {self.timings[category]:8.2f}s "
                  f"(平均: {avg:.4f}s, 占比: {percentage:5.1f}%)")
        print("="*60)

stats = PerformanceStats()

class GitTimeCache:
    """Git 时间缓存，使用批量查询"""
    def __init__(self):
        self.git_root = None
        self.first_commit_cache = {}
        self.last_commit_cache = {}
        self.cache_lock = threading.Lock()
        self.init_git_root()
    
    def init_git_root(self):
        """初始化 Git 仓库根目录"""
        start_time = time.time()
        try:
            cmd = ['git', 'rev-parse', '--show-toplevel']
            result = subprocess.run(cmd, capture_output=True, text=True, timeout=2)
            if result.returncode == 0:
                self.git_root = result.stdout.strip()
                if DEBUG:
                    print(f"[DEBUG] Git 仓库根目录: {self.git_root}")
                stats.add_time("git_init", time.time() - start_time)
                return True
        except Exception as e:
            if DEBUG:
                print(f"[DEBUG] Git 初始化失败: {e}")
        stats.add_time("git_init", time.time() - start_time)
        return False
    
    def batch_query_git_times(self, file_paths):
        """批量查询所有文件的 Git 时间"""
        start_time = time.time()
        
        if not self.git_root or not file_paths:
            stats.add_time("git_batch_query", time.time() - start_time)
            return
        
        try:
            # 获取所有文件的相对路径
            rel_paths = []
            file_map = {}  # 相对路径 -> 原始路径映射
            
            for file_path in file_paths:
                try:
                    abs_path = os.path.abspath(file_path)
                    rel_path = os.path.relpath(abs_path, self.git_root)
                    rel_paths.append(rel_path)
                    file_map[rel_path] = file_path
                except ValueError:
                    continue
            
            if not rel_paths:
                stats.add_time("git_batch_query", time.time() - start_time)
                return
            
            # 方法1：使用 git log --all 批量获取所有文件的信息
            # 这是一个优化技巧：一次性获取所有文件的提交历史
            
            # 先获取所有文件的第一次提交时间
            first_times = self._get_first_commit_times_batch(rel_paths, file_map)
            
            # 再获取所有文件的最后一次提交时间
            last_times = self._get_last_commit_times_batch(rel_paths, file_map)
            
            # 更新缓存
            with self.cache_lock:
                for rel_path, file_path in file_map.items():
                    if rel_path in first_times:
                        self.first_commit_cache[file_path] = first_times[rel_path]
                    if rel_path in last_times:
                        self.last_commit_cache[file_path] = last_times[rel_path]
            
            if DEBUG:
                print(f"[DEBUG] 批量查询了 {len(rel_paths)} 个文件的 Git 时间")
            
        except Exception as e:
            if DEBUG:
                print(f"[DEBUG] 批量 Git 查询失败: {e}")
        
        stats.add_time("git_batch_query", time.time() - start_time)
    
    def _get_first_commit_times_batch(self, rel_paths, file_map):
        """批量获取第一次提交时间"""
        first_times = {}
        
        try:
            # 为每个文件单独查询第一次提交（但比在线程中查询快）
            # 我们可以在一个循环中完成，减少 Git 进程启动次数
            for rel_path, file_path in file_map.items():
                cmd = ['git', 'log', '--reverse', '--format=%at', '--', rel_path]
                result = subprocess.run(cmd, cwd=self.git_root,
                                      capture_output=True, text=True, timeout=1)
                if result.returncode == 0 and result.stdout.strip():
                    timestamps = result.stdout.strip().split('\n')
                    if timestamps and timestamps[0]:
                        first_times[rel_path] = datetime.fromtimestamp(int(timestamps[0]))
        except:
            pass
        
        return first_times
    
    def _get_last_commit_times_batch(self, rel_paths, file_map):
        """批量获取最后一次提交时间"""
        last_times = {}
        
        try:
            # 使用单个命令获取所有文件的最后一次提交时间
            # 使用 --pretty=format 和 NULL 分隔符
            cmd = ['git', 'log', '-1', '--pretty=format:%H%x00%at%x00', '--']
            cmd.extend(rel_paths)
            
            result = subprocess.run(cmd, cwd=self.git_root,
                                  capture_output=True, text=True, timeout=5)
            
            if result.returncode == 0 and result.stdout.strip():
                lines = result.stdout.strip().split('\n')
                for line in lines:
                    if '\x00' in line:
                        parts = line.split('\x00')
                        if len(parts) >= 3:
                            # 第三个部分是相对路径
                            rel_path = parts[2]
                            timestamp = parts[1]
                            if rel_path in file_map and timestamp:
                                last_times[rel_path] = datetime.fromtimestamp(int(timestamp))
        except:
            # 如果批量查询失败，回退到逐个查询
            for rel_path, file_path in file_map.items():
                try:
                    cmd = ['git', 'log', '-1', '--format=%at', '--', rel_path]
                    result = subprocess.run(cmd, cwd=self.git_root,
                                          capture_output=True, text=True, timeout=1)
                    if result.returncode == 0 and result.stdout.strip():
                        last_times[rel_path] = datetime.fromtimestamp(int(result.stdout.strip()))
                except:
                    pass
        
        return last_times

# 全局缓存
git_cache = GitTimeCache() if USE_GIT_HISTORY else None

def get_file_times_from_cache(file_path):
    """从缓存获取文件时间"""
    start_time = time.time()
    
    # 优先从 Git 缓存获取
    if git_cache:
        with git_cache.cache_lock:
            first_time = git_cache.first_commit_cache.get(file_path)
            last_time = git_cache.last_commit_cache.get(file_path)
        
        if first_time and last_time:
            stats.add_time("get_file_times", time.time() - start_time)
            return first_time, last_time
    
    # 文件系统时间
    fs_create = None
    fs_modify = None
    
    try:
        create_timestamp = os.path.getctime(file_path)
        fs_create = datetime.fromtimestamp(create_timestamp)
        
        modify_timestamp = os.path.getmtime(file_path)
        fs_modify = datetime.fromtimestamp(modify_timestamp)
    except Exception as e:
        if DEBUG:
            print(f"[DEBUG] 获取文件时间失败 {file_path}: {e}")
    
    stats.add_time("get_file_times", time.time() - start_time)
    return fs_create or datetime.now(), fs_modify or datetime.now()

def get_copyright_year_fast(file_path):
    """快速生成版权年份"""
    start_time = time.time()
    
    create_time, modify_time = get_file_times_from_cache(file_path)
    
    create_year = create_time.year
    modify_year = modify_time.year
    current_year = datetime.now().year
    
    # 异常年份处理
    if create_year > current_year:
        # 创建年份是未来，使用修改年份
        if modify_year <= current_year:
            result = str(modify_year)
        else:
            # 修改年份也是未来，使用当前年份
            result = str(current_year)
    elif create_year == modify_year:
        result = str(create_year)
    else:
        # 只显示范围如果两个年份都合理
        if 2000 <= create_year <= current_year and 2000 <= modify_year <= current_year:
            result = f"{create_year}-{modify_year}"
        else:
            # 使用合理的年份
            reasonable_year = create_year if 2000 <= create_year <= current_year else modify_year
            if not (2000 <= reasonable_year <= current_year):
                reasonable_year = current_year
            result = str(reasonable_year)
    
    stats.add_time("get_copyright_year", time.time() - start_time)
    return result

def get_username():
    """获取当前用户名"""
    return os.environ.get('USERNAME' if sys.platform.startswith('win') else 'USER', 'Unknown')

def read_lict_file(lict_path):
    """读取模板文件"""
    if not os.path.exists(lict_path):
        print(f"错误: 模板文件 {lict_path} 不存在！")
        sys.exit(1)
    with open(lict_path, 'r', encoding='utf-8') as f:
        return f.read()

def should_skip_copyright_update_fast(content, new_copyright_year):
    """快速检查是否需要更新版权声明"""
    start_time = time.time()
    
    # 简单查找版权行
    lines = content.split('\n')
    for line in lines[:20]:  # 只检查前20行
        if 'Copyright' in line and ('©' in line or 'Copyright' in line):
            # 提取年份
            year_match = re.search(r'(\d{4})(?:-(\d{4}))?', line)
            if year_match:
                existing_start = year_match.group(1)
                existing_end = year_match.group(2) or existing_start
                
                try:
                    start_year = int(existing_start)
                    end_year = int(existing_end)
                    
                    # 解析新的版权年份
                    if '-' in new_copyright_year:
                        new_start, new_end = map(int, new_copyright_year.split('-'))
                        new_year = new_end
                    else:
                        new_year = int(new_copyright_year)
                    
                    if start_year <= new_year <= end_year:
                        stats.add_time("skip_check", time.time() - start_time)
                        return True
                except:
                    pass
    
    stats.add_time("skip_check", time.time() - start_time)
    return False

def process_file_fast(file_path, lict_template, exclude_dirs):
    """快速处理单个文件"""
    start_time = time.time()
    
    # 检查排除目录
    abs_path = os.path.abspath(file_path)
    for ex_dir in exclude_dirs:
        if ex_dir in abs_path:
            stats.add_time("process_file", time.time() - start_time)
            return
    
    try:
        # 读取文件
        read_start = time.time()
        with open(file_path, 'r', encoding='utf-8') as f:
            content = f.read()
        stats.add_time("file_read", time.time() - read_start)
        
        # 获取版权年份
        copyright_year = get_copyright_year_fast(file_path)
        
        # 快速检查是否需要更新
        if should_skip_copyright_update_fast(content, copyright_year):
            stats.add_time("process_file", time.time() - start_time)
            return
        
        # 准备替换参数
        prepare_start = time.time()
        replace_dict = {
            'File': os.path.basename(file_path),
            'Username': get_username(),
            'CopyrightYear': copyright_year
        }
        
        header = lict_template
        for key, value in replace_dict.items():
            header = header.replace(f"%({key})", str(value))

        lines = header.split('\n')
        cpp_header = "/*\n"
        for line in lines:
            cpp_header += " *  " + line + "\n"
        cpp_header += " */"
        stats.add_time("prepare_header", time.time() - prepare_start)
        
        # 检查是否已存在
        if cpp_header.strip() in content:
            stats.add_time("process_file", time.time() - start_time)
            return
        
        # 插入新的
        write_start = time.time()
        with open(file_path, 'w', encoding='utf-8') as f:
            f.write(cpp_header + "\n" + content)
        stats.add_time("file_write", time.time() - write_start)
        
        print(f"已处理: {file_path}")
        
    except Exception as e:
        if DEBUG:
            print(f"[DEBUG] 处理文件失败 {file_path}: {e}")
    
    stats.add_time("process_file", time.time() - start_time)

def find_source_files(root_dir, exclude_dirs):
    """递归查找源文件"""
    start_time = time.time()
    
    source_files = []
    for dirpath, dirs, files in os.walk(root_dir):
        # 过滤排除目录
        dirs[:] = [d for d in dirs if d not in exclude_dirs]
        
        for file in files:
            ext = os.path.splitext(file)[1]
            if ext in SOURCE_EXTS:
                source_files.append(os.path.join(dirpath, file))
    
    stats.add_time("find_files", time.time() - start_time)
    return source_files

def process_files_parallel_fast(file_paths, lict_template, exclude_dirs, max_workers):
    """并行处理文件"""
    start_time = time.time()
    
    with ThreadPoolExecutor(max_workers=max_workers) as executor:
        futures = []
        for file_path in file_paths:
            future = executor.submit(process_file_fast, file_path, lict_template, exclude_dirs)
            futures.append(future)
        
        # 等待所有任务完成
        completed = 0
        for future in as_completed(futures):
            completed += 1
            if DEBUG and completed % 100 == 0:
                print(f"[DEBUG] 已完成 {completed}/{len(futures)} 个文件")
    
    stats.add_time("parallel_processing", time.time() - start_time)

def main():
    import argparse
    
    parser = argparse.ArgumentParser(description='自动插入/更新版权声明')
    parser.add_argument('--lict', default=DEFAULT_LICT_PATH, 
                       help=f'模板文件路径 (默认: {DEFAULT_LICT_PATH})')
    parser.add_argument('--exclude', default='', 
                       help='额外的排除目录，用逗号分隔')
    parser.add_argument('--debug', action='store_true',
                       help='启用调试模式')
    parser.add_argument('--no-git', action='store_true',
                       help='不使用 Git 历史')
    parser.add_argument('--threads', type=int, default=THREAD_COUNT,
                       help=f'线程数 (默认: {THREAD_COUNT})')
    parser.add_argument('--no-parallel', action='store_true',
                       help='禁用并行处理')
    parser.add_argument('--fast', action='store_true',
                       help='快速模式（推荐）')
    args = parser.parse_args()
    
    # 设置全局变量
    global DEBUG, USE_GIT_HISTORY, git_cache
    DEBUG = args.debug
    USE_GIT_HISTORY = not args.no_git
    
    lict_path = args.lict
    extra_exclude = set(args.exclude.split(",")) if args.exclude else set()
    exclude_dirs = DEFAULT_EXCLUDE_DIRS.union(extra_exclude)

    if DEBUG:
        print(f"[DEBUG] 使用Git历史: {USE_GIT_HISTORY}")
        print(f"[DEBUG] 线程数: {args.threads}")
        print(f"[DEBUG] 快速模式: {args.fast}")
        print(f"[DEBUG] 开始收集文件...")
    
    total_start = time.time()
    
    lict_template = read_lict_file(lict_path)
    root_dir = os.getcwd()
    source_files = find_source_files(root_dir, exclude_dirs)
    
    if DEBUG:
        print(f"[DEBUG] 找到 {len(source_files)} 个源文件")
    
    # 初始化 Git 缓存并批量查询
    if USE_GIT_HISTORY and args.fast:
        git_cache = GitTimeCache()
        if git_cache.git_root:
            if DEBUG:
                print(f"[DEBUG] 批量查询 Git 时间信息...")
            git_cache.batch_query_git_times(source_files)
    
    # 处理文件
    if args.no_parallel:
        if DEBUG:
            print(f"[DEBUG] 使用单线程处理...")
        
        for i, file_path in enumerate(source_files, 1):
            if DEBUG and i % 50 == 0:
                print(f"[DEBUG] 已处理 {i}/{len(source_files)} 个文件...")
            process_file_fast(file_path, lict_template, exclude_dirs)
    else:
        if DEBUG:
            print(f"[DEBUG] 使用多线程处理 (线程数: {args.threads})...")
        
        process_files_parallel_fast(source_files, lict_template, exclude_dirs, args.threads)
    
    total_time = time.time() - total_start
    
    print(f"\n处理完成！共处理 {len(source_files)} 个文件")
    print(f"总用时: {total_time:.2f}秒")
    
    # 打印性能统计
    stats.print_stats()

if __name__ == "__main__":
    main()
