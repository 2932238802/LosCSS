
#include "HealthController.h"
#include "core/LosResponse/LosResponse.h"
#include <chrono>
#include <json/value.h>

namespace LosController {

/**
 * @brief 开始计时 L_startTime
 */
const std::chrono::steady_clock::time_point HealthController::L_startTime =
    std::chrono::steady_clock::now();

/**
 * @brief check
 * @param req
 * @param callback
 */
void HealthController::check(const drogon::HttpRequestPtr &req,
                             LOS_CONST_RES_FUNC &&callback) {
  const auto uptimeSec = std::chrono::duration_cast<std::chrono::seconds>(
                             std::chrono::steady_clock::now() - L_startTime)
                             .count();
  Json::Value data;
  data["uptime"] = uptimeSec;
  data["service"] = "LosCssDrogon";
  callback(LosCore::LosResponse::ok(data));
}
} // namespace LosController