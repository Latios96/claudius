//
// Created by Jan on 26.04.2019.
//

#include "CharBufferOperator.h"
char CharBufferOperator::currentChar() {
    return charBuffer[bufferIndex];
}
bool CharBufferOperator::toFirstCharAfterNewLine() {
    return readUntilFirstCharacterAfter('\n');
}
bool CharBufferOperator::toFirstCharAfterSpace() {
    return readUntilFirstCharacterAfter(' ');
}
bool CharBufferOperator::readUntilSpace(std::string &bufferString) {
    return readUntil(bufferString, ' ');
}
bool CharBufferOperator::readUntilNewLine(std::string &bufferString) {
    return readUntil(bufferString, '\n');
}
bool CharBufferOperator::canContinue() {
    return bufferIndex < charBuffer.size() && currentChar() != '\0';
}
bool CharBufferOperator::readUntilFirstCharacterAfter(char untilThis) {
    unsigned oldBufferIndex = bufferIndex;
    while (bufferIndex < charBuffer.size() && currentChar() != untilThis) {
        bufferIndex++;
    }
    if (bufferIndex <= charBuffer.size()) {
        bufferIndex++;
    }
    return oldBufferIndex < bufferIndex;
}
bool CharBufferOperator::readUntil(std::string &bufferString, char untilThis) {
    unsigned int i = 0;
    unsigned oldBufferIndex = bufferIndex;
    while (bufferIndex < charBuffer.size() && currentChar() != untilThis) {
        bufferString.push_back(currentChar());
        bufferIndex++;
    }
    return oldBufferIndex < bufferIndex;
}
