/**
 * Scaled-down 2x2 demo: enumerate row-compatible and column-compatible pairs
 * of 2x2 sudoku sub-squares, indexed by their position in the catalog of all
 * 4! = 24 row-major permutations of {1,2,3,4} (lex order over (c0,c1,c2,c3)).
 *
 * Layout of one 2x2 square (cell index, row-major):
 *      +----+----+
 *      | c0 | c1 |
 *      +----+----+
 *      | c2 | c3 |
 *      +----+----+
 *
 * A 2x2 sudoku board is the 4x4 grid built from four such squares:
 *      +-----+-----+
 *      |  A  |  B  |
 *      +-----+-----+
 *      |  C  |  D  |
 *      +-----+-----+
 *
 * Row independence (A on the left, B on the right):
 *   For each row r in {0,1}, the 4 cells A.row(r) ++ B.row(r) must be the
 *   multiset {1,2,3,4}. Equivalently, B.row(r) is some permutation of
 *   {1,2,3,4} \ A.row(r).
 *
 * Column independence (A on top, B on bottom):
 *   For each column c in {0,1}, A.col(c) ++ B.col(c) must equal {1,2,3,4}.
 */

#include <algorithm>
#include <array>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <utility>
#include <vector>

using Square2x2 = std::array<int, 4>;

constexpr std::size_t kNumPerms2x2 = 24; // 4!

/** All 24 row-major permutations of {1,2,3,4} in lex order on (c0,c1,c2,c3). */
const std::vector<Square2x2> &getAll2x2Permutations()
{
    static const std::vector<Square2x2> perms = [] {
        std::vector<Square2x2> out;
        out.reserve(kNumPerms2x2);
        Square2x2 p{1, 2, 3, 4};
        out.push_back(p);
        while (std::next_permutation(p.begin(), p.end()))
        {
            out.push_back(p);
        }
        return out;
    }();
    return perms;
}

/** True iff concatenating row r of A then row r of B uses each of 1..4 once. */
bool rowsFormFullLine(const Square2x2 &a, const Square2x2 &b)
{
    for (std::size_t r = 0; r < 2; ++r)
    {
        bool seen[5] = {};
        const std::size_t base = r * 2;
        for (std::size_t c = 0; c < 2; ++c)
        {
            const int va = a[base + c];
            if (va < 1 || va > 4 || seen[va])
            {
                return false;
            }
            seen[va] = true;
        }
        for (std::size_t c = 0; c < 2; ++c)
        {
            const int vb = b[base + c];
            if (vb < 1 || vb > 4 || seen[vb])
            {
                return false;
            }
            seen[vb] = true;
        }
    }
    return true;
}

/** True iff stacking A on top of B yields full {1..4} in each column. */
bool columnsFormFullLine(const Square2x2 &a, const Square2x2 &b)
{
    for (std::size_t c = 0; c < 2; ++c)
    {
        bool seen[5] = {};
        const int top0 = a[c];
        const int top1 = a[c + 2];
        const int bot0 = b[c];
        const int bot1 = b[c + 2];
        for (int v : {top0, top1, bot0, bot1})
        {
            if (v < 1 || v > 4 || seen[v])
            {
                return false;
            }
            seen[v] = true;
        }
    }
    return true;
}

/** Ordered pairs (i, j) where square i can sit left of square j and form valid 4-cell rows. */
std::vector<std::pair<std::size_t, std::size_t>> getRowCompatible2x2Pairs()
{
    const std::vector<Square2x2> &perms = getAll2x2Permutations();
    std::vector<std::pair<std::size_t, std::size_t>> pairs;
    pairs.reserve(96); // (4!) * (2!)^2 = 96 by the closed-form count above.
    for (std::size_t i = 0; i < perms.size(); ++i)
    {
        for (std::size_t j = 0; j < perms.size(); ++j)
        {
            if (rowsFormFullLine(perms[i], perms[j]))
            {
                pairs.emplace_back(i, j);
            }
        }
    }
    return pairs;
}

/** Ordered pairs (i, j) where square i can sit above square j and form valid 4-cell columns. */
std::vector<std::pair<std::size_t, std::size_t>> getColumnCompatible2x2Pairs()
{
    const std::vector<Square2x2> &perms = getAll2x2Permutations();
    std::vector<std::pair<std::size_t, std::size_t>> pairs;
    pairs.reserve(96);
    for (std::size_t i = 0; i < perms.size(); ++i)
    {
        for (std::size_t j = 0; j < perms.size(); ++j)
        {
            if (columnsFormFullLine(perms[i], perms[j]))
            {
                pairs.emplace_back(i, j);
            }
        }
    }
    return pairs;
}

// Resultant pair dictionaries...
// extern const so sister TUs (e.g. sp4_try_rows_check_columns.cpp) can index
// into these precomputed compatibility tables without recomputing them. A plain
// `const` at namespace scope would have internal linkage by default in C++.
extern const std::vector<std::pair<int, int>> ValidRowPairs;
extern const std::vector<std::pair<int, int>> ValidColumnPairs;

const std::vector<std::pair<int, int>> ValidRowPairs = {
    {0, 16},  {0, 17},  {0, 22},  {0, 23},  {1, 16},  {1, 17},  {1, 22},  {1, 23},  {2, 10},  {2, 11},  {2, 20},
    {2, 21},  {3, 10},  {3, 11},  {3, 20},  {3, 21},  {4, 8},   {4, 9},   {4, 14},  {4, 15},  {5, 8},   {5, 9},
    {5, 14},  {5, 15},  {6, 16},  {6, 17},  {6, 22},  {6, 23},  {7, 16},  {7, 17},  {7, 22},  {7, 23},  {8, 4},
    {8, 5},   {8, 18},  {8, 19},  {9, 4},   {9, 5},   {9, 18},  {9, 19},  {10, 2},  {10, 3},  {10, 12}, {10, 13},
    {11, 2},  {11, 3},  {11, 12}, {11, 13}, {12, 10}, {12, 11}, {12, 20}, {12, 21}, {13, 10}, {13, 11}, {13, 20},
    {13, 21}, {14, 4},  {14, 5},  {14, 18}, {14, 19}, {15, 4},  {15, 5},  {15, 18}, {15, 19}, {16, 0},  {16, 1},
    {16, 6},  {16, 7},  {17, 0},  {17, 1},  {17, 6},  {17, 7},  {18, 8},  {18, 9},  {18, 14}, {18, 15}, {19, 8},
    {19, 9},  {19, 14}, {19, 15}, {20, 2},  {20, 3},  {20, 12}, {20, 13}, {21, 2},  {21, 3},  {21, 12}, {21, 13},
    {22, 0},  {22, 1},  {22, 6},  {22, 7},  {23, 0},  {23, 1},  {23, 6},  {23, 7}};
const std::vector<std::pair<int, int>> ValidColumnPairs = {
    {0, 7},   {0, 9},   {0, 18},  {0, 23},  {1, 6},   {1, 11},  {1, 12},  {1, 17},  {2, 13},  {2, 15},  {2, 19},
    {2, 21},  {3, 6},   {3, 11},  {3, 12},  {3, 17},  {4, 13},  {4, 15},  {4, 19},  {4, 21},  {5, 7},   {5, 9},
    {5, 18},  {5, 23},  {6, 1},   {6, 3},   {6, 20},  {6, 22},  {7, 0},   {7, 5},   {7, 14},  {7, 16},  {8, 13},
    {8, 15},  {8, 19},  {8, 21},  {9, 0},   {9, 5},   {9, 14},  {9, 16},  {10, 13}, {10, 15}, {10, 19}, {10, 21},
    {11, 1},  {11, 3},  {11, 20}, {11, 22}, {12, 1},  {12, 3},  {12, 20}, {12, 22}, {13, 2},  {13, 4},  {13, 8},
    {13, 10}, {14, 7},  {14, 9},  {14, 18}, {14, 23}, {15, 2},  {15, 4},  {15, 8},  {15, 10}, {16, 7},  {16, 9},
    {16, 18}, {16, 23}, {17, 1},  {17, 3},  {17, 20}, {17, 22}, {18, 0},  {18, 5},  {18, 14}, {18, 16}, {19, 2},
    {19, 4},  {19, 8},  {19, 10}, {20, 6},  {20, 11}, {20, 12}, {20, 17}, {21, 2},  {21, 4},  {21, 8},  {21, 10},
    {22, 6},  {22, 11}, {22, 12}, {22, 17}, {23, 0},  {23, 5},  {23, 14}, {23, 16}};