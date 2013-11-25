#ifndef _PAWN_H
#define _PAWN_H

//using namespace std;

enum Color { BLACK, WHITE, BLANK };

class Pawn
{
	private:
		Color color;
		
	public:
		// =========================================
		// Constructors, Destructor
		// =========================================
		
		Pawn();					// Default constructor: creates a BLANK Pawn
//		Pawn(const Pawn & p);	// Copy Constructor: creates a Pawn from given Pawn
		Pawn(Color c);			// Constructor: creates a Pawn of given Color
//		~Pawn();				// Destructor

		// =========================================
		// Accessors
		// =========================================

		Color getColor() const;	// returns the Pawn's Color

		bool isPawn() const;	// returns true if Pawn is not a BLANK

		Pawn getOpposite() const;	// returns a Pawn of the opposite color from this

		// =========================================
		// Operators
		// =========================================

		bool operator==(const Pawn & rhs) const;	// Returns true if both Pawns have the same Color
		bool operator!=(const Pawn & rhs) const;	// Returns true if both Pawns have different Colors
		Pawn & operator=(Pawn rhs);					// Sets lhs = rhs

};

#endif // _PAWN_H
