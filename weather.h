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
		
		void draw(int x, int y, int tempx, int tempy); // 4x4 grid till we figure out how to do weather better

};

#endif
