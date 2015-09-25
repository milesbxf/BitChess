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

/**
 * Generates king moves for a specified square.
 * @param sq Square index to get a move for.
 * @return Bitboard with 1 indicating a valid move.
 */
static bitchess::Bitboard __gen_king_move__( short sq ) {

	// get a bitboard with the occupancy of the king
	bitchess::Bitboard occ = bitchess::Bitboard::with_bit_set_at(sq);
	bitchess::Bitboard b;

	// shift the occupancy bitboard in each direction to represent
	// a king move
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

/**
 * Gets an array of king moves, indexed by square.
 * @return
 */
static std::array<bitchess::Bitboard, 64> __init_king_move_arr__() {
	std::array<bitchess::Bitboard, 64> arr;
	// generate a move bitboard for each square on the board
	for ( int sq = 0; sq < 64; ++sq ) {
		arr[sq] = __gen_king_move__(sq);
	}
	return arr;
}

/**
 * Used to shorten the declaration. May be used map[piece][sq].
 */
typedef std::map<bitchess::PieceType::PieceType,
		std::array<bitchess::Bitboard, 64>> PieceBitBoardMoveMap;

/**
 * @return the static piece map. Is used map[piece][sq]
 */
static PieceBitBoardMoveMap __lookup_piece_map__() {
	static PieceBitBoardMoveMap map = { { bitchess::PieceType::PieceType::KING,
											__init_king_move_arr__() } };
	return map;
}

}

namespace bitchess {

/**
 * Gets a bitboard of possible moves depending on the piece and the square.
 * @param piece Piece to get moves for. If a piece is invalid (e.g. NO_PIECE) or
 * unimplemented, an assertion will fail. Currently only KING moves are supported.
 * @param sq Board square index in range [0,64]/
 * @return bitboard of possible moves.
 */
bitchess::Bitboard move_lookup( bitchess::PieceType::PieceType piece,
								short sq ) {
	// check that piece is in lookup map.
	assert(__lookup_piece_map__().count(piece) > 0);
	return __lookup_piece_map__()[piece][sq];
}

}

#endif /* UTIL_MOVELOOKUP_HPP_ */
