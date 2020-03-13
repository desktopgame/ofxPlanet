#pragma once
#ifndef CSVR_CSVR_HPP
#define CSVR_CSVR_HPP
#include <string>
#include <vector>
namespace ofxPlanet {
constexpr char separator = ',';
constexpr char newline = '\n';

using CSVLine = std::vector<std::string>;
using CSVTable = std::vector<CSVLine>;
/**
 * CSVParser.
 */
class CSVParser {
       public:
        explicit CSVParser();

        /**
         * @param source
         */
        void parse(const std::string& source);

        /**
         * @param index
         * @return
         */
        CSVTable& getTableAt(int index);

        /**
         * @param index
         * @return
         */
        const CSVTable& getTableAt(int index) const;
        /**
         * @return
         */
        int getTableCount() const;

       private:
        static int parse(int start, const std::string& source, CSVTable& table);
        static bool isNullOrEmpty(const std::string& str);
        std::vector<CSVTable> tables;
};
}  // namespace ofxPlanet
#endif