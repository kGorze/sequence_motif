#ifndef TABUSEARCHBASE_H
#define TABUSEARCHBASE_H

#include <vector>
#include <unordered_set>
#include <deque>
#include "Graph.h"

struct Solution {
    std::vector<bool> inClique; // inClique[v] = czy wierzchołek v jest w rozwiązaniu
    int size;                   // Rozmiar (liczba wierzchołków w klice)
};

struct SolutionHash {
    std::size_t operator()(const Solution &S) const;
};

struct SolutionEq {
    bool operator()(const Solution &A, const Solution &B) const;
};

class TabuSearchBase {
protected:
    const Graph &G;
    int T1_size;    // rozmiar listy tabu T1
    int T2_size;    // rozmiar listy tabu T2
    int MaxIter;    // maksymalna liczba iteracji bez poprawy

    std::deque<Solution> T1_list;
    std::unordered_set<Solution, SolutionHash, SolutionEq> T1_set;
    std::deque<int> T2_list;
    std::unordered_set<int> T2_set;

    int bestFoundSize;
    Solution bestSol;
    int itersSinceImprovement;

    // Metody pomocnicze
    std::vector<int> solutionVertices(const Solution &S) const;
    std::vector<int> computeC(const Solution &S) const;
    Solution makeSolution(const std::vector<int> &verts) const;
    int upperBoundFromS(const Solution &S) const;
    bool isClique(const std::vector<int> &solVec) const;

    virtual Solution selectBestNeighbor(const Solution &S,
                                        const std::vector<Solution> &neighbors) = 0;

    virtual void initialize() = 0;
    void updateBestIfNeeded(const Solution &S);
    void updateTabuListAfterMove(const Solution &S, bool augmenting, int changedVertex);

public:
    TabuSearchBase(const Graph &graph, int T1_sz, int T2_sz, int maxIter);
    virtual ~TabuSearchBase() = default;

    // Główna pętla tabu search
    virtual Solution run() = 0;
};

#endif // TABUSEARCHBASE_H
