// PokerHands.cpp : Defines the entry point for the console application.
//
// by Jason Pollack 

#include "stdafx.h"
#include <string>
#include <iostream>
#include <fstream>
#include "Hand.h"
using namespace std;


//Run from the command line. The single parameter should be a path to a file containing lines of hands. A sample file might
//look like this:
//
// 3s 2h Ad 4c 5c
// 2h 3h 4h 5h 6h
// Qh Jh Kh 10h Ah
// 8s 2h 7d 10c 3s
// 8s 2h 8d 4c 4s
// 4c 4s Ah Ac 4d
// 8s 2h 7d 11c 3s
// As Ah Ac 4c 4d
// As Ah Ac 4c 5c
// Ad 2d 3d 4d 5d
// 8s 2h 7d 3s
// 10s Js Qs Kc As


int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		cout << "Path to input file is required";
		return 0;
	}

	//Get the file path that contains the hands
	const char* path = argv[1];

	//Open the file
	fstream file(path);
	if (file.is_open())
	{
		string sHand;

		//Each line contains a hand, which consists of 5 Card representations separated by spaces
		while (getline(file, sHand))
		{
			//Parse the hand. 
			Hand hand(sHand);

			//Find the best hand from the given hand.
			BestHand bestHand = hand.getBestHand();

			//Show the results
			cout << sHand << " - " << bestHand.getDescription() << endl;
		}
		file.close();
	}
	else
	{
		cout << "The file at path " << path << " could not be opened.";
	}

	return 0;
}

