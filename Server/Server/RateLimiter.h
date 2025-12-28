#pragma once
#include <string>
#include <unordered_map>
#include <chrono>
#include <mutex>

namespace http
{
    struct RateLimitEntry
    {
        int tokens;
        std::chrono::steady_clock::time_point lastRefill;
    };

    class RateLimiter
    {
    private:
        std::unordered_map<std::string, RateLimitEntry> m_limits;
        std::mutex m_mutex;
        const int m_maxTokens;
        const int m_refillRate;
        const std::chrono::seconds m_refillInterval;

    public:
        RateLimiter(int maxTokens = 5, int refillRate = 5, int refillIntervalSeconds = 60)
            : m_maxTokens(maxTokens)
            , m_refillRate(refillRate)
            , m_refillInterval(refillIntervalSeconds)
        {
        }

        bool AllowRequest(const std::string& identifier)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto now = std::chrono::steady_clock::now();

            auto it = m_limits.find(identifier);
            if (it == m_limits.end())
            {
                m_limits[identifier] = { m_maxTokens - 1, now };
                return true;
            }

            auto& entry = it->second;
            auto timeSinceRefill = std::chrono::duration_cast<std::chrono::seconds>(now - entry.lastRefill);

            if (timeSinceRefill >= m_refillInterval)
            {
                entry.tokens = m_maxTokens;
                entry.lastRefill = now;
            }

            if (entry.tokens > 0)
            {
                entry.tokens--;
                return true;
            }

            return false;
        }

        int GetRemainingTokens(const std::string& identifier)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            auto it = m_limits.find(identifier);
            if (it == m_limits.end())
            {
                return m_maxTokens;
            }
            return it->second.tokens;
        }
    };
}
