#include "GameState.h"
#include "Fonts.h"

GameState::GameState(sf::RenderWindow* window, std::map<std::string, sf::Keyboard::Key>* keybinds, std::stack<State*>* states, sf::Font& sharedFont)
	: State(window, keybinds, states),
	player(new Player()),
	raycaster(nullptr),
	map(30, 30, 32),
	minimap(nullptr),
	showMinimap(false),
	debugFont(sharedFont)
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
	showDebug = false;
	usingBSP = false;
	RegenerateWorld([&](Map& map) {
		DFSGenerator dfs;
		dfs.Generate(map);
	});
}

void GameState::InitWorldBSP()
{
	showDebug = false;
	usingBSP = true;
	bspGen = std::make_unique<BSPGenerator>(3, 1.25f, 2);
	RegenerateWorld([&](Map& map) {
		bspGen->Generate(map);
		roomMarkers = bspGen->GetRoomMarkers();
	});
}

void GameState::RegenerateWorld(const std::function<void(Map&)>& generator)
{
	map.Clear(Cell::Wall);
	generator(map);

	// Place player on a random empty cell (odd coords to ensure valid paths)
	if (std::optional<std::pair<int, int>> pos = map.FindRandomEmpty(); pos)
	{
		auto& [px, py] = *pos;
		float offset = map.GetCellSize() * 0.5f;
		player->SetPosition(px * map.GetCellSize() + offset, py * map.GetCellSize() + offset);
	}

	// Reinitialize raycaster with the current map and player
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
		raycaster,
		roomMarkers,
		Fonts::theme
	);

	minimap->SetDrawRoomIDs(showDebug);
}

void GameState::HandleKey(sf::Keyboard::Key code)
{
	if (code == keybinds->at("GAME_EXIT"))
		EndState();

	if (code == sf::Keyboard::Key::R)
		InitWorldDFS();

	if (code == sf::Keyboard::Key::T)
		InitWorldBSP();

	if (code == sf::Keyboard::Key::M)
		showMinimap = !showMinimap;

	if (code == sf::Keyboard::Key::X && usingBSP)
	{
		showDebug = !showDebug;
		if (minimap)
			minimap->SetDrawRoomIDs(showDebug);
	}
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

	if (showMinimap && minimap)
		minimap->Render(*target, player->GetX(), player->GetY(), player->GetAngle());

	if (usingBSP && showDebug)
		bspGen->RenderDebug(*target, map);
}

int GameState::GetCell(int x, int y) const
{
	return static_cast<int>(map.GetCell(x, y));
}
