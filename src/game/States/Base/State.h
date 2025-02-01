#ifndef STATE_H
#define STATE_H

#include "Entities/Characters/Player.h"

class State
{
private:

protected:
	std::stack<State*>* states;
	sf::RenderWindow* window;
	std::map<std::string, sf::Keyboard::Key>* supportedKeys;
	std::map<std::string, sf::Keyboard::Key> keybinds;
	bool quit;

	sf::Vector2i mousePosScreen;
	sf::Vector2i mousePosWindow;
	sf::Vector2f mousePosView;

	// Resources
	std::map<std::string, sf::Texture> textures;

	// Functions
	virtual void InitKeybinds() = 0;

public:
	State(sf::RenderWindow* window, std::map<std::string, sf::Keyboard::Key>* supportedKeys, std::stack<State*>* states);
	virtual ~State();

	const bool& GetQuit() const;
	void EndState();

	virtual void UpdateMousePositions();
	virtual void UpdateInput(float dt) = 0;
	virtual void Update(float dt) = 0;
	virtual void Render(sf::RenderTarget* target = nullptr) = 0;
};

#endif
