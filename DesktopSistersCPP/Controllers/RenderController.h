#pragma once
#include "Imager.h"
#include "TimeController.h"
#include "Configuration.h"
#include "EventController.h"
#include <vector>
#include "ImageLoader.h"

using namespace std;

/// <summary>
/// Our current active scene. Used to sandbox the events and times. Each scene is a section based on the Config::UpdateTime
/// </summary>
class Scene
{
public:
	Scene(string newFilename, struct tm * newTime, Configuration * newConfig, EventController * newEventController);
	~Scene();

	/// <summary>
	/// Returns the filename for the scene
	/// </summary>
	/// <returns>Returns the filename for the scene</returns>
	string GetFilename() const;

	/// <summary>
	/// Returns the current TimeController for the scene
	/// </summary>
	/// <returns>Returns the current TimeController for the scene</returns>
	TimeController * GetTimeController() const;

	/// <summary>
	/// Renders the scene based on the events for the time
	/// </summary>
	/// <returns></returns>
	DesktopSisterRgbaBytes * renderScene();
private:
	/// <summary>
	/// The filename of where we should save our scene to / update to
	/// </summary>
	string filename;

	/// <summary>
	/// Our current rendering scene, Can be for the wallpaper or a file save
	/// </summary>
	DesktopSisterRgbaBytes * wallpaper;

	/// <summary>
	/// The Current TimeController for this scene
	/// </summary>
	TimeController * timeController;

	/// <summary>
	/// The current configuration in order to access latitude, longitude, and update time
	/// </summary>
	Configuration * config;

	/// <summary>
	/// The event controller. Used to render events based on time, location, and randomized data
	/// </summary>
	EventController * eventController;

};

/// <summary>
/// Class that handles our rendering of scenes and events. Each scene that is passed in will be added to a queue which will be rendered then saved.
/// </summary>
class RenderController
{
public:
	RenderController(Configuration * newConfig, ImageLoader * newImageController);
	~RenderController();

	/// <summary>
	/// Adds a scene to be rendered based on time and filename. If the filename is "Wallpaper.bmp" it will set the image to the desktop upon render completion.
	/// </summary>
	void AddSceneToQueue(struct tm * newTime, string filename);

	/// <summary>
	/// Pulses the RenderController. Needed to make the magic happen, should be pulsed as often as possible.
	/// </summary>
	void Pulse();

	/// <summary>
	/// Returns the list count of pending scenes to be rendered
	/// </summary>
	/// <returns></returns>
	int GetSceneQueueCount() const;

private:
	/// <summary>
	/// Bool value to return if we are currently in the process of rendering
	/// </summary>
	bool rendering;

	/// <summary>
	/// Our current configuration file to be used for finding the location and update times
	/// </summary>
	Configuration * config;

	/// <summary>
	/// The event controller. Used to render events based on time, location, and randomized data
	/// </summary>
	EventController * eventController;

	/// <summary>
	/// The image loader used to load in the image data / cached data for the events
	/// </summary>
	ImageLoader * imageController;

	/// <summary>
	/// A list of the scenes waiting to be rendered. Should be 0 upon completion. 
	/// </summary>
	vector<Scene*> scenes;
};

