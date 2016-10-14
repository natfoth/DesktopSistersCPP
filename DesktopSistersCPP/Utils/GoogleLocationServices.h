#pragma once

using namespace std;

/// <summary>
/// A basic class to hold the latitude and longitude for cleaner access
/// </summary>
class MapPoint
{
public:
	MapPoint(float lat, float lon) { latitude = lat; longitude = lon; }

	float latitude;
	float longitude;
};

/// <summary>
/// Class to handle the downloading of the Latitude and Longitude based on the users address
/// </summary>
class GoogleLocationServices
{
public:
	GoogleLocationServices(bool shouldUseHttps);
	~GoogleLocationServices();

	/// <summary>
	/// Const string for the region from latitude and longitude
	/// </summary>
	const string API_REGION_FROM_LATLONG = "maps.googleapis.com/maps/api/geocode/xml?latlng={0},{1}&sensor=false";

	/// <summary>
	/// const string for the latitude and longitude from an address
	/// </summary>
	const string API_LATLONG_FROM_ADDRESS = "maps.googleapis.com/maps/api/geocode/xml?address=";

	/// <summary>
	/// If we should use https instead of http
	/// </summary>
	bool useHttps;

	string UrlProtocolPrefix();
	string APIUrlRegionFromLatLong();
	string APIUrlLatLongFromAddress(string address);

	MapPoint * GetLatLongFromAddress(string address);
};

