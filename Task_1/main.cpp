#include <iostream>
#include <vector>
#include <limits>
#include <iomanip>
#include <omp.h>

class BranchAndBound {
public:
    BranchAndBound(const std::vector<int>& widths, const std::vector<std::vector<int>>& weights)
        : widths(widths), weights(weights), n(widths.size()), bestCost(std::numeric_limits<int>::max()) {}

    void solve() {
        std::vector<int> arrangement(n);
        for (int i = 0; i < n; ++i) {
            arrangement[i] = i; // Inicializace uspořádání s indexy
        }
        branchAndBound(arrangement, 0);
    }

    void printBest() const {
        std::cout << "\nNejlepší cena: " << bestCost << "\nNejlepší uspořádání: ";
        for (auto i : bestArrangement) {
            std::cout << i << " ";
        }
        std::cout << "\n";
    }

private:
    std::vector<int> widths; // Šířky zařízení
    std::vector<std::vector<int>> weights; // Matice váh
    int n; // Počet zařízení
    int bestCost; // Nejlepší cena
    std::vector<int> bestArrangement; // Nejlepší uspořádání

    // Vypočítat vzdálenost d(pi_i, pi_j)
    int distance(int i, int j, const std::vector<int>& arrangement) {
        int sum_widths = 0;
        for (int k = i; k <= j; ++k) {
            sum_widths += widths[arrangement[k]];
        }
        return (widths[arrangement[i]] + widths[arrangement[j]]) / 2 + sum_widths;
    }

    // Vypočítat cenu f_SRFLP(pi)
    int calculateCost(const std::vector<int>& arrangement) {
        int totalCost = 0;
        for (size_t i = 0; i < n; ++i) {
            for (int j = i + 1; j < n; ++j) {
                totalCost += weights[arrangement[i]][arrangement[j]] * distance(i, j, arrangement);
            }
        }
        return totalCost;
    }

    // Hlavní metoda pro Branch and Bound
    void branchAndBound(std::vector<int>& arrangement, int depth) {
        // Pokud dosáhneme hloubky n, vypočítáme cenu
        if (depth == n) {
            int currentCost = calculateCost(arrangement);
            if (currentCost < bestCost) {
                bestCost = currentCost;
                bestArrangement = arrangement; // Uložení nejlepšího uspořádání
            }
        }

        // Rekurzivní větvení
        for (size_t i = depth; i < n; ++i) {
            std::swap(arrangement[depth], arrangement[i]);
            branchAndBound(arrangement, depth + 1);
            std::swap(arrangement[depth], arrangement[i]); // Zpět
        }
    }
};

// Hlavní funkce pro inicializaci dat a spuštění Branch and Bound
int main() {
    // Zadání dat pro SRFLP
    std::vector<int> widths = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}; // Šířky zařízení
    std::vector<std::vector<int>> weights = {
        {0, 30, 17, 11, 24, 25, 24, 17, 16, 22},
        {30, 0, 21, 23, 26, 24, 27, 19, 11, 32},
        {17, 21, 0, 24, 18, 23, 31, 36, 28, 19},
        {11, 23, 24, 0, 19, 18, 33, 25, 20, 28},
        {24, 26, 18, 19, 0, 15, 37, 27, 17, 16},
        {25, 24, 23, 18, 15, 0, 27, 23, 29, 24},
        {24, 27, 31, 33, 37, 27, 0, 27, 31, 24},
        {17, 19, 36, 25, 27, 23, 27, 0, 14, 18},
        {16, 11, 28, 20, 17, 29, 31, 14, 0, 24},
        {22, 32, 19, 28, 16, 24, 24, 18, 24, 0}
    };

    BranchAndBound solver(widths, weights);
    solver.solve();
    solver.printBest();

    return 0;
}