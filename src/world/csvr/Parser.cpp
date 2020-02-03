#include "parser.hpp"

#include <cctype>
#include <iostream>
#include <sstream>
#include <stdexcept>

namespace csvr {
Parser::Parser() : tables() {}
void Parser::parse(const std::string& source) {
        int start = 0;
        while (true) {
                Table table;
                int read = parse(start, source, table);
                if (start >= static_cast<int>(source.size())) {
                        break;
                }
                start += read;
                this->tables.emplace_back(table);
        }
}
Table& Parser::getTableAt(int index) { return tables.at(index); }
const Table& Parser::getTableAt(int index) const { return tables.at(index); }
int Parser::getTableCount() const { return static_cast<int>(tables.size()); }

int Parser::parse(int start, const std::string& source, Table& table) {
        int len = static_cast<int>(source.size());
        int read = 0;
        int columns = 0;
        Line line;
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
bool Parser::isNullOrEmpty(const std::string& str) {
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
}  // namespace csvr