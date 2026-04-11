#ifndef WEATHER_H
#define WEATHER_H

#include <string>
using namespace std;

class WeatherSystem{
	private:
		string weather;

	public: 
		WeatherSystem();

		void setWeather(string w);
		string getWeather() const;

};

#endif
