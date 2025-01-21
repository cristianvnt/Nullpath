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

	std::map<std::string, Button*> buttons;

	//Functions
	void InitFonts();
	void InitKeybinds() override;
	void InitButtons();

public:
	MainMenuState(sf::RenderWindow* window, std::map<std::string, sf::Keyboard::Key>* supportedKeys, std::stack<State*>* states);
	virtual ~MainMenuState();

	void EndState() override;

	void UpdateInput(const float& dt) override;
	void UpdateButtons();
	void Update(const float& dt) override;
	void RenderButtons(sf::RenderTarget* target = nullptr);
	void Render(sf::RenderTarget* target = nullptr) override;
};

#endif