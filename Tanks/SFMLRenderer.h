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
};