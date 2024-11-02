//
// Created by Marek Beran on 02.11.2024.
//

#ifndef BRANCHANDBOUND_H
#define BRANCHANDBOUND_H

#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>
#include <omp.h>

class BranchAndBound {
public:
    BranchAndBound(const std::vector<int>& widths, const std::vector<std::vector<int>>& weights)
        : widths(widths), weights(weights), n(widths.size()), bestCost(std::numeric_limits<int>::max()) {}

    int factorial(int i);

    void solve();

    void printBest() const;

private:
    std::vector<int> widths; // Šířky zařízení
    std::vector<std::vector<int>> weights; // Matice váh
    int n; // Počet zařízení
    int bestCost; // Nejlepší cena
    std::vector<int> bestArrangement; // Nejlepší uspořádání
    int processedNodes;
    int totalNodes;

    // Synchronizovaná metoda pro aktualizaci nejlepšího řešení
    void updateBest(int currentCost, const std::vector<int>& arrangement, int depth);

    // Vypočítat vzdálenost d(pi_i, pi_j)
    int distance(int i, int j, const std::vector<int>& arrangement);

    // Vypočítat cenu f_SRFLP(pi)
    int calculateCost(const std::vector<int>& arrangement);
    // Hlavní metoda pro Branch and Bound
    void branchAndBound(std::vector<int>& arrangement, int depth, bool parallel = false);

    void printProgress(int depth) const;
};



#endif //BRANCHANDBOUND_H
