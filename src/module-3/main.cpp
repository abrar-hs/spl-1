#include "summarizer.h"
#include "text.h"
#include "result.h"

#include <fstream>
#include <iostream>

using namespace std;

static m3::Status loadNote(const string& path, string& text) {
    ifstream input(path, ios::binary);
    if (!input) {
        return {false, "Could not open the note."};
    }

    const size_t maxBytes = 20000;
    char c;
    while (input.get(c)) {
        if (text.size() == maxBytes) {
            return {false, "Please use a note with at most 20,000 bytes."};
        }
        text += c;
    }
    if (!input.eof()) {
        return {false, "Could not read the note."};
    }
    return {};
}

static int summarizeNote(const string& path) {
    string text;
    const m3::Status status = loadNote(path, text);
    if (!status.ok) {
        cerr << status.error << '\n';
        return 1;
    }

    const auto sentences = m3::splitSentences(text);
    if (sentences.empty()) {
        cout << "The note is empty.\n";
        return 0;
    }
    if (sentences.size() > 200) {
        cerr << "Please use a note with at most 200 sentences.\n";
        return 1;
    }

    cerr << "Enter your desired summary length (in sentences): ";
    string answer;
    if (!getline(cin, answer)) {
        return 1;
    }

    int summaryLength;
    size_t parsed;
    try {
        summaryLength = stoi(answer, &parsed);
    } catch (...) {
        cerr << "Enter a positive whole number.\n";
        return 1;
    }
    if (summaryLength <= 0 || parsed != answer.size()) {
        cerr << "Enter a positive whole number.\n";
        return 1;
    }

    for (const string& sentence : m3::summarize(sentences, summaryLength)) {
        cout << sentence << '\n';
    }
    return 0;
}

static void summarizeMenu() {
    while (cin) {
        cout << "\nSummarize a Note\n"
        "1. Summarize\n"
        "2. Export\n"
        "0. Back\n"
        "Choose an option: ";

        string choice;
        if (!getline(cin, choice) || choice == "0") {
            return;
        }
        if (choice == "1") {
            cout << "Enter the note filename: ";
            string path;
            if (!getline(cin, path)) {
                return;
            }
            summarizeNote(path);
        }
        else if (choice == "2") {
            cout << "In Progress...\n";
        }
        else {
            cout << "Choose 1, 2, or 0.\n";
        }
    }
}

static void placeholderMenu(const char* options, char lastOption) {
    while (cin) {
        cout << options << "0. Back\nChoose an option: ";
        string choice;
        if (!getline(cin, choice) || choice == "0") {
            return;
        }
        if (choice.size() == 1 && choice[0] >= '1' && choice[0] <= lastOption) {
            cout << "In Progress...\n";
        }
        else {
            cout << "Choose an option from the menu.\n";
        }
    }
}

int main(int argc, char* argv[]) {
    if (argc == 2) {
        return summarizeNote(argv[1]);
    }
    if (argc != 1) {
        cerr << "Usage: app [note.txt]\n";
        return 1;
    }

    while (cin) {
        cout << "\n#Note Manager & Extractive Summarizer#\n"
                "1. Summarize a Note\n"
                "2. Manage Notes\n"
                "3. Search Notes\n"
                "4. View Note Statistics\n"
                "5. Remove Note\n"
                "0. Exit\nChoose an option: ";
        string choice;
        if (!getline(cin, choice) || choice == "0") {
            return 0;
        }
        if (choice == "1") {
            summarizeMenu();
        }
        else if (choice == "2") {
            placeholderMenu("\nManage Notes\n1. List Notes\n2. Read a Note\n3. Create a Note\n4. Edit a Note\n5. Add Tag\n", '5');
        }
        else if (choice == "3") {
            cout << "In Progress...\n";
        }
        else if (choice == "4") {
            placeholderMenu("\nView Note Statistics\n1. Text Metrics\n2. Top Keywords\n3. Compare Notes\n", '3');
        }
        else if (choice == "5") {
            placeholderMenu("\nRemove Note\n1. Move to Trash\n2. Permanently Delete\n", '2');
        }
        else {
            cout << "Choose an option from the menu.\n";
        }
    }
}
