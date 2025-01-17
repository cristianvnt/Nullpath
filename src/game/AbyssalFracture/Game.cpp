#include "Game.h"

void Game::InitWindow()
{
	std::ifstream ifs("resources/config/window.ini");

	if (!ifs.is_open())
	{
		std::cout << "Current working directory: "
			<< std::filesystem::current_path() << "\n";
		std::cerr << "Error: window.ini could not be opened.\n";
		return;
	}

	std::string title = "None";
	sf::VideoMode windowBounds({ 800, 600 });
	unsigned framerateLimit = 60;
	bool verticalSyncEnabled = false;

	if (ifs)
	{
		std::getline(ifs, title);
		ifs >> windowBounds.size.x >> windowBounds.size.y;
		ifs >> framerateLimit;
		ifs >> verticalSyncEnabled;
	}

	this->window = new sf::RenderWindow(windowBounds, title);
	this->window->setFramerateLimit(framerateLimit);
	this->window->setVerticalSyncEnabled(verticalSyncEnabled);
}

void Game::InitKeys()
{
	std::ifstream ifs("resources/config/supportedKeys.ini");

	if (!ifs.is_open())
	{
		std::cerr << "Error: supported_keys.ini could not be opened.\n";
		return;
	}

	std::string key;
	int key_value = 0;

	while (ifs >> key >> key_value)
		this->supportedKeys[key] = static_cast<sf::Keyboard::Key>(key_value);

	// DEBUG
	for (auto& i : this->supportedKeys)
		std::cout << i.first << " " << static_cast<int>(i.second) << "\n";
}

void Game::InitStates()
{
	this->states.push(new MainMenuState(this->window, &this->supportedKeys));
}

Game::Game()
{
	this->InitWindow();
	this->InitKeys();
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
	while (const std::optional event = this->window->pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			this->window->close();
	}
}

void Game::Update()
{
	this->UpdateEvents();

	if (!this->states.empty())
	{
		this->states.top()->Update(this->dt);

		if (this->states.top()->GetQuit())
		{
			this->states.top()->EndState();
			delete this->states.top();
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
