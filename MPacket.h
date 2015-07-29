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

This file handles the packaging of information sent and received between
the server and the client into Mahjong Packets ("MPacket").
*/

#ifndef MPACKET_H
#define MPACKET_H
#include <SFML/Network.hpp>
#include <SFML/System/String.hpp>
#include <vector>

#include "Tile.h"

const int MAXNUMOFTILES = 14; // Only dealers ever get this many tiles (at the start)

// All MPackets implicitly package a header of this type when stored with <<
// On the receiving end, the header must manually be extracted with >>, so that
// the recepient can determine the type of MPacket to be extracted
enum MPacketHeader { 
	MATCH_UPDATE, // This packet updates the match state
	FIRST_HAND, // Indicates this packet contains the player's first of tiles
	DRAW, // Server sending player information on draw
	INPUT_REQUEST, // Server requesting player input of some form
	SELF_DISCARD, // Player informing server of his discard choice
	DISCARD // The server reports other players' discards
};

class MPacket { // Base class of all Mpackets
protected:
	MPacket() {} // Protected constructor to prevent instantiation of this base class
	sf::Uint8 m_header; // Contains information on the MPacket type
};

class MatchPacket : public MPacket {
public:
	MatchPacket(std::vector<int> scoreChanges, bool repeat) {
		m_header = MATCH_UPDATE;
		m_scoreChanges = scoreChanges;
		m_repeat = repeat;
	}
	MatchPacket() {
		m_header = MATCH_UPDATE;
	}
	std::vector<sf::Int32> getScoreChanges() { return m_scoreChanges; }
	bool getRepeat() { return m_repeat;  }

	// SFML Packet << overload to support
	friend sf::Packet& operator<<(sf::Packet& packet, const MatchPacket& matchPacket) {
		packet << matchPacket.m_header;	

		packet << matchPacket.m_scoreChanges.size();
		for (int i = 0; i < matchPacket.m_scoreChanges.size(); ++i)
			packet << (sf::Int32)matchPacket.m_scoreChanges[i];
		packet << matchPacket.m_repeat;
		return packet;
	}

	// SFML Packet >> overload to support
	friend sf::Packet& operator>>(sf::Packet& packet, MatchPacket& matchPacket) {
		int sizeOfVector;
		packet >> sizeOfVector;
		for (int i = 0; i < sizeOfVector; ++i) {
			int scoreChange;
			packet >> scoreChange;
			matchPacket.m_scoreChanges.push_back(scoreChange);
		}
		packet >> matchPacket.m_repeat;
		return packet;
	}
private:
	std::vector<sf::Int32> m_scoreChanges;
	bool m_repeat;
};

class FirstHandPacket : public MPacket {
public:
	FirstHandPacket(std::vector<Tile>& hand) {
		m_header = FIRST_HAND;
		for (int i = 0; i < 13; ++i) {
			m_hand[i] = hand[i];
		}
	}
	FirstHandPacket() {
		m_header = FIRST_HAND;
	}
	Tile* getHand() { return m_hand; }

	// SFML Packet << overload to support FirstHandPacket
	friend sf::Packet& operator<<(sf::Packet& packet, const FirstHandPacket& fhPacket) {
		packet << fhPacket.m_header;
		for (int i = 0; i < 13; ++i) {
			packet << fhPacket.m_hand[i];
		}
		return packet;
	}

	// SFML Packet >> overload to support FirstHandPacket
	// "packet" should first have its header (packed by operator<<) extracted manually
	friend sf::Packet& operator>>(sf::Packet& packet, FirstHandPacket& fhPacket) {
		for (int i = 0; i < 13; ++i) {
			packet >> fhPacket.m_hand[i];
		}
		return packet;
	}
private:
	Tile m_hand[13];
};

class DrawPacket : public MPacket {
public:
	DrawPacket(Tile draw) {
		m_header = DRAW;
		m_draw = draw;
	}
	DrawPacket() {
		m_header = DRAW;
	}
	Tile getDraw() const { return m_draw; }

	// SFML Packet << overload to support DrawPacket
	friend sf::Packet& operator<<(sf::Packet& packet, const DrawPacket& drawPacket) {
		packet << drawPacket.m_header;
		packet << drawPacket.m_draw;
		return packet;
	}

	// SFML Packet >> overload to support DrawPacket
	// "packet" should first have its header (packed by operator<<) extracted manually
	friend sf::Packet& operator>>(sf::Packet& packet, DrawPacket& drawPacket) {
		packet >> drawPacket.m_draw;
		return packet;
	}
private:
	Tile m_draw;
};

class SelfDiscardPacket : public MPacket {
public:
	SelfDiscardPacket(Tile discard) {
		m_header = SELF_DISCARD;
		m_discard = discard;
	}
	SelfDiscardPacket() {
		m_header = SELF_DISCARD;
	}
	Tile getDiscard() const { return m_discard; }

	// SFML Packet << overload to support SelfDiscardPacket
	friend sf::Packet& operator<<(sf::Packet& packet, const SelfDiscardPacket& sdPacket) {
		packet << sdPacket.m_header;
		packet << sdPacket.m_discard;
		return packet;
	}

	// SFML Packet >> overload to support SelfDiscardPacket
	// "packet" should first have its header (packed by operator<<) extracted manually
	friend sf::Packet& operator>>(sf::Packet& packet, SelfDiscardPacket& sdPacket) {
		packet >> sdPacket.m_discard;
		return packet;
	}
private:
	Tile m_discard;
};

class DiscardPacket : public MPacket {
public:
	DiscardPacket() {
		m_header = DISCARD;
	}
	DiscardPacket(Tile discard) {
		m_header = DISCARD;
		m_discard = discard;
	}
	Tile getDiscard() const { return m_discard; }

	// SFML Packet << overload to support DiscardPacket
	friend sf::Packet& operator<<(sf::Packet& packet, const DiscardPacket& dPacket) {
		packet << dPacket.m_header;
		return packet << dPacket.m_discard;
	}

	// SFML Packet >> overload to support DiscartPacket
	// "packet" should first have its header (packed by operator<<) extracted manually
	friend sf::Packet& operator>>(sf::Packet& packet, DiscardPacket& dPacket) {
		Tile discard;
		packet >> dPacket.m_discard;
		return packet;
	}
private:
	Tile m_discard;
};

#endif