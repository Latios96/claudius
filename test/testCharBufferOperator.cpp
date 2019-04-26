//
// Created by Jan on 26.04.2019.
//

#include <CharBufferOperator.h>
#include "catch.hpp"


TEST_CASE("CharBufferOperator"){

    SECTION("shouldCorrectlyReadUntilNewLine"){
        std::vector<char> buff;
        buff.push_back('a');
        buff.push_back('b');
        buff.push_back('c');
        buff.push_back(' ');
        buff.push_back(' ');
        buff.push_back('\n');
        buff.push_back('e');
        buff.push_back(' ');
        buff.push_back('f');
        buff.push_back(' ');
        buff.push_back('g');
        buff.push_back(' ');
        buff.push_back('\n');
        buff.push_back('h');

        CharBufferOperator bufferOperator(buff);

        bufferOperator.toFirstCharAfterNewLine();
        REQUIRE(bufferOperator.currentChar() == 'e');

        bufferOperator.toFirstCharAfterSpace();
        REQUIRE(bufferOperator.currentChar() == 'f');

        bufferOperator.toFirstCharAfterSpace();
        REQUIRE(bufferOperator.currentChar() == 'g');

        bufferOperator.toFirstCharAfterNewLine();
        REQUIRE(bufferOperator.currentChar() == 'h');
    }
}