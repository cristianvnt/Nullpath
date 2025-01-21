#include "MainMenuState.h"

void MainMenuState::InitFonts()
{
	if (!this->font.openFromFile("resources/Fonts/Stroke.ttf"))
	{
		std::cerr << "Failed to load font!\n";
		return;
	}
}

void MainMenuState::InitKeybinds()
{
	std::ifstream ifs("resources/Config/mainMenuKeybinds.ini");

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

void MainMenuState::InitButtons()
{
	this->buttons["GAME_STATE"] = new Button(350, 350, 150, 50,
		&this->font, "New Game",
		sf::Color(50, 50, 50, 200), sf::Color(150, 150, 150, 255), sf::Color(30, 30, 30, 200));

	this->buttons["EXIT_STATE"] = new Button(375, 410, 100, 50,
		&this->font, "Quit",
		sf::Color(150, 150, 50, 200), sf::Color(150, 150, 150, 255), sf::Color(30, 30, 30, 200));
}

MainMenuState::MainMenuState(sf::RenderWindow* window, std::map<std::string, sf::Keyboard::Key>* supportedKeys, std::stack<State*>* states)
	: State(window, supportedKeys, states)
{
	this->InitFonts();
	this->InitKeybinds();
	this->InitButtons();

	this->background.setSize(sf::Vector2f(window->getSize().x, window->getSize().y));
	this->background.setFillColor(sf::Color::Black);
}

MainMenuState::~MainMenuState()
{
	for (auto it = this->buttons.begin(); it != this->buttons.end(); it++)
		delete it->second;
}

void MainMenuState::EndState()
{
	std::cout << "MainMenuState::EndState()\n";
}

void MainMenuState::UpdateInput(const float& dt)
{
	this->CheckQuitState();
}

void MainMenuState::UpdateButtons()
{
	for (auto& it : this->buttons)
		it.second->Update(this->mousePosView);

	//New Game
	if (this->buttons["GAME_STATE"]->isPressed())
	{
		this->states->push(new GameState(this->window, this->supportedKeys, this->states));
	}

	//Quit
	if (this->buttons["EXIT_STATE"]->isPressed())
	{
		this->quit = true;
	}
}

void MainMenuState::Update(const float& dt)
{
	this->UpdateMousePositions();
	this->UpdateInput(dt);
	this->UpdateButtons();
}

void MainMenuState::RenderButtons(sf::RenderTarget* target)
{
	for (auto& it : this->buttons)
		it.second->Render(target);
}

void MainMenuState::Render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	target->draw(this->background);

	this->RenderButtons(target);
}
