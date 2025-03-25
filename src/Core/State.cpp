#include "State.h"

State::State(sf::RenderWindow* window, std::map<std::string, sf::Keyboard::Key>* keybinds, std::stack<State*>* states)
{
	this->window = window;
	this->keybinds = keybinds;
	this->states = states;
	this->quit = false;
}

State::~State()
{

}

void State::InitKeybinds()
{
}

const bool& State::GetQuit() const
{
	return this->quit;
}

void State::EndState()
{
	this->quit = true;
}

void State::UpdateMousePositions()
{
	this->mousePosScreen = sf::Mouse::getPosition();
	this->mousePosWindow = sf::Mouse::getPosition(*this->window);
	this->mousePosView = this->window->mapPixelToCoords(sf::Mouse::getPosition(*this->window));
}

void State::UpdateInput()
{
}
