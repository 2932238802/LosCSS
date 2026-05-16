#pragma once

#include "common/constants.h"
#include "models/FarmingOperations.h"

#include <drogon/HttpController.h>
#include <drogon/HttpRequest.h>
#include <drogon/orm/CoroMapper.h>
#include <json/value.h>

#include <vector>

namespace LosController {

using FarmingOperationModel = drogon_model::los_css_drogon::FarmingOperations;
using FarmingOperationMapper = drogon::orm::CoroMapper<FarmingOperationModel>;

class FarmingOperationController : public drogon::HttpController<FarmingOperationController> {
public:
  METHOD_LIST_BEGIN
  ADD_METHOD_TO(FarmingOperationController::list,   LosCommon::LOS_API_STR::FARMING_OPERATIONS,        drogon::Get);
  ADD_METHOD_TO(FarmingOperationController::detail, LosCommon::LOS_API_STR::FARMING_OPERATIONS_DETAIL, drogon::Get);
  ADD_METHOD_TO(FarmingOperationController::create, LosCommon::LOS_API_STR::FARMING_OPERATIONS,        drogon::Post);
  ADD_METHOD_TO(FarmingOperationController::update, LosCommon::LOS_API_STR::FARMING_OPERATIONS_DETAIL, drogon::Put);
  ADD_METHOD_TO(FarmingOperationController::del,    LosCommon::LOS_API_STR::FARMING_OPERATIONS_DETAIL, drogon::Delete);
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
  Json::Value onFindAllSuc(const std::vector<FarmingOperationModel> &data,
                           int page, int pageSize, int64_t total);
};

} // namespace LosController
