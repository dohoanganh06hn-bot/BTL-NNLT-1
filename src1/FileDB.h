
#ifndef FILE_DB_H
#define FILE_DB_H

#include <string>
#include <vector>

class FileDB {
public:
    static std::vector<std::string> split(const std::string& s, char delimiter);
    static int getMaxID(const std::string& filename);
    static void appendLine(const std::string& filename, const std::string& line);
    static std::vector<std::string> readAll(const std::string& filename);
    static void writeAll(const std::string& filename, const std::vector<std::string>& lines);
    static void deleteByID(const std::string& filename, int id);
    static void updateByID(const std::string& filename, int id, const std::string& newLine);
    static void addLineAutoID(const std::string& filename, const std::string& dataWithoutID);
};

#endif