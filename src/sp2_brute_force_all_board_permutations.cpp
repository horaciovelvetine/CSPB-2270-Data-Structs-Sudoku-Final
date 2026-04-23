#include "board.hpp"

#include <cstddef>
#include <vector>

// Defined in `sp1_all_square_permutations.cpp`.
const std::vector<square> &getAllSquarePermutationsAsSquares();

/**
 * @brief Recursively generates all possible Sudoku boards by filling each block with every 1..9 permutation.
 *
 * For each 3x3 block (0..8), sets every possible permutation of 1..9, recursing to fill the next block.
 * When all blocks are set (blockIndex == 9), the constructed board is added to @p out.
 *
 * @param blockIndex Current 3x3 block index (0..9).
 * @param board      Board being filled.
 * @param perms      All 9! valid sub-square permutations.
 * @param out        Output vector for complete boards.
 *
 * @note Generates (9!)^9 boards; intended solely for benchmarking or theoretical use.
 */
void fillBoardPermutations(std::size_t blockIndex, Board &board, const std::vector<square> &perms,
                           std::vector<Board> &out)
{
    if (blockIndex == 9)
    {
        out.push_back(board);
        return;
    }
    for (std::size_t pi = 0; pi < perms.size(); ++pi)
    {
        board.setSquare(perms[pi], blockIndex);
        fillBoardPermutations(blockIndex + 1, board, perms, out);
    }
}

/**
 * ! DO NOT RUN THIS, IT WILL NEVER FINISH AND CRASH, THIS IS ONLY MEANT AS AN ILLUSTRATIVE EXAMPLE
 */
std::vector<Board> Board::bruteForceAllBoardPermutations()
{
    const std::vector<square> &perms = getAllSquarePermutationsAsSquares();
    std::vector<Board> result;
    //!! IMPOSSIBLE: (9!)^9 boards; reserve would be impossible
    Board b;
    fillBoardPermutations(0, b, perms, result);
    return result;
}
