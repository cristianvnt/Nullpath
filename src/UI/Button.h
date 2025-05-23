#ifndef BUTTON_H
#define BUTTON_H

#include <iostream>
#include <cstdlib>
#include <sstream>

#include <SFML/Graphics.hpp>

enum ButtonStates
{
	BTN_IDLE = 0,
	BTN_HOVER,
	BTN_ACTIVE
};

class Button
{
private:
	short unsigned buttonState;

	sf::RectangleShape shape;
	sf::Font* font;
	sf::Text text;

	sf::Color textIdleColor;
	sf::Color textHoverColor;
	sf::Color textActiveColor;

	sf::Color idleColor;
	sf::Color hoverColor;
	sf::Color activeColor;

public:
	Button(float x, float y, float width, float height,
		sf::Font* font, std::string text, unsigned charSize,
		sf::Color textIdleColor, sf::Color textHoverColor, sf::Color textActiveColor,
		sf::Color idleColor, sf::Color hoverColor, sf::Color activeColor);
	virtual ~Button();

	// Accessors
	inline const bool isPressed() const { return this->buttonState == BTN_ACTIVE; }

	// Functions
	void Update(const sf::Vector2f mousePos);
	void Render(sf::RenderTarget* target);
};

#endif