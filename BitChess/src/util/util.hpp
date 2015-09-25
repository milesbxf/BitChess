/*
 * util.h
 *
 *  Created on: 24 Sep 2015
 *      Author: miles
 */

#ifndef UTIL_UTIL_HPP_
#define UTIL_UTIL_HPP_

namespace bitchess {

/**
 * Represents a side in chess.
 */
enum Colour { WHITE, BLACK };
namespace PieceType {
/**
 * Represents a type of piece, or NO_PIECE.
 */
enum PieceType { ROOK, KNIGHT, BISHOP, QUEEN, KING, PAWN, NO_PIECE };

}
}

#endif /* UTIL_UTIL_HPP_ */
