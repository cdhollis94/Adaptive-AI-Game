#include "adaptive.h"

#include <iostream>
#include <string>


int main() {
	//SETUP/////////////////////////////////////////////////////////////////////////

	sf::VideoMode videoMode;
	sf::RenderWindow* window;
	sf::RenderWindow** windowPoint = &window;
	initWindow(windowPoint, videoMode);
	sf::Event ev = sf::Event();
	sf::Clock gameClock;

	GameData GameData;
	Players Players;
	gameClock.restart();
	GameData.deltaTime = gameClock.restart().asSeconds();

	//SPRITE SETUP//////
	Textures Textures;
	if (!Textures.playerTexture.loadFromFile("Boxer Spritesheet.png")) {
		std::cout << "Texture load error";
	}
	initPlayers(window, Players, Textures);

	//GAME LOOP////////
	while (isRunning(window)) {
		update(window, ev, Players, GameData, gameClock);
		render(window, Players, GameData);
		GameData.deltaTime = gameClock.restart().asSeconds();
	}
}