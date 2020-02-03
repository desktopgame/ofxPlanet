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
 * csvr::Parser �� CSV �̓Ǝ��g���t�H�[�}�b�g���T�|�[�g����p�[�T�ł��B
 * ���̃p�[�T��CSV�ɂ���ĕ\�����񎟌��z��̔z���ێ����܂��B
 * �p�[�X���ɂ�, �ʏ��CSV���l ,(�J���}) ���J�����̋�؂蕶���Ƃ��Ďg�p���鑼�A
 * �Ȃɂ��Ȃ���s�� CSV�P�̏I�� �Ƃ݂Ȃ��܂��B
 */
class Parser {
       public:
        explicit Parser();

        /**
         * �w���CSVR�t�H�[�}�b�g�̕������ǂݍ���ŁA
         * �e�[�u���Ƃ��Ēǉ����܂��B
         */
        void parse(const std::string& source);

        /**
         * �w��ʒu�̃e�[�u����Ԃ��܂��B
         * @param index
         * @return
         */
        Table& getTableAt(int index);

        /**
         * �w��ʒu�̃e�[�u����Ԃ��܂��B
         * @param index
         * @return
         */
        const Table& getTableAt(int index) const;
        /**
         * �e�[�u���̐���Ԃ��܂��B
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