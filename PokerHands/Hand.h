#pragma once
#include <list>
#include <string>
#include "Card.h"
#include "BestHand.h"

//------------------------------------------------------
//A Hand is a collection of cards. We keep the cards in a list, which can then be sorted
//if needed.
//A Hand is responsible for the lifetime of a Card.
class Card;
typedef std::list<Card*> CardList;
class Hand
{
public:
	Hand();
	Hand(const std::string& representation);
	virtual ~Hand();

	bool initializeFromString(const std::string& representation);

	void addCard(Card* card);
	bool Hand::isValid() const;

	BestHand getBestHand() const;
	BestHand::Type getBestHand(Card::Rank& rank1, Card::Rank& rank2, Card::Suit& suit) const;

protected:
	static const int HAND_SIZE;
	CardList cards;

	bool isFlush(Card::Suit& suit) const;
	bool isStraight(const CardList& order, Card::Rank& highest) const;
	bool isOfAKind(BestHand::Type& kind, Card::Rank& rank1, Card::Rank& rank2) const;

};

