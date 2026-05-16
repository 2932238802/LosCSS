#pragma once
#include "common/constants.h"
#include "common/utils/LosJson.h"
#include "models/SeedSources.h"

#include <drogon/HttpController.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <drogon/HttpTypes.h>
#include <drogon/orm/CoroMapper.h>
#include <drogon/orm/Exception.h>
#include <drogon/utils/coroutine.h>
#include <json/value.h>
#include <vector>

namespace LosController {
using SeedSourceModel = drogon_model::los_css_drogon::SeedSources;
using SeedSourceMapper = drogon::orm::CoroMapper<SeedSourceModel>;

class SeedSourceController : public drogon::HttpController<SeedSourceController> {
public:
  METHOD_LIST_BEGIN
  ADD_METHOD_TO(SeedSourceController::list, LosCommon::LOS_API_STR::SEED_SOURCES, drogon::Get);
  ADD_METHOD_TO(SeedSourceController::detail, LosCommon::LOS_API_STR::SEED_SOURCES_DETAIL,
                drogon::Get);
  ADD_METHOD_TO(SeedSourceController::create, LosCommon::LOS_API_STR::SEED_SOURCES, drogon::Post);
  ADD_METHOD_TO(SeedSourceController::update, LosCommon::LOS_API_STR::SEED_SOURCES_DETAIL,
                drogon::Put);
  ADD_METHOD_TO(SeedSourceController::del, LosCommon::LOS_API_STR::SEED_SOURCES_DETAIL,
                drogon::Delete);
  METHOD_LIST_END


public:
  drogon::Task<> list(drogon::HttpRequestPtr req,
                      std::function<void(const drogon::HttpResponsePtr &)> callback);
  drogon::Task<> detail(drogon::HttpRequestPtr req,
                        std::function<void(const drogon::HttpResponsePtr &)> callback, int32_t id);
  drogon::Task<> create(drogon::HttpRequestPtr req,
                        std::function<void(const drogon::HttpResponsePtr &)> callback);
  drogon::Task<> update(drogon::HttpRequestPtr req,
                        std::function<void(const drogon::HttpResponsePtr &)> callback, int32_t id);
  drogon::Task<> del(drogon::HttpRequestPtr req,
                     std::function<void(const drogon::HttpResponsePtr &)> callback, int32_t id);

private: // tool
  Json::Value onFindAllSuc(const std::vector<SeedSourceModel> &, int page, int pageSize, int total);
};

}; // namespace LosController