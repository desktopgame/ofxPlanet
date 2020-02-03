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
 * csvr::Parser は CSV の独自拡張フォーマットをサポートするパーサです。
 * このパーサはCSVによって表される二次元配列の配列を保持します。
 * パース時には, 通常のCSV同様 ,(カンマ) をカラムの区切り文字として使用する他、
 * なにもない空行を CSV１つの終了 とみなします。
 */
class Parser {
       public:
        explicit Parser();

        /**
         * 指定のCSVRフォーマットの文字列を読み込んで、
         * テーブルとして追加します。
         */
        void parse(const std::string& source);

        /**
         * 指定位置のテーブルを返します。
         * @param index
         * @return
         */
        Table& getTableAt(int index);

        /**
         * 指定位置のテーブルを返します。
         * @param index
         * @return
         */
        const Table& getTableAt(int index) const;
        /**
         * テーブルの数を返します。
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