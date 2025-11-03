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

	botShotTexture.loadFromFile("img/bot_shot.png");
	userShotTexture.loadFromFile("img/user_shot.png");
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

		if (eng->isGameOver() != "") {
			render_game_finished(eng->isGameOver());
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

void SFMLRenderer::render_game_finished(std::string message) {
	while (window.isOpen()) {

		window.clear();
		window.draw(backgrSprite);

		sf::Text msg;
		msg.setFont(font);
		msg.setCharacterSize(50);
		msg.setFillColor(sf::Color::White);
		msg.setString(message);

		sf::FloatRect textBounds = msg.getLocalBounds();
		float posX = (cols * cellSize) / 2.0f - textBounds.width / 2.0f;
		float posY = (rows * cellSize) / 2.0f - textBounds.height / 2.0f;
		msg.setPosition(posX, posY);

		window.draw(msg);
		window.display();

		sf::Event event;
		while (window.pollEvent(event)) {

			if (event.type == sf::Event::Closed) {
				window.close();
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
			sf::Sprite sprite = draw_cells(cellType);
			if (sprite.getTexture() == nullptr) {
				continue;
			}

			float scaleX = (float)cellSize / sprite.getTexture()->getSize().x;
			float scaleY = (float)cellSize / sprite.getTexture()->getSize().y;
			sprite.setScale(scaleX, scaleY);

			sprite.setOrigin(sprite.getTexture()->getSize().x / 2.f, sprite.getTexture()->getSize().y / 2.f);

			sprite.setPosition(j * cellSize + cellSize / 2.f, i * cellSize + cellSize / 2.f);
			window.draw(sprite);
		}
	}

	for (const auto& p : eng->get_projectiles()) {
		sf::Sprite sprite;
		sprite.setTexture(projectileTexture);

		switch (p->get_direction()) {
		case Direction::UP:
			sprite.setRotation(180);
			break;
		case Direction::RIGHT:
			sprite.setRotation(270);
			break;
		case Direction::DOWN:
			sprite.setRotation(0);
			break;
		case Direction::LEFT:
			sprite.setRotation(90);
			break;
		}
		float scaleX = (float)cellSize / sprite.getTexture()->getSize().x;
		float scaleY = (float)cellSize / sprite.getTexture()->getSize().y;
		sprite.setScale(scaleX, scaleY);
		sprite.setOrigin(sprite.getTexture()->getSize().x / 2.f, sprite.getTexture()->getSize().y / 2.f);
		sprite.setPosition(p->get_col_pos()* cellSize + cellSize / 2.f, p->get_row_pos()* cellSize + cellSize / 2.f);
		window.draw(sprite);
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

sf::Sprite SFMLRenderer::draw_cells(char cellType) {
	sf::Sprite sprite;
	switch (cellType) {
	case '#':
		sprite.setTexture(wallTexture);
		break;
	case '^':
		if (eng->user_is_shot()) sprite.setTexture(userShotTexture);
		else {
			sprite.setTexture(userTexture);
		}
		sprite.setRotation(180);
		break;
	case '>':
		if (eng->user_is_shot()) sprite.setTexture(userShotTexture);
		else {
			sprite.setTexture(userTexture);
		}
		sprite.setRotation(270);
		break;
	case 'v':
		if (eng->user_is_shot()) sprite.setTexture(userShotTexture);
		else {
			sprite.setTexture(userTexture);
		}
		sprite.setRotation(0);
		break;
	case '<':
		if (eng->user_is_shot()) sprite.setTexture(userShotTexture);
		else {
			sprite.setTexture(userTexture);
		}
		sprite.setRotation(90);
		break;
	case 'B':
		if (eng->bot_is_shot()) sprite.setTexture(botShotTexture);
		else {
			sprite.setTexture(botTexture);
		}
		sprite.setRotation(180);
		break;
	case 'D':
		if (eng->bot_is_shot()) sprite.setTexture(botShotTexture);
		else {
			sprite.setTexture(botTexture);
		}
		sprite.setRotation(0);
		break;
	case 'b':
		if (eng->bot_is_shot()) sprite.setTexture(botShotTexture);
		else {
			sprite.setTexture(botTexture);
		}
		sprite.setRotation(270);
		break;
	case 'd':
		if (eng->bot_is_shot()) sprite.setTexture(botShotTexture);
		else {
			sprite.setTexture(botTexture);
		}
		sprite.setRotation(90);
		break;
	default:
		break;
	}
	return sprite;
}