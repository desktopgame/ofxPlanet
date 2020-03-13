#include "CSVParser.hpp"

#include <cctype>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace ofxPlanet {
CSVParser::CSVParser() : tables() {}
void CSVParser::parse(const std::string& source) {
        int start = 0;
        while (true) {
                CSVTable table;
                int read = parse(start, source, table);
                if (start >= static_cast<int>(source.size())) {
                        break;
                }
                start += read;
                this->tables.emplace_back(table);
        }
}
CSVTable& CSVParser::getTableAt(int index) { return tables.at(index); }
const CSVTable& CSVParser::getTableAt(int index) const { return tables.at(index); }
int CSVParser::getTableCount() const { return static_cast<int>(tables.size()); }

int CSVParser::parse(int start, const std::string& source, CSVTable& table) {
        int len = static_cast<int>(source.size());
        int read = 0;
        int columns = 0;
        CSVLine line;
        std::stringstream sbuf;
        for (int i = start; i < len; i++) {
                char c = source.at(i);
                read++;
                if (c == separator) {
                        line.emplace_back(sbuf.str());
                        sbuf.str("");
                        sbuf.clear(std::stringstream::goodbit);
                        columns++;
                } else if (c == newline) {
                        if (i == 0) {
                                continue;
                        }
                        auto str = sbuf.str();
                        if (isNullOrEmpty(str) && columns == 0) {
                                return read;
                        } else {
                                line.emplace_back(str);
                                sbuf.str("");
                                sbuf.clear(std::stringstream::goodbit);
                                table.emplace_back(line);
                                line.clear();
                        }
                        columns = 0;
                } else {
                        sbuf << c;
                }
        }
        return read;
}
bool CSVParser::isNullOrEmpty(const std::string& str) {
        if (str.empty()) {
                return true;
        }
        for (auto c : str) {
                if (!std::isspace(c)) {
                        return false;
                }
        }
        return true;
}
}  // namespace ofxPlanet