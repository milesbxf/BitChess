/*
 * search.h
 *
 *  Created on: 24 Sep 2015
 *      Author: miles
 */

#ifndef GAME_EVALUATION_SEARCH_HPP_
#define GAME_EVALUATION_SEARCH_HPP_

#include "../../util/util.hpp"
#include "evaluator.hpp"


namespace bitchess {

class Search {
public:
	Move get_next_move(Position position, Colour side_to_move);

private:
	Evaluator evaluator;
};

}

#endif /* GAME_EVALUATION_SEARCH_HPP_ */
