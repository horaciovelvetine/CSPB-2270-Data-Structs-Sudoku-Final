/**
 * Reference: backtracking over independent 3×3 permutations with
 * row-and-column pruning between table-defined neighbor blocks.
 *
 * Construction: for each of the nine sub-squares (0..8), choose one of the 9!
 * permutations of 1..9 in row-major local order — naive size (9!)^9.
 *
 * **Sole pruning rule:** when placing block k, reject a candidate if it
 * disagrees with any **already placed** entry of kRowColNeighborBlockIndices[k]
 * on a shared full 9-wide row or column (see areNeighborBlocksRowColumnConsistent).
 * Indices in the table with value ≥ k are skipped until those blocks are filled later;
 * each neighbor pair is checked when the higher-index block is placed.
 *
 * There is no additional validity pass over the board permutations.
 * This code does not attempt to detect or reject assignments that might
 * still be "invalid" under other Sudoku rules or any other global criterion
 * beyond those pairwise row/column checks.
 */

#include "models/board.hpp"

#include <array>
#include <cstdint>
#include <vector>

// Defined in `sp1_all_square_permutations.cpp`.
const std::vector<square> &getAllSquarePermutationsAsSquares();

// Blocks that share a full 9-wide row or column (same row-band or same column-stack).
constexpr std::array<std::array<int, 4>, 9> kRowColNeighborBlockIndices = {{
    {{1, 2, 3, 6}},
    {{0, 2, 4, 7}},
    {{0, 1, 5, 8}},
    {{0, 4, 5, 6}},
    {{1, 3, 5, 7}},
    {{2, 3, 4, 8}},
    {{0, 3, 7, 8}},
    {{1, 4, 6, 8}},
    {{2, 5, 6, 7}},
}};

/** Row-major 3x3: local r, c in [0,3) */
int cellAt(const square &sq, std::size_t r, std::size_t c)
{
    return sq.valueAt(r * 3 + c);
}

/** Shared-line check for two blocks in the same row-band or same column-stack (only). */
bool areNeighborBlocksRowColumnConsistent(const square &a, std::size_t idxA, const square &b, std::size_t idxB)
{
    if (idxA / 3 == idxB / 3)
    {
        for (std::size_t r = 0; r < 3; ++r)
        {
            bool in_a[10] = {};
            for (std::size_t c = 0; c < 3; ++c)
            {
                in_a[static_cast<std::size_t>(cellAt(a, r, c))] = true;
            }
            for (std::size_t c = 0; c < 3; ++c)
            {
                if (in_a[static_cast<std::size_t>(cellAt(b, r, c))])
                {
                    return false;
                }
            }
        }
        return true;
    }
    if (idxA % 3 == idxB % 3)
    {
        for (std::size_t c = 0; c < 3; ++c)
        {
            bool in_a[10] = {};
            for (std::size_t r = 0; r < 3; ++r)
            {
                in_a[static_cast<std::size_t>(cellAt(a, r, c))] = true;
            }
            for (std::size_t r = 0; r < 3; ++r)
            {
                if (in_a[static_cast<std::size_t>(cellAt(b, r, c))])
                {
                    return false;
                }
            }
        }
        return true;
    }
    return true;
}

/**
 * For block `newBlock`, only compares `candidate` to already placed neighbors
 * in kRowColNeighborBlockIndices — no other consistency tests.
 */
bool isNewSquareRowColumnCompatibleWithPlaced(const std::array<square, 9> &placed, std::size_t newBlock,
                                              const square &candidate)
{
    for (int nj : kRowColNeighborBlockIndices[newBlock])
    {
        const std::size_t j = static_cast<std::size_t>(nj);
        if (j >= newBlock)
        {
            continue;
        }
        if (!areNeighborBlocksRowColumnConsistent(candidate, newBlock, placed[j], j))
        {
            return false;
        }
    }
    return true;
}

/**
 * Counts assignments with one catalog permutation per block (0..8) such that
 * only the neighbor row/column checks in isNewSquareRowColumnCompatibleWithPlaced
 * are applied — no other invalidity tests.
 *
 * ! DO NOT RUN THIS, IT WILL NEVER FINISH AND CRASH, THIS IS ONLY MEANT AS AN ILLUSTRATIVE EXAMPLE
 */

std::uint64_t Board::countAllAssignmentsPassingRowColumnNeighborPruning()
{
    const std::vector<square> &allSquarePerms = getAllSquarePermutationsAsSquares();
    std::array<square, 9> placed{};

    std::uint64_t total = 0;
    const auto dfs = [&](auto &&self, std::size_t k) -> void {
        if (k == 9)
        {
            ++total;
            return;
        }
        for (const square &cand : allSquarePerms)
        {
            if (!isNewSquareRowColumnCompatibleWithPlaced(placed, k, cand))
            {
                continue;
            }
            placed[k] = cand;
            self(self, k + 1);
        }
    };
    dfs(dfs, 0U);
    return total;
}
