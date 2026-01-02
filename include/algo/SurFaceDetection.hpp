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
 *  - File: SurFaceDetection.hpp
 *  - Username: Administrator
 *  - CopyrightYear: 2025-2026
 */

#pragma once

#include <shared_mutex>
#include <vector>

#include "algo/AlgoBase.hpp"
#include "algo/AlgorithmConfigTraits.hpp"
#include "config/AlogoParams.hpp"
#include "config/ConfigObserver.hpp"

namespace algo {

template <>
struct AlgorithmConfigExtractor<config::SurfaceDetectionConfig> {
  static const config::SurfaceDetectionConfig& extract(
      const config::GlobalConfig& global_cfg) {
    return global_cfg.surface_detection;
  }
};

class SurfaceDetection : public AlgoBase {
 public:
  ALGO_METADATA("SurfaceDetection", "表面检测")
  using Config = config::SurfaceDetectionConfig;
  SurfaceDetection();
  explicit SurfaceDetection(const Config& cfg);
  void process(const CapturedFrame& frame) override;

  std::vector<AlgoParamInfo> get_parameter_info() const override;
  std::vector<AlgoSignalInfo> get_signal_info() const override;

  void update_config(const Config& new_cfg);

 private:
  Config config_;
  mutable std::shared_mutex config_mutex_;
};

}  // namespace algo
