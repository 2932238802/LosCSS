#include <drogon/HttpAppFramework.h>
#include <drogon/drogon.h>
#include <iostream>
#include <trantor/utils/Logger.h>

int main() {
  drogon::app().setLogLevel(trantor::Logger::kDebug);
  drogon::app().loadConfigFile("./config.json");
  drogon::app().run();
  return 0;
}