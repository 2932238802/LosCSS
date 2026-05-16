#pragma once
#include "common/constants.h"
#include <json/value.h>
#include <string>
namespace LosCommon {

class LosJson {
public:
  LOS_STATIC std::string ToCamel(LOS_CONST_STR snake_str);
  LOS_STATIC std::string ToSnake(LOS_CONST_STR camel_str);
  LOS_STATIC Json::Value KeysToSnake(const Json::Value &value);
  LOS_STATIC Json::Value KeysToCamel(const Json::Value &value);
};
} // namespace LosCommon