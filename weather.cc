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
	cycle = 80;
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

	cycle = 80 + rand()%60;
	// lkast longer?
}

// return current weather
string WeatherSystem::getWeather() const{
	return weather;
}

//test work
void WeatherSystem::printWeather(){
	cout << "Weather: " << weather << endl;
}

void WeatherSystem::draw(int tempx, int tempy){
	if(weather == "Clear"){
		return;
	}
	

	// goas across whole screen ut over borders

	for(int y = 0; y < tempy; y++){
		for(int x = 0; x < tempx; x++){

				if(weather == "Fog"){
					if(rand()%6 == 0){
						attron(COLOR_PAIR(5));
						mvaddch(y, x, '=');
						attroff(COLOR_PAIR(5));
					}
				}

				// using ncurses, different pproach possible tho but ncurses seems cool
				if(weather == "Rain"){
					if(rand()%5 == 0){
					attron(COLOR_PAIR(3));
					mvaddch(y, x, '.');
					attroff(COLOR_PAIR(3));
					}
				}
				else if(weather == "Windy"){
					if(rand()%7 == 0){
					attron(COLOR_PAIR(5));
					mvaddch(y, x, '/');
					attroff(COLOR_PAIR(5));
								}
				}
							}
						}
					}
				
					
