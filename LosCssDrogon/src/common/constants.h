
#pragma once
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <functional>
#include <string>

namespace LosCommon {

/**
 * @brief Api str
 * HEALTH 服务器 健康检查
 */
namespace LOS_API_STR {
inline constexpr const char *HEALTH = "/api/health";
}

/**
 * @brief type
 *
 */
#define LOS_STATIC static
using LOS_CONST_STR = const std::string &;
using LOS_STATE = drogon::HttpStatusCode;
using LOS_RES = drogon::HttpResponsePtr;
using LOS_CONST_RES_FUNC = std::function<void(const drogon::HttpResponsePtr &)>;

} // namespace LosCommon