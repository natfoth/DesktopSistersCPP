/// <summary>
/// The base manager class. This will control the pulses and the controllers
/// </summary>

#include "stdafx.h"
#include "Sisters.h"
#include "TimeController.h"
#include <iostream>
#include "ImageLoader.h"


Sisters::Sisters()
	: updateTime(1000), 
	pulseClock(clock()), 
	sceneQueueClock(clock()),
	_locationServices(new GoogleLocationServices(false)), 
	_configuration(new Configuration()), 
	_imageLoader(new ImageLoader()), 
	_renderController(nullptr)
{
	Init();
}

Sisters::Sisters(int newUpdateTime)
	: updateTime(newUpdateTime), 
	pulseClock(clock()), 
	sceneQueueClock(clock()),
	_locationServices(new GoogleLocationServices(false)), 
	_configuration(new Configuration()), 
	_imageLoader(new ImageLoader()), 
	_renderController(nullptr)
{
	Init();
}

Sisters::~Sisters()
{
}

/// <summary>
/// Pulse the main class, This function will be called in a forever loop so we must control the delay to our pulses
/// </summary>
void Sisters::Pulse()
{
	float duration = (clock() - pulseClock) / static_cast<float>(CLOCKS_PER_SEC / 1000);
	if(duration > updateTime)
	{
		//Reset the clock to our current time so that the duration can restart
		pulseClock = clock();

		PulseControllers();
	}
}

/// <summary>
/// init the base classes and structs
/// </summary>
void Sisters::Init()
{
	_configuration->LoadConfiguration();

	if(_configuration->GetAddress().size() == 0)
	{
		string location = "";

		cout << "Please enter your Address:\n>";
		getline(cin, location);

		_configuration->SetAddress(location);

		auto point = _locationServices->GetLatLongFromAddress(location);
		_configuration->SetCoords(point);
	}
	

	

#if defined(TESTLOADANDSAVE)
	auto testImage = _imageLoader->loadBaseDayImage("Landscape.png");
	_imageLoader->saveImage(testImage, "test.bmp");
	return;
#endif

	_renderController = new RenderController(_configuration, _imageLoader);
	auto currentTime = Util::getCurrentDateTime();
	_renderController->AddSceneToQueue(currentTime, "Wallpaper.bmp");

	sceneQueueTime = 1 * 60000; // Convert to milliseconds

#if defined(TESTALL)
	auto baseTimeSister = Util::getCurrentDateTime();
	baseTimeSister->tm_hour = 0;
	baseTimeSister->tm_min = 0;
	baseTimeSister->tm_sec = 0;
	const int minuteIntervals = _configuration->GetUpdateTime();
	auto numOfTimes = 86400 / (60 * minuteIntervals); // every minuteIntervals change events
	for (auto i = 0; i < numOfTimes; i++) // 
	{
		_renderController->AddSceneToQueue(Util::getDateTimeWithNewHour(baseTimeSister, baseTimeSister->tm_hour, baseTimeSister->tm_min, false), to_string(baseTimeSister->tm_hour) + "_" + to_string(baseTimeSister->tm_min) + ".bmp");

		Util::addTimeToStruct(baseTimeSister, 0, minuteIntervals, 0);
	}
#endif
}

/// <summary>
/// Pulse our actual controllers, This function is not time controlled!
/// </summary>
void Sisters::PulseControllers()
{
	float duration = (clock() - sceneQueueClock) / static_cast<float>(CLOCKS_PER_SEC / 1000);
	if (duration > sceneQueueTime)
	{
		//Reset the clock to our current time so that the duration can restart
		sceneQueueClock = clock();

#if !defined(TESTALL)
		_renderController->AddSceneToQueue(Util::getCurrentDateTime(), "Wallpaper.bmp");
#endif
	}
	while (_renderController->GetSceneQueueCount() > 0)
	{
		_renderController->Pulse();
	}
}
