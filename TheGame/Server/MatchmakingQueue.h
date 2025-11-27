#pragma once

#include <vector>
#include <mutex>
#include <chrono>
#include <string>

struct QueuedPlayer {
    int userId;
    std::string username;
    double performanceScore;
    std::chrono::steady_clock::time_point joinTime;
};

class MatchmakingQueue {
public:
    MatchmakingQueue();

    // Add player to queue
    void addPlayer(int userId, const std::string& username, double performanceScore);

    // Remove player from queue
    void removePlayer(int userId);

    // Try to create a match (returns list of player IDs if match found, empty otherwise)
    std::vector<QueuedPlayer> tryMatch();

    // Check if player is in queue
    bool isPlayerInQueue(int userId) const;

    // Get queue size
    size_t getQueueSize() const;

private:
    std::vector<QueuedPlayer> m_queue;
    std::mutex m_mutex;

    // Max wait time before forcing a match (30 seconds)
    static constexpr int MAX_WAIT_SECONDS = 30;

    // Min/max players per game
    static constexpr int MIN_PLAYERS = 2;
    static constexpr int MAX_PLAYERS = 5;
};
