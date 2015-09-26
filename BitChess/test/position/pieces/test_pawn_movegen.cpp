/*
 * test_pawn_movegen.cpp
 *
 *  Created on: 25 Sep 2015
 *      Author: miles
 */

#include "util/util.hpp"
#include "position/pieces/pieces.hpp"

#include "gtest/gtest.h"

using bitchess::pieces::Pawn;
using bitchess::Bitboard;
using bitchess::Colour;

TEST(MoveGen_Pawn, WhiteGetsSingleMoves) {
	Bitboard whitepawn_occ = Bitboard(0xFF00);

	Bitboard overall_occ = Bitboard(0x4CFF00);

	Pawn pawn(whitepawn_occ);

	Bitboard result = pawn.get_single_moves(Colour::WHITE,overall_occ);
	Bitboard expected = 0xB30000;

	ASSERT_EQ(expected,result);
}

TEST(MoveGen_Pawn, BlackGetsSingleMoves) {
	Bitboard blackpawn_occ = Bitboard(0xFF000000000000);

	Bitboard overall_occ = Bitboard(0xFFF00000000000);

	Pawn pawn(blackpawn_occ);

	Bitboard result = pawn.get_single_moves(Colour::BLACK,overall_occ);
	Bitboard expected = 0x0F0000000000;

	ASSERT_EQ(expected,result);
}
