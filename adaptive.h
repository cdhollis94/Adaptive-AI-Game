#pragma once

#include <iostream>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

const int GameHeight = 600;
const int GameWidth = 800;
const int PlayerHeight = 150;
const int PlayerWidth = 45;

//STRUCTS

struct GameData {
	float deltaTime = 0;
	sf::Time elapsed;
	float reactionTime = 2;
	sf::Color backgroundColor = sf::Color::Black;
	bool isPlayerPunching = false;
	bool isPlayerPunchReleased = true;
	bool canPlayerPunchAgain = true;
	bool isPlayerBlocking = false;
	bool isPlayerBlockingPunch = false;
	bool isComPunching = false;
	bool isComBlocking = false;
	bool isComBlockingPunch = false;
	bool isPlayerHit = false;
	bool isComHit = false;
};

struct Textures {
	sf::Texture playerTexture;
};

struct Player {
	sf::Sprite sprite;
	sf::CircleShape star;
	sf::Clock boxerClock;
};

struct Players {
	Player Player1;
	Player Player2;
};

//BASIC FUNCTIONS

void initWindow(sf::RenderWindow** windowPoint, sf::VideoMode videoMode);
bool isRunning(sf::RenderWindow* window);
void initPlayers(sf::RenderWindow* window, Players& Players, Textures& Textures);
void update(sf::RenderWindow* window, sf::Event ev, Players& Players, GameData& GameData, sf::Clock gameClock);
void render(sf::RenderWindow* window, Players& Players, GameData& GameData);

//GAME FUNCTIONS

void comPunch(Players& Players, GameData& GameData);
void comHit(Players& Players, GameData& GameData);
void playerPunch(Players& Players, GameData& GameData);
void playerHit(Players& Players, GameData& GameData);