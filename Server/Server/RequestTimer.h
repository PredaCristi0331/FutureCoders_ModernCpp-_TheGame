#pragma once
#include <chrono>
#include <string>
#include "Logger.h"

namespace http
{
    class RequestTimer
    {
    private:
        std::chrono::high_resolution_clock::time_point m_start;
        std::string m_endpoint;

    public:
        RequestTimer(const std::string& endpoint)
            : m_endpoint(endpoint)
        {
            m_start = std::chrono::high_resolution_clock::now();
        }

        ~RequestTimer()
        {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - m_start);
            
            double milliseconds = duration.count() / 1000.0;
            
            std::string message = "Request to " + m_endpoint + " processed in " + 
                                std::to_string(milliseconds) + " ms";
            
            Logger::Log(Logger::Level::INFO, message);
        }

        RequestTimer(const RequestTimer&) = delete;
        RequestTimer& operator=(const RequestTimer&) = delete;
    };
}
