#include "board.hpp"

#include <algorithm>
#include <array>
#include <cstddef>
#include <iostream>
#include <random>
#include <vector>

bool Board::isRowValid(std::size_t r) const
{
    bool seen[10] = {};
    for (std::size_t c = 0; c < 9; ++c)
    {
        const std::size_t sq = (r / 3) * 3 + (c / 3);
        const std::size_t cell = (r % 3) * 3 + (c % 3);
        const int v = squares[sq].valueAt(cell);
        if (v < 1 || v > 9 || seen[v])
        {
            return false;
        }
        seen[v] = true;
    }
    return true;
}

bool Board::isColumnValid(std::size_t c) const
{
    bool seen[10] = {};
    for (std::size_t r = 0; r < 9; ++r)
    {
        const std::size_t sq = (r / 3) * 3 + (c / 3);
        const std::size_t cell = (r % 3) * 3 + (c % 3);
        const int v = squares[sq].valueAt(cell);
        if (v < 1 || v > 9 || seen[v])
        {
            return false;
        }
        seen[v] = true;
    }
    return true;
}

bool Board::isRowBandValid(std::size_t band) const
{
    const std::size_t start = band * 3;
    return isRowValid(start) && isRowValid(start + 1) && isRowValid(start + 2);
}

bool Board::isColumnStackValid(std::size_t stack) const
{
    const std::size_t start = stack * 3;
    return isColumnValid(start) && isColumnValid(start + 1) && isColumnValid(start + 2);
}

bool Board::isValid() const
{
    // Sub-squares first; any block that isn't itself a permutation of 1..9
    // immediately disqualifies the board and saves the row/column scans.
    for (std::size_t i = 0; i < 9; ++i)
    {
        if (!squares[i].isValid())
        {
            return false;
        }
    }

    for (std::size_t r = 0; r < 9; ++r)
    {
        if (!isRowValid(r))
        {
            return false;
        }
    }

    for (std::size_t c = 0; c < 9; ++c)
    {
        if (!isColumnValid(c))
        {
            return false;
        }
    }

    return true;
}

// bool Board::fillRandomValid(std::mt19937 &rng)
// {
//     LineUsage lineUsage{};
//     return tryFillBlocksInOrder(*this, 0, lineUsage, rng);
// }
