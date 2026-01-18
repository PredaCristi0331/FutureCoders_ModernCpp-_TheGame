#pragma once
#include "Export.h"
#include <string>
#include <fstream>
#include <mutex>
#include <memory>
#include <chrono>
#include <format>

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error
};

class THEGAME_API Logger {
private:
    static std::unique_ptr<Logger> instance_;
    static std::mutex mutex_;

    std::ofstream logFile_;
    LogLevel minLevel_;

    Logger();

    [[nodiscard]] std::string getCurrentTime() const;
    [[nodiscard]] std::string levelToString(LogLevel level) const;

public:
    ~Logger();

    
    static Logger& getInstance();

    
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) = delete;
    Logger& operator=(Logger&&) = delete;

    void setMinLevel(LogLevel level) { minLevel_ = level; }

   
    template<typename... Args>
    void log(LogLevel level, std::string_view format, Args&&... args) {
        if (level < minLevel_) return;

        std::lock_guard<std::mutex> lock(mutex_);

        try {
            std::string message = std::vformat(format, std::make_format_args(args...));
            std::string logEntry = std::format("[{}] [{}] {}\n",
                getCurrentTime(),
                levelToString(level),
                message);

            logFile_ << logEntry;
            logFile_.flush();
        }
        catch (...) {
            
        }
    }

    
    template<typename... Args>
    void debug(std::string_view format, Args&&... args) {
        log(LogLevel::Debug, format, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void info(std::string_view format, Args&&...  args) {
        log(LogLevel::Info, format, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void warning(std::string_view format, Args&&... args) {
        log(LogLevel::Warning, format, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void error(std::string_view format, Args&&...  args) {
        log(LogLevel::Error, format, std::forward<Args>(args)...);
    }
};
