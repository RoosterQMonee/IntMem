#include "Logging.hpp"


std::unique_ptr<std::ofstream> Logger::consoleStream = nullptr;
std::vector<std::pair<std::string, int>> Logger::logList;
std::mutex Logger::mtx;


void Logger::Initialize(const fs::path& logFilePath) {
    FILE* dummy;

    AllocConsole();
    freopen_s(&dummy, "CONOUT$", "w", stdout);
    freopen_s(&dummy, "CONOUT$", "w", stderr);

    consoleStream.reset(new std::ofstream(logFilePath, std::ios_base::app));
    if (!consoleStream->good())
        std::cerr << "Failed to open log file." << std::endl;
}

void Logger::Shutdown() {
    consoleStream.reset();
    FreeConsole();
}