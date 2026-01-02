/*
 *  Copyright © 2025-2026 [caomengxuan666]
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the “Software”), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 *
 *  - File: get_local_ip.h
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

#ifndef UTILS_GET_LOCAL_IP_H_
#define UTILS_GET_LOCAL_IP_H_

#include <string>
#include <vector>
namespace utils {
// 获取本地所有非回环IP地址
extern std::vector<std::string> get_all_local_ips();

// 获取本地IPv4地址（最常用）
extern std::vector<std::string> get_local_ipv4();

// 获取单个主要IPv4地址（推荐使用）
extern std::string get_local_ip();
}  // namespace utils
#endif  // UTILS_GET_LOCAL_IP_H_
