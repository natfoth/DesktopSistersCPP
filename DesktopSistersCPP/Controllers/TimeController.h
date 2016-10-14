#pragma once
/// <summary>
/// Support class to manage and control the time
/// </summary>

#include <tuple>
#include "GoogleLocationServices.h"


using namespace std;

class TimeController
{
public:
	TimeController(struct tm * time, MapPoint * coords);
	~TimeController();

	/// <summary>
	/// Our current time
	/// </summary>
	struct tm * dateTime;

	/// <summary>
	/// The current ratio into the day we are in between 0 - 1
	/// </summary>
	float dayRatio;

	/// <summary>
	/// The current ratio into the night we are in between 0 - 1
	/// </summary>
	float nightRatio;

	/// <summary>
	/// The time that sunrise will occur next
	/// </summary>
	struct tm * sunRise;

	/// <summary>
	/// The time that sunset will occur next
	/// </summary>
	struct tm * sunSet;

	/// <summary>
	/// The location of the sun on the X (Horizontal) axis
	/// </summary>
	float sunX;

	/// <summary>
	/// The location of the sun on the Y (Vertical) axis
	/// </summary>
	float sunY;

	/// <summary>
	/// The location of the moon on the X (Horizontal) axis
	/// </summary>
	float moonX;

	/// <summary>
	/// The location of the moon on the Y (Vertical) axis
	/// </summary>
	float moonY;


	//-----------//
	// Functions //
	//-----------//

	/// <summary>
	/// Updates our controller based on the time parameters passed in on creation of our controller
	/// </summary>
	void Update();

	/// <summary>
	/// Is the time currently in twilight?
	/// </summary>
	/// <returns></returns>
	bool IsTwilight() const;

	/// <summary>
	/// Uses the passed in time as well as the SunRise and SunSet to determine if it is night time
	/// </summary>
	/// <returns>returns if it is Night Time</returns>
	bool IsNightTime() const;

	/// <summary>
	/// Simply returns if it is night time or not
	/// </summary>
	/// <returns>returns if it is Day Time</returns>
	bool IsDayTime() const;

	/// <summary>
	/// Calculates the sun Positions based on Latitude and Longitude
	/// </summary>
	/// <returns>The sun position's X and Y position</returns>
	tuple<float, float> GetSunPos() const;


private:
	/// <summary>
  /// Our current Latitude
  /// </summary>
	float _latitude;

	/// <summary>
	/// Our current Longitude
	/// </summary>
	float _longitude;

	/// <summary>
	/// Updates the internal values for our sun positions
	/// </summary>
	void UpdateSunPosition();

	/// <summary>
	/// Calculates the day and night ratio based on latitude and longitude. Calculation intensive function.
	/// </summary>
	void SetSunCycleRatio();

};

