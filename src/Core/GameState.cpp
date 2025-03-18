#include "GameState.h"

GameState::GameState(sf::RenderWindow* window, std::map<std::string, sf::Keyboard::Key>* supportedKeys, std::stack<State*>* states)
	: State(window, supportedKeys, states)
{
	this->InitKeybinds();
	this->InitTextures();
	this->InitPlayers();
}

GameState::~GameState()
{
	delete this->player;
}

void GameState::InitKeybinds()
{
	std::ifstream ifs("Config/gameStateKeybinds.ini");

	if (!ifs.is_open())
	{
		std::cerr << "Error: gameStateKeybinds.ini could not be opened.\n";
		return;
	}

	std::string fnc;
	std::string keyStr;

	while (ifs >> fnc >> keyStr)
		this->keybinds[fnc] = this->supportedKeys->at(keyStr);
}

void GameState::InitTextures()
{
	if (!this->textures["PLAYER_IDLE"].loadFromFile("Resources/Textures/Sprites/Player/guy1.png"))
	{
		std::cerr << "Error: guy.png could not be loaded.\n";
		return;
	}
}

void GameState::InitPlayers()
{
	this->player = new Player(0.f, 0.f, this->textures["PLAYER_IDLE"]);
}

void GameState::UpdateInput(float dt)
{
	if (sf::Keyboard::isKeyPressed(this->keybinds["MOVE_LEFT"]))
	{
		this->player->Move(-1.f, 0.f, dt);
	}
	if (sf::Keyboard::isKeyPressed(this->keybinds["MOVE_RIGHT"]))
	{
		this->player->Move(1.f, 0.f, dt);
	}
	if (sf::Keyboard::isKeyPressed(this->keybinds["MOVE_UP"]))
	{
		this->player->Move(0.f, -1.f, dt);
	}
	if (sf::Keyboard::isKeyPressed(this->keybinds["MOVE_DOWN"]))
	{
		this->player->Move(0.f, 1.f, dt);
	}
	if (sf::Keyboard::isKeyPressed(this->keybinds["CLOSE"]))
	{
		this->EndState();
	}
}

void GameState::Update(float dt)
{
	this->UpdateMousePositions();
	this->UpdateInput(dt);

	this->player->Update(dt);
}

void GameState::Render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	this->player->Render(target);
}
