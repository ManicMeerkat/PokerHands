#include "Card.h"
#include <string>
using namespace std;


//------------------------------------------------------
//Constructors
Card::Card() : rank(UnknownRank), suit(UnknownSuit)
{
}

//------------------------------------------------------
Card::Card(const Card& other)
{
	rank = other.rank;
	suit = other.suit;
}

//------------------------------------------------------
Card::Card(const string& representation) : rank(UnknownRank), suit(UnknownSuit)
{
	initializeFromRepresentation(representation);
}


//------------------------------------------------------
//Representation is in the format RS, where R is one of the following strings representing a rank:
//A (ace), 2-10 (one or two digits representing 2 through 10), J (Jack), Q (Queen), K (King). Anything else is considered invalid
//And S is one of the following characters representing a suit:
//s (Spades), h (Hearts), c (Clubs), d (Diamonds). Anything else is invalid.
//For example: Ah -> Ace of Hearts, 10s (10 of spades), 2c (2 of clubs), 4w (Invalid)
bool Card::initializeFromRepresentation(const string& representation)
{
	string format = representation;

	//Validate input
	//Ensure we have at least two characters, one for the suit, and (at least) one for the rank
	if (format.size() < 2)
		return false;

	//Get the suit from the end of the string
	char cSuit = format.back();
	format.pop_back();

	//The rest must be the rank
	const char* sRank = format.c_str();
	setRank(sRank);
	setSuit(cSuit);

	//Indicate success or failure
	return isValid();
}

//------------------------------------------------------
//Sets the card's rank base on a string:
//A, J, Q, K, or the numbers 2-10. Anything else is invalid.
void Card::setRank(const char* sRank)
{
	if (!sRank)
		return;

	rank = UnknownRank;

	//First try to convert to a number, in case it's a 2-10
	int number = atoi(sRank);
	if (number == 0)
	{
		//Here if not a number. Try A, J, Q, and K.
		if (strcmp(sRank, "A")  == 0)
			rank = Ace;
		else if (strcmp(sRank, "J") == 0)
			rank = Jack;
		else if (strcmp(sRank, "Q") == 0)
			rank = Queen;
		else if (strcmp(sRank, "K") == 0)
			rank = King;
	}
	else if (number >= 2 && number <= 10)
	{
		rank = (Rank)number; 
	}
}

//------------------------------------------------------
//Sets the suit based on its character code
void Card::setSuit(char cSuit)
{
	switch (cSuit)
	{
		case 's': suit = Spades; break;
		case 'd': suit = Diamonds; break;
		case 'h': suit = Hearts; break;
		case 'c': suit = Clubs; break;
		default: suit = UnknownSuit; break;
	}
}

//------------------------------------------------------
//Get the display name for a suit 
string Card::nameFor(Suit suit)
{
	switch (suit)
	{
	case Hearts: return "Hearts";
	case Clubs: return "Clubs";
	case Diamonds: return "Diamonds";
	case Spades: return "Spades";
	default: return "<unknown>";
	}
}

//------------------------------------------------------
//Gets the name for a rank (optionally the plural) for output purposes
string Card::nameFor(Rank rank, bool plural /*=false*/)
{
	switch (rank)
	{
	case Ace: return plural ? "Aces": "Ace";
	case Two: return plural ? "Deuces" : "Deuce";
	case Three: return plural ? "Threes" : "Three";
	case Four: return  plural ? "Fours" : "Four";
	case Five: return plural ? "Fives" : "Five";
	case Six: return plural ? "Sixes" : "Six";
	case Seven: return plural ? "Sevens" : "Seven";
	case Eight: return plural ? "Eights" : "Eight";
	case Nine: return plural ? "Nines" : "Nine";
	case Ten: return plural ? "Tens" : "Ten";
	case Jack: return plural ? "Jacks" : "Jack";
	case Queen: return plural ? "Queens" : "Queen";
	case King: return plural ? "Kings" : "King";
	default: return "<invalid>";
	}
}

