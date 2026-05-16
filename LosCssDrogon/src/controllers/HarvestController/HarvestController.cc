#include "HarvestController.h"

#include "common/utils/LosGetPageParam.h"
#include "common/utils/LosJson.h"
#include "dto/LosResponse/LosResponse.h"

#include <drogon/drogon.h>

#include <utility>

namespace LosController {

drogon::Task<>
HarvestController::list(drogon::HttpRequestPtr req,
                        std::function<void(const drogon::HttpResponsePtr &)> callback) {
  auto [page, pageSize] = LosCommon::LosGetPageParam(req);
  auto db = drogon::app().getDbClient();

  try {
    HarvestMapper itemsMapper(db);
    HarvestMapper countMapper(db);

    auto items = co_await itemsMapper.paginate(page, pageSize).findAll();
    auto total = co_await countMapper.count();

    callback(LosDto::LosResponse::ok(
        onFindAllSuc(items, page, pageSize, static_cast<int64_t>(total))));
  } catch (const drogon::orm::DrogonDbException &e) {
    callback(LosDto::LosResponse::serverError(e.base().what()));
  }
  co_return;
}

drogon::Task<>
HarvestController::detail(drogon::HttpRequestPtr req,
                          std::function<void(const drogon::HttpResponsePtr &)> callback,
                          int32_t id) {
  auto db = drogon::app().getDbClient();
  HarvestMapper mapper(db);

  try {
    auto item = co_await mapper.findByPrimaryKey(id);
    callback(LosDto::LosResponse::ok(LosCommon::LosJson::KeysToCamel(item.toJson())));
  } catch (const drogon::orm::UnexpectedRows &) {
    callback(LosDto::LosResponse::notFound("记录不存在"));
  } catch (const drogon::orm::DrogonDbException &e) {
    callback(LosDto::LosResponse::serverError(e.base().what()));
  }
  co_return;
}

drogon::Task<>
HarvestController::create(drogon::HttpRequestPtr req,
                          std::function<void(const drogon::HttpResponsePtr &)> callback) {
  auto data = req->getJsonObject();
  if (!data) {
    callback(LosDto::LosResponse::badRequest("请求体不是合法 JSON"));
    co_return;
  }

  Json::Value bodySnake = LosCommon::LosJson::KeysToSnake(*data);

  std::string err;
  if (!HarvestModel::validateJsonForCreation(bodySnake, err)) {
    callback(LosDto::LosResponse::badRequest(err));
    co_return;
  }

  HarvestModel model(bodySnake);
  auto db = drogon::app().getDbClient();
  HarvestMapper mapper(db);

  try {
    auto created = co_await mapper.insert(model);
    callback(LosDto::LosResponse::ok(LosCommon::LosJson::KeysToCamel(created.toJson())));
  } catch (const drogon::orm::DrogonDbException &e) {
    callback(LosDto::LosResponse::serverError(e.base().what()));
  }
  co_return;
}

drogon::Task<>
HarvestController::update(drogon::HttpRequestPtr req,
                          std::function<void(const drogon::HttpResponsePtr &)> callback,
                          int32_t id) {
  auto data = req->getJsonObject();
  if (!data) {
    callback(LosDto::LosResponse::badRequest("请求体不是合法 JSON"));
    co_return;
  }

  Json::Value bodySnake = LosCommon::LosJson::KeysToSnake(*data);

  std::string err;
  if (!HarvestModel::validateJsonForUpdate(bodySnake, err)) {
    callback(LosDto::LosResponse::badRequest(err));
    co_return;
  }

  auto db = drogon::app().getDbClient();
  HarvestMapper mapper(db);

  try {
    auto existing = co_await mapper.findByPrimaryKey(id);
    existing.updateByJson(bodySnake);
    co_await mapper.update(existing);
    callback(LosDto::LosResponse::ok(LosCommon::LosJson::KeysToCamel(existing.toJson())));
  } catch (const drogon::orm::UnexpectedRows &) {
    callback(LosDto::LosResponse::notFound("记录不存在"));
  } catch (const drogon::orm::DrogonDbException &e) {
    callback(LosDto::LosResponse::serverError(e.base().what()));
  }
  co_return;
}

drogon::Task<>
HarvestController::del(drogon::HttpRequestPtr req,
                       std::function<void(const drogon::HttpResponsePtr &)> callback,
                       int32_t id) {
  auto db = drogon::app().getDbClient();
  HarvestMapper mapper(db);

  try {
    auto affected = co_await mapper.deleteByPrimaryKey(id);
    if (affected == 0) {
      callback(LosDto::LosResponse::notFound("记录不存在"));
      co_return;
    }
    Json::Value data;
    data["id"] = id;
    callback(LosDto::LosResponse::ok(LosCommon::LosJson::KeysToCamel(data)));
  } catch (const drogon::orm::DrogonDbException &e) {
    callback(LosDto::LosResponse::serverError(e.base().what()));
  }
  co_return;
}

Json::Value
HarvestController::onFindAllSuc(const std::vector<HarvestModel> &data,
                                int page, int pageSize, int64_t total) {
  Json::Value itemsArray = Json::arrayValue;
  for (const auto &item : data) {
    itemsArray.append(item.toJson());
  }
  Json::Value jsonValue;
  jsonValue["items"] = itemsArray;
  jsonValue["total"] = static_cast<Json::Int64>(total);
  jsonValue["page"] = page;
  jsonValue["pageSize"] = pageSize;
  return LosCommon::LosJson::KeysToCamel(jsonValue);
}

} // namespace LosController
