#include "SeedSourceController.h"
#include "common/utils/LosGetPageParam.h"
#include "common/utils/LosJson.h"
#include "dto/LosResponse/LosResponse.h"

#include <cstdint>
#include <drogon/HttpAppFramework.h>
#include <drogon/orm/Exception.h>
#include <json/value.h>
#include <memory>
#include <vector>

namespace LosController {


/**
 * @brief list 返回列表
 *
 * @param req
 * @param callback
 */
drogon::Task<>
SeedSourceController::list(drogon::HttpRequestPtr req,
                           std::function<void(const drogon::HttpResponsePtr &)> callback) {
  auto [page, pageSize] = LosCommon::LosGetPageParam(req);
  auto db = drogon::app().getDbClient();

  try {
    SeedSourceMapper itemsMapper(db);
    SeedSourceMapper countMapper(db);

    // 同步式写法：等两个查询都完成
    auto items = co_await itemsMapper.paginate(page, pageSize).findAll();
    auto total = co_await countMapper.count();

    callback(
        LosDto::LosResponse::ok(onFindAllSuc(items, page, pageSize, static_cast<int64_t>(total))));
  } catch (const drogon::orm::DrogonDbException &e) {
    callback(LosDto::LosResponse::serverError(e.base().what()));
  }
  co_return;
}



/**
 * @brief detail
 *
 * @param req
 * @param callback
 * @param id
 * @return drogon::Task<>
 */
drogon::Task<>
SeedSourceController::detail(drogon::HttpRequestPtr req,
                             std::function<void(const drogon::HttpResponsePtr &)> callback,
                             int32_t id) {
  auto db = drogon::app().getDbClient();
  SeedSourceMapper mapper(db);
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



/**
 * @brief create
 *
 * @param req
 * @param callback
 * @return drogon::Task<>
 */
drogon::Task<>
SeedSourceController::create(drogon::HttpRequestPtr req,
                             std::function<void(const drogon::HttpResponsePtr &)> callback) {
  auto data = req->getJsonObject();
  if (!data) {
    callback(LosDto::LosResponse::badRequest("请求体不对"));
    co_return;
  }
  // 转成内部 snake
  Json::Value bodySnake = LosCommon::LosJson::KeysToSnake(*data);
  std::string err;
  if (!SeedSourceModel::validateJsonForCreation(bodySnake, err)) {
    callback(LosDto::LosResponse::badRequest(err));
    co_return;
  }
  SeedSourceModel model(bodySnake);
  auto db = drogon::app().getDbClient();
  SeedSourceMapper mapper(db);
  try {
    // created 是新创造出来的 SeedSources 对象
    auto created = co_await mapper.insert(model);
    callback(LosDto::LosResponse::ok(LosCommon::LosJson::KeysToCamel(created.toJson())));
  } catch (const drogon::orm::DrogonDbException &e) {
    callback(LosDto::LosResponse::serverError(e.base().what()));
    co_return;
  }
  co_return;
}



/**
 * @brief update
 *
 * @param req
 * @param callback
 * @param id
 * @return drogon::Task<>
 */
drogon::Task<>
SeedSourceController::update(drogon::HttpRequestPtr req,
                             std::function<void(const drogon::HttpResponsePtr &)> callback,
                             int32_t id) {
  auto data = req->getJsonObject();
  if (!data) {
    callback(LosDto::LosResponse::badRequest("请求错误!"));
    co_return;
  }
  Json::Value bodySnake = LosCommon::LosJson::KeysToSnake(*data);
  std::string err;
  if (!SeedSourceModel::validateJsonForUpdate(bodySnake, err)) {
    callback(LosDto::LosResponse::badRequest(err));
    co_return;
  }
  auto db = drogon::app().getDbClient();
  SeedSourceMapper mapper(db);
  try {
    auto existing = co_await mapper.findByPrimaryKey(id);
    existing.updateByJson(bodySnake);
    co_await mapper.update(existing);
    callback(LosDto::LosResponse::ok(LosCommon::LosJson::KeysToCamel(existing.toJson())));
    co_return;

  } catch (const drogon::orm::UnexpectedRows &) {
    callback(LosDto::LosResponse::notFound("记录不存在"));
    co_return;

  } catch (const drogon::orm::DrogonDbException &e) {
    callback(LosDto::LosResponse::serverError(e.base().what()));
    co_return;
  }
  co_return;
}



/**
 * @brief del
 *
 * @param req
 * @param callback
 * @param id
 * @return drogon::Task<>
 */
drogon::Task<>
SeedSourceController ::del(drogon::HttpRequestPtr req,
                           std::function<void(const drogon::HttpResponsePtr &)> callback,
                           int32_t id) {
  auto db = drogon::app().getDbClient();
  SeedSourceMapper mapper(db);
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



/**
 * @brief onFindAllSuc
 * 找到成功了的异步函数
 * @param data
 */
Json::Value SeedSourceController::onFindAllSuc(const std::vector<SeedSourceModel> &data, int page,
                                               int pageSize, int total) {
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