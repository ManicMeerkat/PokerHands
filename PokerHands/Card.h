#pragma once
#include <string>


//------------------------------------------------------
//Definition for a Card - Contains a rank and a suit
class Card
{
public:
	Card();
	Card(const std::string& representation);
	Card(const Card& other);
	~Card() {}

	bool initializeFromRepresentation(const std::string& representation);

	enum Rank { UnknownRank = -1, Ace = 1, Two, Three, Four, Five, Six, Seven, Eight, Nine, Ten, Jack, Queen, King };
	enum Suit { UnknownSuit = -1, Spades = 0, Hearts, Clubs, Diamonds };

	Rank getRank() const { return rank; }
	Suit getSuit() const { return suit; }
	void setRank(Rank r) { rank = r; }
	void setSuit(Suit s) { suit = s; }

	bool isValid() const { return rank != UnknownRank && suit != UnknownSuit; }
	
	static std::string Card::nameFor(Rank rank, bool plural = false);
	static std::string Card::nameFor(Suit suit);

protected:
	Rank rank;
	Suit suit;

	void setRank(const char* sRank);
	void setSuit(char cSuit);
};

