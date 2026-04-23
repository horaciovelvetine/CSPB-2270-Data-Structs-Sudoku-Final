#pragma once

#include <array>
#include <cstddef>
#include <iostream>

/**
 * @brief Represents a single 3x3 sub-square of a Sudoku board.
 *
 * A square owns nine integer cells, indexed in row-major order
 * (0 = top-left, 8 = bottom-right within the 3x3 block). Cells use
 * the values 1..9 once filled; any other value is treated as empty
 * or invalid by `isValid()` and `print()`.
 */
class square
{
  private:
    // /** @brief Number of distinct orderings of the digits 1..9 (9! == 362880). */
    // static constexpr std::size_t kFactorial9 = 9 * 8 * 7 * 6 * 5 * 4 * 3 * 2; // (* 1) implied...

    /** @brief The nine cell values of this 3x3 block, row-major. */
    std::array<int, 9> values;

  public:
    /**
     * @brief Constructs an empty square with all nine cells zero-initialized.
     */
    square() : values{}
    {
    }

    /**
     * @brief Constructs a square initialized from the given nine cell values.
     *
     * The input array is copied directly into the square's internal cells in
     * row-major order (index 0 = top-left, index 8 = bottom-right). No
     * validation is performed on the values themselves; use `isValid()` to
     * check whether they form a legal 1..9 ordering.
     *
     * @param values The nine cell values to copy into this square.
     */
    explicit square(const std::array<int, 9> &values) : values(values)
    {
    }

    /** @brief Default destructor; the square owns only POD data. */
    ~square() = default;

    /**
     * @brief Checks if the square contains all numbers from 1 to 9 exactly once.
     *
     * Verifies that the internal array of values contains each integer from
     * 1 to 9 with no duplicates and no values outside the 1..9 range.
     *
     * @return true if the square is valid (contains each of 1..9 exactly
     *         once), false otherwise.
     */
    bool isValid() const
    {
        bool seen[10] = {};

        for (int val : values)
        {
            if (val < 1 || val > 9 || seen[val])
            {
                return false;
            }
            seen[val] = true;
        }
        return true;
    }

    /**
     * @brief Returns the value of the cell at the given index.
     *
     * The cell index is row-major within this 3x3 block (0 = top-left,
     * 8 = bottom-right).
     *
     * @param idx Zero-based cell index in `[0, 9)`.
     * @return The integer value stored at that cell.
     * @throws std::out_of_range if `idx >= 9`.
     */
    int valueAt(std::size_t idx) const
    {
        return values.at(idx);
    }

    /**
     * @brief Prints this 3x3 block to `std::cout` with grid lines.
     *
     * Cells outside the range 1..9 (e.g. zero-initialized empty cells)
     * are rendered as a `.`.
     */
    void print() const
    {
        const char *row_sep = "+---+---+---+\n";
        std::cout << row_sep;
        for (int r = 0; r < 3; ++r)
        {
            std::cout << '|';
            for (int c = 0; c < 3; ++c)
            {
                const int v = values[static_cast<std::size_t>(r * 3 + c)];
                const char ch = (v >= 1 && v <= 9) ? static_cast<char>('0' + v) : '.';
                std::cout << ' ' << ch << ' ' << '|';
            }
            std::cout << '\n' << row_sep;
        }
    }
};
