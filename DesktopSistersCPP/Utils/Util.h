#pragma once
/// <summary>
/// Base class that contains many of the basic math functions
/// </summary>

#include <string>
#include "wtypes.h"
#include <vector>
#include <sstream>
#include <ctime>

using namespace std;

namespace Util
{
	inline float pi()
	{
		return float(atan(1) * 4);
	}

	/// <summary>
	/// Converts Radians to Degrees
	/// </summary>
	/// <param name="angleRad">The Radian Angle to convert</param>
	/// <returns>returns the degree for the radian</returns>
	inline float radToDeg(float angleRad)
	{
		return (180.0f * angleRad / pi());
	}

	/// <summary>
	/// Converts Degrees to Radians
	/// </summary>
	/// <param name="angleDeg">A degree</param>
	/// <returns>returns the Radians of the degree</returns>
	inline float degToRad(float angleDeg)
	{
		return pi() * angleDeg / 180.0f;
	}

	/// <summary>
	/// Takes a time and converts it into the julian calendar in order to use it for the celestial calender
	/// </summary>
	/// <param name="year">The current year</param>
	/// <param name="month">the current month</param>
	/// <param name="day">the current day</param>
	/// <returns></returns>
	inline float calcJD(int year, int month, int day)
	{
		if(month <= 2)
		{
			year -= 1;
			month += 12;
		}

		float a = floor(year / 100.0f);
		float b = 2 - a + floor(a / 4);

		return floor(365.25f * (year + 4716)) + floor(30.6001f * (month + 1)) + day + b - 1524.5f;
	}

	/// <summary>
	/// Takes a time and converts it into the julian calendar in order to use it for the celestial calender
	/// </summary>
	/// <param name="fromTime">The current time struct</param>
	/// <returns></returns>
	inline float calcJD(struct tm * fromTime)
	{
		// Don't convert the time, it should already be converted by the time it gets here!!
		return calcJD(fromTime->tm_year, fromTime->tm_mon, fromTime->tm_mday);
	}

	/// <summary>
	/// Converts the julian day to centuries
	/// </summary>
	/// <param name="jd">the current julian day</param>
	/// <returns>returns the number of centuries based on the julian day</returns>
	inline float calcTimeJulianCent(double jd)
	{
		return (float(jd) - 2451545.0f) / 36525.0f;
	}

	/// <summary>
	/// Converts the julian centuries to the julian day
	/// </summary>
	/// <param name="centuries">the current julian century</param>
	/// <returns>returns the julian day based on the current century</returns>
	inline float calcJDFromJulianCent(float centuries)
	{
		return centuries * 36525.0f + 2451545.0f;
	}

	/// <summary>
	/// Calculates the longitude of the sun based on the julian century
	/// </summary>
	/// <param name="t">the current julien century</param>
	/// <returns>returns the longitude of the sun</returns>
	inline double calcGeomMeanLongSun(float t)
	{
		double l = 280.46646 + t * (36000.76983 + 0.0003032 * t);
		while (l > 360.0f)
			l -= 360.0;
		while (l < 0)
			l += 360.0;
		return l;
	}

	/// <summary>
	/// Calculates the gemotric mean anomaly of the sun based on the julian century
	/// </summary>
	/// <param name="t">the current julian century</param>
	/// <returns>returns the geometric mean anomaly of the sun in degrees</returns>
	inline float calcGeomMeanAnomalySun(float t)
	{
		return 357.52911f + t * (35999.05029f - 0.0001537f * t);
	}

	/// <summary>
	/// Calculate the eccentricity of earth's orbit around the sun
	/// </summary>
	/// <param name="t">the current julian century</param>
	/// <returns>returns the eccentricity</returns>
	inline float calcEccentricityEarthOrbit(float t)
	{
		return 0.016708634f - t * (0.000042037f + 0.0000001267f * t);
	}

	/// <summary>
	/// Calculates the center of the sun based on the julian Century
	/// </summary>
	/// <param name="t">the current julian century</param>
	/// <returns>returns the center in degrees</returns>
	inline float calcSunEqOfCenter(float t)
	{
		float m = calcGeomMeanAnomalySun(t);

		float mrad = degToRad(m);
		float sinm = sin(mrad);
		float sin2m = sin(mrad + mrad);
		float sin3m = sin(mrad + mrad + mrad);

		return sinm * (1.914602f - t * (0.004817f + 0.000014f * t)) + sin2m * (0.019993f - 0.000101f * t) + sin3m * 0.000289f;
	}

	/// <summary>
	/// Calculates the true longitude of the sun based on the julian century
	/// </summary>
	/// <param name="t">the current julian century</param>
	/// <returns>returns the suns true longitude in degrees</returns>
	inline float calcSunTrueLong(float t)
	{
		return float(calcGeomMeanLongSun(t) + calcSunEqOfCenter(t));
	}

	/// <summary>
	/// Calculates the true anomaly of the sun based on the current julian century
	/// </summary>
	/// <param name="t">the current julian century</param>
	/// <returns>returns the suns true anomaly in degrees</returns>
	inline float calcSunTrueAnomaly(float t)
	{
		return calcGeomMeanAnomalySun(t) + calcSunEqOfCenter(t);
	}

	/// <summary>
	/// Calculates t he distance to the sun in astronomical units based on the julian century
	/// </summary>
	/// <param name="t">the current julian century</param>
	/// <returns>returns the distance in astronomical units</returns>
	inline float calcSunRadVector(float t)
	{
		float v = calcSunTrueAnomaly(t);
		float e = calcEccentricityEarthOrbit(t);

		return (1.000001018f * (1 - e * e)) / (1 + e * cos(degToRad(v)));
	}

	/// <summary>
	/// Calculates the apparent longitude of the sun based on julian centuries
	/// </summary>
	/// <param name="t">the current julian century</param>
	/// <returns>returns the sun's apparent longitude in degrees</returns>
	inline float calcSunApparentLong(float t)
	{
		float o = calcSunTrueLong(t);

		float omega = 125.04f - 1934.136f * t;
		return o - 0.00569f - 0.00478f * sin(degToRad(omega));
	}

	/// <summary>
	/// Calculates the mean obliquity of the ecliptic of the sun based on the julian century
	/// </summary>
	/// <param name="t">The current julian century</param>
	/// <returns>the mean obliquity in degrees</returns>
	inline float calcMeanObliquityOfEcliptic(float t)
	{
		float seconds = 21.448f - t * (46.8150f + t * (0.00059f - t * (0.001813f)));
		return 23.0f + (26.0f + (seconds / 60.0f)) / 60.0f;
	}

	/// <summary>
	/// Calculates the corrected obliquity of the ecliptic based on the current julian century
	/// </summary>
	/// <param name="t">the current julian century</param>
	/// <returns>returns the corrected obliquity in degrees</returns>
	inline float calcObliquityCorrection(float t)
	{
		float e = calcMeanObliquityOfEcliptic(t);

		float omega = 125.04f - 1934.136f * t;
		return e + 0.00256f * cos(degToRad(omega));
	}

	/// <summary>
	/// Calculates the right ascension of the sun based on the current julian century
	/// </summary>
	/// <param name="t">the current julian century</param>
	/// <returns>returns the sun's right ascension in degrees</returns>
	inline float calcSunRtAscension(float t)
	{
		float e = calcObliquityCorrection(t);
		float lambda = calcSunApparentLong(t);

		float tananum = (cos(degToRad(e)) * sin(degToRad(lambda)));
		float tanadenom = (cos(degToRad(lambda)));
		return radToDeg(atan2(tananum, tanadenom));
	}

	/// <summary>
	/// Calculates the declination of the sun based on the current julian century
	/// </summary>
	/// <param name="t">the current julian century</param>
	/// <returns>returns the sun's declination in degrees</returns>
	inline float calcSunDeclination(float t)
	{
		float e = calcObliquityCorrection(t);
		float lambda = calcSunApparentLong(t);

		float sint = sin(degToRad(e)) * sin(degToRad(lambda));
		return radToDeg(asin(sint));
	}

	/// <summary>
	/// Calculates the difference between true solar time and mean solar time based on the current julian century
	/// </summary>
	/// <param name="t">the current julian century</param>
	/// <returns>returns equation of time in minutes of time</returns>
	inline float calcEquationOfTime(float t)
	{
		float epsilon = calcObliquityCorrection(t);
		float l0 = float(calcGeomMeanLongSun(t));
		float e = calcEccentricityEarthOrbit(t);
		float m = calcGeomMeanAnomalySun(t);

		float y = float(tan(degToRad(epsilon) / 2.0));
		y *= y;

		float sin2l0 = float(sin(2.0 * degToRad(l0)));
		float sinm = float(sin(degToRad(m)));
		float cos2l0 = float(cos(2.0 * degToRad(l0)));
		float sin4l0 = float(sin(4.0 * degToRad(l0)));
		float sin2m = float(sin(2.0 * degToRad(m)));

		float Etime = y * sin2l0 - 2.0f * e * sinm + 4.0f * e * y * sinm * cos2l0
			- 0.5f * y * y * sin4l0 - 1.25f * e * e * sin2m;

		return radToDeg(Etime) * 4.0f; // returns the time based on minutes
	}

	/// <summary>
	/// Calculates the hour angle of the sun at sunrise for the latitude based on the latitude and the solar declination
	/// </summary>
	/// <param name="lat">the current latitude</param>
	/// <param name="solarDec">declination angle of sun in degrees	</param>
	/// <returns>hour angle of sunrise in radians</returns>
	inline float calcHourAngleSunrise(float lat, float solarDec)
	{
		float latRad = degToRad(lat);
		float sdRad = degToRad(solarDec);

		return (acos(cos(degToRad(90.833f)) / (cos(latRad) * cos(sdRad)) - tan(latRad) * tan(sdRad)));
	}

	/// <summary>
	/// calculate the hour angle of the sun at sunset based on the latitude and solar declination
	/// </summary>
	/// <param name="lat">the current lat</param>
	/// <param name="solarDec">declination angle of the sun in degrees</param>
	/// <returns>hour angle of sunset in radians</returns>
	inline float calcHourAngleSunset(float lat, float solarDec)
	{
		float latRad = degToRad(lat);
		float sdRad = degToRad(solarDec);

		return -(acos(cos(degToRad(90.833f)) / (cos(latRad) * cos(sdRad)) - tan(latRad) * tan(sdRad)));
	}

	/// <summary>
	/// calculate the Universal Coordinated Time (UTC) of solar noon for the given day at the given location on earth based on the julian century and the longitude
	/// </summary>
	/// <param name="t">the current julian century</param>
	/// <param name="longitude">the current longitude</param>
	/// <returns>time in minutes from zero Z</returns>
	inline float calcSolNoonUTC(float t, float longitude)
	{
		// First pass uses approximate solar noon to calculate eqtime
		float tnoon = float(calcTimeJulianCent(calcJDFromJulianCent(t) + longitude / 360.0f));
		float eqTime = calcEquationOfTime(tnoon);
		float solNoonUTC = 720.0f + (longitude * 4.0f) - eqTime; // min

		float newt = float(calcTimeJulianCent(calcJDFromJulianCent(t) - 0.5f + solNoonUTC / 1440.0f));

		eqTime = calcEquationOfTime(newt);
		// float solarNoonDec = calcSunDeclination(newt);
		return 720.0f + (longitude * 4.0f) - eqTime; // min
	}

	/// <summary>
	/// Calculates the Universal Coordinated Time (UTC) of sunrise for the given day at the given location on earth using the julian day, the lat, and the longitude
	/// </summary>
	/// <param name="JD">the current julian day</param>
	/// <param name="latitude">the current latitude</param>
	/// <param name="longitude">the current longitude</param>
	/// <returns>time in minutes from zero Z</returns>
	inline float calcSunriseUTC(float JD, float latitude, float longitude)
	{
		float t = calcTimeJulianCent(JD);

		float noonmin = calcSolNoonUTC(t, longitude);
		float tnoon = calcTimeJulianCent(JD + noonmin / 1440.0f);

		float eqTime = calcEquationOfTime(tnoon);
		float solarDec = calcSunDeclination(tnoon);
		float hourAngle = calcHourAngleSunrise(latitude, solarDec);

		float delta = longitude - radToDeg(hourAngle);
		float timeDiff = 4.0f * delta;
		float timeUTC = 720.0f + timeDiff - eqTime;

		float newt = calcTimeJulianCent(calcJDFromJulianCent(t) + timeUTC / 1440.0f);
		eqTime = calcEquationOfTime(newt);
		solarDec = calcSunDeclination(newt);
		hourAngle = calcHourAngleSunrise(latitude, solarDec);
		delta = longitude - radToDeg(hourAngle);
		timeDiff = 4.0f * delta;
		return 720.0f + timeDiff - eqTime;
	}

	/// <summary>
	/// calculate the Universal Coordinated Time (UTC) of sunset for the given day at the given location on earth based on the current julian day and the latitude and longitude
	/// </summary>
	/// <param name="JD">the current julian day</param>
	/// <param name="latitude">the current latitude</param>
	/// <param name="longitude">the current longitude</param>
	/// <returns>time in minutes from zero Z	</returns>
	inline float calcSunSetUTC(float JD, float latitude, float longitude)
	{
		auto t = calcTimeJulianCent(JD);
		auto eqTime = calcEquationOfTime(t);
		auto solarDec = calcSunDeclination(t);
		auto hourAngle = calcHourAngleSunrise(latitude, solarDec);
		hourAngle = -hourAngle;
		auto delta = longitude + radToDeg(hourAngle);
		return 720.0f - (4.0f * delta) - eqTime;	// in minutes
	}

	/// <summary>
	/// calculate the Universal Coordinated Time (UTC) of sunrise for the given day at the given location on earth based on the current julian day and the latitude and longitude
	/// </summary>
	/// <param name="JD">the current julian day</param>
	/// <param name="latitude">the current latitude</param>
	/// <param name="longitude">the current longitude</param>
	/// <returns>time in minutes from zero Z	</returns>
	inline float calcSunRiseUTC(float JD, float latitude, float longitude)
	{
		auto t = calcTimeJulianCent(JD);
		auto eqTime = calcEquationOfTime(t);
		auto solarDec = calcSunDeclination(t);
		auto hourAngle = calcHourAngleSunrise(latitude, solarDec);
		auto delta = longitude + radToDeg(hourAngle);
		auto timeUTC = 720.0f - (4.0f * delta) - eqTime;	// in minutes
		return timeUTC;
	}

	/// <summary>
	/// Creates a string for the time minutes based on the time
	/// </summary>
	/// <param name="minutes"></param>
	/// <returns>returns a string for the time minutes based on the time (04:12)</returns>
	inline string getTimeString(float minutes)
	{
		if ((minutes >= 0) && (minutes < 1440.0f))
		{
			float floatHour = minutes / 60.0f;
			float hour = floor(floatHour);
			float floatMinute = 60.0f * (floatHour - floor(floatHour));
			float minute = floor(floatMinute);
			float floatSec = 60.0f * (floatMinute - floor(floatMinute));
			float second = floor(floatSec + 0.5f);
			if (second > 59)
			{
				second = 0;
				minute += 1;
			}
			if ((second >= 30.0f)) minute++;
			if (minute > 59.0f)
			{
				minute = 0;
				hour += 1;
			}
			return to_string(int(hour)) + ":" + to_string(int(minute));
		}
		else
		{
			return "error";
		}
	}

	/// <summary>
	/// Creates a string for the time minutes based on the time
	/// </summary>
	/// <param name="time">the current time</param>
	/// <param name="timezone">the current timezone</param>
	/// <param name="JD">the current julian day</param>
	/// <param name="dst"></param>
	/// <returns></returns>
	inline string getTimeString(float time, int timezone, bool dst)
	{
		float timeLocal = time + (timezone * 60.0f);
		timeLocal += ((dst) ? 60.0f : 0.0f);
		return getTimeString(timeLocal);
	}

	/// <summary>
	/// Duplicates the time into a new struct using a deep copy
	/// </summary>
	/// <param name="toDuplicate"></param>
	/// <returns></returns>
	inline struct tm * duplicateTime(struct tm * toDuplicate)
	{
		struct tm * copy = new struct tm();
		copy->tm_year = toDuplicate->tm_year;
		copy->tm_mon = toDuplicate->tm_mon;
		copy->tm_mday = toDuplicate->tm_mday;
		copy->tm_hour = toDuplicate->tm_hour;
		copy->tm_min = toDuplicate->tm_min;
		copy->tm_sec = toDuplicate->tm_sec;
		copy->tm_isdst = toDuplicate->tm_isdst;
		copy->tm_wday = toDuplicate->tm_wday;

		return copy;
	}

	/// <summary>
	/// Calculates the date based on a minute
	/// </summary>
	/// <param name="minutes"></param>
	/// <param name="date"></param>
	/// <returns></returns>
	inline struct tm * getDateTime(float minutes, struct tm * date)
	{
		if ((minutes >= 0) && (minutes < 1440.0f))
		{
			auto floatHour = minutes / 60.0f;
			auto hour = floor(floatHour);
			auto floatMinute = 60.0f * (floatHour - floor(floatHour));
			auto minute = floor(floatMinute);
			auto floatSec = 60.0f * (floatMinute - floor(floatMinute));
			auto second = floor(floatSec + 0.5f);
			if (second > 59)
			{
				second = 0;
				minute += 1;
			}
			if ((second >= 30)) minute++;
			if (minute > 59)
			{
				minute = 0;
				hour += 1;
			}

			struct tm newDate = { 0 };
			newDate.tm_year = date->tm_year;
			newDate.tm_mon = date->tm_mon;
			newDate.tm_mday = date->tm_mday;
			newDate.tm_hour = int(hour);
			newDate.tm_min = int(minute);
			newDate.tm_sec = int(second);

			return &newDate;
		}
		else
		{
			return nullptr;
		}
	}

	/// <summary>
	/// Generates a date time struct based on the current time and the timezone
	/// </summary>
	inline struct tm * getDateTime(float time, int timezone, struct tm * date, bool dst)
	{
		auto timeLocal = time + (timezone * 60.0f);
		timeLocal += ((dst) ? 60.0f : 0.0f);
		return getDateTime(timeLocal, date);
	}

	/// <summary>
	/// Returns a new date time for the same passed in time but with a new hour and minute
	/// </summary>
	inline struct tm * getDateTimeWithNewHour(struct tm * currentDateTime, int newHour, int newMinute, bool deleteOriginal)
	{
		struct tm * newDate = new struct tm;
		newDate->tm_year = currentDateTime->tm_year;
		newDate->tm_mon = currentDateTime->tm_mon;
		newDate->tm_mday = currentDateTime->tm_mday;
		newDate->tm_hour = newHour;
		newDate->tm_min = newMinute;
		newDate->tm_sec = currentDateTime->tm_sec;

		if(deleteOriginal)
			delete currentDateTime;

		return newDate;
	}


	/// <summary>
	/// Returns the current Modified Date Time
	/// </summary>
	inline struct tm * getCurrentDateTime()
	{
		time_t t = time(0);   // get time now
		//Local Time is a pointer
		struct tm * now = new struct tm();
		localtime_s(now, &t);

		now->tm_year += 1900;
		now->tm_mon += 1;

		return duplicateTime(now);
	}

	/// <summary>
	/// Returns the time from an anchored point
	/// </summary>
	inline struct tm * getAnchoredDateTime()
	{
		struct tm * then = new struct tm();
		then->tm_year = 2000;
		then->tm_mon = 1;
		then->tm_mday = 1;
		then->tm_hour = 1;
		then->tm_min = 1;
		then->tm_sec = 1;

		return then;
	}
	
	/// <summary>
	/// Converts a struct tm into a time_t
	/// </summary>
	/// <param name="currentDateTime"></param>
	/// <returns></returns>
	inline time_t makeTime(struct tm * currentDateTime)
	{
		struct tm * newDate = new struct tm();
		newDate->tm_year = currentDateTime->tm_year - 1900;
		newDate->tm_mon = currentDateTime->tm_mon - 1;
		newDate->tm_mday = currentDateTime->tm_mday;
		newDate->tm_hour = currentDateTime->tm_hour;
		newDate->tm_min = currentDateTime->tm_min;
		newDate->tm_sec = currentDateTime->tm_sec;

		time_t time = mktime(newDate);

		delete newDate;

		return time;
	}

	/// <summary>
	/// Converts a time_t into a struct tm
	/// </summary>
	inline struct tm * makeTimeStruct(time_t time)
	{
		struct tm *currentDateTime = new struct tm();
		localtime_s(currentDateTime, &time);

		struct tm * newDate = new struct tm();
		newDate->tm_year = currentDateTime->tm_year + 1900;
		newDate->tm_mon = currentDateTime->tm_mon + 1;
		newDate->tm_mday = currentDateTime->tm_mday;
		newDate->tm_hour = currentDateTime->tm_hour;
		newDate->tm_min = currentDateTime->tm_min;
		newDate->tm_sec = currentDateTime->tm_sec;

		return newDate;
	}

	/// <summary>
	/// Adds the time to a struct and rolls over the amount
	/// </summary>
	inline void addTimeToStruct(struct tm* structToAddTo, int hours, int mins, int seconds)
	{
		structToAddTo->tm_hour += hours;
		structToAddTo->tm_min += mins;
		structToAddTo->tm_sec += seconds;

		//Rollover the minutes into hours
		while (structToAddTo->tm_min >= 60)
		{
			structToAddTo->tm_hour += 1;
			structToAddTo->tm_min -= 60;
		}

		//Rollover the hours into days
		while (structToAddTo->tm_hour >= 24)
		{
			structToAddTo->tm_mday += 1;
			structToAddTo->tm_hour -= 24;
		}
	}

	/// <summary>
	/// Gets the timezone offset
	/// </summary>
	/// <returns></returns>
	inline int GetTimezoneOffset()
	{
		LPTIME_ZONE_INFORMATION timeInfo = new _TIME_ZONE_INFORMATION();
		GetTimeZoneInformation(timeInfo);

		auto bios = timeInfo->Bias + timeInfo->DaylightBias;
		bios /= 60; // Convert to hours
		bios *= -1; // Flip the zones as its the difference

		delete timeInfo;
		timeInfo = nullptr;

		return bios;
	}
	
	/// <summary>
	/// Gets the desktop resolution
	/// </summary>
	/// <param name="horizontal"></param>
	/// <param name="vertical"></param>
	inline void GetDesktopResolution(int& horizontal, int& vertical)
	{
		RECT desktop;
		const HWND hDesktop = GetDesktopWindow();

		GetWindowRect(hDesktop, &desktop);

		horizontal = desktop.right;
		vertical = desktop.bottom;
	}

	inline vector<string> splitStringByCharacter(string inputString, char character)
	{
		stringstream stream(inputString);
		std::vector<string> seglist;
		string segment = "";

		while (std::getline(stream, segment, character))
		{
			seglist.push_back(segment);
		}

		return seglist;
	}

	inline float lerp(float a, float b, float f)
	{
		return a + f * (b - a);
	}

	inline int lerp_int(int a, int b, float f)
	{
		//float diff = (float)(b-a);
		//float frac = f*diff;
		//return a + (int)frac;
		return a + int(f * float(b - a));
	}


}

