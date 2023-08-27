#pragma once

#include "gjk.hpp"

#include <jsoncpp/json/value.h>
#include <seasocks/Server.h>
#include <seasocks/StringUtil.h>
#include <seasocks/WebSocket.h>

#include <eigen3/Eigen/Core>

#include <json/json.h>

#include <vector>

class PositionHandler : public seasocks::WebSocket::Handler {
public:
    
    PositionHandler () = default;

    void onConnect(seasocks::WebSocket* connection) override {
        _connections.insert(connection);

        std::cout << "New connection:" << connection->getRequestUri() << ": "
                  << seasocks::formatAddress(connection->getRemoteAddress())
                  << std::endl;
    }

    void onData(seasocks::WebSocket* connection, const char* data) override {
        std::cout << "Received data from " << connection->getRequestUri()
                  << ": " << data << std::endl;

        Json::Reader reader;
        Json::Value root;
        reader.parse(data, root);

        std::cout << "rectangle 1 top " << root["rectangle1"]["top"] << " left " << root["rectangle1"]["left"]  << std::endl;

        std::vector<Eigen::Vector2d> rec1 {{root["rectangle1"]["top"].asDouble(), root["rectangle1"]["left"].asDouble()},
                                           {root["rectangle1"]["top"].asDouble(), root["rectangle1"]["left"].asDouble() + root["rectangle1"]["width"].asDouble()},
                                           {root["rectangle1"]["top"].asDouble() + root["rectangle1"]["height"].asDouble(), root["rectangle1"]["left"].asDouble() + root["rectangle1"]["width"].asDouble()},
                                           {root["rectangle1"]["top"].asDouble() + root["rectangle1"]["height"].asDouble(), root["rectangle1"]["left"].asDouble()}};

        std::vector<Eigen::Vector2d> rec2 {{root["rectangle2"]["top"].asDouble(), root["rectangle2"]["left"].asDouble()},
                                           {root["rectangle2"]["top"].asDouble(), root["rectangle2"]["left"].asDouble() + root["rectangle2"]["width"].asDouble()},
                                           {root["rectangle2"]["top"].asDouble() + root["rectangle2"]["height"].asDouble(), root["rectangle2"]["left"].asDouble() + root["rectangle2"]["width"].asDouble()},
                                           {root["rectangle2"]["top"].asDouble() + root["rectangle2"]["height"].asDouble(), root["rectangle2"]["left"].asDouble()}};

        double dist{0.0};
        auto is_collision = gjk(rec1, rec2, dist); 
        std::cout << "is_collision " << is_collision << " dist " << dist << std::endl;

        Json::Value result;
        result["is_collision"] = is_collision;
        result["dist"] = dist;
        connection->send(result.toStyledString());
    }

    void onDisconnect(seasocks::WebSocket* connection) override {
        _connections.erase(connection);

        std::cout << "Disconnected:" << connection->getRequestUri() << ": "
                  << seasocks::formatAddress(connection->getRemoteAddress())
                  << std::endl;
    }

private:

    std::set<seasocks::WebSocket*> _connections;
};


