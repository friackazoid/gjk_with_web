#include <seasocks/PrintfLogger.h>
#include <seasocks/Server.h>
#include <seasocks/util/RootPageHandler.h>

#include "handlers.hpp"

int main(int /*argc*/, char** /*argv*/) {
    auto logger = std::make_shared<seasocks::PrintfLogger>(
        seasocks::Logger::Level::Debug);
    seasocks::Server server(logger);

    server.addWebSocketHandler("/position",
                               std::make_shared<PositionHandler>());

    server.serve("web", 9090);

    return 0;
}
