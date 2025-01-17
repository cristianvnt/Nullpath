#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "GameState.h"
#include "../Resources/Button.h"

class MainMenuState : public State
{
private:
	//Vars
	sf::RectangleShape background;
	sf::Font font;

	Button* gameStateBTN;

	//Functions
	void InitFonts();
	void InitKeybinds() override;

public:
	MainMenuState(sf::RenderWindow* window, std::map<std::string, sf::Keyboard::Key>* supportedKeys);
	virtual ~MainMenuState();

	void EndState() override;

	void UpdateInput(const float& dt) override;
	void Update(const float& dt) override;
	void Render(sf::RenderTarget* target = nullptr) override;
};

#endif