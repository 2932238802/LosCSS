
#pragma once
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <functional>
#include <string>

namespace LosCommon {

/**
 * @brief Api str
 * HEALTH 服务器 健康检查
 */
namespace LOS_API_STR {
inline constexpr const char *HEALTH = "/api/health";

inline constexpr const char *SEED_SOURCES = "/api/seed-sources";
inline constexpr const char *SEED_SOURCES_DETAIL = "/api/seed-sources/{1}";

inline constexpr const char *SEEDLINGS = "/api/seedlings";
inline constexpr const char *SEEDLINGS_DETAIL = "/api/seedlings/{1}";

inline constexpr const char *PLANTING_AREAS = "/api/planting-areas";
inline constexpr const char *PLANTING_AREAS_DETAIL = "/api/planting-areas/{1}";

inline constexpr const char *FARMING_OPERATIONS = "/api/farming-operations";
inline constexpr const char *FARMING_OPERATIONS_DETAIL = "/api/farming-operations/{1}";

inline constexpr const char *HARVESTS = "/api/harvests";
inline constexpr const char *HARVESTS_DETAIL = "/api/harvests/{1}";

inline constexpr const char *RAW_MATERIAL_INS = "/api/raw-material-ins";
inline constexpr const char *RAW_MATERIAL_INS_DETAIL = "/api/raw-material-ins/{1}";

inline constexpr const char *PROCESSING_FLOWS = "/api/processing-flows";
inline constexpr const char *PROCESSING_FLOWS_DETAIL = "/api/processing-flows/{1}";

inline constexpr const char *QUALITY_CHECKS = "/api/quality-checks";
inline constexpr const char *QUALITY_CHECKS_DETAIL = "/api/quality-checks/{1}";

} // namespace LOS_API_STR


namespace LOS_INT {
inline constexpr int DEFAULT_PAGE = 1;
inline constexpr int DEFAULT_PAGE_SIZE = 20;
inline constexpr int MAX_PAGE_SIZE = 200;
} // namespace LOS_INT


/**
 * @brief type
 *
 */
#define LOS_STATIC static
using LOS_CONST_STR = const std::string &;

} // namespace LosCommon