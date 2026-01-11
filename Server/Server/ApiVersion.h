#pragma once
#include <crow.h>
#include <string>

namespace http
{
    class ApiVersion
    {
    private:
        std::string m_version;
        std::string m_apiName;

    public:
        ApiVersion(const std::string& version = "1.0.0", const std::string& apiName = "FutureCoders Game API")
            : m_version(version), m_apiName(apiName)
        {
        }

        crow::response GetVersion()
        {
            crow::json::wvalue response;
            response["api_name"] = m_apiName;
            response["version"] = m_version;
            response["description"] = "Modern C++ Game Server with Crow Framework";
            response["endpoints"] = crow::json::wvalue::list({
                "/api/version",
                "/health",
                "/stats",
                "/auth/register",
                "/auth/login",
                "/auth/logout",
                "/game/create",
                "/games",
                "/chat"
            });

            return crow::response(200, response);
        }
    };
}
