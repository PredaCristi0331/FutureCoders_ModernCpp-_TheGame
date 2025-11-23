#pragma once
#include <string>
#include <vector>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class NetworkClient {
public:
    NetworkClient(const std::string& baseUrl);

    // Generic HTTP methods
    cpr::Response Get(const std::string& endpoint);
    cpr::Response Post(const std::string& endpoint, const json& payload);

private:
    std::string m_baseUrl;
};
