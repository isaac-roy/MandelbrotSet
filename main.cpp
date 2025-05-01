#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <sstream>
#include <vector>
#include "ComplexPlane.h"
#include <complex>

using namespace std;
using namespace sf;

int main()
{
    VideoMode desktop(                                          // Isaac (All until the first Eric)
        VideoMode::getDesktopMode().width / 2,
        VideoMode::getDesktopMode().height / 2
    );

    RenderWindow window(desktop, "Mandelbrot Set");

    ComplexPlane mandelbrotPlane(
        window.getSize().x,
        window.getSize().y
    );

    Font font;

    if(!font.loadFromFile("/usr/share/fonts/truetype/ubuntu/UbuntuSans[wdth,wght].ttf"))
    {
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf"))
		{
			cout << "Unable to load font from memory" << endl;
			return -1;
		}
    }

	Text userText;
	userText.setFont(font);
	userText.setCharacterSize(16);
    userText.setFillColor(Color::White);
	userText.setPosition(25, 12.5);

    while(window.isOpen())
    {
        /*
		****************************************
		Handle the players input
		****************************************
		*/

        Event event;
		while(window.pollEvent(event))
		{
		    if(event.type == Event::Closed)
		    {
				// Quit the game when the window is closed
				window.close();
		    }

            if(event.type == Event::MouseButtonPressed)                                     // Eric
            {
                if(event.mouseButton.button == Mouse::Left)
                {
                    mandelbrotPlane.zoomIn();
                    mandelbrotPlane.setCenter(Mouse::getPosition(window));
                }

                if(event.mouseButton.button == Mouse::Right)
                {
                    mandelbrotPlane.zoomOut();
                    mandelbrotPlane.setCenter(Mouse::getPosition(window));
                }
            }

            if(event.type == Event::MouseMoved)                                         // Isaac
            {
                mandelbrotPlane.setMouseLocation(Mouse::getPosition(window));
            }
        }
        if(Keyboard::isKeyPressed(Keyboard::Escape))                                    // Isaac
		{
			window.close();
		}

        /*
		****************************************
		Update
		****************************************
		*/

        mandelbrotPlane.updateRender();                                                 // Eric
        mandelbrotPlane.loadText(userText);

        /*
		****************************************
		Draw
		****************************************
		*/

        window.clear();
        window.draw(mandelbrotPlane);
        window.draw(userText);
		window.display();
    }

    return 0;
}
