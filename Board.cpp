#include "Board.h"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <cmath>

//Move::Move() : xsource(0), ysource(0), xdest(0), ydest(0) {}

Move::Move(unsigned short int sx, unsigned short int sy,
		unsigned short int dx, unsigned short int dy)
	: xsource(sx), ysource(sy), xdest(dx), ydest(dy) {}



/* Default constructor
 * Creates the starting Board, white to move.
 * All subsequent moves are generated in a tree
 */
Board::Board() : turn(Pawn(WHITE)), move(""), children(0), sibling(0), child(0)
{
	// Initialize starting Board
	for (short i = 0; i < SIZE; i++)
	{
		this->board[i][0] = Pawn(BLACK);
		for (short j = 1; j < SIZE - 1; j++)
			this->board[i][j] = Pawn(BLANK);
		this->board[i][SIZE-1] = Pawn(WHITE);
	}

	// Generate moves tree
	this->addMoves();
}

/* Copy constructor
 * Creates a Board from the given pointer to another Board
 * If Move is given, performs the move on the created board
 */
Board::Board(const Board * b, const Move * m)
	: turn(b->turn), move(""), children(0), sibling(0), child(0)
{
	short int black = 0, white = 0, oldBlack = 0, oldWhite = 0;
	// Copy the Board from b
	for (short i = 0; i < SIZE; i++)
		for (short j = 0; j < SIZE; j++)
		{
			this->board[i][j] = b->board[i][j];
			// Count the white and black Pawns
			switch(this->board[i][j].getColor())
			{
				case BLACK:
					oldBlack++;
					break;
				case WHITE:
					oldWhite++;
					break;
			}
		}
	// Make the move if it is given
	this->board[m->xdest][m->ydest] = this->board[m->xsource][m->ysource];
	this->board[m->xsource][m->ysource] = Pawn(BLANK);
	this->turn = this->turn.getOpposite();
	// Count the white and black Pawns after the move
	for (short i = 0; i < SIZE; i++)
		for (short j = 0; j < SIZE; j++)
		{
			switch(this->board[i][j].getColor())
			{
				case BLACK:
					black++;
					break;
				case WHITE:
					white++;
					break;
			}
		}
	stringstream moveString;
	moveString << char(m->xsource + 97) << SIZE - m->ysource;
	if (black == oldBlack && white == oldWhite)	// no captures
		moveString << "-";
	else
		moveString << "x";
	moveString << char(m->xdest + 97) << SIZE - m->ydest;
	move = moveString.str();
}

// ==========================================
// Accessors
// ==========================================
/*
void printLine()
{
	const char corner = '+';
	const char floor  = '-';
	const char space  = ' ';

	cout << space;
	cout << corner;
	for (int i = 0; i < SIZE; i++)
	{
		for (int j = 0; j < 3; j++)
			cout << floor;
		cout << corner;
	}
	cout << endl;
}

void Board::printBoard() const
{
	const char blank = ' ';
	const char black = '*';
	const char white = 'o';
	const char error = '!';
	const char wall	 = '|';
	const char space = ' ';

	printLine();
	// Traverse board
	for (int j = 0; j < SIZE; j++)
	{
		cout << SIZE - j;
		cout << wall;
		cout << space;
		for (int i = 0; i < SIZE; i++)
		{
			switch(this->board[i][j].getColor())
			{
				case BLACK:
					cout << black;
					cout << space;
					cout << wall;
					cout << space;
					break;
				case WHITE:
					cout << white;
					cout << space;
					cout << wall;
					cout << space;
					break;
				case BLANK:
					cout << blank;
					cout << space;
					cout << wall;
					cout << space;
					break;
				default:
					cout << error;
					cout << space;
					cout << wall;
					cout << space;
					break;
			}
		}
		cout << endl;
		printLine();
	}
	char c = 'a';
	cout << space << space << space;
	for (int i = 0; i < SIZE; i++)
		cout << c++ << space << space << space;
	cout << endl;

	switch(this->turn.getColor())
	{
		case BLACK:
			cout << "Black (" << black << ") to move: ";
			break;
		case WHITE:
			cout << "White (" << white << ") to move: ";
			break;
		default:
			cout << "Error: undetermined turn!";
			break;
	}
}
*/
/*
void Board::printMoves() const
{
	if (this->child != 0)
	{
		this->printBoard();
		this->child->printMoves();
	}
	if (this->sibling != 0)
		this->sibling->printMoves();
//	while (c->sibling != 0)
//	if (this->child != 0)
//		this->printBoard();
//		Board * c = this->child;
//		c->printBoard();
//		cout << endl;
//		while (c->sibling != 0)
//		{
//			c = c->sibling;
//			c->printBoard();
//			cout << endl;
//			c->printMoves();
//		}
//	}
}
*/

string Board::printMove() const
{
	return move;
}

char Board::printPawn(unsigned short int x, unsigned short int y) const
{
	switch(this->board[x][y].getColor())
	{
		case BLACK:
			return *BLACK_ASCII;
			break;
		case WHITE:
			return *WHITE_ASCII;
			break;
		case BLANK:
			return *BLANK_ASCII;
			break;
	}
}

string Board::printWinner() const
{
	string winner = "";
	if (this->isVictory())
	{
		switch(this->turn.getColor())
		{
			case BLACK:
				winner += "Winner: WHITE (" + string(WHITE_ASCII) + ")";
				break;
			case WHITE:
				winner += "Winner: BLACK (" + string(BLACK_ASCII) + ")";
				break;
			default:
				break;
		}
	}
	return winner; 
}

string Board::printTurn() const
{
	string toMove = "";
	switch(this->turn.getColor())
	{
		case BLACK:
			toMove += "Black (" + string(BLACK_ASCII) + ") to move";
			break;
		case WHITE:
			toMove += "White (" + string(WHITE_ASCII) + ") to move";
			break;
		default:
			break;
	}
	return toMove;
}

Board * Board::doMove()
{
	// Select a move at random
	unsigned long int n = rand() % this->children;
	// Traverse the children to select the chosen move
	if (n == 0)
		return this->child;
	Board * c = this->child;
	for (int i = 0; i < n; i++)
		c = c->sibling;

	return c;
}

Board * Board::doMove(const Move * move)
{
	Board * b = new Board(this, move);
	Board * c = this->child;
	// Search the children for the board position
	if ((*c) == (*b))
		return c;
	else
	{
		while (c->sibling != 0)
		{
			c = c->sibling;
			if ((*c) == (*b))
				return c;
		}
	}
	return 0;
}

bool Board::isLegal(const Move * move) const
{
	// Check for out of bounds
	if (move->xsource >= SIZE || move->xsource < 0)
		return false;
	if (move->ysource >= SIZE || move->ysource < 0)
		return false;
	if (move->xdest >= SIZE || move->xdest < 0)
		return false;
	if (move->ydest >= SIZE || move->ydest < 0)
		return false;
	// Check for move more than one square away
	if (abs(move->xsource - move->xdest) > 1)
		return false;
	if (abs(move->ysource - move->ydest) > 1)
		return false;
	// Check for move less than one square away
	if (abs(move->ysource - move->ydest) < 1)
		return false;

	// White and black pawns move in different directions
	switch(this->board[move->xsource][move->ysource].getColor())
	{
		case BLACK:	// black moves down the board (+y)
			if (move->ydest <= move->ysource)
				return false;
			break;
		case WHITE:	// white moves up the board (-y)
			if (move->ydest >= move->ysource)
				return false;
			break;
		default:	// error
			return false;
			break;
	}

	// Check for free space on forward move
	if (move->xsource == move->xdest)
	{
		if (this->board[move->xdest][move->ydest] != Pawn(BLANK))
			return false;
	}
	// Check for opponent pawn on diagonal move
	else
	{
		if (this->board[move->xsource][move->ysource].getOpposite() 
				== this->board[move->xdest][move->ydest])
			return true;
		else
			return false;
	}

	// All checks passed, so move is legal
	return true;
}

/* addMoves
 * Recursively generates all of this Boards' subsequent moves
 */
void Board::addMoves()
{
	//////////////////////////////////////////
	// Before adding moves, check the board
	// for victory conditions. If the board
	// is already a victory, do not add moves.
	//////////////////////////////////////////
	short int white = 0, black = 0;
	// Check last rank (black home row) for white pawns
	for (short i = 0; i < SIZE; i++)
	{
		if (this->board[i][0] == Pawn(WHITE))
			return;
		else if (this->board[i][0] == Pawn(BLACK))
			black++;
	}
	// Check the middle ranks for black and white pawns
	for (short j = 1; j < SIZE - 1; j++)
	{
		for (short i = 0; i < SIZE; i++)
		{
			if (this->board[i][j] == Pawn(WHITE))
				white++;
			else if (this->board[i][j] == Pawn(BLACK))
				black++;
		}
	}
	// Check first rank (white home row) for black pawns
	for (short i = 0; i < SIZE; i++)
	{
		if (this->board[i][SIZE-1] == Pawn(BLACK))
			return;
		else if (this->board[i][SIZE-1] == Pawn(WHITE))
			white++;
	}
	// Check whether one side has been eliminated
	if (! white || ! black)
		return;

	// Else all victory conditions except no moves have been checked,
	//  none found. However, no moves condition should be impossible
	//  due to the way board is created.

	////////////////////////////////
	// Begin move generation
	////////////////////////////////
	
	// Traverse board.
	// For every Pawn found
	//  check for legal moves. For each legal move found,
	//  generate a new board and add it to the sibling moves
	//  list. Then addMoves for every board on the list.
	for (short j = 0; j < SIZE; j++)
	{
		for (short i = 0; i < SIZE; i++)
		{
			// If the Pawn found has a turn
			if (this->board[i][j] == this->turn)
			{
				// Search all adjacent squares for legal moves
				for (short x = -1; x <= 1; x++)
				{
					for (short y = -1; y <= 1; y++)
					{
						Move * move = new Move(i, j, i + x, j + y);
						if (this->isLegal(move))	// add legal move
						{
							Board * b = new Board(this, move);
							if (this->child == 0)
							{
								this->child = b;
								this->children++;
								this->child->addMoves();
							}
							else
							{
								Board * s = this->child;
								while (s->sibling != 0)
									s = s->sibling;
								s->sibling = b;
								this->children++;
								s->sibling->addMoves();
							}
						}
					}
				}
			}
		}
	}
}

void Board::pruneMove(Board * b)
{
	// Move to prune is this Board's direct child
	// Set child to sibling, and delete former child recursively
	if (this->child == b)
		this->child = b->sibling;
	else	// Move to prune is a sibling of this Board's child
	{
		// Traverse siblings until correct one has been found
		// Cut sibling out of the sibling list, and delete former sibling recursively
		Board * s = this->child;
		while (s->sibling != b)
			s = s->sibling;
		s->sibling = b->sibling;
	}
	// Delete former Board recursively
	b->sibling = 0;
	deleteMove(b);
	this->children--;
}

void Board::deleteMove(Board * b)
{
	if (b->sibling)
		deleteMove(b->sibling);
	if (b->child)
		deleteMove(b->child);
	delete b;
}

bool Board::isVictory() const
{
	return ! this->children;
}

bool Board::isPlayer(unsigned short int x, unsigned short int y) const
{
	if (this->board[x][y] == Pawn(WHITE))
		return true;
	else
		return false;
}

bool Board::operator==(const Board & rhs) const	// Returns true if both Boards have the same board
{
	for (int i = 0; i < SIZE; i++)
		for (int j = 0; j < SIZE; j++)
			if (this->board[i][j] != rhs.board[i][j])
				return false;
	return true;
}

bool Board::operator!=(const Board & rhs) const	// Returns true if both Boards have different boards
{
	return ! (*this == rhs);
}
