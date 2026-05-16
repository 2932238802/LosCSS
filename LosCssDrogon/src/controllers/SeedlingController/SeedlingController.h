#pragma once

#include "common/constants.h"
#include "models/Seedlings.h"

#include <drogon/HttpController.h>
#include <drogon/HttpRequest.h>
#include <drogon/orm/CoroMapper.h>
#include <json/value.h>

#include <vector>

namespace LosController {

using SeedlingModel = drogon_model::los_css_drogon::Seedlings;
using SeedlingMapper = drogon::orm::CoroMapper<SeedlingModel>;

class SeedlingController : public drogon::HttpController<SeedlingController> {
public:
  METHOD_LIST_BEGIN
  ADD_METHOD_TO(SeedlingController::list,   LosCommon::LOS_API_STR::SEEDLINGS,        drogon::Get);
  ADD_METHOD_TO(SeedlingController::detail, LosCommon::LOS_API_STR::SEEDLINGS_DETAIL, drogon::Get);
  ADD_METHOD_TO(SeedlingController::create, LosCommon::LOS_API_STR::SEEDLINGS,        drogon::Post);
  ADD_METHOD_TO(SeedlingController::update, LosCommon::LOS_API_STR::SEEDLINGS_DETAIL, drogon::Put);
  ADD_METHOD_TO(SeedlingController::del,    LosCommon::LOS_API_STR::SEEDLINGS_DETAIL, drogon::Delete);
  METHOD_LIST_END

public:
  drogon::Task<> list(drogon::HttpRequestPtr req,
                      std::function<void(const drogon::HttpResponsePtr &)> callback);

  drogon::Task<> detail(drogon::HttpRequestPtr req,
                        std::function<void(const drogon::HttpResponsePtr &)> callback,
                        int32_t id);

  drogon::Task<> create(drogon::HttpRequestPtr req,
                        std::function<void(const drogon::HttpResponsePtr &)> callback);

  drogon::Task<> update(drogon::HttpRequestPtr req,
                        std::function<void(const drogon::HttpResponsePtr &)> callback,
                        int32_t id);

  drogon::Task<> del(drogon::HttpRequestPtr req,
                     std::function<void(const drogon::HttpResponsePtr &)> callback,
                     int32_t id);

private:
  Json::Value onFindAllSuc(const std::vector<SeedlingModel> &data,
                           int page, int pageSize, int64_t total);
};

} // namespace LosController
