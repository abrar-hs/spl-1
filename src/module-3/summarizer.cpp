#include "summarizer.h"
#include "text.h"
#include "sorting.h"

#include <cmath>

using namespace std;

using Matrix = vector<vector<double>>;

static Matrix buildTfIdf(const vector<string>& sentences) {
    vector<string> vocabulary;
    vector<vector<size_t>> wordIds;

    for (const string& sentence : sentences) {
        vector<size_t> ids;
        for (const string& word : m3::tokenize(sentence)) {
            size_t id = 0;
            while (id < vocabulary.size() && vocabulary[id] != word) {
                ++id;
            }
            if (id == vocabulary.size()) {
                vocabulary.push_back(word);
            }
            ids.push_back(id);
        }
        wordIds.push_back(ids);
    }

    Matrix weights(sentences.size(), vector<double>(vocabulary.size(), 0));
    for (size_t i = 0; i < wordIds.size(); ++i) {
        for (size_t id : wordIds[i]) {
            ++weights[i][id];
        }
    }

    for (size_t id = 0; id < vocabulary.size(); ++id) {
        int frequency = 0;
        for (const auto& row : weights) {
            if (row[id] > 0) {
                ++frequency;
            }
        }
        double idf = log((sentences.size() + 1.0) / (frequency + 1.0)) + 1;
        for (auto& row : weights) {
            row[id] *= idf;
        }
    }
    return weights;
}

static double cosine(const vector<double>& a, const vector<double>& b) {
    double dot = 0;
    double lengthA = 0;
    double lengthB = 0;
    for (size_t i = 0; i < a.size(); ++i) {
        dot += a[i] * b[i];
        lengthA += a[i] * a[i];
        lengthB += b[i] * b[i];
    }
    if (lengthA == 0 || lengthB == 0) {
        return 0;
    }
    return dot / sqrt(lengthA * lengthB);
}

static Matrix buildGraph(const Matrix& weights) {
    size_t n = weights.size();
    Matrix graph(n, vector<double>(n, 0));
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = i + 1; j < n; ++j) {
            double similarity = cosine(weights[i], weights[j]);
            if (similarity >= 0.1) {
                graph[i][j] = similarity;
                graph[j][i] = similarity;
            }
        }
    }
    return graph;
}

static vector<double> rankSentences(const Matrix& graph) {
    size_t n = graph.size();
    const double damping = 0.85;
    vector<double> scores(n, 1.0 / n);
    vector<double> outgoing(n, 0);
    for (size_t i = 0; i < n; ++i) {
        for (double weight : graph[i]) {
            outgoing[i] += weight;
        }
    }

    for (int step = 0; step < 50; ++step) {
        vector<double> next(n, (1 - damping) / n);
        for (size_t from = 0; from < n; ++from) {
            for (size_t to = 0; to < n; ++to) {
                double fraction = 1.0 / n;
                if (outgoing[from] > 0) {
                    fraction = graph[from][to] / outgoing[from];
                }
                next[to] += damping * scores[from] * fraction;
            }
        }
        scores = next;
    }
    return scores;
}

vector<string> m3::summarize(
    const vector<string>& sentences, int count) {
    if (sentences.empty() || count <= 0) {
        return {};
    }

    Matrix weights = buildTfIdf(sentences);
    Matrix graph = buildGraph(weights);
    vector<double> scores = rankSentences(graph);
    vector<bool> selected(sentences.size(), false);

    vector<m3::Index> order;
    for (m3::Index i = 0; i < sentences.size(); ++i) {
        order.push_back(i);
    }
    
    m3::mergeSortIndices(order, [&scores](m3::Index a, m3::Index b) {
        return scores[a] > scores[b];
    });
    for (int pick = 0; pick < count && static_cast<size_t>(pick) < order.size(); ++pick) {
        selected[order[pick]] = true;
    }

    vector<string> summary;
    for (size_t i = 0; i < sentences.size(); ++i) {
        if (selected[i]) {
            summary.push_back(sentences[i]);
        }
    }
    return summary;
}
