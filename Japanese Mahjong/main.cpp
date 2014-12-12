/*
Designed by Xuan Yuan Huang, a desperate UCLA undergrad CS student.
Copyright 2014 Xuan Yuan Huang

Built with the SFML 2.1 Engine.
SFML 2.1 is licensed under zlib/libpng. 

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

Developed in Visual Studio 2012 (thanks for the free copy, MS Dreamspark!).
Visual Studio is, of course, Microsoft's proprietary IDE.
*/
#include <SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/System/String.hpp>
#include <iostream>

int main()
{
	// The following block is all test code from SFML's setup tutorial
	/*
    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }
	*/

	sf::TcpSocket socket;
	socket.setBlocking(true);
	socket.connect("127.0.0.1", 53000); // TODO: Allow user to input host IP!

	/* Debug code
	sf::Text text;
	sf::Font font;
	font.loadFromFile("arial.ttf"); /
	// select the font
	text.setFont(font); // font is a sf::Font

	// set the string to display
	text.setString("test");

	// set the character size
	text.setCharacterSize(24); // in pixels, not points!

	// set the color
	text.setColor(sf::Color::Red);

	// set the text style
	text.setStyle(sf::Text::Bold | sf::Text::Underlined);

    sf::RenderWindow window(sf::VideoMode(1280, 720), "SFML works!");

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(text);
        window.display();
    }
	*/ 



    return 0;
}
