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

#include "Match.h"

Match::Match(int dealerNo) {
	m_round = EAST_ROUND;
	m_hand = 0;
	m_handsUntilNextRound = 4;

	for (int i = 0; i < NUM_PLAYERS; ++i) {
		m_scores[i] = 25000;
	}

	int seat = EAST_SEAT;
	for (int i = dealerNo; i < dealerNo + NUM_PLAYERS; ++i) {
		m_seats[i % NUM_PLAYERS] = seat;
		seat = (seat + 1) % 4;
	}
}



bool Match::isActive() const {
	if (m_round >= NUM_OF_ROUNDS) return false;
	for (int i = 0; i < NUM_PLAYERS; i++) {
		// Yes, players with exactly 0 points can continue
		if (m_scores[i] < 0) return false; 
	}
	return true;
}

void Match::update(std::vector<int> scoreChanges , bool repeat) {
	for (int i = 0; i < NUM_PLAYERS; ++i)
		m_scores[i] += scoreChanges[i];
	if (!repeat) { // If this hand counts towards round advancement
		if (m_handsUntilNextRound == 1) { // If this was the last hand of the round
			m_round++;
			m_handsUntilNextRound = 4;
			m_hand = 0;
		}
		else {
			--m_handsUntilNextRound;
		}
	}
}

