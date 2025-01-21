#ifndef STATE_H
#define STATE_H

#include "../Entities/Entity.h"

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

	//Resources
	std::vector<sf::Texture> textures;

	//Functions
	virtual void InitKeybinds() = 0;

public:
	State(sf::RenderWindow* window, std::map<std::string, sf::Keyboard::Key>* supportedKeys, std::stack<State*>* states);
	virtual ~State();

	const bool& GetQuit() const;

	virtual void CheckQuitState();
	virtual void EndState() = 0;
	virtual void UpdateMousePositions();
	virtual void UpdateInput(const float& dt) = 0;
	virtual void Update(const float& dt) = 0;
	virtual void Render(sf::RenderTarget* target = nullptr) = 0;
};

#endif
