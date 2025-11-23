#include "NetworkClient.h"
#include <iostream>

NetworkClient::NetworkClient(const std::string& baseUrl) : m_baseUrl(baseUrl) {}

cpr::Response NetworkClient::Get(const std::string& endpoint) {
    std::string url = m_baseUrl + endpoint;
    return cpr::Get(cpr::Url{url});
}

cpr::Response NetworkClient::Post(const std::string& endpoint, const json& payload) {
    std::string url = m_baseUrl + endpoint;
    return cpr::Post(cpr::Url{url},
                     cpr::Body{payload.dump()},
                     cpr::Header{{"Content-Type", "application/json"}});
}
