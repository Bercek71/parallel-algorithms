//
// Created by Marek Beran on 02.11.2024.
//

#include "BranchAndBound.h"

int BranchAndBound::factorial(int n) {
    if(n == 0) {
        return 1;
    }
    return n * factorial(n - 1);
}

    void BranchAndBound::solve() {
        std::vector<int> arrangement(n);
        for (int i = 0; i < n; ++i) {
            arrangement[i] = i; // Inicializace uspořádání s indexy
        }
        totalNodes = factorial(n) * 2.71;
        branchAndBound(arrangement, 0, true);
    }

    void BranchAndBound::printBest() const {
        std::cout << "\nNejlepší cena: " << bestCost << "\nNejlepší uspořádání: ";
        for (auto i : bestArrangement) {
            std::cout << i << " ";
        }
        std::cout << "\n";
    }


    void BranchAndBound::updateBest(int currentCost, const std::vector<int>& arrangement, int depth) {
        if (currentCost < bestCost) {
        #pragma omp critical
        {
                bestCost = currentCost;
                bestArrangement = arrangement; // Uložení nejlepšího uspořádání
        }

        }
    }

    int BranchAndBound::distance(int i, int j, const std::vector<int>& arrangement) {
        int sum_widths = 0;
        for (int k = i; k <= j; ++k) {
            sum_widths += widths[arrangement[k]];
        }
        return (widths[arrangement[i]] + widths[arrangement[j]]) / 2 + sum_widths;
    }

    int BranchAndBound::calculateCost(const std::vector<int>& arrangement) {
        int totalCost = 0;
        for (size_t i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                totalCost += weights[arrangement[i]][arrangement[j]] * distance(i, j, arrangement);
            }
        }
        return totalCost;
    }

    // Hlavní metoda pro Branch and Bound
    void BranchAndBound::branchAndBound(std::vector<int>& arrangement, int depth, bool parallel) {
/*
    #pragma omp atomic
        processedNodes++;

        // Print progress
    if(processedNodes % 10000 == 0) {
            printProgress(processedNodes);
    }

*/
        // Pokud dosáhneme hloubky n, vypočítáme cenu
        if (depth >= n) {
            int currentCost = calculateCost(arrangement);
            updateBest(currentCost, arrangement, depth); // Aktualizace nejlepšího řešení
            return;
        }

        // Paralelní větvení
        if(!parallel) {
            for (size_t i = depth; i < n; ++i) {
                std::vector<int> newArrangement = arrangement; // Vytvoření nové permutace
                std::swap(newArrangement[depth], newArrangement[i]);
                branchAndBound(newArrangement, depth + 1);
            }
            return;
        }

        #pragma omp parallel for
        for (size_t i = depth; i < n; ++i) {
            std::vector<int> newArrangement = arrangement; // Vytvoření nové permutace
            std::swap(newArrangement[depth], newArrangement[i]);
            branchAndBound(newArrangement, depth + 1);

        }
    }
