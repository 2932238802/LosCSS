#pragma once

#include "common/constants.h"
#include <drogon/HttpResponse.h>
#include <drogon/HttpTypes.h>
#include <json/value.h>
namespace LosCore {
using LosCommon::LOS_CONST_STR;
using LosCommon::LOS_RES;
using LosCommon::LOS_STATE;

class LosResponse {

public:
  LOS_STATIC LOS_RES ok(const Json::Value &data); // 成功 并有数据
  LOS_STATIC LOS_RES ok();                        // 成功没有数据
  LOS_STATIC LOS_RES
  fail(int code, LOS_CONST_STR msg,
       LOS_STATE state = drogon::HttpStatusCode::k400BadRequest); // 失败的时候
  LOS_STATIC LOS_RES badRequest(LOS_CONST_STR msg);               // 400
  LOS_STATIC LOS_RES notFound(LOS_CONST_STR msg);                 // 400
  LOS_STATIC LOS_RES serverError(LOS_CONST_STR msg);              // 500

private:
  LOS_STATIC LOS_RES build(int code, const std::string &msg,
                           const Json::Value &data,
                           drogon::HttpStatusCode status_code);
};

} // namespace LosCore