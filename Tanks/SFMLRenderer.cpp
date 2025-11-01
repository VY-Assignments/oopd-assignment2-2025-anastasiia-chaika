#include "SFMLRenderer.h"
#include "GameEngine.h"
#include <SFML/Graphics.hpp>

SFMLRenderer::SFMLRenderer(std::unique_ptr<IGameEngine> engine) {
	eng = std::move(engine);

	rows = eng->get_field().size();
	cols = eng->get_field()[0].size();
	cellSize = 40;

	window.create(sf::VideoMode(cols*cellSize, rows*cellSize), "Tanks");
	window.setFramerateLimit(60);

	userTexture.loadFromFile("img/user.png");
	botTexture.loadFromFile("img/bot.png");
	projectileTexture.loadFromFile("img/projectile.png");
	wallTexture.loadFromFile("img/wall.png");
	backgrTexture.loadFromFile("img/grass.jpg");
	backgrSprite.setTexture(backgrTexture);

	float scaleX = (cols * cellSize) / (float)backgrTexture.getSize().x;
	float scaleY = (rows * cellSize) / (float)backgrTexture.getSize().y;
	backgrSprite.setScale(scaleX, scaleY);

	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
}

void SFMLRenderer::mainLoop() {
	while (window.isOpen()) {
		eng->update_field();
		render();
		eng->bot_shoot();

		if (eng->isGameOver()) {
			return;
		}
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;
			case sf::Event::KeyPressed:
				Direction d = Direction::NODIRECTION;
				switch (event.key.code) {
				case sf::Keyboard::Up:
					d = Direction::UP;
					break;
				case sf::Keyboard::Down:
					d = Direction::DOWN;
					break;
				case sf::Keyboard::Left:
					d = Direction::LEFT;
					break;
				case sf::Keyboard::Right:
					d = Direction::RIGHT;
					break;
				case sf::Keyboard::Space:
					eng->user_shoot();
					break;
				}
				if (d != Direction::NODIRECTION) {
					eng->move_user_tank(d);
				}
				break;
			}
		}
	}
}



void SFMLRenderer::render() {
	window.clear();
	window.draw(backgrSprite);

	const auto& field_grid = eng->get_field();

	for (int i = 0; i < field_grid.size(); i++) {
		for (int j = 0; j < field_grid[i].size(); j++) {
			char cellType = field_grid[i][j];
			sf::Sprite sprite;

			switch (cellType) {
			case '#':
				sprite.setTexture(wallTexture);
				break;
			case '^':
			case '>': 
			case 'v': 
			case '<':
				sprite.setTexture(userTexture);
				break;
			case 'B':
			case 'D':
			case 'b':
			case 'd':
				sprite.setTexture(botTexture);
				break;
			case '*':
				sprite.setTexture(projectileTexture);
				break;
			default:
				continue;
			}
			float scaleX = (float)cellSize / sprite.getTexture()->getSize().x;
			float scaleY = (float)cellSize / sprite.getTexture()->getSize().y;
			sprite.setScale(scaleX, scaleY);

			sprite.setOrigin(sprite.getTexture()->getSize().x / 2.f, sprite.getTexture()->getSize().y / 2.f);

			sprite.setPosition(j * cellSize + cellSize / 2.f, i * cellSize + cellSize / 2.f);

			if (cellType == '^') sprite.setRotation(180);
			else if (cellType == '>') sprite.setRotation(270);
			else if (cellType == 'v') sprite.setRotation(0);
			else if (cellType == '<') sprite.setRotation(90);

			if (cellType == 'B') sprite.setRotation(180);
			else if (cellType == 'b') sprite.setRotation(270);
			else if (cellType == 'D') sprite.setRotation(0);
			else if (cellType == 'd') sprite.setRotation(90);

			window.draw(sprite);
		}
	}

	sf::Text userHP;
	userHP.setFont(font);
	userHP.setCharacterSize(25);
	userHP.setFillColor(sf::Color::White);
	userHP.setString("User HP: " + std::to_string(eng->get_user_hp()));
	userHP.setPosition(5, 5);
	window.draw(userHP);

	sf::Text botHP;
	botHP.setFont(font);
	botHP.setCharacterSize(25);
	botHP.setFillColor(sf::Color::White);
	botHP.setString("Bot HP: " + std::to_string(eng->get_bot_hp()));
	botHP.setPosition(650, 5);
	window.draw(botHP);

	window.display();
}