#include "adaptive.h"
#include <iostream>
#include <random>

//BASIC FUNCTIONS//////////////////////////////////////////////////////////////////

void initWindow(sf::RenderWindow** window, sf::VideoMode videoMode) {
	videoMode.height = GameHeight;
	videoMode.width = GameWidth;
	*window = new sf::RenderWindow(videoMode, "Adaptive Fight", sf::Style::Titlebar | sf::Style::Close);
	(*window)->setFramerateLimit(60);
}

bool isRunning(sf::RenderWindow* window) {
	return window->isOpen();
}

void initPlayers(sf::RenderWindow* window, Players& Players, Textures& Textures) {
	Players.Player1.sprite.setTexture(Textures.playerTexture);
	Players.Player1.sprite.setScale(5.f, 5.f);
	Players.Player1.sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
	Players.Player1.sprite.setPosition(window->getSize().x / 5 * 1, window->getSize().y / 5 * 2);

	Players.Player2.sprite.setTexture(Textures.playerTexture);
	Players.Player2.sprite.setScale(-5.f, 5.f);
	Players.Player2.sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
	Players.Player2.sprite.setPosition(window->getSize().x / 5 * 4, window->getSize().y / 5 * 2);
}

void update(sf::RenderWindow* window, sf::Event ev, Players& Players, GameData& GameData, sf::Clock gameClock) {
	if (GameData.isPlayerBlockingPunch) {
		Players.Player1.sprite.setTextureRect(sf::IntRect(64, 0, 64, 64));
		GameData.canPlayerPunchAgain = false;
	}
	if (GameData.isPlayerBlocking && !GameData.isPlayerHit) {
		Players.Player1.sprite.setTextureRect(sf::IntRect(64, 0, 64, 64));
	}
	else if (!GameData.isPlayerPunching && !GameData.isPlayerBlocking && !GameData.isPlayerBlockingPunch) {
		Players.Player1.sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
	}


	if (GameData.isComBlocking) {
		Players.Player2.sprite.setTextureRect(sf::IntRect(64, 0, 64, 64));
	}
	else if (!GameData.isComPunching && !GameData.isComBlocking) {
		Players.Player2.sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
	}
	if (!GameData.isPlayerPunching && GameData.isPlayerPunchReleased) {
		GameData.canPlayerPunchAgain = true;
	}


	while (window->pollEvent(ev)) {
		switch (ev.type) {
		case sf::Event::Closed:
			window->close();
			break;
		case sf::Event::KeyPressed:
			if (ev.key.code == sf::Keyboard::Space) {
				GameData.isPlayerPunchReleased = false;
			}
			if (ev.key.code == sf::Keyboard::Space && !GameData.isPlayerPunching && !GameData.isPlayerHit && GameData.canPlayerPunchAgain) {
				GameData.isPlayerPunching = true;
				GameData.canPlayerPunchAgain = false;
				Players.Player1.boxerClock.restart();
			}
			if (ev.key.code == sf::Keyboard::M) {
				GameData.isPlayerBlocking = true;
			}
			break;
		case sf::Event::KeyReleased:
			if (ev.key.code == sf::Keyboard::Space) {
				GameData.isPlayerPunchReleased = true;
			}
			if (ev.key.code == sf::Keyboard::M) {
				GameData.isPlayerBlocking = false;
			}
			break;
		}
	}
	if (std::rand() % 100 == 0 && !GameData.isComPunching && !GameData.isComHit) {
		GameData.isComPunching = true;
		Players.Player2.boxerClock.restart();
	}

	if (GameData.isPlayerHit) {
		playerHit(Players, GameData);
	}
	if (GameData.isComHit) {
		comHit(Players, GameData);
	}
	if (GameData.isPlayerPunching) {
		playerPunch(Players, GameData);
	}
	if (GameData.isComPunching) {
		comPunch(Players, GameData);
	}
}

void render(sf::RenderWindow* window, Players& Players, GameData& GameData) {
	window->clear(GameData.backgroundColor);
	if (GameData.isPlayerHit) {
		Players.Player1.sprite.setTextureRect(sf::IntRect(128, 0, 64, 64));
	}
	if (GameData.isComHit) {
		Players.Player2.sprite.setTextureRect(sf::IntRect(128, 0, 64, 64));
	}
	window->draw(Players.Player1.sprite);
	window->draw(Players.Player2.sprite);
	window->display();
}

void comPunch(Players& Players, GameData& GameData) {
	if (Players.Player2.boxerClock.getElapsedTime().asSeconds() < 0.5) {
		Players.Player2.sprite.setTextureRect(sf::IntRect(192, 0, 64, 64));
	}
	else if (Players.Player2.boxerClock.getElapsedTime().asSeconds() < 1) {
		Players.Player2.sprite.setTextureRect(sf::IntRect(256, 0, 64, 64));
		if (!GameData.isPlayerBlocking and !GameData.isPlayerBlockingPunch) {
			GameData.isPlayerPunching = false;
			GameData.isPlayerHit = true;
			Players.Player1.boxerClock.restart();
		}
		else {
			GameData.isPlayerBlockingPunch = true;
		}
	}
	else if (Players.Player2.boxerClock.getElapsedTime().asSeconds() >= 1) {
		Players.Player2.sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
		GameData.isComPunching = false;
		if (GameData.isPlayerBlockingPunch) {
			GameData.isPlayerBlockingPunch = false;
		}
		if (!GameData.isPlayerBlocking) {
			Players.Player1.sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
		}
	}
}

void comHit(Players& Players, GameData& GameData) {
	if (Players.Player2.boxerClock.getElapsedTime().asSeconds() >= 0.5) {
		GameData.isComHit = false;
		Players.Player2.sprite.setTextureRect(sf::IntRect(128, 0, 64, 64));
	}
}

void playerPunch(Players& Players, GameData& GameData) {
	if (Players.Player1.boxerClock.getElapsedTime().asSeconds() < 0.2) {
		Players.Player1.sprite.setTextureRect(sf::IntRect(192, 0, 64, 64));
	}
	else if (Players.Player1.boxerClock.getElapsedTime().asSeconds() < 0.5) {
		Players.Player1.sprite.setTextureRect(sf::IntRect(256, 0, 64, 64));
		if (!GameData.isComBlocking) {
			GameData.isComPunching = false;
			GameData.isComHit = true;
			Players.Player2.boxerClock.restart();
		}
	}
	else if (Players.Player1.boxerClock.getElapsedTime().asSeconds() >= 0.5) {
		Players.Player1.sprite.setTextureRect(sf::IntRect(0, 0, 64, 64));
		GameData.isPlayerPunching = false;
	}
}

void playerHit(Players& Players, GameData& GameData) {
	if (Players.Player1.boxerClock.getElapsedTime().asSeconds() >= 0.5) {
		GameData.isPlayerHit = false;
		Players.Player1.sprite.setTextureRect(sf::IntRect(128, 0, 64, 64));
	}
}