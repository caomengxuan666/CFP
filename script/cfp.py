#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
CFP 构建工具（类 Cargo）
- 默认 list 显示所有存在的构建配置
- 自动排除 CMake 内部文件（CMakeFiles/...）
- 支持 run --target -- args...
"""

import os
import sys
import platform
import argparse
import subprocess
import json
from pathlib import Path

# ========== 配置 ==========
CMAKE_PATH = r"C:\Program Files\CMake\bin\cmake.EXE"
PROJECT_ROOT = r"D:/codespace/CFP"

BUILD_DIR_DEBUG = os.path.join(PROJECT_ROOT, "cmake-build-debug")
BUILD_DIR_RELEASE = os.path.join(PROJECT_ROOT, "cmake-build-release")

VS_INSTALL_PATH = r"C:\Program Files\Microsoft Visual Studio\18\Community"
ARCH = "x64"
CACHE_FILENAME = ".cfp_executables.json"

# 默认构建类型
DEFAULT_BUILD_TYPE = "debug"  # 可选 "debug" 或 "release"


def get_build_dir(is_release: bool) -> str:
    return BUILD_DIR_RELEASE if is_release else BUILD_DIR_DEBUG


def get_cache_path(build_dir: str) -> str:
    return os.path.join(build_dir, CACHE_FILENAME)


def init_msvc_env():
    if platform.system() != "Windows":
        print("[WARN] 非 Windows，跳过 MSVC 初始化")
        return os.environ.copy()

    vcvars_path = os.path.join(VS_INSTALL_PATH, "VC", "Auxiliary", "Build", "vcvarsall.bat")
    if not os.path.exists(vcvars_path):
        print(f"[WARN] 找不到 vcvarsall.bat: {vcvars_path}，继续尝试运行...")
        return os.environ.copy()

    cmd = f'"{vcvars_path}" {ARCH} && set'
    try:
        result = subprocess.run(
            cmd,
            shell=True,
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            encoding="gbk",
            timeout=30  # 添加超时设置，避免长时间等待
        )
    except subprocess.TimeoutExpired:
        print("[WARN] MSVC 环境初始化超时，继续尝试运行...")
        return os.environ.copy()
    except subprocess.CalledProcessError as e:
        print(f"[WARN] MSVC 初始化失败: {e}, 继续尝试运行...")
        return os.environ.copy()

    env = {}
    for line in result.stdout.splitlines():
        if "=" in line:
            key, value = line.split("=", 1)
            env[key] = value
    return env


def run_command(cmd, env, cwd=None):
    print(f"\n[CMD] {' '.join(map(str, cmd))}")
    try:
        result = subprocess.run(
            cmd,
            shell=False,
            check=True,
            stdout=subprocess.PIPE,
            stderr=subprocess.STDOUT,
            encoding="utf-8",
            env=env,
            cwd=cwd,
        )
        if result.stdout.strip():
            print(result.stdout)
        return True
    except subprocess.CalledProcessError as e:
        print(f"[ERROR] 命令失败:\n{e.stdout}", file=sys.stderr)
        return False
    except FileNotFoundError:
        print(f"[ERROR] 找不到命令: {cmd[0]}", file=sys.stderr)
        return False


def scan_executables(build_dir: str):
    """扫描 .exe，排除 CMake 内部文件"""
    build_path = Path(build_dir)
    if not build_path.exists():
        return []
    exes = []
    for exe in build_path.rglob("*.exe"):
        if not exe.is_file():
            continue
        rel = exe.relative_to(build_path).as_posix()
        # 修复：更全面地排除 CMake 内部文件，使用小写进行比较
        lower_rel = rel.lower()
        if ("cmakefiles" in lower_rel or 
            "/cmake/" in lower_rel or 
            "compilerid" in lower_rel):
            continue
        stem = os.path.splitext(rel)[0]
        exes.append(stem)
    return sorted(set(exes))


def save_executable_cache(build_dir: str, exe_list: list[str]):
    cache_path = get_cache_path(build_dir)
    with open(cache_path, "w", encoding="utf-8") as f:
        json.dump({"version": 1, "executables": exe_list}, f, indent=2)


def load_executable_cache(build_dir: str):
    cache_path = get_cache_path(build_dir)
    if not os.path.exists(cache_path):
        return None
    try:
        with open(cache_path, "r", encoding="utf-8") as f:
            data = json.load(f)
            if data.get("version") == 1 and isinstance(data.get("executables"), list):
                return data["executables"]
    except (json.JSONDecodeError, OSError):
        pass
    return None


def get_executables_with_cache(build_dir: str, use_cache: bool = True):
    if use_cache:
        cached = load_executable_cache(build_dir)
        if cached is not None:
            return cached
    exes = scan_executables(build_dir)
    if use_cache:
        save_executable_cache(build_dir, exes)
    return exes


def resolve_target_exe_path(build_dir: str, target_name: str | None, exe_names: list[str]):
    if not exe_names:
        print(f"[ERROR] 未在 '{build_dir}' 中找到任何可执行文件！")
        sys.exit(1)

    if target_name is None:
        if len(exe_names) == 1:
            target_name = exe_names[0]
        else:
            print(f"[ERROR] 找到多个可执行文件，请使用 --target 指定一个：")
            for name in exe_names:
                print(f"  {name}")
            sys.exit(1)

    if target_name in exe_names:
        return os.path.join(build_dir, target_name + ".exe")

    # 改进：如果找不到精确匹配，尝试从路径部分提取文件名进行匹配
    # 首先查找完全匹配或路径末尾匹配
    matches = [name for name in exe_names if name == target_name or name.endswith("/" + target_name) or name.startswith(target_name + "/")]
    
    # 如果还是没找到，尝试按文件名部分匹配（即路径的最后一部分是目标名称）
    if not matches:
        matches = [name for name in exe_names if Path(name).name == target_name]

    if len(matches) == 1:
        return os.path.join(build_dir, matches[0] + ".exe")
    if len(matches) > 1:
        # 如果用户提供了target参数但有多个匹配，更智能地选择
        exact_name_matches = [name for name in matches if name == target_name or Path(name).name == target_name]
        if len(exact_name_matches) == 1:
            return os.path.join(build_dir, exact_name_matches[0] + ".exe")
        
        print(f"[ERROR] 多个可执行文件匹配 '{target_name}'：")
        for m in matches:
            print(f"  {m}")
        sys.exit(1)

    print(f"[ERROR] 未找到名为 '{target_name}' 的可执行文件。可用选项：")
    for name in sorted(exe_names):
        print(f"  {name}")
    sys.exit(1)


def determine_build_type(args):
    """确定构建类型，根据命令行参数或默认值"""
    if hasattr(args, 'release') and args.release:
        return True  # release
    elif hasattr(args, 'debug') and args.debug:
        return False  # debug
    else:
        # 使用默认构建类型
        return DEFAULT_BUILD_TYPE == "release"


def cmd_build(args):
    is_release = determine_build_type(args)
    build_dir = get_build_dir(is_release)
    build_type = "Release" if is_release else "Debug"
    print(f"🔧 配置: {build_type}")
    print(f"📁 构建目录: {build_dir}")

    env = init_msvc_env()
    src_dir = PROJECT_ROOT
    if not os.path.exists(os.path.join(src_dir, "CMakeLists.txt")):
        print(f"[ERROR] 未找到 CMakeLists.txt")
        sys.exit(1)

    config_cmd = [
        CMAKE_PATH,
        "-S", src_dir,
        "-B", build_dir,
        "-G", "Ninja",
        f"-DCMAKE_BUILD_TYPE={build_type}",
        "-DCMAKE_EXPORT_COMPILE_COMMANDS=ON",
        "--no-warn-unused-cli"
    ]
    print("⚙️  正在配置项目...")
    if not run_command(config_cmd, env):
        sys.exit(1)

    build_cmd = [
        CMAKE_PATH,
        "--build", build_dir,
        "--config", build_type,
        "--", "-j", str(os.cpu_count() or 8)
    ]
    print("🔨 正在构建项目...")
    if not run_command(build_cmd, env):
        sys.exit(1)

    print(f"\n✅ 构建完成 ({build_type})")
    exe_list = scan_executables(build_dir)
    save_executable_cache(build_dir, exe_list)
    print(f"📦 发现 {len(exe_list)} 个可执行文件")


def cmd_run(args):
    is_release = determine_build_type(args)
    build_dir = get_build_dir(is_release)

    # 检查构建目录是否存在，如果不存在，尝试另一个构建类型
    if not os.path.exists(build_dir):
        alternative_build_dir = get_build_dir(not is_release)
        alternative_build_type = "Release" if not is_release else "Debug"
        
        if os.path.exists(alternative_build_dir):
            print(f"⚠️  {build_dir} 不存在，使用 {alternative_build_type} 构建目录")
            build_dir = alternative_build_dir
            is_release = not is_release
        else:
            build_config = "Release" if is_release else "Debug"
            print(f"[ERROR] 构建目录不存在: {build_dir} ({build_config})")
            print(f"请先运行: cfp build --{build_config.lower()}")
            sys.exit(1)

    exe_names = get_executables_with_cache(build_dir, use_cache=True)
    exe_path = resolve_target_exe_path(build_dir, args.target, exe_names)

    env = init_msvc_env()
    cmd = [exe_path] + (args.extra_args or [])
    print(f"🚀 运行: {' '.join(cmd)}")
    try:
        # 切换到项目根目录再运行
        subprocess.run(cmd, env=env, check=True, cwd=PROJECT_ROOT)
    except subprocess.CalledProcessError as e:
        sys.exit(e.returncode)


def cmd_list(args):
    # 如果指定了具体配置，只列该配置
    if hasattr(args, 'debug') and args.debug or hasattr(args, 'release') and args.release:
        is_release = hasattr(args, 'release') and args.release
        build_dir = get_build_dir(is_release)
        config_name = "Release" if is_release else "Debug"

        if not os.path.exists(build_dir):
            print(f"[ERROR] 构建目录不存在: {build_dir}")
            return

        exe_names = get_executables_with_cache(build_dir, use_cache=True)
        if not exe_names:
            print(f"ℹ️  {config_name} 中未找到任何可执行文件")
        else:
            print(f"📋 可执行文件列表（{config_name}）:")
            for i, name in enumerate(exe_names, 1):
                print(f"  {i}. {Path(name).name} [{name}]")
        return

    # 否则，默认列出所有存在的配置
    configs = [
        (False, "Debug", BUILD_DIR_DEBUG),
        (True, "Release", BUILD_DIR_RELEASE)
    ]

    found_any = False
    for is_release, name, build_dir in configs:
        if os.path.exists(build_dir):
            exe_names = get_executables_with_cache(build_dir, use_cache=True)
            print(f"\n📊 {name} 配置 ({os.path.basename(build_dir)}):")
            if exe_names:
                found_any = True
                for i, exe in enumerate(exe_names, 1):
                    print(f"  {i}. {Path(exe).name} [{exe}]")
            else:
                print("  (无用户可执行文件)")
        # else: 不打印不存在的配置（保持简洁）

    if not found_any:
        print("ℹ️  Debug 和 Release 目录中均未找到用户可执行文件")


def main():
    parser = argparse.ArgumentParser(prog="cfp", description="CFP 构建工具（类 Cargo）")
    subparsers = parser.add_subparsers(dest="command", required=True)

    def add_build_type(parser):
        group = parser.add_mutually_exclusive_group()
        group.add_argument("--debug", action="store_true", help="使用 Debug 配置")
        group.add_argument("--release", action="store_true", help="使用 Release 配置")

    # build
    build_parser = subparsers.add_parser("build", help="编译项目")
    add_build_type(build_parser)

    # run
    run_parser = subparsers.add_parser("run", help="运行可执行文件")
    add_build_type(run_parser)
    run_parser.add_argument("--target", type=str, help="指定可执行文件名（可以是名称或路径）")
    run_parser.add_argument("extra_args", nargs=argparse.REMAINDER, help="传递给程序的额外参数")

    # list
    list_parser = subparsers.add_parser("list", help="列出所有可执行文件（默认显示所有存在的配置）")
    add_build_type(list_parser)  # 允许 list --debug 单独查看

    args = parser.parse_args()

    # 注意：build 和 run 仍需要默认值
    if args.command in ("build", "run"):
        if not hasattr(args, 'debug') or not args.debug:
            if not hasattr(args, 'release') or not args.release:
                # 使用默认构建类型
                if DEFAULT_BUILD_TYPE == "release":
                    setattr(args, 'release', True)
                else:
                    setattr(args, 'debug', True)

    if args.command == "build":
        cmd_build(args)
    elif args.command == "run":
        cmd_run(args)
    elif args.command == "list":
        cmd_list(args)


if __name__ == "__main__":
    main()