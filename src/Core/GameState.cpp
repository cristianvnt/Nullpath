#include "GameState.h"

GameState::GameState(sf::RenderWindow* window, std::map<std::string, sf::Keyboard::Key>* keybinds, std::stack<State*>* states)
	: State(window, keybinds, states)
{
	this->InitPlayers();
	this->InitRaycaster();
}

GameState::~GameState()
{
	delete this->player;
	delete this->raycaster;
	delete[] this->mapData;
}

void GameState::InitPlayers()
{
	this->player = new Player();

	int tileX = static_cast<int>(this->player->GetX()) / this->tileSize;
	int tileY = static_cast<int>(this->player->GetY()) / this->tileSize;

	if (tileX < 0 || tileX >= this->mapWidth || tileY < 0 || tileY >= this->mapHeight)
		std::cerr << "Error: Player starting pos outside the map!\n";
	else if (this->GetTile(tileX, tileY) != 0)
		std::cerr << "Error: Player starting pos is on a wall tiles! (tile value: " << this->mapData[tileY * this->mapWidth + tileX] << " )\n";
}

void GameState::InitRaycaster()
{
	this->mapWidth = 10;
	this->mapHeight = 10;
	this->tileSize = 32;

	const int staticMap[] = 
	{
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 2, 0, 2, 0, 3, 0, 0, 1,
		1, 0, 0, 0, 0, 0, 3, 0, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 2, 2, 2, 2, 2, 2, 0, 1,
		1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
		1, 0, 3, 3, 3, 3, 3, 3, 0, 1,
		1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
	};

	this->mapData = new int[mapWidth * mapHeight];
	std::memcpy(this->mapData, staticMap, sizeof(staticMap));

	this->raycaster = new Raycaster(
		this->window->getSize().x,
		this->window->getSize().y,
		this->mapData,
		this->mapWidth,
		this->mapHeight,
		this->tileSize
	);
}

void GameState::Update(float dt)
{
	this->UpdateMousePositions();
	this->UpdateInput();
	this->player->Update(dt, [this](int x, int y) {
		return this->GetTile(x, y);
		});
}

void GameState::UpdateInput()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
	{
		this->EndState();
	}
}

void GameState::Render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	// this->player->Render(target);
	this->raycaster->Render(*target, this->player->GetX(), this->player->GetY(), this->player->GetAngle());
}

int GameState::GetTile(int x, int y) const
{
	if (x < 0 || y < 0 || x >= this->mapWidth || y >= this->mapHeight)
		return -1;
	return this->mapData[this->mapWidth * y + x];
}
