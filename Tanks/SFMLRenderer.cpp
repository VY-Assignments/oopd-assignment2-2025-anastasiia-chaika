#include "SFMLRenderer.h"
#include "GameEngine.h"
#include <SFML/Graphics.hpp>
#include "GameFinished.h"
#include "BotTank.h"
#include "CellType.h"
#include "GameMode.h"

#include <iostream>

SFMLRenderer::SFMLRenderer(std::unique_ptr<IGameEngine> e) {
	eng = std::move(e);

	rows = 20;
	cols = 20;
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

	buttonEasyT.loadFromFile("img/easy.png");
	buttonHardT.loadFromFile("img/hard.png");

	buttonRestartT.loadFromFile("img/restart.png");
	buttonExitT.loadFromFile("img/exit.png");

	buttonEasyS.setTexture(buttonEasyT);
	buttonHardS.setTexture(buttonHardT);

	buttonRestartS.setTexture(buttonRestartT);
	buttonExitS.setTexture(buttonExitT);

	float scaleX = (cols * cellSize) / (float)backgrTexture.getSize().x;
	float scaleY = (rows * cellSize) / (float)backgrTexture.getSize().y;
	backgrSprite.setScale(scaleX, scaleY);

	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
}

void SFMLRenderer::mainLoop() {
	GameMode mode = render_game_start();
	eng = IGameEngine::create_game_engine();
	eng->start_game(mode);
	game_finished = false;
	while (window.isOpen()) {
		eng->update_field();
		render();

		eng->bot_shoot();

		if (eng->isGameOver() != GameFinished::INPROGRESS) {
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

void SFMLRenderer::render_game_finished(GameFinished status) {
	while (window.isOpen()) {

		window.clear();
		window.draw(backgrSprite);

		sf::Text msg;
		msg.setFont(font);
		msg.setCharacterSize(50);
		msg.setFillColor(sf::Color::White);
		if (status == GameFinished::USERLOST) {
			msg.setString("Game Over!");
		}
		else {
			msg.setString("Victory!");
		}


		sf::FloatRect textBounds = msg.getLocalBounds();
		float posX = (cols * cellSize) / 2.0f - textBounds.width / 2.0f;
		float posY = (rows * cellSize) / 2.0f - textBounds.height / 2.0f;
		msg.setPosition(posX, posY);

		float windowWidth = cols * cellSize;
		float windowHeight = rows * cellSize;

		float buttonWidth = windowWidth * 0.45f;
		float buttonHeight = windowHeight * 0.15f;

		float scaleX = buttonWidth / buttonRestartS.getTexture()->getSize().x;
		float scaleY = buttonHeight / buttonExitS.getTexture()->getSize().y;

		buttonRestartS.setScale(scaleX, scaleY);
		buttonExitS.setScale(scaleX, scaleY);

		float centerX = (windowWidth - buttonWidth) / 2.f;
		float spacing = 60.f;
		float centerY = (windowHeight - (2 * buttonHeight + spacing)) / 2.f;

		buttonRestartS.setPosition(centerX, centerY);
		buttonExitS.setPosition(centerX, centerY + buttonHeight + spacing);

		msg.setPosition((windowWidth - msg.getGlobalBounds().width) / 2.f, 100);

		window.draw(buttonRestartS);
		window.draw(buttonExitS);
		window.draw(msg);
		window.display();

		sf::Event event;
		while (window.pollEvent(event)) {

			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::MouseButtonPressed: {
				sf::Vector2i mousePos = sf::Mouse::getPosition(window);
				sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
				if (buttonRestartS.getGlobalBounds().contains(mousePosF)) {
					eng->restart();
					mainLoop();
				}
				else if (buttonExitS.getGlobalBounds().contains(mousePosF)) {
					window.close();
				}
				break;
			}
			}
		}
	}
}

GameMode SFMLRenderer::render_game_start() {
	float windowWidth = cols * cellSize;
	float windowHeight = rows * cellSize;

	float buttonWidth = windowWidth * 0.45f;
	float buttonHeight = windowHeight * 0.15f;

	float scaleX = buttonWidth / buttonEasyS.getTexture()->getSize().x;
	float scaleY = buttonHeight / buttonEasyS.getTexture()->getSize().y;

	buttonEasyS.setScale(scaleX, scaleY);
	buttonHardS.setScale(scaleX, scaleY);

	float centerX = (windowWidth - buttonWidth) / 2.f;
	float spacing = 60.f;
	float centerY = (windowHeight - (2 * buttonHeight + spacing)) / 2.f;

	buttonEasyS.setPosition(centerX, centerY);
	buttonHardS.setPosition(centerX, centerY + buttonHeight + spacing);

	sf::Text msg;
	msg.setFont(font);
	msg.setCharacterSize(50);
	msg.setFillColor(sf::Color::White);
	msg.setString("Choose game mode: ");
	msg.setPosition((windowWidth - msg.getGlobalBounds().width) / 2.f, 100);

	while (window.isOpen()) {
		window.clear();
		window.draw(backgrSprite);

		window.draw(msg);
		window.draw(buttonEasyS);
		window.draw(buttonHardS);

		window.display();
		
		sf::Event event;
		while (window.pollEvent(event)) {
			switch (event.type) {
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::MouseButtonPressed: {
					sf::Vector2i mousePos = sf::Mouse::getPosition(window);
					sf::Vector2f mousePosF(static_cast<float>(mousePos.x), static_cast<float>(mousePos.y));
					if (buttonEasyS.getGlobalBounds().contains(mousePosF)) {
						return GameMode::EASY;
					}
					else if (buttonHardS.getGlobalBounds().contains(mousePosF)) {
						return GameMode::HARD;
					}
					break;
				}
			}
		}
	}
}

void SFMLRenderer::render() {
	window.clear();
	window.draw(backgrSprite);

	const std::vector<std::vector<CellType>>& field_grid = eng->get_field();

	for (int i = 0; i < field_grid.size(); i++) {
		for (int j = 0; j < field_grid[i].size(); j++) {
			CellType cellType = field_grid[i][j];
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

	for (int i = 0;i< eng->get_bot_tanks().size(); i++) {
		sf::Sprite sprite;

		if (eng->bot_is_shot(i)) sprite.setTexture(botShotTexture);
		else sprite.setTexture(botTexture);

		Direction d = eng->get_bot_tanks()[i]->get_direction();
		switch (d) {
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
		sprite.setPosition(eng->get_bot_tanks()[i]->get_col_pos() * cellSize + cellSize / 2.f, eng->get_bot_tanks()[i]->get_row_pos() * cellSize + cellSize / 2.f);

		window.draw(sprite);
	}

	sf::Text userHP;
	userHP.setFont(font);
	userHP.setCharacterSize(25);
	userHP.setFillColor(sf::Color::White);
	userHP.setString("User HP: " + std::to_string(eng->get_user_hp()));
	userHP.setPosition(5, 5);
	window.draw(userHP);

	for (int i = 0; i<eng->get_bot_tanks().size();i++) {
		sf::Text botHP;
		botHP.setFont(font);
		botHP.setCharacterSize(25);
		botHP.setFillColor(sf::Color::White);
		botHP.setString("Bot HP: " + std::to_string(eng->get_bot_hp(i)));
		botHP.setPosition(650, 5 + i*25);
		window.draw(botHP);
	}

	window.display();	
}

sf::Sprite SFMLRenderer::draw_cells(CellType cellType) {
	sf::Sprite sprite;
	switch (cellType) {
	case CellType::WALL:
		sprite.setTexture(wallTexture);
		break;
	case CellType::USERTANK: {
		if (eng->user_is_shot()) sprite.setTexture(userShotTexture);
		else {
			sprite.setTexture(userTexture);
		}
		Direction d = eng->get_user_direction();
		switch (d) {
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
		break;
	}

	default:
		break;
	}
	return sprite;
}