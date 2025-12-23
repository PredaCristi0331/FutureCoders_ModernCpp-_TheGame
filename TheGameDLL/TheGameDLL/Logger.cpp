#include "Logger.h"
#include <iostream>

std::unique_ptr<Logger> instance_ = nullptr;
std::mutex mutex_;

Logger::Logger() : minLevel_(LogLevel::Debug) {
    logFile_.open("thegame.log", std::ios::app);
    if (!logFile_.is_open()) {
        std::cerr << "Failed to open log file\n";
    }
}

Logger:: ~Logger() {
    if (logFile_.is_open()) {
        logFile_.close();
    }
}

Logger& Logger::getInstance() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (!instance_) {
        instance_ = std::unique_ptr<Logger>(new Logger());
    }
    return *instance_;
}

std::string Logger::getCurrentTime() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);

    char buffer[100];
    std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&time));
    return buffer;
}

std::string Logger::levelToString(LogLevel level) const {
    switch (level) {
    case LogLevel::Debug:  return "DEBUG";
    case LogLevel::Info: return "INFO";
    case LogLevel::Warning:  return "WARNING";
    case LogLevel::Error: return "ERROR";
    default: return "UNKNOWN";
    }
}