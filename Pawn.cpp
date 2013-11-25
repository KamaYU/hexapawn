#include "Pawn.h"

////////////////////////////////////////////////
// Pawn class definitions
////////////////////////////////////////////////

// =================================
// Constructors, Destructor
// =================================

/* Default constructor: creates a BLANK Pawn
 */
Pawn::Pawn() : color(BLANK) {}

/* Constructor: creates a Pawn of given Color
 */
Pawn::Pawn(Color c) : color(c) {}

// ==================================
// Accessors
// ==================================

/* getColor: returns the color of the Pawn
 */
Color Pawn::getColor() const { return color; }

/* isPawn: returns true if Pawn is not BLANK
 */
bool Pawn::isPawn() const
{
	if (color != BLANK)
		return true;
	else
		return false;
}

Pawn Pawn::getOpposite() const
{
	switch(color)
	{
		case BLACK:
			return Pawn(WHITE);
			break;
		case WHITE:
			return Pawn(BLACK);
			break;
		default:	// error
			return Pawn(BLANK);
			break;
	}
}


// ===================================
// Operators
// ===================================

bool Pawn::operator==(const Pawn & rhs) const
{
	if (color == rhs.color)
		return true;
	else
		return false;
}

bool Pawn::operator!=(const Pawn & rhs) const
{
	return ! (*this == rhs);
}

Pawn & Pawn::operator=(Pawn rhs)
{
	this->color = rhs.getColor();
	return *this;
}
