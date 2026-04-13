#include "weather.h"
#include "map.h"
#include "actor.h"
#include <cstdlib>
#include <iostream>

using namespace std;

// Weather System using a timer based approach, rand similar to real games
// In-game cycle rand?
WeatherSystem::WeatherSystem(){
	weather = "Clear";
	cycle = 10;
}

void WeatherSystem::Update(const Map&, const XY& pos){
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
