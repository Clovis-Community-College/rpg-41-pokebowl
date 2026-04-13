#include "weather.h"
#include "map.h"
#include "actor.h"
#include <cstdlib>
#include <iostream>
#include <ncurses.h>

using namespace std;

// Weather System using a timer based approach, rand similar to real games
// In-game cycle rand?
WeatherSystem::WeatherSystem(){
	weather = "Clear";
	cycle = 10;
}

void WeatherSystem::Update(const Map& world, const XY& pos){
	cycle--;
	// rand this

	if(cycle > 0){
		return;
	}
	
	int r = rand() %4;
	//testing approach, wip for a better method for weather

	if(r==0){
		weather = "Clear";
	}
	else if(r == 1){
		weather = "Rain";
	}
	else if(r == 2){
		weather = "Fog";
	}
	else if(r == 3){
		weather ="Windy";
	}

	cycle =10;
}

// return current weather
string WeatherSystem::getWeather() const{
	return weather;
}

//test work
void WeatherSystem::printWeather(){
	cout << "Weather: " << weather << endl;
}

void WeatherSystem::draw(int x, int y, int tempx, int tempy){
	if(weather == "Clear"){
		return;
	}
	// made this a 4x4 grid so it doesnt mess w grass/rain, will remove grid once i get a better system and/or sprites


	// also need to fix randomizer

	for(int i = -2; i <= 1; i++){
		for(int j = -2; j <= 1; j++){
			int shiftx = x + j;
			int shifty = y + i;

			if(shiftx >= 0 && shiftx < tempx && shifty >= 0 && tempy){
				if(shiftx == x && shifty == y){
					continue;
				}


				// using ncurses, different pproach possible tho but ncurses seems cool
				if(weather == "Rain"){
					attron(COLOR_PAIR(3));
					mvaddch(shifty, shiftx, '.');
					attroff(COLOR_PAIR(3));
				}
				else if(weather == "Windy"){
					attron(COLOR_PAIR(5));
					mvaddch(shifty, shiftx, '/');
					attroff(COLOR_PAIR(5));
								}
							}
						}
					}
					}
					
