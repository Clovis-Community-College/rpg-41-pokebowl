#ifndef WEATHER_H
#define WEATHER_H

#include <string>
using namespace std;

//declare others
class Map;
struct XY;

class WeatherSystem{
	private:
		string weather;
		int cycle;

	public: 
		WeatherSystem();

		void Update(const Map& world, const XY& pos);

		string getWeather() const;
			
		void printWeather();


};

#endif
