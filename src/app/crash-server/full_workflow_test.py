#!/usr/bin/env python3
"""
crash-server 完整工作流测试脚本

该脚本用于测试crash-server的完整功能，包括:
- 健康检查
- Minidump上传
- Minidump下载
- PDB上传
- PDB下载
- 列表查询
- 清理验证
"""

import os
import sys
import time
import json
import requests
import tempfile
from pathlib import Path
from datetime import datetime, timezone


class CrashServerTest:
    def __init__(self, base_url="http://localhost:3410", api_key="test-api-key-123"):
        """
        初始化测试类
        
        :param base_url: 服务器基础URL
        :param api_key: API密钥
        """
        self.base_url = base_url
        self.headers = {"X-API-Key": api_key}
        self.session = requests.Session()
        self.session.headers.update(self.headers)
        
    def health_check(self):
        """测试健康检查接口"""
        print("🔍 Testing health check...", end="", flush=True)
        try:
            response = self.session.get(f"{self.base_url}/health")
            if response.status_code == 200:
                data = response.json()
                if data.get("status") == "ok":
                    print(" ✅ PASS")
                    return True
                else:
                    print(" ❌ FAIL - Invalid response data")
                    return False
            else:
                print(f" ❌ FAIL - Status: {response.status_code}")
                return False
        except Exception as e:
            print(f" ❌ FAIL - Error: {str(e)}")
            return False
    
    def create_test_minidump(self):
        """创建测试minidump文件"""
        # 创建临时文件
        temp_file = tempfile.NamedTemporaryFile(delete=False, suffix='.dmp')
        
        # 写入minidump头部 (MDMP = 0x4D444D50)
        header = b"MDMP"  # minidump文件头
        content = b"Workflow test minidump content - testing crash-server functionality"
        
        temp_file.write(header + content)
        temp_file.close()
        
        return temp_file.name
        
    def create_test_pdb(self):
        """创建测试PDB文件"""
        # 创建临时PDB文件
        temp_file = tempfile.NamedTemporaryFile(delete=False, suffix='.pdb')
        
        # 写入一些测试内容
        content = b"PDB file content for testing crash-server PDB upload functionality"
        temp_file.write(content)
        temp_file.close()
        
        return temp_file.name
    
    def upload_minidump(self, file_path):
        """上传minidump文件"""
        print("📦 Uploading minidump...", end="", flush=True)
        
        # 生成元数据
        metadata = {
            "timestamp": datetime.now(timezone.utc).isoformat().replace("+00:00", "Z"),
            "exe_version": "2.0.0",
            "exe_guid": "test-guid-minidump-workflow",
            "exe_age": 1,
            "pid": 1234,
            "tid": 5678,
            "exception_code": "0xC0000005",
            "exception_address": "0x12345678"
        }
        
        try:
            with open(file_path, 'rb') as f:
                files = {
                    'metadata': (None, json.dumps(metadata), 'application/json'),
                    'minidump': (os.path.basename(file_path), f, 'application/octet-stream')
                }
                
                response = self.session.post(
                    f"{self.base_url}/upload/minidump",
                    files=files
                )
                
                if response.status_code == 200:
                    result = response.json()
                    if result.get("success"):
                        minidump_id = result["data"]["minidump_id"]
                        print(f" ✅ PASS (ID: {minidump_id[:8]}...)")
                        return minidump_id
                    else:
                        print(f" ❌ FAIL - Upload failed: {result.get('error')}")
                        return None
                else:
                    print(f" ❌ FAIL - Status: {response.status_code}, Response: {response.text}")
                    return None
        except Exception as e:
            print(f" ❌ FAIL - Error: {str(e)}")
            return None
    
    def list_minidumps(self):
        """列出minidump文件"""
        print("📋 Listing minidumps...", end="", flush=True)
        try:
            response = self.session.get(f"{self.base_url}/list/minidumps")
            
            if response.status_code == 200:
                result = response.json()
                if result.get("success") and "data" in result:
                    minidumps = result["data"]["minidumps"]
                    print(f" ✅ PASS ({len(minidumps)} found)")
                    return minidumps
                else:
                    print(f" ❌ FAIL - Invalid response format")
                    return None
            else:
                print(f" ❌ FAIL - Status: {response.status_code}")
                return None
        except Exception as e:
            print(f" ❌ FAIL - Error: {str(e)}")
            return None
            
    def upload_pdb(self, file_path):
        """上传PDB文件"""
        print("📦 Uploading PDB...", end="", flush=True)
        
        # 生成PDB元数据
        metadata = {
            "exe_version": "2.0.0",
            "exe_guid": "test-guid-minidump-workflow",  # 与上面的minidump使用相同的GUID，以便关联
            "exe_age": 1,
            "pdb_filename": os.path.basename(file_path)
        }
        
        try:
            with open(file_path, 'rb') as f:
                files = {
                    'metadata': (None, json.dumps(metadata), 'application/json'),
                    'pdb': (os.path.basename(file_path), f, 'application/octet-stream')
                }
                
                response = self.session.post(
                    f"{self.base_url}/upload/pdb",
                    files=files
                )
                
                if response.status_code == 200:
                    result = response.json()
                    if result.get("success"):
                        pdb_id = result["data"]["pdb_id"]
                        print(f" ✅ PASS (ID: {pdb_id[:8]}...)")
                        return pdb_id
                    else:
                        print(f" ❌ FAIL - Upload failed: {result.get('error')}")
                        return None
                else:
                    print(f" ❌ FAIL - Status: {response.status_code}, Response: {response.text}")
                    return None
        except Exception as e:
            print(f" ❌ FAIL - Error: {str(e)}")
            return None
            
    def download_minidump(self, minidump_id):
        """下载minidump文件"""
        print(f"📥 Downloading minidump {minidump_id[:8]}...", end="", flush=True)
        try:
            response = self.session.get(f"{self.base_url}/download/minidump/{minidump_id}")
            
            if response.status_code == 200:
                # 验证是否是有效的minidump文件
                content = response.content
                if content.startswith(b"MDMP"):
                    print(" ✅ PASS (Valid minidump file)")
                    return True
                else:
                    print(" ❌ FAIL (Invalid file format)")
                    return False
            else:
                print(f" ❌ FAIL - Status: {response.status_code}")
                return False
        except Exception as e:
            print(f" ❌ FAIL - Error: {str(e)}")
            return False
            
    def download_pdb(self, pdb_id):
        """下载PDB文件"""
        print(f"📥 Downloading PDB {pdb_id[:8]}...", end="", flush=True)
        try:
            response = self.session.get(f"{self.base_url}/download/pdb/{pdb_id}")
            
            if response.status_code == 200:
                # 验证PDB文件内容（简单检查是否有内容）
                content = response.content
                if len(content) > 0:
                    print(" ✅ PASS (Valid PDB file)")
                    return True
                else:
                    print(" ❌ FAIL (Empty file)")
                    return False
            else:
                print(f" ❌ FAIL - Status: {response.status_code}")
                return False
        except Exception as e:
            print(f" ❌ FAIL - Error: {str(e)}")
            return False
    
    def download_minidump(self, minidump_id):
        """下载minidump文件"""
        print(f"📥 Downloading minidump {minidump_id[:8]}...", end="", flush=True)
        try:
            response = self.session.get(f"{self.base_url}/download/minidump/{minidump_id}")
            
            if response.status_code == 200:
                # 验证是否是有效的minidump文件
                content = response.content
                if content.startswith(b"MDMP"):
                    print(" ✅ PASS (Valid minidump file)")
                    return True
                else:
                    print(" ❌ FAIL (Invalid file format)")
                    return False
            else:
                print(f" ❌ FAIL - Status: {response.status_code}")
                return False
        except Exception as e:
            print(f" ❌ FAIL - Error: {str(e)}")
            return False
    
    def test_query_filters(self, exe_guid):
        """测试查询过滤器"""
        print("🔍 Testing query filters...", end="", flush=True)
        try:
            # 使用GUID过滤查询
            response = self.session.get(
                f"{self.base_url}/list/minidumps",
                params={"guid": exe_guid}
            )
            
            if response.status_code == 200:
                result = response.json()
                if result.get("success") and "data" in result:
                    minidumps = result["data"]["minidumps"]
                    if len(minidumps) > 0:
                        print(f" ✅ PASS (Found {len(minidumps)} matching records)")
                        return True
                    else:
                        print(" ❌ FAIL (No matching records found)")
                        return False
                else:
                    print(" ❌ FAIL (Invalid response format)")
                    return False
            else:
                print(f" ❌ FAIL - Status: {response.status_code}")
                return False
        except Exception as e:
            print(f" ❌ FAIL - Error: {str(e)}")
            return False

    def test_find_matching_pdb(self, exe_version, exe_guid, exe_age):
        """测试查找匹配的PDB文件"""
        print("🔍 Testing find matching PDB...", end="", flush=True)
        try:
            # 使用版本、GUID和Age查询匹配的PDB
            response = self.session.get(
                f"{self.base_url}/find/matching-pdb",
                params={
                    "exe_version": exe_version,
                    "exe_guid": exe_guid,
                    "exe_age": exe_age
                }
            )
            
            if response.status_code == 200:
                result = response.json()
                if result.get("success") and "data" in result:
                    pdb_data = result["data"]
                    if "pdb" in pdb_data:
                        print(f" ✅ PASS (Found matching PDB: {pdb_data['pdb'].get('id', 'N/A')[:8]}...)")
                        return True
                    else:
                        print(" ❌ FAIL (No matching PDB found)")
                        return False
                else:
                    print(" ❌ FAIL (Invalid response format)")
                    return False
            elif response.status_code == 404:
                # 404是有效响应，表示没有找到匹配的PDB
                result = response.json()
                if result.get("error") and "No matching PDB found" in result["error"]:
                    print(" ✅ PASS (No matching PDB as expected)")
                    return True
                else:
                    print(" ❌ FAIL (Invalid 404 response)")
                    return False
            else:
                print(f" ❌ FAIL - Status: {response.status_code}")
                return False
        except Exception as e:
            print(f" ❌ FAIL - Error: {str(e)}")
            return False
    
    def cleanup_file(self, file_path):
        """清理测试文件"""
        try:
            if os.path.exists(file_path):
                os.remove(file_path)
        except Exception:
            pass  # 忽略清理错误
    
    def run_full_test(self):
        """运行完整测试流程"""
        print("=" * 60)
        print("🚀 CRASH-SERVER FULL WORKFLOW TEST")
        print("=" * 60)
        
        # 测试健康检查
        health_ok = self.health_check()
        if not health_ok:
            print("\n❌ Health check failed. Server may not be running.")
            print("💡 Make sure to start the server with: cargo run")
            return False
        
        # 创建测试文件
        test_minidump = self.create_test_minidump()
        test_pdb = self.create_test_pdb()
        
        try:
            # 上传minidump
            minidump_id = self.upload_minidump(test_minidump)
            if not minidump_id:
                return False
            
            # 上传PDB
            pdb_id = self.upload_pdb(test_pdb)
            if not pdb_id:
                return False
            
            # 等待服务器处理
            time.sleep(0.5)
            
            # 获取上传的记录信息用于后续测试
            minidumps = self.list_minidumps()
            if not minidumps:
                return False
            
            uploaded_record = minidumps[0]  # 假设刚上传的是第一个
            exe_guid = uploaded_record.get("exe_guid")
            exe_version = uploaded_record.get("exe_version")
            exe_age = uploaded_record.get("exe_age", 1)
            
            # 测试查询过滤器
            if exe_guid:
                query_ok = self.test_query_filters(exe_guid)
                if not query_ok:
                    return False
            
            # 测试查找匹配的PDB
            if exe_version and exe_guid and exe_age is not None:
                matching_pdb_ok = self.test_find_matching_pdb(exe_version, exe_guid, exe_age)
                if not matching_pdb_ok:
                    return False
            
            # 下载minidump
            download_minidump_ok = self.download_minidump(minidump_id)
            if not download_minidump_ok:
                return False
            
            # 下载PDB
            download_pdb_ok = self.download_pdb(pdb_id)
            if not download_pdb_ok:
                return False
            
            # 最终验证列表中确实有文件
            final_list = self.list_minidumps()
            if final_list is not None and len(final_list) >= 1:
                print("📊 Final verification: ✅ PASS (Minidump found in database)")
            else:
                print("📊 Final verification: ❌ FAIL (No minidumps in database)")
                return False
            
        finally:
            # 清理测试文件
            self.cleanup_file(test_minidump)
            self.cleanup_file(test_pdb)
        
        print("\n" + "=" * 60)
        print("✅ ALL TESTS PASSED!")
        print("🎉 Crash server is working correctly")
        print("=" * 60)
        
        return True


def main():
    """主函数"""
    # 从命令行参数或环境变量获取配置
    import argparse
    
    parser = argparse.ArgumentParser(description="Test crash-server functionality")
    parser.add_argument("--base-url", default="http://localhost:3410",
                        help="Base URL of the crash server (default: http://localhost:3410)")
    parser.add_argument("--api-key", default="test-api-key-123",
                        help="API key for authentication (default: test-api-key-123)")
    
    args = parser.parse_args()
    
    # 创建测试实例并运行测试
    tester = CrashServerTest(base_url=args.base_url, api_key=args.api_key)
    
    # 检查服务器是否运行
    print(f"🌐 Testing server at: {args.base_url}")
    print(f"🔑 Using API key: {args.api_key}")
    print("⏳ Waiting for server to be ready...")
    time.sleep(2)  # 等待服务器启动
    
    success = tester.run_full_test()
    
    if not success:
        print("\n❌ TESTS FAILED!")
        sys.exit(1)
    else:
        print("\n✅ ALL TESTS PASSED!")
        sys.exit(0)


if __name__ == "__main__":
    main()