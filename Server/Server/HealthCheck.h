#pragma once
#include <crow.h>
#include <chrono>
#include <string>
#include <sstream>
#include <iomanip>

namespace http
{
    class HealthCheck
    {
    private:
        std::chrono::system_clock::time_point m_startTime;
        std::string m_version;

    public:
        HealthCheck(const std::string& version = "1.0.0")
            : m_version(version)
        {
            m_startTime = std::chrono::system_clock::now();
        }

        crow::response GetHealth()
        {
            auto now = std::chrono::system_clock::now();
            auto uptime = std::chrono::duration_cast<std::chrono::seconds>(now - m_startTime);

            int hours = uptime.count() / 3600;
            int minutes = (uptime.count() % 3600) / 60;
            int seconds = uptime.count() % 60;

            std::string uptimeStr = std::to_string(hours) + "h " +
                                   std::to_string(minutes) + "m " +
                                   std::to_string(seconds) + "s";

            auto timeT = std::chrono::system_clock::to_time_t(now);
            std::tm tm;
            localtime_s(&tm, &timeT);
            std::stringstream ss;
            ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");

            crow::json::wvalue response;
            response["status"] = "healthy";
            response["version"] = m_version;
            response["uptime"] = uptimeStr;
            response["timestamp"] = ss.str();
            response["server"] = "CROW HTTP Server";

            return crow::response(200, response);
        }
    };
}
