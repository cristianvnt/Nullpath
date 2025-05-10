#include "GameState.h"
#include <algorithm>
#include <array>
#include <random>
#include <cstring>

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

void GameState::generateMazeDFS()
{
	for (int i = 0; i < mapWidth * mapHeight; i++)
		mapData[i] = 1;

	visited.assign(mapWidth * mapHeight, false);

	int sx = static_cast<int>(player->GetX() / tileSize);
	int sy = static_cast<int>(player->GetY() / tileSize);

	carveDFS(sx, sy);
}

void GameState::carveDFS(int x, int y)
{
	int idx = y * mapWidth + x;
	visited[idx] = true;
	mapData[idx] = 0;

	std::array<std::pair<int, int>, 4> dirs = {
		std::make_pair(0, -1),
		std::make_pair(1, 0),
		std::make_pair(0, 1),
		std::make_pair(-1, 0)
	};

	std::shuffle(dirs.begin(), dirs.end(), rng);

	for (auto &[dx, dy] : dirs)
	{
		int nx = x + dx;
		int ny = y + dy;

		if (nx >= 1 && nx < mapWidth - 1 && ny >= 1 && ny < mapHeight - 1 && !visited[ny * mapWidth + nx])
			carveDFS(nx, ny);
	}
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

	visited.assign(mapWidth * mapHeight, false);
	generateMazeDFS();

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
		this->EndState();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::R))
		generateMazeDFS();
}

void GameState::Render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	// Mini-map
	float miniTile = tileSize * 0.25f;
	sf::RectangleShape cell({ miniTile, miniTile });
	const float margin = 10.f;
	for (int y = 0; y < mapHeight; y++) 
	{
		for (int x = 0; x < mapWidth; x++) 
		{
			int v = mapData[y * mapWidth + x];
			cell.setFillColor(v > 0 ? sf::Color(60, 60, 60) : sf::Color(180, 180, 180));
			cell.setPosition({ margin + x * miniTile, margin + y * miniTile });
			target->draw(cell);
		}
	}

	// And its border
	sf::RectangleShape border({ mapWidth * miniTile + 2.f, mapHeight * miniTile + 2.f });
	border.setPosition({ margin - 1.f, margin - 1.f });
	border.setFillColor(sf::Color::Transparent);
	border.setOutlineColor(sf::Color::White);
	border.setOutlineThickness(1.f);
	target->draw(border);

	// 2.5D render
	this->raycaster->Render(
		*target,
		this->player->GetX(),
		this->player->GetY(),
		this->player->GetAngle()
	);
}

int GameState::GetTile(int x, int y) const
{
	if (x < 0 || y < 0 || x >= this->mapWidth || y >= this->mapHeight)
		return -1;
	return this->mapData[this->mapWidth * y + x];
}
