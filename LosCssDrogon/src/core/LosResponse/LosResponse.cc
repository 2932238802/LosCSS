
#include "LosResponse.h"
#include <drogon/HttpResponse.h>
#include <json/value.h>

namespace LosCore {

/**
 * @brief  成功 并有数据
 *
 * @param data
 * @return LOS_STATIC
 */
LOS_RES LosResponse::ok(const Json::Value &data) {
  return build(0, "success", data, drogon::HttpStatusCode::k200OK);
}

/**
 * @brief 成功没有数据
 *
 * @return LOS_RES
 */
LOS_RES LosResponse::ok() {
  return build(0, "success", Json::Value(), drogon::HttpStatusCode::k200OK);
}

/**
 * @brief  失败的时候
 *
 * @param code
 * @param msg
 * @param state
 * @return LOS_RES
 */
LOS_RES
LosResponse::fail(int code, LOS_CONST_STR msg, LOS_STATE state) {
  return build(code, msg, Json::Value(), state);
}

/**
 * @brief 400 无效请求
 *
 * @param msg
 * @return LOS_RES
 */
LOS_RES LosResponse::badRequest(LOS_CONST_STR msg) {
  return build(400, msg, Json::Value(), drogon::HttpStatusCode::k400BadRequest);
}

/**
 * @brief 404 未找到资源
 *
 * @param msg
 * @return LOS_RES
 */
LosCommon::LOS_RES LosResponse::notFound(LOS_CONST_STR msg) {
  return build(404, msg, Json::Value(), drogon::HttpStatusCode::k404NotFound);
}

/**
 * @brief 500 服务器错误
 *
 * @param msg
 * @return LOS_RES
 */
LOS_RES LosResponse::serverError(LOS_CONST_STR msg) {
  return build(500, msg, Json::Value(),
               drogon::HttpStatusCode::k500InternalServerError);
}

/**
 * @brief 构建响应
 *
 * @param code
 * @param msg
 * @param data
 * @param status_code
 * @return LOS_RES
 */
LOS_RES LosResponse::build(int code, const std::string &msg,
                           const Json::Value &data,
                           drogon::HttpStatusCode status_code) {
  Json::Value root;
  root["code"] = code;
  root["message"] = msg;
  root["data"] = data;
  auto res = drogon::HttpResponse::newHttpJsonResponse(root);
  res->setStatusCode(status_code);
  return res;
}
} // namespace LosCore
