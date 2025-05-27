#include "MainMenuState.h"
#include "Utils/Fonts.h"

MainMenuState::MainMenuState(sf::RenderWindow* window, std::map<std::string, sf::Keyboard::Key>* keybinds, std::stack<State*>* states)
	: State(window, keybinds, states)
{
	this->InitBackground();
	this->InitFonts();
	this->InitKeybinds();
	this->InitButtons();
}

MainMenuState::~MainMenuState()
{
	for (auto it = this->buttons.begin(); it != this->buttons.end(); it++)
		delete it->second;
}

void MainMenuState::InitBackground()
{
	this->background.setSize({
		static_cast<float>(this->window->getSize().x),
		static_cast<float>(this->window->getSize().y)
		});

	if (!this->backgroundTexture.loadFromFile("Resources/Textures/background.png"))
	{
		std::cerr << "Error: failed to load texture background.png\n";
		return;
	}

	this->background.setTexture(&this->backgroundTexture);
}

void MainMenuState::InitFonts()
{
	if (!Fonts::theme.openFromFile("Resources/Fonts/ThemeVCK.ttf"))
	{
		std::cerr << "Error: Failed to load font!\n";
		return;
	}
}

void MainMenuState::InitKeybinds()
{
	std::ifstream ifs("Config/keybinds.ini");

	if (!ifs.is_open())
	{
		std::cerr << "Error: keybinds.ini could not be opened.\n";
		return;
	}

	std::string fnc, keyStr;

	while (ifs >> fnc >> keyStr)
	{
		sf::Keyboard::Key keyCode = sf::Keyboard::Key::Unknown;

		if (keyStr == "Escape") 
			keyCode = sf::Keyboard::Key::Escape;
		else if (keyStr == "Enter") 
			keyCode = sf::Keyboard::Key::Enter;

		(*this->keybinds)[fnc] = keyCode;
	}

	ifs.close();
}

void MainMenuState::InitButtons()
{
	this->buttons["GAME_STATE"] = new Button(870.f, 350.f, 150.f, 50.f,
		&Fonts::theme, "New Game", 70,
		sf::Color(255, 0, 0, 200), sf::Color(250, 250, 250, 250), sf::Color(30, 30, 30, 50),
		sf::Color(50, 50, 50, 0), sf::Color(150, 150, 150, 0), sf::Color(30, 30, 30, 0));

	this->buttons["EXIT_STATE"] = new Button(890.f, 650.f, 100.f, 50.f,
		&Fonts::theme, "Quit", 70,
		sf::Color(255, 0, 0, 200), sf::Color(250, 250, 250, 250), sf::Color(30, 30, 30, 50),
		sf::Color(50, 50, 50, 0), sf::Color(150, 150, 150, 0), sf::Color(30, 30, 30, 0));
}

void MainMenuState::HandleKey(sf::Keyboard::Key code)
{
	if (code == keybinds->at("GAME_STATE"))
	{
		this->states->push(new GameState(this->window, this->keybinds, this->states, Fonts::theme));
	}

	if (code == keybinds->at("MAINMENU_EXIT"))
	{
		window->close();
		return;
	}
}

void MainMenuState::UpdateButtons()
{
	for (auto& it : this->buttons)
		it.second->Update(this->mousePosView);

	// New Game
	if (this->buttons["GAME_STATE"]->isPressed())
	{
		this->states->push(new GameState(this->window, this->keybinds, this->states, Fonts::theme));
	}

	// Quit
	if (this->buttons["EXIT_STATE"]->isPressed())
	{
		this->EndState();
	}
}

void MainMenuState::Update(float dt)
{
	if (!window->hasFocus())
		return;

	this->UpdateMousePositions();
	this->UpdateInput();
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
