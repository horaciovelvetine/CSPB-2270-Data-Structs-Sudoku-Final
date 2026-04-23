/**
 * Scaled-down 2x2 variant: enumerate every valid shidoku board by
 * **trying row pairs, then checking column pairs.**
 *
 * Uses the precomputed compatibility tables exported by
 * `sp4_row_column_independence_2x2.cpp`:
 *
 *      ValidRowPairs    = { (x, y) : x can sit LEFT  of y and fill both rows }
 *      ValidColumnPairs = { (x, y) : x can sit ABOVE  y and fill both cols }
 *
 * Board layout (row-major block order: A=0, B=1, C=2, D=3):
 *      +-----+-----+
 *      |  A  |  B  |
 *      +-----+-----+
 *      |  C  |  D  |
 *      +-----+-----+
 *
 * A (A, B, C, D) tuple is a valid shidoku board iff **all four** of the
 * following hold, which we verify strictly by table lookup:
 *      (A, B) in ValidRowPairs     (top rows fill {1..4})
 *      (C, D) in ValidRowPairs     (bottom rows fill {1..4})
 *      (A, C) in ValidColumnPairs  (left columns fill {1..4})
 *      (B, D) in ValidColumnPairs  (right columns fill {1..4})
 *
 * Algorithm ("try rows, check columns"):
 *      for (A, B) in ValidRowPairs:
 *          for (C, D) in ValidRowPairs:
 *              if (A, C) in ValidColumnPairs AND (B, D) in ValidColumnPairs:
 *                  emit (A, B, C, D)
 *
 * The column-pair membership test is folded into 24 32-bit bitmasks so the
 * inner check is a pair of `(mask >> idx) & 1` ops. Contrast with the
 * set-merge formulation in `sp5_try_row_column_merge_2x2.cpp`, which instead
 * intersects R+(C) with C-(B) at block D.
 */

#include <array>
#include <cstddef>
#include <cstdint>
#include <utility>
#include <vector>

// Defined in `sp4_row_column_independence_2x2.cpp`.
extern const std::vector<std::pair<int, int>> ValidRowPairs;
extern const std::vector<std::pair<int, int>> ValidColumnPairs;

constexpr std::size_t kNumPerms2x2 = 24; // 4!

/**
 * One shidoku board as the four catalog indices of its 2x2 blocks in
 * row-major block order: (A, B, C, D) at block positions (0, 1, 2, 3).
 *
 * Kept token-identical to the sibling definition in
 * `sp5_try_row_column_merge_2x2.cpp` so the two TUs agree under ODR.
 */
struct ShidokuBoardIndices
{
    std::size_t a;
    std::size_t b;
    std::size_t c;
    std::size_t d;
};

/**
 * 24-entry lookup where bit y of entry x is set iff (x, y) in ValidColumnPairs.
 * Lets the vertical-pair membership test collapse to `(mask[x] >> y) & 1`.
 */
const std::array<std::uint32_t, kNumPerms2x2> &getValidColumnPairMasks2x2()
{
    static const auto masks = [] {
        std::array<std::uint32_t, kNumPerms2x2> out{};
        for (const std::pair<int, int> &p : ValidColumnPairs)
        {
            const std::size_t x = static_cast<std::size_t>(p.first);
            const std::size_t y = static_cast<std::size_t>(p.second);
            out[x] |= (std::uint32_t{1} << y);
        }
        return out;
    }();
    return masks;
}

/** True iff (x, y) in ValidColumnPairs, via a single bit test. */
bool isValidColumnPair2x2(std::size_t x, std::size_t y)
{
    const auto &masks = getValidColumnPairMasks2x2();
    return ((masks[x] >> y) & 1U) != 0U;
}

/**
 * Counts every (A, B, C, D) whose top and bottom row pairs come from
 * ValidRowPairs and whose induced column pairs (A, C) and (B, D) both
 * appear in ValidColumnPairs.
 *
 * Returns 288 — the standard enumeration of distinct shidoku grids —
 * matching `countAllShidokuBoardsBySetMerge` in the sibling TU.
 */
std::uint64_t countAllShidokuBoardsByTryRowsCheckColumns()
{
    std::uint64_t total = 0;
    for (const std::pair<int, int> &top : ValidRowPairs)
    {
        const std::size_t a = static_cast<std::size_t>(top.first);
        const std::size_t b = static_cast<std::size_t>(top.second);
        for (const std::pair<int, int> &bot : ValidRowPairs)
        {
            const std::size_t c = static_cast<std::size_t>(bot.first);
            const std::size_t d = static_cast<std::size_t>(bot.second);
            if (isValidColumnPair2x2(a, c) && isValidColumnPair2x2(b, d))
            {
                ++total;
            }
        }
    }
    return total;
}

/**
 * Materializes every valid shidoku board as a 4-tuple of catalog indices
 * via the "try rows, check columns" strategy. Output size is 288 entries,
 * in the order produced by the nested ValidRowPairs iteration (top outer,
 * bottom inner). Ordering is not expected to match the sibling routine.
 */
std::vector<ShidokuBoardIndices> getAllShidokuBoardsByTryRowsCheckColumns()
{
    std::vector<ShidokuBoardIndices> boards;
    boards.reserve(288);
    for (const std::pair<int, int> &top : ValidRowPairs)
    {
        const std::size_t a = static_cast<std::size_t>(top.first);
        const std::size_t b = static_cast<std::size_t>(top.second);
        for (const std::pair<int, int> &bot : ValidRowPairs)
        {
            const std::size_t c = static_cast<std::size_t>(bot.first);
            const std::size_t d = static_cast<std::size_t>(bot.second);
            if (isValidColumnPair2x2(a, c) && isValidColumnPair2x2(b, d))
            {
                boards.push_back({a, b, c, d});
            }
        }
    }
    return boards;
}
