/*
 * movelookup.hpp
 *
 *  Created on: 25 Sep 2015
 *      Author: miles
 */

#ifndef UTIL_MOVELOOKUP_HPP_
#define UTIL_MOVELOOKUP_HPP_

#include "bitboard.hpp"
#include "util.hpp"

#include <array>
#include <map>
#include <cassert>

namespace {

static bitchess::Bitboard __gen_king_move__(short sq) {
	bitchess::Bitboard occ = bitchess::Bitboard::with_bit_set_at(sq);
		bitchess::Bitboard b;

		b = occ.nortOne();
		b |= occ.noEaOne();
		b |= occ.eastOne();
		b |= occ.soEaOne();
		b |= occ.soutOne();
		b |= occ.soWeOne();
		b |= occ.westOne();
		b |= occ.noWeOne();

		return b;
}

static std::array<bitchess::Bitboard, 64> __init_king_move_arr__() {
	std::array<bitchess::Bitboard, 64> arr;
	for(int sq = 0; sq < 64; ++sq) {
		arr[sq] = __gen_king_move__(sq);
	}
	return arr;
}


static std::array<bitchess::Bitboard, 64> __lookup_king_move__() {
	static std::array<bitchess::Bitboard, 64> arr = __init_king_move_arr__();
	return arr;
}

typedef std::map<bitchess::PieceType::PieceType,std::array<bitchess::Bitboard, 64>> PieceBitBoardMoveMap;

static PieceBitBoardMoveMap __lookup_piece_map__() {
	static PieceBitBoardMoveMap map = {
		{bitchess::PieceType::PieceType::KING, __init_king_move_arr__() }
	};
	return map;
}

}


namespace bitchess {

bitchess::Bitboard move_lookup(bitchess::PieceType::PieceType piece, short sq) {
	assert(__lookup_piece_map__().count(piece) > 0);
	return __lookup_piece_map__()[piece][sq];
}

}

#endif /* UTIL_MOVELOOKUP_HPP_ */
