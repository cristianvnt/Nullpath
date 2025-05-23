#include "Game.h"

Game::Game()
{
	this->InitWindow();
	this->InitKeybinds();
	this->InitStates();
}

Game::~Game()
{
	delete this->window;

	while (!this->states.empty())
	{
		delete this->states.top();
		this->states.pop();
	}
}

void Game::InitVariables()
{
	this->window = nullptr;
	this->fullscreen = false;
	this->dt = 0.f;
}

void Game::InitWindow()
{
	std::ifstream ifs("Config/window.ini");

	if (!ifs.is_open())
	{
		std::cerr << "Error: window.ini could not be opened.\n";
		return;
	}

	this->videoModes = sf::VideoMode::getFullscreenModes();

	std::string title = "None";
	sf::VideoMode windowBounds = sf::VideoMode::getDesktopMode();
	bool fullscreen = false;
	unsigned framerateLimit = 60;
	bool verticalSyncEnabled = false;
	unsigned antialiasingLevel = 0;

	if (ifs)
	{
		std::getline(ifs, title);
		ifs >> windowBounds.size.x >> windowBounds.size.y;
		ifs >> fullscreen;
		ifs >> framerateLimit;
		ifs >> verticalSyncEnabled;
		ifs >> antialiasingLevel;
	}
	ifs.close();

	this->fullscreen = fullscreen;
	this->windowSettings.antiAliasingLevel = antialiasingLevel;

	if (this->fullscreen)
		this->window = new sf::RenderWindow(windowBounds, title, sf::State::Fullscreen, this->windowSettings);
	else
		this->window = new sf::RenderWindow(windowBounds, title, sf::Style::Titlebar | sf::Style::Close | sf::Style::Resize, 
			sf::State::Windowed, this->windowSettings);

	this->window->setKeyRepeatEnabled(false);
	this->window->setFramerateLimit(framerateLimit);
	this->window->setVerticalSyncEnabled(verticalSyncEnabled);
}

void Game::InitKeybinds()
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
		else if (keyStr == "O")
			keyCode = sf::Keyboard::Key::O;

		this->keybinds[fnc] = keyCode;
	}

	ifs.close();
}

void Game::InitStates()
{
	this->states.push(new MainMenuState(this->window, &this->keybinds, &this->states));
}

void Game::EndApplication()
{
	std::cout << "Ending Application\n";
	this->window->close();
}

void Game::UpdateDt()
{
	this->dt = this->dtClock.restart().asSeconds();
}

void Game::UpdateEvents()
{
	std::optional<sf::Event> eventOpt;
	while (eventOpt = window->pollEvent())
	{
		auto& event = *eventOpt;
		if (event.is<sf::Event::Closed>())
			window->close();

		if (auto keyEvent = event.getIf<sf::Event::KeyPressed>())
			states.top()->HandleKey(keyEvent->code);
	}
}

void Game::Update()
{
	this->UpdateEvents();

	if (!this->states.empty())
	{
		auto top = this->states.top();
		top->Update(this->dt);

		if (top->GetQuit())
		{
			delete top;
			this->states.pop();
		}
	}
	else
	{
		this->EndApplication();
	}
}

void Game::Render()
{
	this->window->clear();

	// Render stuff
	if (!this->states.empty())
		this->states.top()->Render();

	this->window->display();
}

void Game::Run()
{
	while (this->window->isOpen())
	{
		this->UpdateDt();
		this->Update();
		this->Render();
	}
}
