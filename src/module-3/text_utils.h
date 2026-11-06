#ifndef M3_TEXT_UTILS_H
#define M3_TEXT_UTILS_H

include <string>

namespace m3 {
    bool isLetter(unsigned char c);
    bool isDigital(unsigned char c);
    bool isSpace(unsigned char c);
    bool isWordByte(unsigned char c);
    unsigned char toLower(unsigned char c);
    std::string lowerAscii(const std::string& text);
    std::string trim(const std::string& text);
    std::string::size_type complexityExpressionEnd(const std::string& text, std::string::size_type start);
    bool isTagLine(const std::string& text);
}

#endif