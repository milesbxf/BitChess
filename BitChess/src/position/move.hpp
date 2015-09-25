/*
 * move.h
 *
 *  Created on: 24 Sep 2015
 *      Author: miles
 */

#ifndef POSITION_MOVE_HPP_
#define POSITION_MOVE_HPP_

#include "util/util.hpp"

namespace bitchess {

namespace move {
namespace PromotionType {
/**
 * Represents the type of pawn promotion in this move: either NONE
 * or the piece that the pawn is promoted to.
 */
enum PromotionType {
	NONE, QUEEN, ROOK, KNIGHT, BISHOP
};
}
namespace SpecMoveType{
/**
 * Represents the type of special move.
 */
enum SpecMoveType {
	NONE, DOUBLE_PAWN_PUSH, EN_PASSANT, CASTLE_KING, CASTLE_QUEEN
};
}
}

/**
 * This data structure represents a half-move in chess; that is, the movement of
 * a piece from one square to another (other than the castling move, which is the
 * movement of two pieces).
 */
class Move {
public:
	short sq_origin;	//origin square, where piece is moved from
	short sq_target;	//target square, where piece is moved to
	bool is_capture;	//whether move results in capture of an opposing piece
	bool is_check;		//whether move results in check of opposing king
	bool is_checkmate;	//whether move results in checkmate of opposing side
	bitchess::PieceType piece; //piece that moves
	move::PromotionType::PromotionType promotion_type; //type of promotion, if any
	move::SpecMoveType::SpecMoveType special_move; //type of special move, if any

	/**
	 * Gets the move in standard algebraic notation, e.g. bishop to e4 is Be4. See
	 * https://en.wikipedia.org/wiki/Algebraic_notation_(chess) for more info.
	 * @return the move described in standard algebraic notation.
	 */
	std::string get_in_algebraic();
	/**
	 * Gets the move in coordinate notation, e.g. moving a piece from d4 to h4 would be
	 * d4h4.
	 * @return the move in coordinate notation.
	 */
	std::string get_in_coordinate();
};

}
#endif /* POSITION_MOVE_HPP_ */
