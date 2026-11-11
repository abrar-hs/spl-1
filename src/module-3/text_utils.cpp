#include "text_utils.h"

using namespace std;

namespace m3 {
    bool isLetter(unsigned char c) {
        return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
    }
    bool isDigit(unsigned char c) {
        return c >= '0' && c <= '9';
    }
    bool isSpace(unsigned char c) {
        return c == ' ' || c == '\t' || c == '\n' || c == '\r' || c == '\f' || c == '\v';
    }
    bool isWordByte(unsigned char c) {
        return isLetter(c) || isDigit(c) || c > 127;
    }
    unsigned char toLower(unsigned char c) {
        return c >= 'A' && c <= 'Z' ? static_cast<unsigned char>(c - 'A' + 'a') : c;
    }
    string lowerAscii(const string& text) {
        string result = text;
        for (string::size_type i = 0; i < result.size(); ++i) {
            result[i] = static_cast<char>(toLower(static_cast<unsigned char>(result[i])));
        }
        return result;
    }
    string trim(const string& text) {
        string::size_type first = 0;
        string::size_type last = text.size();
        while (first < last && isSpace(static_cast<unsigned char>(text[first]))) {
            ++first;
        }
        while (last > first && isSpace(static_cast<unsigned char>(text[last - 1]))) {
            --last;
        }
        return text.substr(first, last - first);
    }
    string::size_type complexityExpressionEnd(const string& text, string::size_type start) {
        using Size = string::size_type;
        constexpr Size kMaxBody = 20;
        if (start >= text.size() || text.size() - start < 3 ||
            toLower(static_cast<unsigned char>(text[start])) != 'o' || text[start + 1] != '(' ||
            (start > 0 && isWordByte(static_cast<unsigned char>(text[start - 1])))) {
            return string::npos;
        }
        Size i = start + 2;
        for (Size count = 0; i < text.size() && count <= kMaxBody; ++count, ++i) {
            if (text[i] == ')') {
                return i + 1;
            }
            const unsigned char c = static_cast<unsigned char>(text[i]);
            if (count == kMaxBody || !(isLetter(c) || isDigit(c) || c == ' ' || c == '+' || c == '*' || c == '^' || c == '/' || c == '-')) {
                    break;
            }
        }
        return string::npos;
    }

    bool isTagLine(const string& text) {
        const string line = trim(text);
        if (line.empty()) {
            return false;
        }
        string::size_type i = 0;
        while (i < line.size()) {
            if (line[i++] != '#') {
                return false;
            }
            const auto start = i;
            while (i < line.size() && (isWordByte(static_cast<unsigned char>(line[i])) || line[i] == '_' || line[i] == '-')) {
                ++i;
            }
            if (i == start || (i < line.size() && !isSpace(static_cast<unsigned char>(line[i])))) {
                return false;
            }
            while (i < line.size() && isSpace(static_cast<unsigned char>(line[i]))) {
                ++i;
            }
        }
        return true;
    }
}
