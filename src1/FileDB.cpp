

#include "FileDB.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iostream>

std::vector<std::string> FileDB::split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int FileDB::getMaxID(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return 0;
    int maxId = 0;
    std::string line;
    while (getline(file, line)) {
        auto data = split(line, '|');
        if (!data.empty()) {
            try {
                int id = std::stoi(data[0]);
                if (id > maxId) maxId = id;
            } catch (...) {}
        }
    }
    return maxId;
}

void FileDB::appendLine(const std::string& filename, const std::string& line) {
    std::ofstream file(filename, std::ios::app);
    if (!file.is_open()) return;
    file << line << "\n";
}

std::vector<std::string> FileDB::readAll(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);
    std::string line;
    if (!file.is_open()) return lines;
    while (getline(file, line)) lines.push_back(line);
    return lines;
}

void FileDB::writeAll(const std::string& filename, const std::vector<std::string>& lines) {
    std::ofstream file(filename, std::ios::trunc);
    if (!file.is_open()) return;
    for (const auto& line : lines) file << line << "\n";
}

void FileDB::deleteByID(const std::string& filename, int id) {
    auto lines = readAll(filename);
    lines.erase(std::remove_if(lines.begin(), lines.end(),
        [id](const std::string& line){
            auto parts = split(line, '|');
            if (parts.empty()) return false;
            try { return std::stoi(parts[0]) == id; } catch(...) { return false; }
        }), lines.end());
    writeAll(filename, lines);
}

void FileDB::updateByID(const std::string& filename, int id, const std::string& newLine) {
    auto lines = readAll(filename);
    for (auto &line : lines) {
        auto parts = split(line, '|');
        if (!parts.empty()) {
            try { if(std::stoi(parts[0]) == id) line = newLine; } catch(...) {}
        }
    }
    writeAll(filename, lines);
}

void FileDB::addLineAutoID(const std::string& filename, const std::string& dataWithoutID) {
    int newID = getMaxID(filename) + 1;
    appendLine(filename, std::to_string(newID) + "|" + dataWithoutID);
}
