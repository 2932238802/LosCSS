
#include "HealthController.h"
#include "dto/LosResponse/LosResponse.h"
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
                             std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
  const auto uptimeSec = std::chrono::duration_cast<std::chrono::seconds>(
                             std::chrono::steady_clock::now() - L_startTime)
                             .count();
  Json::Value data;
  data["uptime"] = uptimeSec;
  data["service"] = "LosCssDrogon";
  callback(LosDto::LosResponse::ok(data));
}
} // namespace LosController