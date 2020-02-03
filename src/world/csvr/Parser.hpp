#pragma once
#ifndef CSVR_CSVR_HPP
#define CSVR_CSVR_HPP
#include <string>
#include <vector>

namespace csvr {
constexpr char separator = ',';
constexpr char newline = '\n';

using Line = std::vector<std::string>;
using Table = std::vector<Line>;
/**
 * Parser.
 */
class Parser {
       public:
        explicit Parser();

        /**
         * @param source
         */
        void parse(const std::string& source);

        /**
         * @param index
         * @return
         */
        Table& getTableAt(int index);

        /**
         * @param index
         * @return
         */
        const Table& getTableAt(int index) const;
        /**
         * @return
         */
        int getTableCount() const;

       private:
        static int parse(int start, const std::string& source, Table& table);
        static bool isNullOrEmpty(const std::string& str);
        std::vector<Table> tables;
};
}  // namespace csvr
#endif