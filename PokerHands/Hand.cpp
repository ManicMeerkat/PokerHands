#include "Hand.h"
#include <sstream>
#include <string>
#include <map>
using namespace std;

//Assume a typical 5 card poker hand. The logic for a 7 card hand or others is not included.
const int Hand::HAND_SIZE = 5;

//Function object used to help sort a hand. Two sorting modes are available--one for which
//ace is considered low (a wheel), and one for which ace is considered high.
class ByRank
{
public:
	ByRank(bool aceHigh) : aceIsHigh(aceHigh)
	{
	}

	bool operator()(Card* left, Card* right)
	{
		//Order by normal ranking if ace is low, or if the left card is not an ace
		if (!aceIsHigh || left->getRank() != Card::Ace)
			return left->getRank() < right->getRank();

		//Here if ace is high, and left card is an ace.
		return false;
	}
protected:
	bool aceIsHigh;
};

//------------------------------------------------------
//Constructors
Hand::Hand(void)
{
}

//------------------------------------------------------
Hand::Hand(const std::string& representation)
{
	initializeFromString(representation);
}

//------------------------------------------------------
//Since a Hand manages the lifetime of Cards, delete cards that we hold
Hand::~Hand(void)
{
	//Cleanup memory for Cards
	for (CardList::iterator it = cards.begin(); it != cards.end(); it++)
	{
		Card* pCard = *it;
		delete pCard;
	}
}

//------------------------------------------------------
//A hand is considered valid if it contains exactly 5 valid cards
bool Hand::isValid() const
{
	return cards.size() == HAND_SIZE;
}

//------------------------------------------------------
//Add a card to the card
void Hand::addCard(Card* card)
{
	//Our list becomes responsible for the lifetime of a card
	cards.push_back(card);
}

//------------------------------------------------------
//Given a complete string representation of a hand, parse out each card and
//add to the hand.
bool Hand::initializeFromString(const std::string& representation)
{
	stringstream ss(representation);
	string cardRepresentation;
	while (getline(ss, cardRepresentation, ' '))
	{
		Card* pCard = new Card(cardRepresentation);
		//If it's a valid card, add to hand.
		if (pCard->isValid())
		{
			addCard(pCard);
		}
		else
		{
			//Otherwise discard it.
			delete pCard;
			pCard = NULL;
		}
	}

	//It's considered a success if we now have a hand of 5 valid cards
	return isValid();
}

//------------------------------------------------------
//Returns true if the hand is a flush. If is, the output parameter suit contains the suit it's a flush of.
bool Hand::isFlush(Card::Suit& suit) const
{
	suit = Card::UnknownSuit;

	Card::Suit firstSuit = Card::UnknownSuit;
	for (CardList::const_iterator it = cards.begin(); it != cards.end(); it++)
	{
		Card* pCard = *it;

		//If it's the first card, just set the suit
		if (firstSuit == Card::UnknownSuit)
			firstSuit = pCard->getSuit();

		//Otherwise check if the suit is not the same
		else if (firstSuit != pCard->getSuit())
			return false;
	}

	//Here if all suits are the same. Set the output param
	suit = firstSuit;
	return true;
}

//------------------------------------------------------
//Returns true if the hand contains a straight. Input CardList is an already-sorted
//list (ace low, or ace high). Output parameter Rank holds the rank of the highest
//card in the straight, if it is a straight, else UnknownRank.
bool Hand::isStraight(const CardList& order, Card::Rank& highest) const
{
	Card::Rank lastRank = Card::UnknownRank;

	for (CardList::const_iterator it = order.begin(); it != order.end(); it++)
	{
		Card* pCard = *it;
		if (lastRank != Card::UnknownRank)
		{
			//It's a straight if the card is exactly one higher than the previous card
			if (pCard->getRank() - lastRank != 1)
			{
				//One special exception: If the last rank was a king and the current one is an ace, it's ok
				if (pCard->getRank() == Card::Ace && lastRank == Card::King)
				{
					//Do nothing
				}
				else
				{
					//Not a straight
					return false;
				}
			}
		}
		lastRank = pCard->getRank();
	}
	highest = lastRank;
	return true;
}

//------------------------------------------------------
//Returns true if the hand is one of the 'of-a-kind' hands, including pair, two pair, three of a kind, four of a kind
//and full house. Output parameters kind, rank1, and rank2 are specified if true. rank1 is always the stronger
//rank if it is a multiple rank hand, such as two pair or full house.
bool Hand::isOfAKind(BestHand::Type& kind, Card::Rank& rank1, Card::Rank& rank2) const
{
	kind = BestHand::HighCard;
	rank1 = rank2 = Card::UnknownRank;

	//Distribute count of cards per 'bucket', where each bucket is a rank found in the hand. The logic
	//for figuring the hand depends on how many cards of each type we've found.
	map<Card::Rank, int> rankOccurrences;
	for (CardList::const_iterator it = cards.begin(); it != cards.end(); it++)
	{
		Card* pCard = *it;
		rankOccurrences[pCard->getRank()]++;
	}

	//If one of each kind, there is no 'of a kind'
	if (rankOccurrences.size() == HAND_SIZE)
		return false;

	//Now look at map and see if we have 4 of a kind, 3 of a kind, a pair, two pair, or full house
	for (map<Card::Rank, int>::iterator it = rankOccurrences.begin(); it != rankOccurrences.end(); it++)
	{
		//Rank is in 'first' field, Count is in 'second' field
		Card::Rank rank = it->first;
		int count = it->second;

		if (count == 4)
		{
			kind = BestHand::FourOfAKind;
			rank1 = rank;
			return true;
		}
		else if (count == 3)
		{
			if (kind == BestHand::Pair) //We already found a pair, let's change to Full House
			{
				rank2 = rank1; //Let's keep the 3 of a kind in rank1, the pair in rank2
				rank1 = rank;
				kind = BestHand::FullHouse;
				return true;
			}
			else //No pair found yet
			{
				kind = BestHand::ThreeOfAKind; //We may change later to full house if we find another pair somewhere
				rank1 = rank;
				//Keep looking
			}
		}
		else if (count == 2)
		{
			if (kind == BestHand::Pair) //We already found a pair
			{
				kind = BestHand::TwoPair;

				//Put the higher rank first
				if (rank > rank1)
				{
					rank2 = rank1;
					rank1 = rank;
				}
				else
				{
					rank2 = rank;
				}
				return true;
			}
			else if (kind == BestHand::ThreeOfAKind) //We already found three of a kind, change to full house
			{
				kind = BestHand::FullHouse;
				rank2 = rank;
				return true;
			}
			else
			{
				kind = BestHand::Pair;
				rank1 = rank;
				//Keep looking
			}
		}
	}

	//We've looked at everything, found nothing of note
	return false;
}

//------------------------------------------------------
//Initialize a BestHand object from this Hand.
BestHand Hand::getBestHand() const
{
	return BestHand(*this);
}

//------------------------------------------------------
//Called by Best Hand, this does the evaluation work of determining the best poker hand based on the rules.
BestHand::Type Hand::getBestHand(Card::Rank& rank1, Card::Rank& rank2, Card::Suit& suit) const
{
	//If it's not a valid hand, indicate so.
	if (!isValid())
		return BestHand::InvalidHand;

	//Initialize output parameters
	rank1 = rank2 = Card::UnknownRank;
	suit = Card::UnknownSuit;

	//First see if there is a flush
	Card::Suit flushSuit = Card::UnknownSuit;
	bool isAFlush = isFlush(flushSuit);

	//Check next for 'of a kind' (pair, two pair, three of a kind, four of a kind)
	BestHand::Type ofAKind = BestHand::HighCard;
	Card::Rank ofAKindRank1 = Card::UnknownRank;
	Card::Rank ofAKindRank2 = Card::UnknownRank;
	bool isAKind = isOfAKind(ofAKind, ofAKindRank1, ofAKindRank2);

	//Sort cards in two ways: ace low and ace high
	CardList orderedList(cards);
	orderedList.sort(ByRank(true));

	//Keep the highest card around, will be useful for several types of hands
	Card::Rank highestCard = orderedList.back()->getRank();

	//Check for straights
	Card::Rank highCardInStraight = Card::UnknownRank;
	bool isAStraight = isStraight(orderedList, highCardInStraight);
	
	//If it's not an ace-high straigh, maybe it's an ace-low straight
	if (!isAStraight)
	{
		orderedList.sort(ByRank(false));
		isAStraight = isStraight(orderedList, highCardInStraight);
	}


	//Now we know everything there is to know. Try to determine the best hand from strongest to weakest.
	if (isAStraight && isAFlush)
	{
		rank1 = highestCard;
		suit = flushSuit;
		return BestHand::StraightFlush;
	}

	if (ofAKind == BestHand::FourOfAKind)
	{
		rank1 = ofAKindRank1;
		return BestHand::FourOfAKind;
	}

	if (ofAKind == BestHand::FullHouse)
	{
		rank1 = ofAKindRank1;
		rank2 = ofAKindRank2;
		return BestHand::FullHouse;
	}

	if (isAFlush)
	{
		suit = flushSuit;
		return BestHand::Flush;
	}

	if (isAStraight)
	{
		rank1 = highCardInStraight;
		return BestHand::Straight;
	}

	if (ofAKind == BestHand::ThreeOfAKind)
	{
		rank1 = ofAKindRank1;
		return BestHand::ThreeOfAKind;
	}

	if (ofAKind == BestHand::TwoPair)
	{
		rank1 = ofAKindRank1;
		rank2 = ofAKindRank2;
		return BestHand::TwoPair;
	}

	if (ofAKind == BestHand::Pair)
	{
		rank1 = ofAKindRank1;
		return BestHand::Pair;
	}

	//Here if there is absolutely nothing better
	rank1 = highestCard;
	return BestHand::HighCard;
}