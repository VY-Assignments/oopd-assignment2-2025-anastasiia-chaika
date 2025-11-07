#pragma once
#include <SFML/Graphics.hpp>
#include "GameEngine.h"
#include "BotTank.h"
#include "CellType.h"
#include "GameMode.h"
#include <memory>
#include <string>

class SFMLRenderer {
public:
	SFMLRenderer(std::unique_ptr<IGameEngine> engine);
	void mainLoop();
private:
	sf::RenderWindow window;
	void render();
	void render_game_finished(GameFinished status);
	GameMode render_game_start();
	std::unique_ptr<IGameEngine> eng = nullptr;
	sf::Sprite draw_cells(CellType c);

	sf::Texture userTexture;
	sf::Texture botTexture;
	sf::Texture projectileTexture;
	sf::Texture wallTexture;
	sf::Texture backgrTexture;
	sf::Sprite backgrSprite;
	sf::Texture botShotTexture;
	sf::Texture userShotTexture;
	sf::Texture buttonEasyT;
	sf::Sprite buttonEasyS;
	sf::Texture buttonHardT;
	sf::Sprite buttonHardS;

	int cellSize;
	int rows;
	int cols;

	sf::Font font;
};