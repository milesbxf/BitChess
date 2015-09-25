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

class Piece {
public:
	Piece();
	Piece(int initial_sq);
	virtual std::vector<bitchess::Move> get_pseudolegal_moves();
	virtual ~Piece();

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
