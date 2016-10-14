/// <summary>
/// Contains the basic information that is saved between sessions.
/// </summary>

#pragma once
#include "GoogleLocationServices.h"


class Configuration
{
public:
	Configuration();
	~Configuration();

	/// <summary>
	/// Sets the configuration filename on disk
	/// </summary>
	/// <param name="newFilename"></param>
	void SetConfigurationFilename(string newFilename);

	/// <summary>
	/// Returns the current configurations filename on disk
	/// </summary>
	/// <returns>Returns the current configurations filename on disk</returns>
	string GetConfigurationFilename() const;

	/// <summary>
	/// Sets the users current Latitude and Longitude
	/// </summary>
	/// <param name="newCoords">The new set of coords in Latitude and Longitude</param>
	void SetCoords(MapPoint * newCoords);

	/// <summary>
	/// Returns the users saved Latitude and Longitude
	/// </summary>
	/// <returns>Returns the users saved Latitude and Longitude</returns>
	MapPoint * GetCoords() const;

	/// <summary>
	/// Returns the current Screen Width
	/// </summary>
	/// <returns>Returns the current Screen Width</returns>
	int GetScreenWidth() const;

	/// <summary>
	/// Returns the current Screen Height
	/// </summary>
	/// <returns>Returns the current Screen Height</returns>
	int GetScreenHeight() const;

	/// <summary>
	/// Sets how long it is between each refresh of the desktop in minutes
	/// </summary>
	/// <param name="minutes">How long between each refresh of the desktop in minutes</param>
	void SetUpdateTime(int minutes);

	/// <summary>
	/// Returns the update time in minutes between each desktop refresh
	/// </summary>
	/// <returns>Returns the update time in minutes between each desktop refresh</returns>
	int GetUpdateTime() const;

	/// <summary>
	/// Sets the current Users Address
	/// </summary>
	/// <param name="newAddress"></param>
	void SetAddress(string newAddress);

	/// <summary>
	/// Gets the current Users Address
	/// </summary>
	/// <returns></returns>
	string GetAddress() const;

	/// <summary>
	/// Loads the configuration with the saved filename
	/// </summary>
	void LoadConfiguration();

	/// <summary>
	/// Loads the configuration from a specified filename
	/// </summary>
	/// <param name="withFilename"></param>
	void LoadConfiguration(string withFilename);

	/// <summary>
	/// Saves the saved configuration to a file
	/// </summary>
	void SaveConfiguration() const;

	/// <summary>
	/// Saves the current configuration to a file
	/// </summary>
	/// <param name="withFilename"></param>
	void SaveConfiguration(string withFilename) const;

private:
	/// <summary>
	/// The Latitude and Longitude of the saved location
	/// </summary>
	MapPoint* coords;

	/// <summary>
	/// The users Screen Width
	/// </summary>
	int screenWidth;

	/// <summary>
	/// The Users Screen Height
	/// </summary>
	int screenHeight;

	/// <summary>
	/// How Often to refresh the events
	/// </summary>
	int updateTime; // in minutes

	/// <summary>
	/// The stored users current address
	/// </summary>
	string addressString;

	/// <summary>
	/// The Configuration Filename on disk
	/// </summary>
	string filename;

	/// <summary>
	/// Updates the public variables for the screen width and height
	/// </summary>
	void GetDesktopResolution();
};

