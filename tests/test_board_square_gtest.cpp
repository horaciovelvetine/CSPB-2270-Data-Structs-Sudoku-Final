#include "board.hpp"

#include <array>
#include <gtest/gtest.h>
#include <random>

// Defined in `sp1_all_square_permutations.cpp`.
square getSquarePermutation(std::size_t index);

// Row-major 9x9: index = r * 9 + c.
void fillBoardFromRowMajor9x9(Board &board, const std::array<int, 81> &g)
{
    for (std::size_t br = 0; br < 3; ++br)
    {
        for (std::size_t bc = 0; bc < 3; ++bc)
        {
            const std::size_t block = br * 3 + bc;
            std::array<int, 9> cells{};
            for (int r = 0; r < 3; ++r)
            {
                for (int c = 0; c < 3; ++c)
                {
                    const std::size_t gr = br * 3 + static_cast<std::size_t>(r);
                    const std::size_t gc = bc * 3 + static_cast<std::size_t>(c);
                    cells[static_cast<std::size_t>(r * 3 + c)] = g[gr * 9 + gc];
                }
            }
            board.setSquare(square(cells), block);
        }
    }
}


//! ========================================================================================>
//! === SQUARE.hpp Tests
//! ========================================================================================>

TEST(Square, DefaultUnfilledIsNotValid)
{
    square s;
    EXPECT_FALSE(s.isValid());
}

TEST(Square, FirstPermutationIsValid)
{
    const square s = getSquarePermutation(0);
    EXPECT_TRUE(s.isValid());
    for (int i = 0; i < 9; ++i)
    {
        EXPECT_EQ(i + 1, s.valueAt(static_cast<std::size_t>(i)));
    }
}

TEST(Square, DuplicateMakesInvalid)
{
    const std::array<int, 9> dup{1, 1, 2, 3, 4, 5, 6, 7, 8};
    EXPECT_FALSE(square(dup).isValid());
}

TEST(Square, OutOfRangeMakesInvalid)
{
    const std::array<int, 9> bad{0, 2, 3, 4, 5, 6, 7, 8, 9};
    EXPECT_FALSE(square(bad).isValid());
}

//! ========================================================================================>
//! === BOARD.hpp Tests
//! ========================================================================================>

TEST(Board, SolvedPuzzleIsValid)
{
    // A completed, valid 9x9 solution (row-major of full grid).
    const std::array<int, 81> solved = {
        5, 3, 4, 6, 7, 8, 9, 1, 2, //
        6, 7, 2, 1, 9, 5, 3, 4, 8, //
        1, 9, 8, 3, 4, 2, 5, 6, 7, //
        8, 5, 9, 7, 6, 1, 4, 2, 3, //
        4, 2, 6, 8, 5, 3, 7, 9, 1, //
        7, 1, 3, 9, 2, 4, 8, 5, 6, //
        9, 6, 1, 5, 3, 7, 2, 8, 4, //
        2, 8, 7, 4, 1, 9, 6, 3, 5, //
        3, 4, 5, 2, 8, 6, 1, 7, 9, //
    };
    Board b;
    fillBoardFromRowMajor9x9(b, solved);
    EXPECT_TRUE(b.isValid());
    for (std::size_t r = 0; r < 9; ++r)
    {
        EXPECT_TRUE(b.isRowValid(r));
    }
    for (std::size_t c = 0; c < 9; ++c)
    {
        EXPECT_TRUE(b.isColumnValid(c));
    }
}

TEST(Board, EmptyBoardIsNotValid)
{
    Board b;
    EXPECT_FALSE(b.isValid());
}

TEST(Board, RepeatingSameBlockInEverySubsquareIsNotValid)
{
    // Each 3x3 is a valid 1..9, but every row of the 9x9 has repeated digits.
    Board b;
    const square block = getSquarePermutation(0);
    for (std::size_t i = 0; i < 9; ++i)
    {
        b.setSquare(block, i);
    }
    EXPECT_TRUE(b.getSquare(0).isValid());
    EXPECT_FALSE(b.isValid());
}

TEST(Board, IsRowColumnBandStackHelpersConsistent)
{
    const std::array<int, 81> solved = {
        5, 3, 4, 6, 7, 8, 9, 1, 2, //
        6, 7, 2, 1, 9, 5, 3, 4, 8, //
        1, 9, 8, 3, 4, 2, 5, 6, 7, //
        8, 5, 9, 7, 6, 1, 4, 2, 3, //
        4, 2, 6, 8, 5, 3, 7, 9, 1, //
        7, 1, 3, 9, 2, 4, 8, 5, 6, //
        9, 6, 1, 5, 3, 7, 2, 8, 4, //
        2, 8, 7, 4, 1, 9, 6, 3, 5, //
        3, 4, 5, 2, 8, 6, 1, 7, 9, //
    };
    Board b;
    fillBoardFromRowMajor9x9(b, solved);
    for (std::size_t band = 0; band < 3; ++band)
    {
        EXPECT_TRUE(b.isRowBandValid(band));
    }
    for (std::size_t stack = 0; stack < 3; ++stack)
    {
        EXPECT_TRUE(b.isColumnStackValid(stack));
    }
}
