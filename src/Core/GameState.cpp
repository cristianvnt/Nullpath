#include "GameState.h"

GameState::GameState(sf::RenderWindow* window, std::map<std::string, sf::Keyboard::Key>* keybinds, std::stack<State*>* states)
	: State(window, keybinds, states),
	player(new Player()),
	raycaster(nullptr),
	map(20, 10, 32),
	minimap(nullptr)
{
	this->InitWorld();
}

GameState::~GameState()
{
	delete this->player;
	delete this->raycaster;
	delete this->minimap;
}

void GameState::InitWorld()
{
	map.GenerateMapDFS();

	// Place player ona random empty cell (odd coords to ensure valid paths)
	auto [px, py] = map.FindRandomEmpty();
	float offset = map.GetTileSize() / 2.f;
	player->SetPosition(px * map.GetTileSize() + offset, py * map.GetTileSize() + offset);

	// Reinit raycaster with the current map and player
	if (raycaster)
		delete raycaster;

	raycaster = new Raycaster(
		window->getSize().x,
		window->getSize().y,
		map.GetData(),
		map.GetWidth(),
		map.GetHeight(),
		map.GetTileSize()
	);

	if (minimap)
		delete minimap;

	minimap = new Minimap(
		map.GetWidth(),
		map.GetHeight(),
		map.GetTileSize(),
		map.GetData(),
		raycaster
	);
}

void GameState::HandleKey(sf::Keyboard::Key code)
{
	if (code == keybinds->at("GAME_EXIT"))
		EndState();

	if (code == sf::Keyboard::Key::R)
		InitWorld();
}

void GameState::Update(float dt)
{
	if (!window->hasFocus())
		return;

	this->UpdateMousePositions();
	this->UpdateInput();
	this->player->Update(dt, [this](int x, int y) { return this->GetTile(x, y); });
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

	minimap->Render(*target, player->GetX(), player->GetY(), player->GetAngle());
}

int GameState::GetTile(int x, int y) const
{
	return map.GetTile(x, y);
}
