#ifndef BUTTON_H
#define BUTTON_H

class Button
{
private:

public:
	Button();
	virtual ~Button();

	// Functions
	void Update(const sf::Vector2f& mousePos);
	void Render(sf::RenderTarget& target);
};

#endif