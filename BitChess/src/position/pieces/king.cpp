/*
 * king.cpp
 *
 *  Created on: 29 Sep 2015
 *      Author: miles
 */

#include "pieces.hpp"
#include "position/move.hpp"

#include <vector>


using bitchess::pieces::King;

/**
 * Gets all pseudolegal moves available to the King. Some moves may result in the
 * king moving into check, therefore not being legal; these must be checked.
 * @return a vector of pseudolegal moves.
 */
std::vector<bitchess::Move> King::get_pseudolegal_moves(
		Bitboard all_occupancy, Bitboard own_occupancy,
		Bitboard opp_occupancy ) {

	assert(occupancy.bits.count() == 1); // kings should always be on the board!

	//get king location
	short king_loc = occupancy.ls1b();
	assert(king_loc >= 0); //king_loc would be -1 if no bits are set, however this is impossible

	// calc bitboards of moves and captures
	Bitboard king_moves = bitchess::move_lookup(bitchess::PieceType::KING,
												king_loc)
							& ~all_occupancy;
	Bitboard king_caps = bitchess::move_lookup(bitchess::PieceType::KING,
												king_loc)
							& opp_occupancy;

	// count number of moves and captures
	short num_moves = king_moves.bits.count();
	short num_caps = king_caps.bits.count();

	// prealloc vector with resulting pseudolegal moves
	std::vector<bitchess::Move> moves(num_moves + num_caps);

	// iterate over bits of king_moves and create moves, allocating to moves[i]
	for ( int i = 0; i < num_moves; ++i ) {
		__scan_moves__(king_loc, king_moves, moves[i], false);
	}
	// same for king_caps
	for ( int i = 0; i < num_caps; ++i ) {
		__scan_moves__(king_loc, king_caps, moves[num_moves + i], true);
	}

	return moves;
}
