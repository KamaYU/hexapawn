#ifndef _BOARD_H
#define _BOARD_H

#include <string>
#include "Pawn.h"
using namespace std;

// The game board dimensions
#define SIZE 3
#define BLACK_ASCII "X"
#define WHITE_ASCII "O"
#define BLANK_ASCII " "

typedef Pawn Turn;

struct Move
{
	int xsource, ysource, xdest, ydest;
//	Move();
	Move(int sx, int sy, int dx, int dy);
};

class Board
{
	private:
		Pawn board[SIZE][SIZE];		// The game board configuration
		Turn turn;					// The Pawn of the current player whose turn it is to move
		string move;				// The Move that led to this Board
		unsigned long int children;	// The number of child moves of this Board
		Board * child;				// Pointer to the next child move
		Board * sibling;			// Pointer to the next sibling move

		// =========================================
		// Private helper modifiers
		// =========================================
		void addMoves();	// Recursively generates all child/sibling moves
		void deleteMove(Board * b);

	public:
		// =========================================
		// Constructors
		// =========================================
		
		Board();
		Board(const Board * b, const Move * m);

		// =========================================
		// Accessors
		// =========================================

		bool isLegal(const Move * move) const;
		bool isVictory() const;
		bool isPlayer(unsigned short int x, unsigned short int y) const;
		char printPawn(unsigned short int x, unsigned short int y) const;
		string printMove() const;
		string printWinner() const;
		string printTurn() const;

		// =========================================
		// Modifiers
		// =========================================

		Board * doMove();
		Board * doMove(const Move * move);
		void pruneMove(Board * b);

		// =========================================
		// Operators
		// =========================================

		bool operator==(const Board & rhs) const;	// Returns true if both Boards have the same board
		bool operator!=(const Board & rhs) const;	// Returns true if both Boards have different boards
//		Pawn * operator[](const unsigned short int idx);
};

#endif // _BOARD_H
