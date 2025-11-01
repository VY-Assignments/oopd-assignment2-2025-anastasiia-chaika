#include "SFMLRenderer.h"
#include "GameEngine.h"
#include <SFML/Graphics.hpp>

SFMLRenderer::SFMLRenderer(std::unique_ptr<IGameEngine> engine) {
	window.create(sf::VideoMode(800, 600), "Tanks");
	window.setFramerateLimit(60);
	eng = std::move(engine);
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
	window.clear(sf::Color::Black);

	const auto& field_grid = eng->get_field();
	int cellSize = 20;

	for (int i = 0; i < field_grid.size(); i++) {
		for (int j = 0; j < field_grid[i].size(); j++) {
			sf::RectangleShape cell(sf::Vector2f(cellSize, cellSize));
			cell.setPosition(i * cellSize, j * cellSize);

			switch (field_grid[i][j]) {
			case '#':
				cell.setFillColor(sf::Color::White);
				break;
			case '^': case '>': case 'v': case '<':
				cell.setFillColor(sf::Color::Green);
				break;
			case 'B':
				cell.setFillColor(sf::Color::Red);
				break;
			case '*':
				cell.setFillColor(sf::Color::Blue);
				break;
			default:
				cell.setFillColor(sf::Color::Black);
				break;
			}

			window.draw(cell);
		}
	}

	window.display();
}