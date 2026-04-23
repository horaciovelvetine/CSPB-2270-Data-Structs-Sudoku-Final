#pragma once

#include "square.hpp"

#include <array>
#include <cstddef>
#include <cstdint>
#include <random>
#include <vector>

/**
 * @brief Represents a full 9x9 Sudoku board as nine 3x3 sub-squares.
 *
 * The board owns nine `square` objects laid out in row-major block order:
 *
 *   0 1 2
 *   3 4 5
 *   6 7 8
 *
 * Within each square, cells are also indexed in row-major order (0..8),
 * so a board cell at `(row r, col c)` maps to:
 *
 *   square index = (r / 3) * 3 + (c / 3)
 *   cell index   = (r % 3) * 3 + (c % 3)
 */
class Board
{
  private:
    /** @brief The nine 3x3 sub-squares making up this board. */
    std::array<square, 9> squares;

  public:
    /** @brief Constructs an empty board with all nine sub-squares default-initialized. */
    Board() = default;

    /** @brief Default destructor; the board owns only its array of squares. */
    ~Board() = default;

    /**
     * @brief Checks whether the entire board satisfies Sudoku constraints.
     *
     * The board is considered valid iff:
     *   - every 3x3 sub-square contains the digits 1..9 exactly once,
     *   - every row of the 9x9 grid contains the digits 1..9 exactly once, and
     *   - every column of the 9x9 grid contains the digits 1..9 exactly once.
     *
     * @return true if all rows, columns, and sub-squares are valid; false otherwise.
     */
    bool isValid() const;

    /**
     * @brief Returns true iff row `r` of the 9x9 board contains 1..9 exactly once.
     *
     * @param r Zero-based row index in `[0, 9)`.
     */
    bool isRowValid(std::size_t r) const;

    /**
     * @brief Returns true iff column `c` of the 9x9 board contains 1..9 exactly once.
     *
     * @param c Zero-based column index in `[0, 9)`.
     */
    bool isColumnValid(std::size_t c) const;

    /**
     * @brief Returns true iff all three rows in the given row-band are valid.
     *
     * @param band Zero-based row-band index (0 = top, 1 = middle, 2 = bottom).
     */
    bool isRowBandValid(std::size_t band) const;

    /**
     * @brief Returns true iff all three columns in the given column-stack are valid.
     *
     * @param stack Zero-based column-stack index (0 = left, 1 = middle, 2 = right).
     */
    bool isColumnStackValid(std::size_t stack) const;

    /**
     * @brief Returns a mutable reference to the sub-square at `idx`.
     *
     * @param idx Zero-based block index in `[0, 9)`, in row-major block order.
     * @return Reference to the requested `square`.
     * @throws std::out_of_range if `idx >= 9`.
     */
    square &getSquare(std::size_t idx)
    {
        return squares.at(idx);
    }

    /**
     * @brief Returns a const reference to the sub-square at `idx`.
     *
     * @param idx Zero-based block index in `[0, 9)`, in row-major block order.
     * @return Const reference to the requested `square`.
     * @throws std::out_of_range if `idx >= 9`.
     */
    const square &getSquare(std::size_t idx) const
    {
        return squares.at(idx);
    }

    /**
     * @brief Sets the sub-square at `idx` to a copy of `sq`.
     *
     * @param sq  The `square` instance to copy into the board.
     * @param idx Zero-based block index in `[0, 9)`, in row-major block order.
     * @throws std::out_of_range if `idx >= 9`.
     */
    void setSquare(const square &sq, std::size_t idx)
    {
        squares.at(idx) = sq;
    }

    /**
     * @brief Returns a mutable reference to all nine sub-squares.
     *
     * @return Reference to the underlying `std::array` of sub-squares.
     */
    std::array<square, 9> &getSquares()
    {
        return squares;
    }

    /**
     * @brief Returns a const reference to all nine sub-squares.
     *
     * @return Const reference to the underlying `std::array` of sub-squares.
     */
    const std::array<square, 9> &getSquares() const
    {
        return squares;
    }

    /**
     * @brief Prints all nine 3x3 sub-squares to `std::cout`.
     *
     * Sub-squares are printed in row-major block order (0 = top-left,
     * 8 = bottom-right). Each sub-square is rendered by `square::print()`
     * and consecutive sub-squares are separated by a blank line.
     */
    void print() const
    {
        for (std::size_t i = 0; i < 9; ++i)
        {
            squares[i].print();
            if (i + 1 < 9)
            {
                std::cout << '\n';
            }
        }
    }

    /**
     * @brief Enumerates all boards formed by choosing one of 9! sub-squares per block.
     *
     * There are (9!)^9 such boards; this is infeasible to run. Present for
     * specification; implementation lives in `sp2_brute_force_all_board_permutations.cpp`.
     */
    static std::vector<Board> bruteForceAllBoardPermutations();

    /**
     * @brief Counts block assignments (one 9! permutation per block) that pass only
     * pairwise row/column neighbor pruning between already-placed blocks.
     *
     * For each block, candidates are the same catalog as `getAllSquarePermutationsAsSquares()`.
     * When placing block `k`, a candidate is rejected if it disagrees on a shared 9-wide row or
     * column with any *already placed* lower-index neighbor in the fixed neighbor table — no
     * full-board validity pass and no other Sudoku rules beyond that pruning.
     *
     * The search space is still astronomically large; the count is defined for theory/reference
     * and is not expected to complete in practice. Implementation lives in
     * `sp3_row_column_prune_board_permutations.cpp`.
     */
    static std::uint64_t countAllAssignmentsPassingRowColumnNeighborPruning();
};
