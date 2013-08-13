#include "BestHand.h"
#include "Hand.h"
#include <sstream>
using namespace std;

//------------------------------------------------------
//BestHand class definition
//------------------------------------------------------
BestHand::BestHand(const Hand& hand) : rank1(Card::UnknownRank), rank2(Card::UnknownRank), suit(Card::UnknownSuit), type(InvalidHand)
{
	type = hand.getBestHand(rank1, rank2, suit);
}

//------------------------------------------------------
//Outputs the human readable description for the BestHand
string BestHand::getDescription() const
{
	//Based on the type of BestHand, use 
	stringstream str;
	string sSuit = Card::nameFor(suit);
	switch (type)
	{
	case InvalidHand:
		str << "Invalid Hand";
		break;
	case HighCard:
		str << "High Card: " << Card::nameFor(rank1);
		break;
	case Pair:
		str << "Pair: " << Card::nameFor(rank1, true);
		break;
	case TwoPair:
		str << "Two Pair: " << Card::nameFor(rank1, true) << " over " << Card::nameFor(rank2, true);
		break;
	case ThreeOfAKind:
		str << "Three of a Kind: " << Card::nameFor(rank1, true);
		break;
	case Straight:
		str << "Straight to " << Card::nameFor(rank1);
		break;
	case Flush:
		str << "Flush: " << Card::nameFor(suit);
		break;
	case FullHouse:
		str << "Full House: " << Card::nameFor(rank1, true) << " full of " << Card::nameFor(rank2, true);
		break;
	case FourOfAKind:
		str << "Four of a Kind: " << Card::nameFor(rank1, true);
		break;
	case StraightFlush:
		//Special case--for royal flush
		if (rank1 == Card::Ace)
		{
			str << "Royal Flush! (" << Card::nameFor(suit) <<")";
		}
		else
		{
			str << "Straight Flush (" << Card::nameFor(suit) << ") to " << Card::nameFor(rank1);
		}
		break;
	default:
		str << "<unknown>";
		break;
	}

	return str.str();
}
