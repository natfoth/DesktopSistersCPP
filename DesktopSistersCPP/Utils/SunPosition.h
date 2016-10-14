#pragma once

/// <summary>
/// Inline Functions for calculating the position of the sun in the sky
/// </summary>
#include <tuple>

using namespace std;

namespace SunPosition
{
	float deg2Rad = Util::pi() / 180.0f;
	float rad2Deg = 180.0f / Util::pi();

	/// <summary>
	/// Corrects the angle between Radians and Degrees
	/// </summary>
	/// <param name="angleInRadians"></param>
	/// <returns></returns>
	inline double CorrectAngle(double angleInRadians) {
		if (angleInRadians < 0) {
			return 2 * Util::pi() - fmod(abs(angleInRadians), (2 * Util::pi()));
		}
		else if (angleInRadians > 2 * Util::pi()) {
			return fmod(angleInRadians, (2 * Util::pi()));
		}
		else {
			return angleInRadians;
		}
	}

	/// <summary>
	/// Heavily intensive function to calculate the position of the sun within the sky and returns it matching to the screen in an X Y format
	/// </summary>
	/// <param name="dateTime">The date we wish to find the coordiantes for</param>
	/// <param name="latitude">The latitude of the observer</param>
	/// <param name="longitude">The longitude of the observer</param>
	/// <returns></returns>
	inline tuple<float, float> CalculateSunPosition(struct tm * dateTime, float latitude, float longitude)
	{
		auto currentYear = (dateTime->tm_year + 1900);
		// Number of days from J2000.0.  
		float julianDate = 367 * currentYear -
			static_cast<int>((7.0f / 4.0f) * (currentYear +
				static_cast<int>(((dateTime->tm_mon + 1.0f) + 9.0f) / 12.0f))) +
				static_cast<int>((275.0f * (dateTime->tm_mon + 1)) / 9.0f) +
			dateTime->tm_wday - 730531.5f;

		float julianCenturies = julianDate / 36525.0f;

		float siderealTimeHours = 6.6974f + 2400.0513f * julianCenturies;

		float siderealTimeUT = siderealTimeHours +
			(366.2422f / 365.2422f) * static_cast<float>(dateTime->tm_hour);

		double siderealTime = siderealTimeUT * 15 + longitude;

		// Refine to number of days (fractional) to specific time.  
		julianDate += static_cast<float>(dateTime->tm_hour) / 24.0f;
		julianCenturies = julianDate / 36525.0f;

		// Solar Coordinates  
		double meanLongitude = CorrectAngle(deg2Rad *
			(280.466f + 36000.77f * julianCenturies));

		double meanAnomaly = CorrectAngle(deg2Rad *
			(357.529f + 35999.05f * julianCenturies));

		double equationOfCenter = deg2Rad * ((1.915f - 0.005f * julianCenturies) *
			sin(meanAnomaly) + 0.02f * sin(2 * meanAnomaly));

		double elipticalLongitude =
			CorrectAngle(meanLongitude + equationOfCenter);

		double obliquity = (23.439f - 0.013f * julianCenturies) * deg2Rad;

		// Right Ascension  
		double rightAscension = atan2(
			cos(obliquity) * sin(elipticalLongitude),
			cos(elipticalLongitude));

		double declination = asin(
			sin(rightAscension) * sin(obliquity));

		// Horizontal Coordinates  
		double hourAngle = CorrectAngle(siderealTime * deg2Rad) - rightAscension;

		if (hourAngle > Util::pi()) {
			hourAngle -= 2 * Util::pi();
		}

		double altitude = asin(sin(latitude * deg2Rad) *
			sin(declination) + cos(latitude * deg2Rad) *
			cos(declination) * cos(hourAngle));

		// Nominator and denominator for calculating Azimuth  
		// angle. Needed to test which quadrant the angle is in.  
		double aziNom = -sin(hourAngle);
		double aziDenom =
			tan(declination) * cos(latitude * deg2Rad) -
			sin(latitude * deg2Rad) * cos(hourAngle);

		double azimuth = atan(aziNom / aziDenom);

		if (aziDenom < 0) // In 2nd or 3rd quadrant  
		{
			azimuth += Util::pi();
		}
		else if (aziNom < 0) // In 4th quadrant  
		{
			azimuth += 2 * Util::pi();
		}

		return make_tuple(float(altitude * rad2Deg), float(azimuth * rad2Deg));
	}
}

