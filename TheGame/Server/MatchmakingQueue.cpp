#include "MatchmakingQueue.h"
#include <algorithm>

MatchmakingQueue::MatchmakingQueue() {}

void MatchmakingQueue::addPlayer(int userId, const std::string& username, double performanceScore) {
    std::lock_guard<std::mutex> lock(m_mutex);

    // Check if player already in queue
    auto it = std::find_if(m_queue.begin(), m_queue.end(),
        [userId](const QueuedPlayer& p) { return p.userId == userId; });

    if (it != m_queue.end()) {
        return; // Player already queued
    }

    QueuedPlayer player;
    player.userId = userId;
    player.username = username;
    player.performanceScore = performanceScore;
    player.joinTime = std::chrono::steady_clock::now();

    m_queue.push_back(player);
}

void MatchmakingQueue::removePlayer(int userId) {
    std::lock_guard<std::mutex> lock(m_mutex);

    m_queue.erase(
        std::remove_if(m_queue.begin(), m_queue.end(),
            [userId](const QueuedPlayer& p) { return p.userId == userId; }),
        m_queue.end()
    );
}

std::vector<QueuedPlayer> MatchmakingQueue::tryMatch() {
    std::lock_guard<std::mutex> lock(m_mutex);

    std::vector<QueuedPlayer> match;

    if (m_queue.size() < MIN_PLAYERS) {
        return match; // Not enough players
    }

    auto now = std::chrono::steady_clock::now();

    // Check if any player has waited too long (30 seconds)
    bool hasWaitedTooLong = false;
    for (const auto& player : m_queue) {
        auto waitTime = std::chrono::duration_cast<std::chrono::seconds>(now - player.joinTime).count();
        if (waitTime >= MAX_WAIT_SECONDS) {
            hasWaitedTooLong = true;
            break;
        }
    }

    // If someone waited too long, match with whoever is available (min 2 players)
    if (hasWaitedTooLong) {
        size_t numPlayers = std::min(m_queue.size(), static_cast<size_t>(MAX_PLAYERS));
        match.assign(m_queue.begin(), m_queue.begin() + numPlayers);

        // Remove matched players from queue
        m_queue.erase(m_queue.begin(), m_queue.begin() + numPlayers);
        return match;
    }

    // Normal matching: wait for at least 2 players and try to match by score
    if (m_queue.size() >= MIN_PLAYERS) {
        // Sort by performance score
        std::vector<QueuedPlayer> sorted = m_queue;
        std::sort(sorted.begin(), sorted.end(),
            [](const QueuedPlayer& a, const QueuedPlayer& b) {
                return a.performanceScore > b.performanceScore;
            });

        // Take up to MAX_PLAYERS with similar scores
        size_t numPlayers = std::min(sorted.size(), static_cast<size_t>(MAX_PLAYERS));

        // Check if we have enough players with similar scores (within 1.0 score difference)
        std::vector<QueuedPlayer> similarPlayers;
        similarPlayers.push_back(sorted[0]);

        for (size_t i = 1; i < numPlayers; ++i) {
            if (std::abs(sorted[i].performanceScore - sorted[0].performanceScore) <= 1.5) {
                similarPlayers.push_back(sorted[i]);
            }
        }

        // If we have at least MIN_PLAYERS with similar scores, match them
        if (similarPlayers.size() >= MIN_PLAYERS) {
            match = similarPlayers;

            // Remove matched players from queue
            for (const auto& matched : match) {
                m_queue.erase(
                    std::remove_if(m_queue.begin(), m_queue.end(),
                        [&matched](const QueuedPlayer& p) { return p.userId == matched.userId; }),
                    m_queue.end()
                );
            }
        }
    }

    return match;
}

bool MatchmakingQueue::isPlayerInQueue(int userId) const {
    std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(m_mutex));

    return std::find_if(m_queue.begin(), m_queue.end(),
        [userId](const QueuedPlayer& p) { return p.userId == userId; }) != m_queue.end();
}

size_t MatchmakingQueue::getQueueSize() const {
    std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(m_mutex));
    return m_queue.size();
}
