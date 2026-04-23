/**
 * All 9! orderings of digits 1..9 in a 3×3, row-major (self-contained; no .hpp).
 * Other .cpp files that need these symbols declare the ones they use and link
 * this translation unit (see sp2 / sp3 / tests). `writeAllSquarePermutationsToCsv`
 * can dump the table to a CSV (used by the main driver).
 */

#include "square.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <vector>

constexpr std::size_t kFactorial9 = 9 * 8 * 7 * 6 * 5 * 4 * 3 * 2; // 9!

std::vector<std::array<int, 9>> makeAllSquarePermutations()
{
    std::array<int, 9> perm{};
    for (int i = 0; i < 9; ++i)
    {
        perm[static_cast<std::size_t>(i)] = i + 1;
    }
    std::vector<std::array<int, 9>> out;
    out.reserve(kFactorial9);
    out.emplace_back(perm);
    while (std::next_permutation(perm.begin(), perm.end()))
    {
        out.emplace_back(perm);
    }
    return out;
}

const std::vector<std::array<int, 9>> gAllSquarePermutations = makeAllSquarePermutations();

std::vector<std::array<int, 9>> generateAllSquarePermutations()
{
    return gAllSquarePermutations;
}

square getSquarePermutation(std::size_t index)
{
    return square(gAllSquarePermutations.at(index));
}

const std::vector<square> &getAllSquarePermutationsAsSquares()
{
    static const std::vector<square> asSquares = [] {
        std::vector<square> v;
        v.reserve(gAllSquarePermutations.size());
        for (const auto &cells : gAllSquarePermutations)
        {
            v.emplace_back(cells);
        }
        return v;
    }();
    return asSquares;
}

// Helper to output the result of this to a CSV file for reference and checking...
bool writeAllSquarePermutationsToCsv(const std::filesystem::path &outputPath)
{
    if (const std::filesystem::path parent = outputPath.parent_path(); !parent.empty())
    {
        std::filesystem::create_directories(parent);
    }
    std::ofstream out(outputPath);
    if (!out)
    {
        return false;
    }
    out << "c0,c1,c2,c3,c4,c5,c6,c7,c8\n";
    for (const std::array<int, 9> &row : gAllSquarePermutations)
    {
        for (std::size_t i = 0; i < row.size(); ++i)
        {
            if (i > 0)
            {
                out << ',';
            }
            out << row[i];
        }
        out << '\n';
    }
    return static_cast<bool>(out);
}
