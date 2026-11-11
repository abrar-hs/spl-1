# Note Manager & Extractive Summarizer

# Module 3

This C++17 program summarizes one short English plain-text note. It uses TF-IDF, cosine similarity, and LexRank to select sentences from the note and print them in their original order.

## Build and run

Open a terminal in the folder containing the source files. Make sure `g++` is installed.

**Windows PowerShell**

```powershell
g++ -std=c++17 *.cpp -o app.exe
.\app.exe
```

**Ubuntu**

```bash
g++ -std=c++17 *.cpp -o app
./app
```

## Use the menu

1. Choose `1` for **Summarize a Note**.
2. Choose `1` again for **Summarize**.
3. Enter the filename, such as `note.txt`.
4. Enter the desired summary length in sentences and press Enter.

If you ask for more sentences than the note contains, the program prints all available sentences. Choose `0` to go back from a submenu or exit from the main menu.

**Export** and the unfinished actions under **Manage Notes**, **Search Notes**, **View Note Statistics**, and **Remove Note** display `In Progress...`.

## Summarize directly

Pass the filename as an argument to skip the menu. The program still asks for the sentence count.

```powershell
.\app.exe note.txt
```

```bash
./app note.txt
```

To save the summary while the Export menu option is pending, use shell redirection. The sentence-count prompt remains visible in the terminal. Choose a new output filename if you want to keep an existing file.

```powershell
.\app.exe note.txt > summary.txt
```

```bash
./app note.txt > summary.txt
```

## Files

| File | What it does |
| --- | --- |
| `main.cpp` | Reads the note, displays the menus, asks for the summary length, and prints the result. |
| `result.h` | Defines `m3::Status` for reporting success or a file-loading error. |
| `text.h`, `text.cpp` | Declare and implement sentence splitting and tokenization. |
| `text_utils.h`, `text_utils.cpp` | Provide character checks and ASCII lowercase conversion. |
| `stopwords.h` | Stores common English words in a vector and checks them with a linear search. |
| `sorting.h` | Provides stable merge sort for ranking sentence indexes. |
| `summarizer.h`, `summarizer.cpp` | Declare and implement TF-IDF, cosine similarity, LexRank, and sentence selection. |
| `note.txt` | Provides sample input for testing. |

## How the summary is generated

1. Split the note into sentences. Tokenize each sentence, lowercase the words, and filter common words for scoring.
2. Build a vocabulary from the remaining words. Represent each sentence as a TF-IDF vector with one column per vocabulary word.
3. Compare sentence pairs with cosine similarity. Keep connections with a similarity of at least `0.1` and use the similarity as the edge weight.
4. Run 50 rounds of LexRank to score the sentences. Select the highest-scoring sentences and print them in their original order.

## Limits

- Accepts one plain-text note at a time, up to **20,000 bytes** and **200 sentences**.
- Treats every period, question mark, and exclamation mark as a sentence boundary. Abbreviations and decimals can split incorrectly.
- Tokenizes English letters and applies an English stop-word list.
- Can select repetitive sentences or sentences that rely on earlier context.


