#!/usr/bin/env python3
"""
crash-server 完整工作流测试脚本（包含新增字段）

该脚本用于测试crash-server的完整功能，特别是测试新增的两个字段:
- exe_time_date_stamp
- exe_size_of_image
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

    def create_test_minidump(self, size_kb=16):
        """创建测试minidump文件"""
        # 创建临时文件
        temp_file = tempfile.NamedTemporaryFile(delete=False, suffix=".dmp")

        # 写入minidump头部 (MDMP = 0x4D444D50)
        header = b"MDMP"  # minidump文件头
        
        # 生成文件内容（指定大小）
        content_size = size_kb * 1024 - len(header)
        content = b"A" * content_size

        temp_file.write(header + content)
        temp_file.close()

        print(f"📁 Created test minidump: {temp_file.name} ({size_kb}KB)")
        return temp_file.name

    def create_large_test_minidump(self, size_mb=16):
        """创建大型测试minidump文件（模拟真实场景）"""
        # 创建临时文件
        temp_file = tempfile.NamedTemporaryFile(delete=False, suffix=".dmp")

        # 写入minidump头部
        header = b"MDMP"
        temp_file.write(header)
        
        # 写入大量数据（使用随机数据）
        import random
        chunk_size = 1024 * 1024  # 1MB chunks
        
        for i in range(size_mb):
            # 生成1MB的随机数据
            data = bytes([random.randint(0, 255) for _ in range(chunk_size)])
            temp_file.write(data)
            
        temp_file.close()
        
        file_size = os.path.getsize(temp_file.name)
        print(f"📁 Created large test minidump: {temp_file.name} ({file_size/1024/1024:.1f}MB)")
        return temp_file.name

    def upload_minidump_simple(self, file_path, metadata=None):
        """
        简化版的minidump上传测试，专注于基本功能
        """
        print(f"📤 Uploading minidump: {os.path.basename(file_path)}...", end="", flush=True)

        # 如果没提供metadata，使用默认值
        if metadata is None:
            metadata = {
                "timestamp": datetime.now(timezone.utc).isoformat().replace("+00:00", "Z"),
                "exe_version": "1.0.0",
                "exe_guid": "test-guid-123456",
                "exe_age": 0,
                "pid": 1234,
                "tid": 5678,
                "exception_code": "0xC0000005",
                "exception_address": "0x12345678",
            }

        try:
            with open(file_path, "rb") as f:
                files = {
                    "metadata": (None, json.dumps(metadata), "application/json"),
                    "minidump": (
                        os.path.basename(file_path),
                        f,
                        "application/octet-stream",
                    ),
                }

                response = self.session.post(
                    f"{self.base_url}/upload/minidump", 
                    files=files,
                    timeout=30  # 30秒超时
                )

                if response.status_code == 200:
                    result = response.json()
                    if result.get("success"):
                        minidump_id = result["data"]["minidump_id"]
                        print(f" ✅ PASS (ID: {minidump_id[:8]}...)")
                        return minidump_id, result
                    else:
                        print(f" ❌ FAIL - Upload failed: {result.get('error')}")
                        return None, result
                else:
                    print(f" ❌ FAIL - Status: {response.status_code}")
                    print(f"   Response: {response.text[:200]}")
                    return None, None
        except Exception as e:
            print(f" ❌ FAIL - Error: {str(e)}")
            return None, None

    def test_new_fields_small(self):
        """测试新增字段 - 使用小文件"""
        print("\n" + "="*60)
        print("🧪 TESTING NEW FIELDS (small file)")
        print("="*60)
        
        # 创建一个小型测试文件
        test_file = self.create_test_minidump(size_kb=16)
        
        try:
            # 测试1: 包含新增字段的metadata
            metadata_with_new_fields = {
                "timestamp": datetime.now(timezone.utc).isoformat().replace("+00:00", "Z"),
                "exe_version": "1.0.0",
                "exe_guid": "test-guid-new-fields-001",
                "exe_age": 1,
                "pid": 1001,
                "tid": 2001,
                "exception_code": "0xC0000005",
                "exception_address": "0x7FF66002A363",
                "exe_time_date_stamp": 1768031195,  # 新增字段
                "exe_size_of_image": 2899968,        # 新增字段
            }
            
            print("📝 Testing with new fields (exe_time_date_stamp, exe_size_of_image)...")
            minidump_id, result = self.upload_minidump_simple(test_file, metadata_with_new_fields)
            
            if minidump_id:
                print(f"✅ Successfully uploaded with new fields!")
                return True
            else:
                print(f"❌ Failed to upload with new fields")
                return False
                
        finally:
            # 清理文件
            if os.path.exists(test_file):
                os.remove(test_file)

    def test_new_fields_large(self):
        """测试新增字段 - 使用大文件（模拟真实场景）"""
        print("\n" + "="*60)
        print("🧪 TESTING NEW FIELDS (large file - 16MB)")
        print("="*60)
        
        # 创建一个大型测试文件（16MB）
        test_file = self.create_large_test_minidump(size_mb=16)
        
        try:
            # 使用与C++客户端完全相同的metadata
            metadata_exact = {
                "timestamp": "2026-01-10T07:56:37.805Z",
                "exe_version": "0.1.0.1",
                "exe_guid": "f5f6a5e0-f09a-5456-5078-001768031180",
                "exe_age": 0,
                "pid": 15424,
                "tid": 740,
                "exception_code": "0xC0000005",
                "exception_address": "0x00007FF66002A363",
                "exe_time_date_stamp": 1768031195,  # 新增字段
                "exe_size_of_image": 2899968,        # 新增字段
            }
            
            print("📝 Testing with exact same metadata as C++ client...")
            print(f"   exe_time_date_stamp: {metadata_exact['exe_time_date_stamp']}")
            print(f"   exe_size_of_image: {metadata_exact['exe_size_of_image']}")
            
            minidump_id, result = self.upload_minidump_simple(test_file, metadata_exact)
            
            if minidump_id:
                print(f"✅ Successfully uploaded 16MB file with new fields!")
                return True
            else:
                print(f"❌ Failed to upload 16MB file with new fields")
                return False
                
        finally:
            # 清理文件
            if os.path.exists(test_file):
                os.remove(test_file)

    def test_new_fields_optional(self):
        """测试新增字段为可选（不提供新增字段）"""
        print("\n" + "="*60)
        print("🧪 TESTING NEW FIELDS (optional - without new fields)")
        print("="*60)
        
        # 创建一个小型测试文件
        test_file = self.create_test_minidump(size_kb=16)
        
        try:
            # 测试：不包含新增字段的metadata
            metadata_without_new_fields = {
                "timestamp": datetime.now(timezone.utc).isoformat().replace("+00:00", "Z"),
                "exe_version": "1.0.0",
                "exe_guid": "test-guid-no-new-fields",
                "exe_age": 2,
                "pid": 1002,
                "tid": 2002,
                "exception_code": "0xC0000005",
                "exception_address": "0x12345678",
                # 故意不提供 exe_time_date_stamp 和 exe_size_of_image
            }
            
            print("📝 Testing without new fields (should work with #[serde(default)])...")
            minidump_id, result = self.upload_minidump_simple(test_file, metadata_without_new_fields)
            
            if minidump_id:
                print(f"✅ Successfully uploaded without new fields (optional works)!")
                return True
            else:
                print(f"❌ Failed to upload without new fields")
                return False
                
        finally:
            # 清理文件
            if os.path.exists(test_file):
                os.remove(test_file)

    def test_pdb_new_fields(self):
        """测试PDB上传的新增字段"""
        print("\n" + "="*60)
        print("🧪 TESTING PDB UPLOAD WITH NEW FIELDS")
        print("="*60)
        
        # 创建一个测试PDB文件
        temp_file = tempfile.NamedTemporaryFile(delete=False, suffix=".pdb")
        temp_file.write(b"PDB test content with new fields")
        temp_file.close()
        
        try:
            # PDB metadata包含新增字段
            metadata = {
                "exe_version": "1.0.0",
                "exe_guid": "test-guid-pdb-new-fields",
                "exe_age": 3,
                "pdb_filename": "test.pdb",
                "exe_time_date_stamp": 1768031195,  # 新增字段
                "exe_size_of_image": 2899968,        # 新增字段
            }
            
            print("📦 Uploading PDB with new fields...", end="", flush=True)
            
            with open(temp_file.name, "rb") as f:
                files = {
                    "metadata": (None, json.dumps(metadata), "application/json"),
                    "pdb": ("test.pdb", f, "application/octet-stream"),
                }

                response = self.session.post(
                    f"{self.base_url}/upload/pdb", 
                    files=files,
                    timeout=30
                )

                if response.status_code == 200:
                    result = response.json()
                    if result.get("success"):
                        pdb_id = result["data"]["pdb_id"]
                        print(f" ✅ PASS (ID: {pdb_id[:8]}...)")
                        return True
                    else:
                        print(f" ❌ FAIL - Upload failed: {result.get('error')}")
                        return False
                else:
                    print(f" ❌ FAIL - Status: {response.status_code}")
                    print(f"   Response: {response.text[:200]}")
                    return False
                    
        finally:
            # 清理文件
            if os.path.exists(temp_file.name):
                os.remove(temp_file.name)

    def run_detailed_new_fields_test(self):
        """运行详细的新字段测试"""
        print("=" * 60)
        print("🚀 CRASH-SERVER NEW FIELDS DETAILED TEST")
        print("=" * 60)

        # 测试健康检查
        health_ok = self.health_check()
        if not health_ok:
            print("\n❌ Health check failed. Server may not be running.")
            print("💡 Make sure to start the server with: cargo run")
            return False

        test_results = []
        
        # 测试1: 小文件 + 新字段
        result1 = self.test_new_fields_small()
        test_results.append(("Small file with new fields", result1))
        time.sleep(1)  # 给服务器一点时间处理
        
        # 测试2: 大文件 + 新字段（模拟真实场景）
        result2 = self.test_new_fields_large()
        test_results.append(("Large file with new fields", result2))
        time.sleep(1)
        
        # 测试3: 可选字段测试（不提供新字段）
        result3 = self.test_new_fields_optional()
        test_results.append(("Without new fields (optional)", result3))
        time.sleep(1)
        
        # 测试4: PDB上传的新字段
        result4 = self.test_pdb_new_fields()
        test_results.append(("PDB with new fields", result4))
        
        # 打印测试结果汇总
        print("\n" + "=" * 60)
        print("📊 TEST RESULTS SUMMARY")
        print("=" * 60)
        
        all_passed = True
        for test_name, passed in test_results:
            status = "✅ PASS" if passed else "❌ FAIL"
            print(f"{test_name}: {status}")
            if not passed:
                all_passed = False
        
        if all_passed:
            print("\n" + "=" * 60)
            print("🎉 ALL NEW FIELDS TESTS PASSED!")
            print("The new fields (exe_time_date_stamp, exe_size_of_image) are working correctly.")
            print("=" * 60)
            return True
        else:
            print("\n" + "=" * 60)
            print("❌ SOME TESTS FAILED")
            print("Check server logs for detailed error information.")
            print("=" * 60)
            return False

    def test_debug_metadata(self):
        """调试metadata格式问题"""
        print("\n" + "="*60)
        print("🔧 DEBUG METADATA FORMAT")
        print("="*60)
        
        # 创建测试文件
        test_file = self.create_test_minidump(size_kb=1)
        
        # 多种metadata格式测试
        test_cases = [
            {
                "name": "Exact C++ client format",
                "metadata": {
                    "timestamp": "2026-01-10T07:56:37.805Z",
                    "exe_version": "0.1.0.1",
                    "exe_guid": "f5f6a5e0-f09a-5456-5078-001768031180",
                    "exe_age": 0,
                    "pid": 15424,
                    "tid": 740,
                    "exception_code": "0xC0000005",
                    "exception_address": "0x00007FF66002A363",
                    "exe_time_date_stamp": 1768031195,
                    "exe_size_of_image": 2899968,
                }
            },
            {
                "name": "New fields as strings (if server expects strings)",
                "metadata": {
                    "timestamp": "2026-01-10T07:56:37.805Z",
                    "exe_version": "0.1.0.1",
                    "exe_guid": "f5f6a5e0-f09a-5456-5078-001768031180",
                    "exe_age": 0,
                    "pid": 15424,
                    "tid": 740,
                    "exception_code": "0xC0000005",
                    "exception_address": "0x00007FF66002A363",
                    "exe_time_date_stamp": "1768031195",  # 字符串
                    "exe_size_of_image": "2899968",        # 字符串
                }
            },
            {
                "name": "New fields as null",
                "metadata": {
                    "timestamp": "2026-01-10T07:56:37.805Z",
                    "exe_version": "0.1.0.1",
                    "exe_guid": "f5f6a5e0-f09a-5456-5078-001768031180",
                    "exe_age": 0,
                    "pid": 15424,
                    "tid": 740,
                    "exception_code": "0xC0000005",
                    "exception_address": "0x00007FF66002A363",
                    "exe_time_date_stamp": None,  # null
                    "exe_size_of_image": None,     # null
                }
            }
        ]
        
        all_passed = True
        for test_case in test_cases:
            print(f"\n📝 Testing: {test_case['name']}")
            print(f"   JSON: {json.dumps(test_case['metadata'])}")
            
            minidump_id, result = self.upload_minidump_simple(test_file, test_case['metadata'])
            
            if minidump_id:
                print(f"✅ PASS")
            else:
                print(f"❌ FAIL")
                if result:
                    print(f"   Server error: {result.get('error')}")
                all_passed = False
        
        # 清理文件
        if os.path.exists(test_file):
            os.remove(test_file)
            
        return all_passed


def main():
    """主函数"""
    import argparse

    parser = argparse.ArgumentParser(description="Test crash-server new fields functionality")
    parser.add_argument(
        "--base-url",
        default="http://localhost:3410",
        help="Base URL of the crash server (default: http://localhost:3410)",
    )
    parser.add_argument(
        "--api_key",
        default="test-api-key-123",
        help="API key for authentication (default: test-api-key-123)",
    )
    parser.add_argument(
        "--debug",
        action="store_true",
        help="Run debug tests for metadata format issues",
    )

    args = parser.parse_args()

    # 创建测试实例
    tester = CrashServerTest(base_url=args.base_url, api_key=args.api_key)

    print(f"🌐 Testing server at: {args.base_url}")
    print(f"🔑 Using API key: {args.api_key}")
    print("⏳ Waiting for server to be ready...")
    time.sleep(2)  # 等待服务器启动

    if args.debug:
        # 运行调试测试
        success = tester.test_debug_metadata()
    else:
        # 运行完整的新字段测试
        success = tester.run_detailed_new_fields_test()

    if not success:
        print("\n❌ TESTS FAILED!")
        print("\n💡 Debug suggestions:")
        print("1. Check server logs for detailed error messages")
        print("2. Make sure database has the new fields added")
        print("3. Verify #[serde(default)] is working correctly")
        print("4. Test with --debug flag for metadata format debugging")
        sys.exit(1)
    else:
        print("\n✅ ALL TESTS PASSED!")
        sys.exit(0)


if __name__ == "__main__":
    main()