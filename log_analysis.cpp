#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <ctime>
#include <iomanip>

// Structure to hold parsed log data
struct LogEntry {
    std::string fileType;
    std::string origin;
    std::string timestamp;
    std::string message;
};

// Function to parse raw log line
LogEntry parseLogLine(const std::string& line) {
    LogEntry entry;
    std::istringstream ss(line);
    std::getline(ss, entry.timestamp, ' ');
    std::getline(ss, entry.fileType, ' ');
    std::getline(ss, entry.origin, ' ');
    std::getline(ss, entry.message);

    return entry;
}

// Function to convert log data to CSV
void writeCSV(const std::vector<LogEntry>& logs, const std::string& fileName) {
    std::ofstream csvFile(fileName);
    csvFile << "Timestamp,File Type,Origin,Message\n";
    for (const auto& log : logs) {
        csvFile << log.timestamp << "," << log.fileType << "," << log.origin << "," << log.message << "\n";
    }
    csvFile.close();
}

// Function to manually convert log data to JSON
void writeJSON(const std::vector<LogEntry>& logs, const std::string& fileName) {
    std::ofstream jsonFile(fileName);
    jsonFile << "[\n";
    for (size_t i = 0; i < logs.size(); ++i) {
        const auto& log = logs[i];
        jsonFile << "  {\n";
        jsonFile << "    \"timestamp\": \"" << log.timestamp << "\",\n";
        jsonFile << "    \"fileType\": \"" << log.fileType << "\",\n";
        jsonFile << "    \"origin\": \"" << log.origin << "\",\n";
        jsonFile << "    \"message\": \"" << log.message << "\"\n";
        jsonFile << "  }";
        if (i != logs.size() - 1) {
            jsonFile << ",";
        }
        jsonFile << "\n";
    }
    jsonFile << "]\n";
    jsonFile.close();
}

// Function to segregate logs by criteria
std::map<std::string, std::vector<LogEntry>> segregateByCriteria(const std::vector<LogEntry>& logs, const std::string& criteria) {
    std::map<std::string, std::vector<LogEntry>> segregatedLogs;
    
    for (const auto& log : logs) {
        std::string key;
        if (criteria == "fileType") {
            key = log.fileType;
        } else if (criteria == "origin") {
            key = log.origin;
        } else if (criteria == "timestamp") {
            key = log.timestamp.substr(0, 10); // Example: use the first 10 characters for date
        }
        segregatedLogs[key].push_back(log);
    }
    
    return segregatedLogs;
}

int main() {
    std::string logFileName = "raw_logs.txt";
    std::ifstream logFile(logFileName);

    if (!logFile.is_open()) {
        std::cerr << "Failed to open log file." << std::endl;
        return 1;
    }

    std::vector<LogEntry> logEntries;
    std::string line;

    // Reading and parsing the log file line by line
    while (std::getline(logFile, line)) {
        logEntries.push_back(parseLogLine(line));
    }

    logFile.close();

    // Convert to CSV
    writeCSV(logEntries, "logs.csv");

    // Convert to JSON
    writeJSON(logEntries, "logs.json");

    // Example: Segregating by file type
    auto segregatedByFileType = segregateByCriteria(logEntries, "fileType");

    // Writing segregated logs by file type (optional)
    for (const auto& entry : segregatedByFileType) {
        const std::string& key = entry.first;
        const std::vector<LogEntry>& logs = entry.second;
        
        std::string csvFileName = "logs_by_fileType_" + key + ".csv";
        writeCSV(logs, csvFileName);
    }

    std::cout << "Log conversion and segregation complete." << std::endl;

    return 0;
}
