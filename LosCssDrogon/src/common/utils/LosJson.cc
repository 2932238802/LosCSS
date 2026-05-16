#include "LosJson.h"
#include "common/constants.h"
#include <cctype>
#include <json/value.h>

namespace LosCommon {
/**
 * @brief ToSnake
 *
 * @param camel_str
 * @return std::string
 */
std::string LosJson::ToSnake(LOS_CONST_STR camel_str) {
  std::string res;
  res.reserve(camel_str.size() + 4);
  for (auto a : camel_str) {
    if (std::isupper(static_cast<unsigned char>(a))) {
      if (!res.empty()) {
        res.push_back('_');
      }
      res.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(a))));
    } else {
      res.push_back(a);
    }
  }
  return res;
}



/**
 * @brief

 *
 * @param snake_str
 * @return std::string
 */
std::string LosJson::ToCamel(LOS_CONST_STR snake_str) {
  std::string str;
  str.reserve(snake_str.size());
  bool isFlag = false;
  for (auto c : snake_str) {
    if (c == '_') {
      isFlag = true;
      continue;
    }
    if (isFlag) {
      str.push_back(std::toupper(static_cast<unsigned char>(c)));
      isFlag = false;
    } else {
      str.push_back(c);
    }
  }
  return str;
}



/**
 * @brief KeysToCamel
 *
 * @param value
 * @return Json::Value
 */
Json::Value LosJson::KeysToCamel(const Json::Value &value) {

  // 如果是 数组 直接转换
  if (value.isArray()) {
    Json::Value arr(Json::arrayValue);
    for (const auto &a : value) {
      arr.append(KeysToCamel(a));
    }
    return arr;
  }
  if (value.isObject()) {
    Json::Value obj(Json::objectValue);
    for (const auto &key : value.getMemberNames()) {
      obj[ToCamel(key)] = KeysToCamel(value[key]);
    }
    return obj;
  }
  return value;
}



/**
 * @brief
 *
 * @param value
 * @return Json::Value
 */
Json::Value LosJson::KeysToSnake(const Json::Value &input) {
  if (input.isArray()) {
    Json::Value arr(Json::arrayValue);
    for (const auto &item : input) {
      arr.append(KeysToSnake(item));
    }
    return arr;
  }
  if (input.isObject()) {
    Json::Value obj(Json::objectValue);
    for (const auto &key : input.getMemberNames()) {
      obj[ToSnake(key)] = KeysToSnake(input[key]);
    }
    return obj;
  }
  return input;
}
} // namespace LosCommon