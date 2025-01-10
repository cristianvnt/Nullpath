#include "Game.h"

void Game::InitWindow()
{
	std::ifstream wCfg("window.ini");

	std::string title = "None";
	sf::VideoMode windowBounds({ 800, 600 });
	unsigned framerateLimit = 60;
	bool verticalSyncEnabled = false;

	if (wCfg.is_open())
	{
		std::getline(wCfg, title);
		wCfg >> windowBounds.size.x >> windowBounds.size.y;
		wCfg >> framerateLimit;
		wCfg >> verticalSyncEnabled;
	}

	this->window = new sf::RenderWindow(windowBounds, title);
	this->window->setFramerateLimit(framerateLimit);
	this->window->setVerticalSyncEnabled(verticalSyncEnabled);
}

Game::Game()
{
	this->InitWindow();
}

Game::~Game()
{
	delete this->window;
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
}

void Game::Render()
{
	this->window->clear();

	// Render stuff

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
