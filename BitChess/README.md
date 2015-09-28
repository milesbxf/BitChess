#BitChess
##Overview
BitChess is a simple C++ chess engine. It is intended to be used with [Xboard/Winboard protocol](http://www.gnu.org/software/xboard/engine-intf.html) compatible chess GUIs (with potential UCI support in the future). This doc outlines the implementation and architecture behind the project.

###Goals
[ordered by importance]
####Goals of first version
- Generation of all legal moves in a particular position
- The use of a simpler on-the-fly algorithm for generating moves for sliding pieces (rooks/bishops/queens) based on the [o^(o-2r](https://chessprogramming.wikispaces.com/Subtracting+a+rook+from+a+blocking+piece) trick (with [diagonal](https://chessprogramming.wikispaces.com/Hyperbola+Quintessence) support)
- Selection of a legal move based on a basic evaluation of a position from amount of material
- Full compatibility for use with a XBoard/Winboard GUI (could potentially reuse source from another opensource chess engine)

- Correctness over speed: whilst reasonably optimal data structures shall be used (e.g. hash maps and bitboards) the focus shall be on producing a working engine than a speedy one. Simple calculations such as king and knight move generation will be optimised through the use of lookup tables.
- Time management shall also not be implemented, as searches are effectively 1-ply (meaning only the engine's next move is considered, rather than calculating, e.g. 7 moves ahead)

####Future goals
- Evaluation of a position
- Search for optimal move based on evaluation
- Time management
- The use of [Magic](https://chessprogramming.wikispaces.com/Magic+Bitboards) [Bitboards](http://www.pradu.us/old/Nov27_2008/Buzz/research/magic/Bitboards.pdf) for rapid sliding piece move generation via table lookup
- Optimisation of move generation (e.g. generate moves in an optimal order)
- General optimisation
- UCI compatibility

###Architecture
The central controller is the **Game** class, which communicates with the user through an **XBoard protocol class** and external GUI. The **Game** class keeps track of moves played, and updates the current **Position** with moves received. The **Position** consists of a collection of **Piece** (s) which generate their own pseudo-legal **Move** (s). The **Moves** of each **Piece** on the side to move are combined in a single collection in the **Position** which lists available pseudo-legal moves. An **Evaluator** analyses the consequence of each move, assigning it a score. The **Searcher** searches through the evaluated positions to find the best next move to play, and the **Game** passes this to the interface.



##Requirements of each class 
###Board representation
For ease of access and display, each position has an [8x8 array](https://chessprogramming.wikispaces.com/8x8+Board) of pieces associated with it. Move generation and evaluation, are, however, represented by [bitboards](https://chessprogramming.wikispaces.com/Bitboards), a bitset of 64 bits each representing a square. Each piece type has an occupancy bitboard, e.g. the black rook occupancy might look like:`
1 . . . . . . 1
. . . . . . . .
. . . . . . . .
. . . . . . . .
. . . . . . . .
. . . . . . . .
. . . . . . . .
. . . . . . . .`
(which would be `100000010000...` in binary or `0x8100000000000000` in hex). Using a combination of these fundamental atomic bitboards, various shift operations (to simulate moving a piece), bit operations (e.g. bitwise `OR` to get the union of each bitset) and special bitmasks a variety of more complex bitboards can be calculated for evaluation and move generation. For example, bitwise `OR` of all the pieces generates an overall square occupancy bitboard, with `1` representing any piece:`
1 1 1 1 1 1 1 1
1 1 1 1 1 1 1 1
. . . . . . . .
. . . . . . . .
. . . . . . . .
. . . . . . . .
1 1 1 1 1 1 1 1
1 1 1 1 1 1 1 1`
The bitwise `NOT` operation gets the complement, which shows empty squares.

###`int main()`
Initialises the XBoardInterface which controls the program.

###XBoardCtrl
Is a singleton. Parses and processes XBoard commands from a given input/outputstream, controlling the `Game` class.

- constructs the class, using given input and output streams for I/O.
- Sends a list of features to enable/disable on `out`
- Polls `stdin` for input and call relevant commands

A detailed list of implementation of XBoard commands is below.

###Game
Also a singleton. Controls the decision making process, guided by signals from an external source, e.g. `XBoardCtrl`. 

- A polling function can be set as a callback
- Calls the polling function regularly
- Requests next move from the `Searcher`, passing the `current_position`.

###Searcher
Searches for the next best legal move to play.

- Uses the `Evaluator` to evaluate the `current_position`. 
- Requests a list of legal moves from the `current_position`
- evaluates the moves, selecting the most promising.

###Evaluator
Provides an evaluation of a given position.

- Counts pieces of each side on the board by value (e.g. queen = 9, rook = 5 etc) - 
- returns the difference between the sides to indicate whether white or black is "winning".

###Position
Represents a particular arrangement of pieces on the chessboard as a collection of `Piece`s, providing a list of possible legal moves and other information for evaluation.

- Generates all possible legal moves from the current position.

###Piece
Represents a type of piece (e.g. ROOK) and generates moves and other information for evaluation

- Generates possible moves for the piece
- Represents piece occupancy as a bitboard

See below for more specific information for each piece type.

###Move
Data structure which stores information about a move

- Origin square
- Target square
- Whether the move results in a pawn promotion, and if so which piece is the result {none, queen, rook, bishop, knight}
- Whether move results in a capture
- Whether move results in check
- Whether move results in checkmate
- Whether move is a special move: {none, double pawn push, en passant, king castle, queen castle}
- Piece type that moved
- get the move in algebraic notation as a string 
- get the move in coordinate notation as a string


###XBoard commands
####From XBoard to engine
Command					| Result
------------------------|--------
`xboard`				| Sent at start of engine process; if not received engine quits in error 
`protover` *N*			|`protocol_mode = 1` by default, but if this is sent `protocol_mode = N` and the engine will reply with a `feature` command.
`new`					| engine sets board to standard position, with white to move and engine playing black, with time controls reset. Do not ponder even if pondering on, reset search depth.
`variant`				| not supported, engine replies with error message
`quit`					| engine cleans up and exits
`force`					| set force mode, with engine playing neither colour. Moves should be checked for legality but no thinking, pondering or movemaking should be done.
`go`					| Leave force mode, engine plays colour on move, start clock. Begin thinking.
`playother`				| (protocol level 2), set engine to play colour not on move, start opponents clock.
`MOVE`					| sent when opponent performs a move. Check legality of move, update board. If not in force mode, stop oppo clock, start own clock and thinking, and make a move.
`usermove MOVE`			| same as above, but must be enabled as a feature
`?`						| move now
`ping N`				| after other commands have been processed, reply with `pong N`, where N is the same decimal number.
result RESULT {COMMENT}	| End game.
setboard FEN			| set board to FEN position (enabled in features)
`edit`					| put engine in 'edit' mode, where subcommands (below) are enabled.
`c`						| Edit mode: change colour.
`Pa4`					| Edit mode: place pawn of current colour on a4
`xa4`					| Edit mode: clear a4
`#`						| Edit mode: clear board
`.`						| Edit mode: leave edit mode.
`hint`					| no-op (send hint)
`bk`					| send back "Not supported"
`undo`					| go back a move
`remove`				| undo two moves and continue playing same colour
`pause/resume`			| pause/resume any thinking/pondering and stop clocks

##### No-op commands
These are recognised as valid commands, but don't actually do anything in the engine (currently)
Command					| Result
------------------------|--------
`draw`					| no-op (opponent offers draw, this engine fights to the death!)
`accepted`\|`rejected`	| no-op
`random`				| no-op
`level MPS BASE INC`	| no-op (set time controls)
`st TIME`				| no-op (set time)
`sd DEPTH`				| no-op (limit depth)
`nps NODE_RATE`			| no-op (limit nodes searched)
`hard`					| no-op (turn on thinking/pondering)
`easy`					| no-op (turn off thinking/pondering)
`post`					| no-op (turn on thinking/pondering output)
`nopost`				| no-op (turn off thinking/pondering output)
`analyze`				| no-op (turn on analyse mode)
`name X`				| no-op (inform engine of opponent name)
`rating`				| no-op (in ICS mode, send opponent rating)
`ics HOSTNAME`			| no-op (whether engine is playing a local user or an ICS server)
`computer`				| no-op (opponent is another computer engine)
`memory N`				| no-op (inform engine of how much memory in N megabytes it can use)
`cores N`				| no-op (number of cores engine can use)
`egtpath TYPE PATH`		| no-op (where endgame databases can be found)
`exclude MOVE`,`include MOVE`,`exclude all`,`include all`| no-op (exclude/include specified moves)
`setscore SCORE DEPTH`	| no-op
`lift SQUARE`,`put SQUARE`,`hover SQUARE`| no-op
`option NAME[=VALUE]`	| no-op (changes the value of a user-defined option)

####From engine to XBoard
Command					| Sent when
------------------------|--------
`feature``FEATURE1=VALUE1` `FEATURE2=VALUE2` etc| At beginning of game, with [ping=1, playother=1, setboard=1, myname=BitChess, variants=normal, reuse=0, colors=0, analyze=0, nps=0]
`Illegal move (REASON): MOVE`	| if engine receives a move that is a move but not legal in current position, e.g. `Illegal move: e2e4;Illegal move (in check): Nf3; Illegal move (moving into check): e1g1`.
`Error (ERRORTYPE): COMMAND`	| if a command is received that the engine does not understand or implement, e.g. `Error (ambiguous move): Nf3,Error (unknown command): analyze,Error (command not legal now): undo,Error (too many parameters): level 1 2 3 4 5 6 7`
`move MOVE`				| Make a move chosen by the engine
`RESULT {COMMENT}`		| Checkmate/stalemate is detected by the engine

##### Non implemented commands
Command					| Sent when
------------------------|--------
`resign`				| not implem(engine fights to the death)
`offer draw`			| not implem (engine fights to the death)
`tellopponent MESSAGE`	| not implem
`tellothers MESSAGE`	| not implem
`tellall MESSAGE`		| not implem
`telluser MESSAGE`		| not implem
`tellusererror MESSAGE`	| not implem
`askuser REPTAG MESSAGE`| not implem
`tellicsnoalias MESSAGE`| not implem
`# COMMENT`				| not implem

##Implementation

###XBoardCtrl
- `XBoardCtrl(istream in, ostream out)`:
	- constructs the class, using the given input and output streams for I/O.
	- Checks whether the streams are open, throws an exception if not
	- Sends a list of features to enable/disable on `out`

- `map<string, function> command_map`:
	- is a function lookup table for all relevant xboard commands. Each function has the call signature `void f(vector<string>)`

- `parse_command(string commandstr)`:
	- ignores empty commandstr
	- splits the `commandstr` by spaces to get command and optional parameters
	- if no spaces, whole `commandstr` is the command
	- looks up the command in `command_map` 
	- and passes a `vector<string>` of parameters to the function

- `poll()`:
	- Polls `stdin` for input
	- If input received, wait for a `\n` and pass to `parse_command`
	- Else pass control back to the calling function
	- Passed as a callback function to the `Game` component

###Game
- `Game::Game(polling_function)`:
	- `polling_function` is called regularly to poll for input
- `current_position` with getter and setter
- `private Searcher searcher`
- `Move Game::get_next_move()`:
	- `return searcher.get_next_move(current_position,side_to_move);`
- `void Game::do_oppo_move(Move move)`:
	- update `current_position`
	- add `move` to `moves_list`
	- TODO: engine turn logic

###Searcher

- `Searcher::Searcher()`
- `private Evaluator evaluator`
- `Move Searcher::get_next_move(Position position,side_to_move)`
	-  `evaluator.evaluate(position)`
	- loop through `position.get_all_legal_moves(side_to_move)`
		- create next position with `position.with_move(move)`
		- evaluate position and add to results (with, e.g. `pair<float,Move>`)
	- sort results
	- choose move with best evaluation

###Evaluator

- `float Evaluator::evaluate(Position position, side_to_move)`:
	- `map<piece_type,int> counts = position.get_piece_counts()`

	- loop through pieces in `counts[side_to_move]`
		- multiply piece count by piece multiplier
		- `eval_side_to_move = `summation
	- `eval_other_side` = repeat for other side
	- `return eval_side_to_move - eval_other_side`

###Position

- `map<side,map<piece_type,Piece>> map_pieces`
	- stores pieces in map, e.g. to access black queen = `map_pieces[black][queen]`
- `array<piece*, 64> array_pieces`
	- stores pointers to the same pieces in their board locations or to the piece type `PIECE_NONE`
- `map<piece_type,int> position.get_piece_counts()`
	- Loop through pieces in maps:
		- assign `map_pieces[side][piece].occupancy.count()` to a `map<piece_type,int>`and return it
- `vector<Move> get_all_legal_moves(side_to_move)`
	- Loop through pieces of `side_to_move`:
		- append all moves from `piece.get_pseudolegal_moves()`
	- TODO: `test_checks(move_list)` check moves for checks on own or opposite king
		- remove any resulting in check on own king (illegal)
		- set `check=true` flag to any resulting in check on opp king

###Piece
- `Piece::Piece()`
- `Piece::Piece(int initial_square)`
	- Initialise occupancy with initial_square set
- `bitboard occupancy`
	- contains occupancy info on bitboard (1 represents occupancy)
- `virtual vector<Move> get_legal_moves()`
	- implemented by subclasses to calculate possible moves

###Move
Data structure which stores information about a move

- `short sq_origin`
- `short sq_target`
- `promotion_type promotion`
- `bool is_capture`
- `bool is_check`
- `bool is_checkmate`
- `spec_move_type special_move`: 
- piece_type piece
- `string get_in_algebraic()`
- `string get_in_coordinate()`
- `enum promotion_type {NONE, QUEEN, ROOK, KNIGHT,BISHOP}`
- `enum spec_move_type {NONE, DOUBLE_PAWN_PUSH, EN_PASSANT, CASTLE_KING, CASTLE_QUEEN}`


###Bitboard
Currently just `typedef bitset<64> bitboard`. Shift operations defined separately; alternatively could extend `bitset<64>` class to include these

###Global consts/enums
`enum side {WHITE, BLACK}`
`enum piece_type {ROOK,KNIGHT,BISHOP,QUEEN,KING,PAWN,NONE}`

###Piece subclasses
####PieceNone
Indicates an empty square.

- `vector<Move> get_pseudolegal_moves()`
	- returns empty vector

####Rook
- `vector<Move> get_pseudolegal_moves()`
	-  bitscan to get squares of rooks
	- loop through squares
		- `moves = get_file_attacks(occupancy,sq) & own_occupancy`
		- bitscan `moves` to get move target squares
		- create & append moves, with origin=rook sq, target = target squares
		- `possible_captures = moves & opp_occupancy`

####Bishop
- `vector<Move> get_pseudolegal_moves()`
	-  Similar process for Rook, except `get_bishop_attacks` is used instead

####Queen
- `vector<Move> get_pseudolegal_moves()`
	-  Similar process, combining both Rook and Bishop `get_psuedolegal_moves()`

####King
- `vector<Move> get_pseudolegal_moves()`
	-  `vanilla_moves = move_lookup[king][square]`
	- Get possible knight moves from a precalculated lookup table.
	- `possible_checks = vanilla_moves & opp[king].occupancy`
	- `possible_captures = vanilla_moves & opp_occupancy`
		- Get possible captures with the union of possible moves and opponent occupancy (can only capture opponents)
		- bitscan `possible_captures`: array of `target_squares`
			- create & append new `Move` with capture=`true`, origin = `square` and `target_square`
	- `possible_moves = vanilla_moves & ~own_occupancy & ~captures`
		- Get other moves with union of squares not occupied by own pieces (complement of own_occupancy) and complement of captures to avoid double counting moves
			- create & append new Move with origin = `square` and `target_square`
	- TODO: `test_checks(move_list)` check moves for checks on own or opposite king
		- remove any resulting in check on own king (illegal)
		- set `check=true` flag to any resulting in check on opp king
####Knight
- `vector<Move> get_pseudolegal_moves()`
	- bitscan occupancy to get knight occupied squares
	- loop through squares
		-  `vanilla_moves = move_lookup[knight][square]`
			- Get possible knight moves from a precalculated lookup table.
		- `possible_checks = vanilla_moves & opp[king].occupancy`
		- `possible_captures = vanilla_moves & opp_occupancy`
			- Get possible captures with the union of possible moves and opponent occupancy (can only capture opponents)
			- bitscan `possible_captures`: array of `target_squares`
				- create & append new `Move` with capture=`true`, origin = `square` and `target_square`
		- `possible_moves = vanilla_moves & ~own_occupancy & ~captures`
			- Get other moves with union of squares not occupied by own pieces (complement of own_occupancy) and complement of captures to avoid double counting moves
				- create & append new Move with origin = `square` and `target_square`
			- 
####Pawn
- `vector<Move> get_pseudolegal_moves()`
	-  `single_moves = shift(own pawn occupancy) & ~own_occupancy & ~opp_occupancy`
		- `end_rank_mask` excludes pawns pushing for promotion
		-  where shift= north for white and south for black
		- checks pawns do not clash with pieces on square in front
	- `double_moves = shift(own_pawn_occupancy & double_pawn_mask[side]) & ~own_occupancy & ~opp_occupancy`
		- as above but applies mask to restrict to pawns that have not moved yet
		- create & append `Move::create_double_pawn_move(origin,target)` for each bit
	- `captures = (shift_diagonal(own pawn occupancy) & opp_occupancy) | (shift_diagonal(own pawn occupancy) & en_passant)`
		- shift diagonal shifts all pawns forward and to left/right
		- union with opponent pieces to show captures
		- if any en passant captures are available show them
	- `promotions = single_moves & back_rank_mask`
		- also do `single_moves -= promotions` to exclude duplicates
		- append `promotion_moves(sq, capture=false)` which will create vector of moves with possible promotion options (queen,rook etc).
	- `promotion_captures = captures & back_rank_mask`
		- again `single_moves -= promotion_captures`
		- append `promotion_moves(sq, capture=true)

	- now create & append moves for `single_moves`