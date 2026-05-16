#pragma once

#include "common/constants.h"
#include "models/QualityChecks.h"

#include <drogon/HttpController.h>
#include <drogon/HttpRequest.h>
#include <drogon/orm/CoroMapper.h>
#include <json/value.h>

#include <vector>

namespace LosController {

using QualityCheckModel = drogon_model::los_css_drogon::QualityChecks;
using QualityCheckMapper = drogon::orm::CoroMapper<QualityCheckModel>;

class QualityCheckController : public drogon::HttpController<QualityCheckController> {
public:
  METHOD_LIST_BEGIN
  ADD_METHOD_TO(QualityCheckController::list, LosCommon::LOS_API_STR::QUALITY_CHECKS, drogon::Get);
  ADD_METHOD_TO(QualityCheckController::detail, LosCommon::LOS_API_STR::QUALITY_CHECKS_DETAIL,
                drogon::Get);
  ADD_METHOD_TO(QualityCheckController::create, LosCommon::LOS_API_STR::QUALITY_CHECKS,
                drogon::Post);
  ADD_METHOD_TO(QualityCheckController::update, LosCommon::LOS_API_STR::QUALITY_CHECKS_DETAIL,
                drogon::Put);
  ADD_METHOD_TO(QualityCheckController::del, LosCommon::LOS_API_STR::QUALITY_CHECKS_DETAIL,
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
  Json::Value onFindAllSuc(const std::vector<QualityCheckModel> &data, int page, int pageSize,
                           int64_t total);
};

} // namespace LosController
