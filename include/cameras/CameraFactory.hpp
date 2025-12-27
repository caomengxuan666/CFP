/*
 *  Copyright © 2025 [caomengxuan666]
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
 *  - File: CameraFactory.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025
 */

#pragma once

// 这个文件的意图是直接从配置文件中读取相机的配置信息且应用
// 虽然每个相机都有自己的配置文件，我们也需要管理多个相机的实例。
#include <memory>
#include <string>

struct CameraConfig;
class CameraCapture;
enum class CameraBrand { DVP, IK, MIND };

std::unique_ptr<CameraCapture> create_camera(const CameraBrand& brand,
                                             const std::string& identifier,
                                             const CameraConfig& config);
