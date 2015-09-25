/*
 * position.h
 *
 *  Created on: 24 Sep 2015
 *      Author: miles
 */

#ifndef POSITION_POSITION_HPP_
#define POSITION_POSITION_HPP_

#include <map>
#include <array>
#include <vector>

namespace bitchess {

class Position {
public:
	Position();
	Position(std::string FEN_string);

	std::map<PieceType, int> get_piece_counts();
	std::vector<Move> get_all_legal_moves(Colour side_to_move);


private:
	std::vector<Move> test_checks(Colour side_to_move, std::vector<Move> &move_list);

	std::map<Colour, std::map<PieceType,pieces::Piece>> map_pieces;
	std::array<*pieces::Piece,64> array_pieces;

};

}

#endif /* POSITION_POSITION_HPP_ */
