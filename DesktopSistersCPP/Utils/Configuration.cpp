/// <summary>
/// Contains the basic information that is saved between sessions.
/// </summary>

#include "stdafx.h"
#include "Configuration.h"
#define _X86_
#include <windef.h>
#include <winuser.h>

#include <iostream>
#include <fstream>


Configuration::Configuration()
	: coords(new MapPoint(0, 0)),
	screenWidth(0),
	screenHeight(0),
	updateTime(5),
	addressString(""),
	filename("Config.conf")
{
	GetDesktopResolution();
}


Configuration::~Configuration()
{
}

/// <summary>
/// Sets the configuration filename on disk
/// </summary>
/// <param name="newFilename"></param>
void Configuration::SetConfigurationFilename(string newFilename)
{
	filename = newFilename;
	SaveConfiguration();
}

/// <summary>
/// Returns the current configurations filename on disk
/// </summary>
/// <returns>Returns the current configurations filename on disk</returns>
string Configuration::GetConfigurationFilename() const
{
	return filename;
}

/// <summary>
/// Sets the users current Latitude and Longitude
/// </summary>
/// <param name="newCoords">The new set of coords in Latitude and Longitude</param>
void Configuration::SetCoords(MapPoint * newCoords)
{
	delete coords;
	coords = newCoords;
	SaveConfiguration();
}

/// <summary>
/// Returns the users saved Latitude and Longitude
/// </summary>
/// <returns>Returns the users saved Latitude and Longitude</returns>
MapPoint* Configuration::GetCoords() const
{
	return coords;
}

/// <summary>
/// Returns the current Screen Width
/// </summary>
/// <returns>Returns the current Screen Width</returns>
int Configuration::GetScreenWidth() const
{
	return screenWidth;
}

/// <summary>
/// Returns the current Screen Height
/// </summary>
/// <returns>Returns the current Screen Height</returns>
int Configuration::GetScreenHeight() const
{
	return screenHeight;
}

/// <summary>
/// Sets how long it is between each refresh of the desktop in minutes
/// </summary>
/// <param name="minutes">How long between each refresh of the desktop in minutes</param>
void Configuration::SetUpdateTime(int minutes)
{
	updateTime = minutes;
	SaveConfiguration();
}

/// <summary>
/// Returns the update time in minutes between each desktop refresh
/// </summary>
/// <returns>Returns the update time in minutes between each desktop refresh</returns>
int Configuration::GetUpdateTime() const
{
	return updateTime;
}

/// <summary>
/// Sets the current Users Address
/// </summary>
/// <param name="newAddress"></param>
void Configuration::SetAddress(string newAddress)
{
	addressString = newAddress;
	SaveConfiguration();
}

/// <summary>
/// Gets the current Users Address
/// </summary>
/// <returns></returns>
string Configuration::GetAddress() const
{
	return addressString;
}

/// <summary>
/// Loads the configuration with the saved filename
/// </summary>
void Configuration::LoadConfiguration()
{
	LoadConfiguration(filename);
}

/// <summary>
/// Loads the configuration from a specified filename
/// </summary>
/// <param name="withFilename"></param>
void Configuration::LoadConfiguration(string withFilename)
{
	ifstream fin(withFilename.c_str());

	if (fin.is_open())
	{
		fin >> noskipws;
		fin.unsetf(ios_base::skipws);

		string loadedString = "";

		string line;
		vector<string> settings;
		while (!fin.eof())
		{
			getline(fin, line);
			settings.push_back(line);
		}

		fin.close();

		double lat = stod(settings[0]);
		double longi = stod(settings[1]);

		MapPoint * newPoint = new MapPoint(float(lat), float(longi));
		delete coords;
		coords = newPoint;

		updateTime = stoi(settings[2]);

		addressString = settings[3];
	}
}

/// <summary>
/// Saves the saved configuration to a file
/// </summary>
void Configuration::SaveConfiguration() const
{
	SaveConfiguration(filename);
}

/// <summary>
/// Saves the current configuration to a file
/// </summary>
/// <param name="withFilename"></param>
void Configuration::SaveConfiguration(string withFilename) const
{
	ofstream out(withFilename.c_str());
	out << coords->latitude << "\n" << coords->longitude << "\n" << updateTime << "\n" << addressString;

	out.close();
}

/// <summary>
/// Updates the public variables for the screen width and height
/// </summary>
void Configuration::GetDesktopResolution()
{
	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);
	screenWidth = desktop.right;
	screenHeight = desktop.bottom;
}
