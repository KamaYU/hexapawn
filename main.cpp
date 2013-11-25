#include <iostream>
#include <sstream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <ncurses.h>
#include "Board.h"
using namespace std;

#define int2str(i) dynamic_cast<ostringstream &>(ostringstream() << dec << i).str().c_str()
#define char2str(c) string(1, c).c_str()

#define YMARGIN 0
#define XMARGIN 0
#define YCURSOR SIZE - 1
#define XCURSOR 1
#define MOVESWIN 20
#define RULESWIN 11
#define NMOVE 1
#define RESET -1
#define SQUARE 3
#define YAXIS 2
#define XAXIS 1
#define LOWER 97
#define SLEEP 1000000000

int main()
{
	// Seed random generator for move selection
	srand(time(NULL));

	// Initialize ncurses
	initscr();					// start ncurses mode
	start_color();				// use colors
	cbreak();					// capture input from user without line breaks
	keypad(stdscr, TRUE);		// enable F-keys and number pad
	noecho();					// do not echo user input
	clear();					// clear the screen before use
	refresh();					// refresh the main window

	unsigned short int ymargin = YMARGIN, xmargin = XMARGIN, ycursor = YCURSOR, xcursor = XCURSOR;

	/////////////////////////////////////////////////////////////////////
	// Create and draw axes window
	WINDOW * axeswin = newwin(SIZE + 1, SIZE * SQUARE + 1, ymargin, xmargin);
	for (short y = 0; y < SIZE; y++)
		mvwprintw(axeswin, ymargin++, xmargin, int2str(SIZE - y));
	xmargin += YAXIS;
	for (short x = 0; x < SIZE; x++)
	{
		mvwprintw(axeswin, ymargin, xmargin, char2str(char(x + LOWER)));
		xmargin += SQUARE;
	}
	wrefresh(axeswin);
	//
	/////////////////////////////////////////////////////////////////////

	ymargin = 0;
	xmargin = 1;

	/////////////////////////////////////////////////////////////////////
	// Create the gameboard window
	WINDOW * boardwin = newwin(SIZE, SIZE * SQUARE, ymargin, xmargin);
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_WHITE, COLOR_BLACK);
	//
	/////////////////////////////////////////////////////////////////////

	ymargin = 0;
	xmargin = 0;

	/////////////////////////////////////////////////////////////////////
	// Create the rules window
	WINDOW * ruleswin = newwin(RULESWIN, COLS, LINES - RULESWIN - 1, xmargin);
	mvwprintw(ruleswin, ++ymargin, xmargin, "Controls:");
	mvwprintw(ruleswin, ++ymargin, xmargin, "ARROWS: move cursor, SPACE: select pawn/place pawn");
	mvwprintw(ruleswin, ++ymargin, xmargin, "Q: quit, N: new game, R: reset AI, T: train AI");
	mvwprintw(ruleswin, ++ymargin, xmargin, "==================================================");
	mvwprintw(ruleswin, ++ymargin, xmargin, "How to play:");
	mvwprintw(ruleswin, ++ymargin, xmargin, "Pawns may move one space forward, or capture diagonally.");
	mvwprintw(ruleswin, ++ymargin, xmargin, "To win:");
	mvwprintw(ruleswin, ++ymargin, xmargin, "1. Capture all opponent pawns;");
	mvwprintw(ruleswin, ++ymargin, xmargin, "2. Promote your pawn to opponent's home rank; or");
	mvwprintw(ruleswin, ++ymargin, xmargin, "3. Prevent your opponent from moving on his turn.");
	wrefresh(ruleswin);
	//
	/////////////////////////////////////////////////////////////////////

	ymargin = 0;
	xmargin = SIZE * SQUARE + 3;

	/////////////////////////////////////////////////////////////////////
	// Create the moves window
	WINDOW * moveswin = newwin(LINES - RULESWIN, MOVESWIN, ymargin, xmargin);
	//
	/////////////////////////////////////////////////////////////////////

	// TODO:
	// Check if AI skill level has been saved, load it

	// Initialize game board
	Board * newGame = new Board();	// always points to the first Board (root)
	Board * game = newGame;			// current board in current game, progresses down the tree
	Board * prev;// = game;			// previous board in current game
	Board * parent;// = prev;			// parent of previous board 

	unsigned short int nmove = NMOVE;
	unsigned long int ngame = 0, lost = 0;
	short int ysource = RESET, xsource = RESET, ydest = RESET, xdest = RESET;
	bool victory = false, aiVictory = false;
	bool quit = false, train = false;
	// Print turn
	mvwprintw(moveswin, 0, 0, game->printTurn().c_str());
	wrefresh(moveswin);

	// Play the game
	int ch = 0;
	while (! quit)
	{
		// Parse user input
		switch(ch)
		{
			case int('r'):
			case int('R'):
				// Reset AI to lowest skill level
				newGame = new Board();	// Re-create full moves tree
				//** FALLTHROUGH **//
			case int('n'):
			case int('N'):
				// Reset parameters to start new game
				game = newGame;
				victory = false;
				nmove = NMOVE;
				ysource = RESET;
				xsource = RESET;
				ydest = RESET;
				xdest = RESET;
				ycursor = YCURSOR;
				xcursor = XCURSOR;
				wclear(boardwin);
				wclear(moveswin);
				break;
			case int('t'):
			case int('T'):
/*				// Run computer vs computer until skill at maximum
				unsigned long int tgame = 0;
				victory = false;
				while (tgame < 1000)
				{
					game = newGame;
					while (! victory)
					{
						// Do white move
						parent = prev;
						prev = game;
						game = game->doMove();
						// If white wins, "learning" ai loses
						if (game->isVictory())
						{
							victory = true;
							// prune losing move
							parent->pruneMove(prev);
						}
						// Do black move
						if (! game->isVictory())
							game = game->doMove();

					}
					tgame++;
				}
*/				break;
			case int('q'):
			case int('Q'):
				// Perform pre-exit things, such as saving AI
				quit = true;
				endwin();
				return 0;
				break;
			default:
				break;
		}
		if (! victory)
		{
			switch(ch)
			{
				case KEY_DOWN:
					if (ycursor < SIZE - 1)
						ycursor++;
					break;
				case KEY_UP:
					if (ycursor > 0)
						ycursor--;
					break;
				case KEY_RIGHT:
					if (xcursor < SIZE * SQUARE - 2)
						xcursor += SQUARE;
					break;
				case KEY_LEFT:
					if (xcursor - 1 > 0)
						xcursor -= SQUARE;
					break;
				case int(' '):
					// Check if source has been set; if not, set it
					if (ysource < 0)
					{
						ysource = ycursor;
						xsource = (xcursor - 1) / SIZE;
						// Check that source is a player-controlled pawn
						if (game->isPlayer(xsource, ysource))
						{
							stringstream move;
							move << nmove << ": " << char(xsource + LOWER) << SIZE - ysource << "-";
							mvwprintw(moveswin, nmove, 0, move.str().c_str());
						}
						else	// reset the source selection
						{
							ysource = RESET;
							xsource = RESET;
						}
					}
					else 
					{
						// Check if dest has been set; if not, set it
						if (ydest < 0)
						{
							ydest = ycursor;
							xdest = (xcursor - 1) / SIZE;
						}
						// If source is the same as the dest, reset them
						if (ysource == ydest && xsource == xdest)
						{
							ysource = RESET;
							xsource = RESET;
							ydest = RESET;
							xdest = RESET;
							mvwprintw(moveswin, nmove, 0, "                ");
						}
						// Otherwise, perform the move
						else
						{
							Move * move = new Move(xsource, ysource, xdest, ydest);
							// Check if move is legal on this board
							if (game->isLegal(move))
							{
								// Perform human move
								parent = prev;
								prev = game;
								game = game->doMove(move);
								mvwprintw(moveswin, nmove, 3, game->printMove().c_str());

								// Check for human victory
								if (! game->isVictory())
								{
									// TODO: sleep
									//for (int i = 0; i < SLEEP; i++)
									//{}

									// Perform computer move
									parent = prev;
									prev = game;
									game = game->doMove();
									mvwprintw(moveswin, nmove, 9, game->printMove().c_str());

									// Check for AI victory
									if (! game->isVictory())
										nmove++;
									else	// AI has won!
									{
										mvwprintw(moveswin, nmove, 14, "#");
										mvwprintw(moveswin, ++nmove, 0, game->printWinner().c_str());
										aiVictory = true;
									}
								}
								else	// Human has won!
								{
									mvwprintw(moveswin, nmove, 8, "#");
									mvwprintw(moveswin, ++nmove, 0, game->printWinner().c_str());
									aiVictory = false;
								}
							}
							else
								mvwprintw(moveswin, nmove, 0, "                ");

							// Reset the source and dest
							ysource = RESET;
							xsource = RESET;
							ydest = RESET;
							xdest = RESET;
						}
					}
					break;

				default:
					break;
			}
		}

		if (! victory)
		{
			// Draw gameboard window
			for (short y = 0; y < SIZE; y++)
			{
				for (short x = 0; x < SIZE; x++)
				{
					wattron(boardwin, COLOR_PAIR((x + y) % 2 + 1));
					stringstream square;
					square << " " << game->printPawn(x, y) << " ";
					mvwprintw(boardwin, y, x * SQUARE, square.str().c_str());
					wattroff(boardwin, COLOR_PAIR((x + y) % 2 + 1));
				}
			}
			// Print turn
			mvwprintw(moveswin, 0, 0, game->printTurn().c_str());
			wrefresh(moveswin);
			wmove(boardwin, ycursor, xcursor);
			wrefresh(boardwin);

			// Perform victory routines
			if (game->isVictory())
			{
				ngame++;
				// Prune tree to remove the losing branch nodes
				if (! aiVictory)
				{
					parent->pruneMove(prev);
					lost++;
				}
				victory = true;
			}
		}
		ch = getch();
	}
	endwin();
	return 0;
//3 .  .  .
//2 .  .  .
//1 .  .  .
//..a..b..c.

}

/*
	init_pair(1, COLOR_RED, COLOR_BLACK);
	attron(A_UNDERLINE | COLOR_PAIR(1));
	attroff(A_UNDERLINE | COLOR_PAIR(1));

	getch();
	endwin();
	Board * b = new Board();

	while (! b->isVictory())
	{
		string move;
		if (b)
			b->printBoard();
		else
			cout << "ERROR" << endl;
		cin >> move;
		short sx, sy, dx, dy;
		sx = move[0] - 97;
		dx = move[2] - 97;
		sy = SIZE - (move[1] - 48);
		dy = SIZE - (move[3] - 48);
//		cout << "(" << sx << "," << sy << ") -> (" << dx << "," << dy << ")" << endl;
		Move * theMove = new Move(sx, sy, dx, dy);
		b = b->doMove(theMove);
		if (b)
			b->printBoard();
		else
			cout << "ERROR" << endl;
		if (! b->isVictory())
			b = b->doMove();
	}
	cout << "VICTORY!" << endl;









	return 0;
}
*/
