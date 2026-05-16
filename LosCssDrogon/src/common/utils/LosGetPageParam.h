#pragma once

#include "common/constants.h"
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <utility>

namespace LosCommon {

/**
 * @brief LosGetPageParam
 *
 * 从解析中 截取出 page 和 pageSize
 *
 * @param req
 * @return std::pair<int, int>
 */
inline std::pair<int, int> LosGetPageParam(const drogon::HttpRequestPtr &req) {
  const std::string &page = req->getParameter("page");
  const std::string &pageSize = req->getParameter("pageSize");

  int pageInt = LosCommon::LOS_INT::DEFAULT_PAGE;
  int pageSizeInt = LosCommon::LOS_INT::DEFAULT_PAGE_SIZE;

  try {
    if (!page.empty()) {
      pageInt = std::stoi(page);
    }
  } catch (...) {
    pageInt = LosCommon::LOS_INT::DEFAULT_PAGE;
  }

  try {
    if (!pageSize.empty()) {
      pageSizeInt = std::stoi(pageSize);
    }
  } catch (...) {
    pageSizeInt = LosCommon::LOS_INT::DEFAULT_PAGE_SIZE;
  }

  if (pageInt <= 0) {
    pageInt = LosCommon::LOS_INT::DEFAULT_PAGE;
  }
  if (pageSizeInt <= 0) {
    pageSizeInt = LosCommon::LOS_INT::DEFAULT_PAGE_SIZE;
  }
  if (pageSizeInt > LosCommon::LOS_INT::MAX_PAGE_SIZE) {
    pageSizeInt = LosCommon::LOS_INT::MAX_PAGE_SIZE;
  }

  return {pageInt, pageSizeInt};
}

} // namespace LosCommon
