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
enum PromotionType {
	NONE, QUEEN, ROOK, KNIGHT, BISHOP
};
}
namespace SpecMoveType{
enum SpecMoveType {
	NONE, DOUBLE_PAWN_PUSH, EN_PASSANT, CASTLE_KING, CASTLE_QUEEN
};
}
}

class Move {
public:
	short sq_origin;
	short sq_target;
	bool is_capture;
	bool is_check;
	bool is_checkmate;
	bitchess::PieceType piece;
	move::PromotionType::PromotionType promotion_type;
	move::SpecMoveType::SpecMoveType special_move;

	std::string get_in_algebraic();
	std::string get_in_coordinate();
};

}
#endif /* POSITION_MOVE_HPP_ */
