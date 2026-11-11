#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <vector>

namespace m3 {

    std::vector<std::string> splitSentences(const std::string& text);
    std::vector<std::string> tokenize(const std::string& sentence);

}

#endif
