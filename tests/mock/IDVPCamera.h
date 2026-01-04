/*
 *  Copyright © 2026 [caomengxuan666]
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
 *  - File: IDVPCamera.h
 *  - Username: Administrator
 *  - CopyrightYear: 2026
 */

// tests/mock/IDVPCamera.h
#pragma once
#include <memory>
#include <string>
#include <vector>

class FrameProcessor;
class DvpConfig;

class IDVPCamera {
 public:
  virtual ~IDVPCamera() = default;

  virtual bool connect(const std::string& id) = 0;
  virtual void disconnect() = 0;
  virtual bool isConnected() const = 0;

  virtual void setExposure(double us) = 0;
  virtual double getExposure() const = 0;

  virtual void setGain(float gain) = 0;
  virtual float getGain() const = 0;

  virtual void setHardwareIsp(bool enable) = 0;
  virtual bool getHardwareIsp() const = 0;

  virtual void setTriggerMode(bool enable) = 0;
  virtual bool getTriggerMode() const = 0;

  virtual void setConfig(const DvpConfig& config) = 0;
  virtual DvpConfig getConfig() const = 0;

  virtual void startCapture() = 0;
  virtual void stopCapture() = 0;
  virtual bool isCapturing() const = 0;

  virtual std::vector<uint8_t> getFrameData() = 0;
  virtual void registerFrameCallback(
      const std::shared_ptr<FrameProcessor>& callback) = 0;
};
