#include "../headers/Game.h"

void Game::initWindow()
{
	this->window = new sf::RenderWindow(sf::VideoMode({ 800u, 600u }), "The Abyssal Shift");
	this->window->setFramerateLimit(60);
}

Game::Game()
{
	this->initWindow();
}

Game::~Game()
{
	delete this->window;
}

void Game::updateEvents()
{
	while (const std::optional event = this->window->pollEvent())
	{
		if (event->is<sf::Event::Closed>())
			this->window->close();
	}
}

void Game::update()
{
	this->updateEvents();
}

void Game::render()
{
	this->window->clear();

	// Render stuff

	this->window->display();
}

void Game::run()
{
	while (this->window->isOpen())
	{
		this->update();
		this->render();
	}
}
