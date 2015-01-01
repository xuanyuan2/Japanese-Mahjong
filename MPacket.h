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

enum MPacketHeader { 
	STATE_UPDATE, // This packet contains information about players
	FIRST_HAND, // Indicates this packet contains the player's first of tiles
	DRAW, // Server sending player information on draw
	DISCARD_SELF, // Player informing server of his discard choice
	DISCARD // The server reports other players' discards
};

class MPacket { // Base class of all Mpackets
public:
	Tile getHeader() { return m_header; } // Return this packet's header
protected:
	MPacket() {} // Protected constructor to prevent instantiation of this base class
	Tile m_header; // Contains information on the MPacket type
};

class StatePacket : public MPacket {
public:
	StatePacket() {
		m_header = STATE_UPDATE;
	}

	//// SFML Packet << overload to support
	//friend sf::Packet& operator<<(sf::Packet& packet, const StatePacket& statePacket) {
	//}

	//// SFML Packet >> overload to support
	//friend sf::Packet& operator>>(sf::Packet& packet, StatePacket& statePacket) {

	//}
private:
	// State update info goes here
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
	const Tile* getHand() { return m_hand; }

	// SFML Packet << overload to support
	friend sf::Packet& operator<<(sf::Packet& packet, const FirstHandPacket& fhPacket) {
		for (int i = 0; i < 13; ++i) {
			packet << fhPacket.m_hand[i];
		}
		return packet;
	}

	// SFML Packet >> overload to support
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
	const Tile getDraw() { return m_draw; }

	// SFML Packet << overload to support
	friend sf::Packet& operator<<(sf::Packet& packet, const DrawPacket& drawPacket) {
		return packet << drawPacket.m_draw;
	}

	// SFML Packet >> overload to support
	friend sf::Packet& operator>>(sf::Packet& packet, DrawPacket& drawPacket) {
		return packet >> drawPacket.m_draw;
	}
private:
	Tile m_draw;
};

class DiscardSelfPacket : public MPacket {
public:
	DiscardSelfPacket() {
		m_header = DISCARD_SELF;
	}
	const Tile getDiscard() { return m_discard; }

	//// SFML Packet << overload to support
	//friend sf::Packet& operator<<(sf::Packet& packet, const DiscardSelfPacket& dsPacket) {

	//}

	//// SFML Packet >> overload to support
	//friend sf::Packet& operator>>(sf::Packet& packet, DiscardSelfPacket& dsPacket) {

	//}
private:
	Tile m_discard;
};

class DiscardPacket : public MPacket {
public:
	DiscardPacket() {
		m_header = DISCARD;
	}
	const Tile getDiscard() { return m_discard; }

	//// SFML Packet << overload to support
	//friend sf::Packet& operator<<(sf::Packet& packet, const DiscardPacket& dPacket) {

	//}

	//// SFML Packet >> overload to support
	//friend sf::Packet& operator>>(sf::Packet& packet, DiscardPacket& dPacket) {

	//}
private:
	Tile m_discard;
};

#endif