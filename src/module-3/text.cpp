#include "text.h"
#include "text_utils.h"
#include "stopwords.h"

using namespace std;

vector<string> m3::splitSentences(const string& text) {
    vector<string> sentences;
    string sentence;

    for (char c : text) {
        if (m3::isSpace(static_cast<unsigned char>(c))) {
            if (!sentence.empty() && sentence.back() != ' ') {
                sentence += ' ';
            }
        }
        else {
            sentence += c;
        }

        if (c == '.' || c == '?' || c == '!') {
            sentences.push_back(sentence);
            sentence.clear();
        }
    }

    if (!sentence.empty()) {
        if (sentence.back() == ' ') {
            sentence.pop_back();
        }
        sentences.push_back(sentence);
    }
    return sentences;
}

vector<string> m3::tokenize(const string& sentence) {
    vector<string> words;
    string word;
    const m3::StopWords stopWords;

    for (char c : sentence + ' ') {
        c = static_cast<char>(m3::toLower(static_cast<unsigned char>(c)));
        if (m3::isLetter(static_cast<unsigned char>(c))) {
            word += c;
        }
        else {
            if (!word.empty() && !stopWords.contains(word)) {
                words.push_back(word);
            }
            word.clear();
        }
    }
    return words;
}
