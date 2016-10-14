#include "stdafx.h"
#include "RenderController.h"
#include <chrono>

/// <summary>
/// Our current active scene. Used to sandbox the events and times. Each scene is a section based on the Config::UpdateTime
/// </summary>
Scene::Scene(string newFilename, struct tm * newTime, Configuration * newConfig, EventController * newEventController)
	: filename(newFilename),
	wallpaper(nullptr),
	config(newConfig),
	eventController(newEventController)
{
	timeController = new TimeController(newTime, config->GetCoords());
	timeController->Update();	
}


Scene::~Scene()
{
	delete timeController;
	timeController = nullptr;
}

/// <summary>
/// Returns the filename for the scene
/// </summary>
/// <returns>Returns the filename for the scene</returns>
string Scene::GetFilename() const
{
	return filename;
}

/// <summary>
/// Returns the current TimeController for the scene
/// </summary>
/// <returns>Returns the current TimeController for the scene</returns>
TimeController* Scene::GetTimeController() const
{
	return timeController;
}

/// <summary>
/// Renders the scene based on the events for the time
/// </summary>
/// <returns></returns>
DesktopSisterRgbaBytes* Scene::renderScene()
{
	//don't allocate anything until we render it
	wallpaper = new DesktopSisterRgbaBytes(config->GetScreenWidth(), config->GetScreenHeight());

	eventController->RenderEvents(wallpaper, timeController);

	return wallpaper;
}

/// <summary>
/// Class that handles our rendering of scenes and events. Each scene that is passed in will be added to a queue which will be rendered then saved.
/// </summary>
RenderController::RenderController(Configuration * newConfig, ImageLoader * newImageController)
	: rendering(false),
	  config(newConfig),
	  eventController(new EventController(newConfig, newImageController)),
	  imageController(newImageController)
{
}


RenderController::~RenderController()
{
}

/// <summary>
/// Adds a scene to be rendered based on time and filename. If the filename is "Wallpaper.bmp" it will set the image to the desktop upon render completion.
/// </summary>
void RenderController::AddSceneToQueue(struct tm* newTime, string filename)
{
	auto newScene = new Scene(filename, newTime, config, eventController);
	scenes.push_back(newScene);
}

/// <summary>
/// Pulses the RenderController. Needed to make the magic happen, should be pulsed as often as possible.
/// </summary>
void RenderController::Pulse()
{
	if (scenes.size() == 0 || rendering)
		return;

	typedef std::chrono::high_resolution_clock Clock;
	auto t1 = Clock::now();

	rendering = true;
	auto sceneToRender = scenes[0];
	scenes.erase(scenes.begin());

	auto renderedScene = sceneToRender->renderScene();

	//Timer
	auto t2 = Clock::now();
	auto duration = (t2 - t1);
	auto countDur = duration.count();

	cout << "Finished rendering time : " << to_string(sceneToRender->GetTimeController()->dateTime->tm_hour) << ":" << to_string(sceneToRender->GetTimeController()->dateTime->tm_min) << " - Rendered in : " << to_string(countDur / 1000000.0) << " milliseconds\n";


	imageController->saveImage(renderedScene, sceneToRender->GetFilename());

	if(sceneToRender->GetFilename() == "Wallpaper.bmp")
	{
		char path[255];
		_fullpath(path, sceneToRender->GetFilename().c_str(), 255);

		wchar_t wpath[255];

		size_t outSize;
		mbstowcs_s(&outSize, wpath, strlen(path) + 1, path, strlen(path) + 1 - 1);
		LPWSTR lppath = wpath;

		SystemParametersInfo(SPI_SETDESKWALLPAPER, 0, lppath, SPIF_UPDATEINIFILE);
	}


	delete renderedScene;
	delete sceneToRender;
	rendering = false;
}

int RenderController::GetSceneQueueCount() const
{
	return scenes.size();
}
