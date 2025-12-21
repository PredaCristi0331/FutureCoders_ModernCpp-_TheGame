#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <mutex>

namespace http
{
    class Logger
    {
    private:
        static std::mutex m_mutex;
        
        static std::string GetCurrentTime()
        {
            auto now = std::chrono::system_clock::now();
            auto time = std::chrono::system_clock::to_time_t(now);
            std::tm tm;
            localtime_s(&tm, &time);
            
            std::stringstream ss;
            ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
            return ss.str();
        }

    public:
        enum class Level
        {
            INFO,
            WARNING,
            ERR,
            SUCCESS
        };

        static void Log(Level level, const std::string& message)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            
            std::string levelStr;
            switch (level)
            {
            case Level::INFO:    levelStr = "[INFO]    "; break;
            case Level::WARNING: levelStr = "[WARNING] "; break;
            case Level::ERR:     levelStr = "[ERROR]   "; break;
            case Level::SUCCESS: levelStr = "[SUCCESS] "; break;
            }

            std::cout << "[" << GetCurrentTime() << "] " << levelStr << message << std::endl;
        }

        static void LogRequest(const std::string& method, const std::string& path)
        {
            Log(Level::INFO, "HTTP " + method + " " + path);
        }

        static void LogResponse(int statusCode, const std::string& path)
        {
            Level level = (statusCode >= 200 && statusCode < 300) ? Level::SUCCESS : Level::WARNING;
            Log(level, "Response " + std::to_string(statusCode) + " for " + path);
        }
    };

    inline std::mutex Logger::m_mutex;
}
