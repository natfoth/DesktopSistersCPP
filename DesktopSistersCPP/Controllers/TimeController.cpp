#include "stdafx.h"
#include "TimeController.h"
#include "SunPosition.h"

TimeController::TimeController(struct tm * time, MapPoint * coords)
	: dateTime(time),
	  dayRatio(0),
	  nightRatio(0),
	  sunRise(nullptr),
	  sunSet(nullptr),
	  sunX(0),
	  sunY(0),
	  moonX(0), 
	  moonY(0),
	  _latitude(coords->latitude),
	  _longitude(coords->longitude)
{
}

TimeController::~TimeController()
{
	delete dateTime;
	delete sunRise;
	delete sunSet;
}

/// <summary>
/// Updates our controller based on the time parameters passed in on creation of our controller
/// </summary>
void TimeController::Update()
{
	SetSunCycleRatio();
	UpdateSunPosition();
}

/// <summary>
/// Is the time currently in twilight?
/// </summary>
/// <returns></returns>
bool TimeController::IsTwilight()
{
	auto sunPos = GetSunPos();
	return (get<0>(sunPos) > -6 && get<1>(sunPos) < 0);
}

/// <summary>
/// Uses the passed in time as well as the SunRise and SunSet to determine if it is Day time
/// </summary>
/// <returns>returns if it is Night Time</returns>
bool TimeController::IsDayTime()
{
	return difftime(Util::makeTime(dateTime), Util::makeTime(sunRise)) > 0 && difftime(Util::makeTime(dateTime), Util::makeTime(sunSet)) < 0;
}

/// <summary>
/// Simply returns if it is Day time or not
/// </summary>
/// <returns>returns if it is Day Time</returns>
bool TimeController::IsNightTime()
{
	return !IsDayTime();
}

/// <summary>
/// Calculates the sun Positions based on Latitude and Longitude
/// </summary>
/// <returns>The sun position's X and Y position</returns>
tuple<float, float> TimeController::GetSunPos()
{
	return SunPosition::CalculateSunPosition(dateTime, _latitude, _longitude);
}

/// <summary>
/// Updates the internal values for our sun positions
/// </summary>
void TimeController::UpdateSunPosition()
{
	int resW;
	int resH;
	Util::GetDesktopResolution(resW, resH);
	const int minHeight = 300;

	sunX = resW * dayRatio;
	if (dayRatio < 0.5)
		sunY = (resH - (resH * dayRatio)) - minHeight;
	else
		sunY = (resH - (resH * (1.0f - dayRatio))) - minHeight;

	moonX = resW*nightRatio;
	if (nightRatio < 0.5)
		moonY = (resH - (resH* nightRatio)) - minHeight;
	else
		moonY = (resH - (resH * (1.0f - nightRatio))) - minHeight;
}

/// <summary>
/// Calculates the day and night ratio based on latitude and longitude. Calculation intensive function.
/// </summary>
void TimeController::SetSunCycleRatio()
{
	sunX = 0;
	sunY = 0;
	moonX = 0;
	moonY = 0;
	dayRatio = 0;
	nightRatio = 0;

	auto date = dateTime;


	auto zone = Util::GetTimezoneOffset(); // this is based on your location and not the input location!!


	auto jd = Util::calcJD(date);
	float sunRiseDouble = Util::calcSunRiseUTC(jd, _latitude, _longitude);
	float sunSetDouble = Util::calcSunSetUTC(jd, _latitude, _longitude);

	auto sunriseString = Util::getTimeString(sunRiseDouble, zone, false);
	auto sunsetString = Util::getTimeString(sunSetDouble, zone, false);

	if (sunriseString == "error" || sunsetString == "error")
	{
		dayRatio = 0.0;
		nightRatio = 0.0;
		return;
	}

	auto splitSunRiseString = Util::splitStringByCharacter(sunriseString, ':');
	auto sunRiseHour = splitSunRiseString[0];
	auto sunRiseMinute = splitSunRiseString[1];

	delete sunRise;
	sunRise = Util::getDateTimeWithNewHour(dateTime, stoi(sunRiseHour), stoi(sunRiseMinute), false);

	auto splitSunSetString = Util::splitStringByCharacter(sunsetString, ':');
	auto sunSetHour = splitSunSetString[0];
	auto sunSetMinute = splitSunSetString[1];

	delete sunSet;
	sunSet = Util::getDateTimeWithNewHour(dateTime, stoi(sunSetHour), stoi(sunSetMinute), false);
	auto prevDaySunSet = Util::getDateTimeWithNewHour(dateTime, stoi(sunSetHour), stoi(sunSetMinute), false);
	prevDaySunSet->tm_mday -= 1;

	auto anchoredTime = Util::getAnchoredDateTime();
	auto diffStartTime = difftime(Util::makeTime(sunRise), Util::makeTime(anchoredTime));
	auto diffEndTime = difftime(Util::makeTime(sunSet), Util::makeTime(anchoredTime));
	auto diffPrevSunsetEndTime = difftime(Util::makeTime(prevDaySunSet), Util::makeTime(anchoredTime));
	auto diffCurrentTime = difftime(Util::makeTime(dateTime), Util::makeTime(anchoredTime));

	delete anchoredTime;
	anchoredTime = nullptr;

	delete prevDaySunSet;
	prevDaySunSet = nullptr;

	if (IsDayTime())
	{
		dayRatio = (float(diffCurrentTime - diffStartTime) / float(diffEndTime - diffStartTime));
	}

	if(IsNightTime())
	{
		if (dateTime->tm_hour <= 12 && IsNightTime())
		{
			nightRatio = float(diffCurrentTime - diffPrevSunsetEndTime) / float(diffStartTime - diffPrevSunsetEndTime);
		}
		else
			nightRatio = float(diffCurrentTime - diffEndTime) / float(diffEndTime - diffStartTime);
	}

}
