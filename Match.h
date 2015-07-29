/*
Designed by Xuan Yuan Huang, a desperate UCLA undergrad CS student.
Copyright 2014 Xuan Yuan Huang

Built with the SFML 2.2 Engine.
SFML 2.2 is licensed under zlib/libpng.

For my code:
	This program is free software: you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program.  If not, see <http://www.gnu.org/licenses/>.
Because I'm a desperate undergrad that needs to prove his worth.

Developed in Visual Studio 2013 (thanks for the free copy, MS Dreamspark!).
Visual Studio is, of course, Microsoft's proprietary IDE.

This file manages the state of the match for client and server - rounds,
scores, etc. 
*/

#include <vector>

const int NUM_OF_ROUNDS = 2; // This number ("han-chan") is typical of Japanese Mahjong
const int INITIAL_SCORE = 25000; // This is also a typical value
const int NUM_PLAYERS = 4;

// These are the names of the four possible rounds, in order
enum Rounds {
	EAST_ROUND,
	SOUTH_ROUND,
	WEST_ROUND,
	NORTH_ROUND
};

// These are the names of the player seating positions (going clockwise)
enum Seats {
	EAST_SEAT, // Also the dealer of any given hand
	NORTH_SEAT,
	WEST_SEAT,
	SOUTH_SEAT
};
// CAUTION: player seat winds have both inverted order relative to the round winds and
// flipped orientation relative to an actual compass (west is to south's RIGHT, etc.).
// Between each hand (unless a hand is being repeated), seat winds rotate such that
// east player becomes north, north becomes west, etc.
// Turn order flows counter-clockwise instead of clockwise, so that
// east player -> south player -> west player -> north player -> east again.

class Match {
public:
	Match(int dealerNo); // Constructor - will initialize match with default parameters
	int getRound() const { return m_round; }
	int getHand() const { return m_hand;  }
	int getDealer() const; // Returns the dealer of the current hand
	int getSeat(int playerNo) const { return m_seats[playerNo]; } // Returns the seat of the indicated player
	int getCurrentPlayer() const { return m_turn; } // Returns whose turn (to discard) it currently is
	int getNumTilesLeft() const { return m_numTilesLeft;  } // Returns how many tiles are left on the live wall
	void nextTurn(); // Advance to the next turn
	bool isActive() const; // Checks to see if the match can continue or is over
	// Updates scores based on the differences passed into the array
	// If the repeat flag is set to true, the hand is "repeated" - it does not
	// count towards the 4 hand per round limit
	// The hand count is then incremented
	void update(std::vector<int> scoreChanges, bool repeat); 
private:
	int m_numTilesLeft; // Number of tiles on the live wall
	int m_round; // The current round number (uses the enum Rounds)
	int m_hand; // The current hand number
	int m_turn; // Whose turn (to discard) it currently is (uses the enum Seats)
	int m_handsUntilNextRound; 
	int m_scores[NUM_PLAYERS]; // Current scores of all players
	int m_seats[NUM_PLAYERS]; // The seat of all players
};