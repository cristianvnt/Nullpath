#include "MainMenuState.h"

void MainMenuState::InitFonts()
{
	if (!this->font.openFromFile("fonts/Stroke.ttf")) 
	{
		std::cerr << "Failed to load font!" << std::endl;
		return;
	}
}

void MainMenuState::InitKeybinds()
{
	std::ifstream ifs("gameStateKeybinds.ini");

	if (!ifs.is_open())
	{
		std::cerr << "Error: gameStateKeybinds.ini could not be opened.\n";
		return;
	}

	if (ifs)
	{
		std::string fnc;
		std::string keyStr;

		while (ifs >> fnc >> keyStr)
			this->keybinds[fnc] = this->supportedKeys->at(keyStr);
	}
}

MainMenuState::MainMenuState(sf::RenderWindow* window, std::map<std::string, sf::Keyboard::Key>* supportedKeys)
	: State(window, supportedKeys)
{
	this->InitFonts();
	this->InitKeybinds();

	this->gameStateBTN = new Button(100, 100, 150, 50, 
		&this->font, "New Game", 
		sf::Color(50, 50, 50, 200), sf::Color(150, 150, 150, 255), sf::Color(30, 30, 30, 200));

	this->background.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	this->background.setFillColor(sf::Color::Black);
}

MainMenuState::~MainMenuState()
{
	delete this->gameStateBTN;
}

void MainMenuState::EndState()
{
	std::cout << "MainMenuState::EndState()\n";
}

void MainMenuState::UpdateInput(const float& dt)
{
	this->CheckQuitState();
}

void MainMenuState::Update(const float& dt)
{
	this->UpdateMousePositions();
	this->UpdateInput(dt);

	this->gameStateBTN->Update(this->mousePosView);
}

void MainMenuState::Render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	target->draw(this->background);

	this->gameStateBTN->Render(target);
}
