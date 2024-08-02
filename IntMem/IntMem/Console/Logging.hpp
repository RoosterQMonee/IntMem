#pragma once

#include <windows.h>
#include <filesystem>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <thread>
#include <mutex>


namespace fs = std::filesystem;
enum LogType { Info, Warning, Error };


class Logger {
private:
    static std::ostream& GetOutputStream(LogType type) {
        switch (type) {
            case LogType::Info:
                return std::cout;
            case LogType::Warning:
                return std::cerr;
            default:
                return std::cout;
        }
    }

    static std::unique_ptr<std::ofstream> consoleStream;
    static std::vector<std::pair<std::string, int>> logList;
    static std::mutex mtx; // Mutex for thread safety

    Logger() = delete; // Prevent instantiation
    ~Logger() = delete; // Prevent destruction

public:
    static void Initialize(const fs::path& logFilePath);
    static void Shutdown();

    template<LogType lt, typename... Args>
    static void Log(Args&&... args) {
        std::ostringstream oss;

        switch (lt) {
            case LogType::Info:
                oss << "[INFO] ";
                break;
            case LogType::Warning:
                oss << "[WARNING] ";
                break;
            case LogType::Error:
                oss << "[ERROR] ";
                break;
        }

        ((oss << std::forward<decltype(args)>(args)), ...);
        oss << std::endl;

        std::lock_guard<std::mutex> lock(mtx);
        logList.emplace_back(std::move(oss.str()), __LINE__);
        GetOutputStream(lt) << oss.str();
    }

    static const std::vector<std::pair<std::string, int>>& GetLogs() {
        std::lock_guard<std::mutex> lock(mtx);
        return logList;
    }
};