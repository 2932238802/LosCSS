#pragma once

#include "common/constants.h"
#include "models/Harvests.h"

#include <drogon/HttpController.h>
#include <drogon/HttpRequest.h>
#include <drogon/orm/CoroMapper.h>
#include <json/value.h>

#include <vector>

namespace LosController {

using HarvestModel = drogon_model::los_css_drogon::Harvests;
using HarvestMapper = drogon::orm::CoroMapper<HarvestModel>;

class HarvestController : public drogon::HttpController<HarvestController> {
public:
  METHOD_LIST_BEGIN
  ADD_METHOD_TO(HarvestController::list,   LosCommon::LOS_API_STR::HARVESTS,        drogon::Get);
  ADD_METHOD_TO(HarvestController::detail, LosCommon::LOS_API_STR::HARVESTS_DETAIL, drogon::Get);
  ADD_METHOD_TO(HarvestController::create, LosCommon::LOS_API_STR::HARVESTS,        drogon::Post);
  ADD_METHOD_TO(HarvestController::update, LosCommon::LOS_API_STR::HARVESTS_DETAIL, drogon::Put);
  ADD_METHOD_TO(HarvestController::del,    LosCommon::LOS_API_STR::HARVESTS_DETAIL, drogon::Delete);
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
  Json::Value onFindAllSuc(const std::vector<HarvestModel> &data,
                           int page, int pageSize, int64_t total);
};

} // namespace LosController
