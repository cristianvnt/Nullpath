#include "GameState.h"
#include <algorithm>
#include <array>
#include <random>
#include <cstring>

GameState::GameState(sf::RenderWindow* window, std::map<std::string, sf::Keyboard::Key>* keybinds, std::stack<State*>* states)
	: State(window, keybinds, states),
	player(new Player()),
	raycaster(nullptr),
	mapWidth(20),
	mapHeight(10),
	tileSize(32),
	mapData(new int[mapWidth * mapHeight]),
	visited(mapWidth * mapHeight, false),
	rng(std::random_device{}())
{
	this->InitRaycaster();
}

GameState::~GameState()
{
	delete this->player;
	delete this->raycaster;
	delete[] this->mapData;
}

void GameState::InitRaycaster()
{
	// Reset visited flags and generate new maze
	visited.assign(mapWidth * mapHeight, false);
	GenerateMazeDFS();

	// Place player ona random empty cell (odd coords to ensure valid paths)
	int px, py;
	do
	{
		px = (rng() % (mapWidth / 2)) * 2 + 1;
		py = (rng() % (mapHeight / 2)) * 2 + 1;
	} while (mapData[py * mapWidth + px] != 0);

	player->SetPosition(px * tileSize, py * tileSize);

	// Reinit raycaster with the current map and player
	if (raycaster)
		delete raycaster;

	this->raycaster = new Raycaster(
		this->window->getSize().x,
		this->window->getSize().y,
		this->mapData,
		this->mapWidth,
		this->mapHeight,
		this->tileSize
	);
}

void GameState::GenerateMazeDFS()
{
	// Fill entire grid with walls
	std::fill(mapData, mapData + mapWidth * mapHeight, 1);
	visited.assign(mapWidth * mapHeight, false);

	// Start at random odd coords
	int sx = (rng() % (mapWidth / 2)) * 2 + 1;
	int sy = (rng() % (mapHeight / 2)) * 2 + 1;

	CarveDFS(sx, sy);
}

void GameState::CarveDFS(int x, int y)
{
	int idx = y * mapWidth + x;
	// Mark current cell as empty and visited
	mapData[idx] = 0;
	visited[idx] = true;

	std::array<std::pair<int, int>, 4> dirs = {
		std::make_pair(0, -1),
		std::make_pair(1, 0),
		std::make_pair(0, 1),
		std::make_pair(-1, 0)
	};
	std::shuffle(dirs.begin(), dirs.end(), rng);

	for (auto& [dx, dy] : dirs)
	{
		int nx = x + dx * 2;
		int ny = y + dy * 2;

		if (nx >= 1 && ny >= 1 && nx < mapWidth - 1 && ny < mapHeight - 1 && !visited[ny * mapWidth + nx])
		{
			// Remove walls between cells and recurse into next cell
			mapData[(y + dy) * mapWidth + (x + dx)] = 0;
			CarveDFS(nx, ny);
		}
	}
}

void GameState::HandleKey(sf::Keyboard::Key code)
{
	if (code == keybinds->at("GAME_EXIT"))
		EndState();

	if (code == sf::Keyboard::Key::R)
		InitRaycaster();
}

void GameState::Update(float dt)
{
	if (!window->hasFocus())
		return;

	this->UpdateMousePositions();
	this->UpdateInput();
	this->player->Update(dt, [this](int x, int y) { return this->GetTile(x, y); });
}


void GameState::RenderMiniMap(sf::RenderTarget& target)
{
	float miniTile = tileSize * 0.6f;
	sf::RectangleShape cell({ miniTile, miniTile });
	const float margin = 10.f;
	for (int y = 0; y < mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			int v = mapData[y * mapWidth + x];
			cell.setFillColor(v > 0 ? sf::Color(30, 60, 60) : sf::Color(180, 180, 180));
			cell.setPosition({ margin + x * miniTile, margin + y * miniTile });
			target.draw(cell);
		}
	}

	// Draw border around
	sf::RectangleShape border({ mapWidth * miniTile + 2.f, mapHeight * miniTile + 2.f });
	border.setPosition({ margin - 1.f, margin - 1.f });
	border.setFillColor(sf::Color::Transparent);
	border.setOutlineColor(sf::Color::White);
	border.setOutlineThickness(3.f);
	target.draw(border);

	// World player position
	float worldX = player->GetX();
	float worldY = player->GetY();

	// Grid coordinates
	float cellX = worldX / tileSize;
	float cellY = worldY / tileSize;

	// Mini-map dot coordinates
	float dotX = margin + cellX * miniTile;
	float dotY = margin + cellY * miniTile;

	// Dot player on mini-map
	sf::CircleShape dot(miniTile * 0.25f);
	dot.setOrigin({ dot.getRadius(), dot.getRadius() });
	dot.setFillColor(sf::Color::Red);
	dot.setPosition({ dotX, dotY });
	target.draw(dot);
}

void GameState::Render(sf::RenderTarget* target)
{
	if (!target)
		target = this->window;

	// 2.5D render
	this->raycaster->Render(
		*target,
		this->player->GetX(),
		this->player->GetY(),
		this->player->GetAngle()
	);

	RenderMiniMap(*target);
}

int GameState::GetTile(int x, int y) const
{
	if (x < 0 || y < 0 || x >= this->mapWidth || y >= this->mapHeight)
		return -1;
	return this->mapData[this->mapWidth * y + x];
}
