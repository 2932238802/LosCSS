#pragma once

#include "common/constants.h"
#include <drogon/HttpResponse.h>
#include <drogon/HttpTypes.h>
#include <json/value.h>

namespace LosDto {
using LosCommon::LOS_CONST_STR;

class LosResponse {

public:
  LOS_STATIC drogon::HttpResponsePtr ok(const Json::Value &data); // 成功 并有数据
  LOS_STATIC drogon::HttpResponsePtr ok();                        // 成功没有数据
  LOS_STATIC drogon::HttpResponsePtr
  fail(int code, LOS_CONST_STR msg,
       drogon::HttpStatusCode state = drogon::HttpStatusCode::k400BadRequest); // 失败的时候
  LOS_STATIC drogon::HttpResponsePtr badRequest(LOS_CONST_STR msg);            // 400
  LOS_STATIC drogon::HttpResponsePtr notFound(LOS_CONST_STR msg);              // 400
  LOS_STATIC drogon::HttpResponsePtr serverError(LOS_CONST_STR msg);           // 500

private:
  LOS_STATIC drogon::HttpResponsePtr build(int code, LOS_CONST_STR &msg, const Json::Value &data,
                                           drogon::HttpStatusCode status_code);
};

} // namespace LosDto