#pragma once
#include "common/constants.h"
#include <chrono>
#include <drogon/HttpController.h>
#include <drogon/HttpRequest.h>

namespace LosController {
using LosCommon::LOS_CONST_STR;

/**
 * @brief HealthController
 * drogon::HttpController<HealthController>
 * 这里是 类型传递
 */
class HealthController : public drogon::HttpController<HealthController> {
public:
  METHOD_LIST_BEGIN
  ADD_METHOD_TO(HealthController::check, LosCommon::LOS_API_STR::HEALTH, drogon::Get);
  METHOD_LIST_END

public:
  // 检查是不是正常的
  // LosCommon::LOS_API_STR::HEALTH
  void check(const drogon::HttpRequestPtr &req,
             std::function<void(const drogon::HttpResponsePtr &)> &&callback);

private:
  // 记录 开始时间
  static const std::chrono::steady_clock::time_point L_startTime;
};
} // namespace LosController