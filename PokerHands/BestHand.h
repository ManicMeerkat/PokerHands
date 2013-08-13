#pragma once
#include <string>
#include "Card.h"

//------------------------------------------------------
//BestHand is the representation of the best possible hand.
//It doesn't have cards. It only has components of a description of the best hand.
//Type is the quality of the hand, in order from weakest to strongest
//Suit is the name of the suit making the hand, for which suit is appropriate (flushes)
//Rank1 is the rank of the primary card rank making up the hand. This is the stronger of the two ranks in a two pair or a full house,
//or the single rank for a high card, pair, four of a kind, or the highest card in a straight
//Rank2 is the rank of the secondary (or weaker) card in the hand, where appropriate, such as the lower card rank in
//a two pair or full house.
class Hand;
class BestHand
{
public:
	BestHand(const Hand& hand);
	enum Type {InvalidHand, HighCard, Pair, TwoPair, ThreeOfAKind, Straight, Flush, FullHouse, FourOfAKind, StraightFlush};

	std::string getDescription() const;

protected:
	Type type;
	Card::Rank rank1;
	Card::Rank rank2;
	Card::Suit suit;
};
