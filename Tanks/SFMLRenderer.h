#pragma once
#include <SFML/Graphics.hpp>
#include "IRenderer.h"
#include"Field.h"
#include "GameEngine.h"
#include <memory>

class SFMLRenderer : public IRenderer{
public:
	SFMLRenderer(std::unique_ptr<IGameEngine> engine);
	void mainLoop() override;
private:
	sf::RenderWindow window;
	void render();
	std::unique_ptr<IGameEngine> eng;

	sf::Texture userTexture;
	sf::Texture botTexture;
	sf::Texture projectileTexture;
	sf::Texture wallTexture;
	sf::Texture backgrTexture;
	sf::Sprite backgrSprite;

	int cellSize;
	int rows;
	int cols;

	sf::Font font;
};