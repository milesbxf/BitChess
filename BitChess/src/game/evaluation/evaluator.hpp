/*
 * evaluator.h
 *
 *  Created on: 24 Sep 2015
 *      Author: miles
 */

#ifndef GAME_EVALUATION_EVALUATOR_HPP_
#define GAME_EVALUATION_EVALUATOR_HPP_


namespace bitchess {

class Evaluator {
public:
	static float evaluate(Position position, Colour side_to_move);
};

}

#endif /* GAME_EVALUATION_EVALUATOR_HPP_ */
