/*
 * pawn.hpp
 *
 *  Created on: 25 Sep 2015
 *      Author: miles
 */

#include "pieces.hpp"
#include "position/move.hpp"

#include <vector>

using bitchess::Bitboard;
using bitchess::Move;
using bitchess::pieces::Pawn;

Bitboard Pawn::get_single_moves(Colour colour,Bitboard all_occupancy) {
	Bitboard occ_wo_pawns = all_occupancy & ~occupancy;

	Bitboard shifted_one;
	if(colour==WHITE) {
		shifted_one = occupancy.nortOne();
	} else {
		shifted_one = occupancy.soutOne();
	}

	return shifted_one & ~occ_wo_pawns;
}

std::vector<Move> Pawn::get_pseudolegal_moves() {
	return std::vector<Move>();
}
