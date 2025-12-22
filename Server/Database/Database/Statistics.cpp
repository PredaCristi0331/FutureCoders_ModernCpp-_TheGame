#include "Statistics.h"
#include "DatabaseManager.h"

UserProfile Statistics::getUserProfile(int userId) {
    return DatabaseManager::getUserProfile(userId);
}
