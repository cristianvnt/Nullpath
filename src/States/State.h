#ifndef STATE_H
#define STATE_H

#include "Entities/Player.h"
#include <map>
#include <stack>
#include <string>
#include <fstream>
#include <iostream>

class State
{
private:

protected:
	std::stack<State*>* states;
	sf::RenderWindow* window;
	std::map<std::string, sf::Keyboard::Key>* keybinds;
	bool quit;

	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	// Resources
	std::map<std::string, sf::Texture> textures;

	// Functions
	virtual void InitKeybinds();

public:
	State(sf::RenderWindow* window, std::map<std::string, sf::Keyboard::Key>* keybinds, std::stack<State*>* states);
	virtual ~State();

	const bool& GetQuit() const;
	void EndState();

	virtual void HandleKey(sf::Keyboard::Key code) = 0;

	virtual void UpdateMousePositions();
	virtual void UpdateInput();
	virtual void Update(float dt) = 0;
	virtual void Render(sf::RenderTarget* target = nullptr) = 0;
};

#endif
