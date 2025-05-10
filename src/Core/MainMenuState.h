#ifndef MAIN_MENU_STATE_H
#define MAIN_MENU_STATE_H

#include "GameState.h"
#include "UI/Button.h"

class MainMenuState : public State
{
private:
	// Vars
	sf::Texture backgroundTexture;
	sf::RectangleShape background;
	sf::Font font;

	std::map<std::string, Button*> buttons;

	// Functions
	void InitVariables();
	void InitBackground();
	void InitFonts();
	void InitKeybinds() override;
	void InitButtons();

public:
	MainMenuState(sf::RenderWindow* window, std::map<std::string, sf::Keyboard::Key>* supportedKeys, std::stack<State*>* states);
	virtual ~MainMenuState();

	void HandleKey(sf::Keyboard::Key code) override;

	void UpdateButtons();
	void Update(float dt) override;

	void RenderButtons(sf::RenderTarget* target = nullptr);
	void Render(sf::RenderTarget* target = nullptr) override;
};

#endif