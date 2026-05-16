#pragma once

#include "common/constants.h"
#include "models/RawMaterialIns.h"

#include <drogon/HttpController.h>
#include <drogon/HttpRequest.h>
#include <drogon/orm/CoroMapper.h>
#include <json/value.h>

#include <vector>

namespace LosController {

using RawMaterialInModel = drogon_model::los_css_drogon::RawMaterialIns;
using RawMaterialInMapper = drogon::orm::CoroMapper<RawMaterialInModel>;

class RawMaterialInController : public drogon::HttpController<RawMaterialInController> {
public:
  METHOD_LIST_BEGIN
  ADD_METHOD_TO(RawMaterialInController::list, LosCommon::LOS_API_STR::RAW_MATERIAL_INS,
                drogon::Get);
  ADD_METHOD_TO(RawMaterialInController::detail, LosCommon::LOS_API_STR::RAW_MATERIAL_INS_DETAIL,
                drogon::Get);
  ADD_METHOD_TO(RawMaterialInController::create, LosCommon::LOS_API_STR::RAW_MATERIAL_INS,
                drogon::Post);
  ADD_METHOD_TO(RawMaterialInController::update, LosCommon::LOS_API_STR::RAW_MATERIAL_INS_DETAIL,
                drogon::Put);
  ADD_METHOD_TO(RawMaterialInController::del, LosCommon::LOS_API_STR::RAW_MATERIAL_INS_DETAIL,
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

private:
  Json::Value onFindAllSuc(const std::vector<RawMaterialInModel> &data, int page, int pageSize,
                           int64_t total);
};

} // namespace LosController
