#pragma once
#include <crow.h>
#include <map>
#include <mutex>
#include <string>
#include <chrono>

namespace http
{
    struct ServerStats
    {
        int totalRequests = 0;
        int activeGames = 0;
        int registeredUsers = 0;
        int activeUsers = 0;
        std::string uptime;
        std::chrono::system_clock::time_point startTime;

        ServerStats()
        {
            startTime = std::chrono::system_clock::now();
        }

        void UpdateUptime()
        {
            auto now = std::chrono::system_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - startTime);
            
            int hours = duration.count() / 3600;
            int minutes = (duration.count() % 3600) / 60;
            int seconds = duration.count() % 60;
            
            uptime = std::to_string(hours) + "h " + 
                     std::to_string(minutes) + "m " + 
                     std::to_string(seconds) + "s";
        }
    };

    class StatsManager
    {
    private:
        ServerStats m_stats;
        std::mutex m_mutex;

    public:
        void IncrementRequests()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_stats.totalRequests++;
        }

        void SetActiveGames(int count)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_stats.activeGames = count;
        }

        void SetRegisteredUsers(int count)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_stats.registeredUsers = count;
        }

        void SetActiveUsers(int count)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_stats.activeUsers = count;
        }

        crow::response GetStats()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_stats.UpdateUptime();

            crow::json::wvalue response;
            response["total_requests"] = m_stats.totalRequests;
            response["active_games"] = m_stats.activeGames;
            response["registered_users"] = m_stats.registeredUsers;
            response["active_users"] = m_stats.activeUsers;
            response["uptime"] = m_stats.uptime;
            response["status"] = "running";

            return crow::response(200, response);
        }
    };
}
