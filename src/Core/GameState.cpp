#include "GameState.h"

GameState::GameState(sf::RenderWindow* window, std::map<std::string, sf::Keyboard::Key>* keybinds, std::stack<State*>* states)
	: State(window, keybinds, states),
	player(new Player()),
	raycaster(nullptr),
	map(30, 30, 32),
	minimap(nullptr)
{
	this->InitWorldDFS();
}

GameState::~GameState()
{
	delete this->player;
	delete this->raycaster;
	delete this->minimap;
}

void GameState::InitWorldDFS()
{
	RegenerateWorld([&](Map& map) {
		DFSGenerator dfs;
		dfs.Generate(map);
	});
}

void GameState::InitWorldBSP()
{
	RegenerateWorld([&](Map& map) {
		BSPGenerator bsp(3, 1.25f, 2);
		bsp.Generate(map);
	});
}

void GameState::RegenerateWorld(const std::function<void(Map&)>& generator)
{
	map.Clear(Cell::Wall);
	generator(map);

	// Place player ona random empty cell (odd coords to ensure valid paths)
	if (std::optional<std::pair<int, int>> pos = map.FindRandomEmpty(); pos)
	{
		auto& [px, py] = *pos;
		float offset = map.GetCellSize() * 0.5f;
		player->SetPosition(px * map.GetCellSize() + offset, py * map.GetCellSize() + offset);
	}

	// Reinit raycaster with the current map and player
	if (raycaster)
		delete raycaster;

	raycaster = new Raycaster(
		window->getSize().x,
		window->getSize().y,
		map.GetData(),
		map.GetWidth(),
		map.GetHeight(),
		map.GetCellSize()
	);

	if (minimap)
		delete minimap;

	minimap = new Minimap(
		map.GetWidth(),
		map.GetHeight(),
		map.GetCellSize(),
		map.GetData(),
		raycaster
	);
}

void GameState::HandleKey(sf::Keyboard::Key code)
{
	if (code == keybinds->at("GAME_EXIT"))
		EndState();

	if (code == sf::Keyboard::Key::R)
		InitWorldDFS();

	if (code == sf::Keyboard::Key::T)
		InitWorldBSP();
}

void GameState::Update(float dt)
{
	if (!window->hasFocus())
		return;

	this->UpdateMousePositions();
	this->UpdateInput();
	this->player->Update(dt, [this](int x, int y) { return this->GetCell(x, y); });
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

int GameState::GetCell(int x, int y) const
{
	return static_cast<int>(map.GetCell(x, y));
}
