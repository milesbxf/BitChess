/*
 * pieces.h
 *
 *  Created on: 24 Sep 2015
 *      Author: miles
 */

#ifndef POSITION_PIECES_PIECES_HPP_
#define POSITION_PIECES_PIECES_HPP_

#include "util/bitboard.hpp"
#include "position/move.hpp"

#include <vector>

namespace bitchess {
namespace pieces {

/**
 * The Piece abstract class represents each type of piece on the board. It is intended
 * to be overridden to represent more specific pieces e.g. a subclass might represent the
 * white rooks. It can evaluate the pseudolegal moves available; these moves require
 * further testing to ensure they do not result in check.
 */
class Piece {
public:
	/**
	 * Gets all pseudolegal moves available to the Piece(s). These moves may result in
	 * the king being in check, therefore they are not guaranteed to be legal and must be
	 * tested.
	 * @return a vector of pseudolegal moves.
	 */
	virtual std::vector<bitchess::Move> get_pseudolegal_moves();
	virtual ~Piece();

protected:
	/**
	 * Initialises the piece with empty occupancy, i.e. it is not on the board.
	 */
	Piece();
	/**
	 * Initialises the piece with occupancy on initial_sq. Further squares may be occupied.
	 * @param initial_sq Initial square that the piece occupies.
	 */
	Piece(int initial_sq);

private:
	bitchess::Bitboard occupancy;
};

class PieceNone : public Piece {
};

class Rook : public Piece {
};

class Bishop : public Piece {
};

class Queen : public Piece {
};

class King : public Piece {
};

class Knight : public Piece {
};

class Pawn : public Piece {
};

}

}

#endif /* POSITION_PIECES_PIECES_HPP_ */
