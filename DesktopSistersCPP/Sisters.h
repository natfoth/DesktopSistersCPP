#pragma once

/// <summary>
/// The base manager class. This will control the pulses and the controllers
/// </summary>

#include <ctime>
#include "GoogleLocationServices.h"
#include "Configuration.h"
#include "RenderController.h"

class Sisters
{
public:
	Sisters();
	Sisters(int newUpdateTime);
	~Sisters();

	/// <summary>
	/// Pulse the main class, This function will be called in a forever loop so we must control the delay to our pulses
	/// </summary>
	void Pulse();

private:
	/// <summary>
	/// How Often in milliseconds to Pulse the controllers
	/// </summary>
	int updateTime;

	/// <summary>
	/// Keeps track of the current time for the pulser
	/// </summary>
	std::clock_t pulseClock;

	/// <summary>
	/// Tracks the current time before adding another update to the queue
	/// </summary>
	int sceneQueueTime;

	/// <summary>
	/// Tracks the current time before adding another update to the queue
	/// </summary>
	std::clock_t sceneQueueClock;

	/// <summary>
	/// Google's Location Services in order to get Lat and Longitude from addresses
	/// </summary>
	GoogleLocationServices * _locationServices;

	/// <summary>
	/// Our local configuration
	/// </summary>
	Configuration * _configuration;

	/// <summary>
	/// Controls the loading of images
	/// </summary>
	ImageLoader * _imageLoader;

	/// <summary>
	/// Our main render controller, controls actually rendering everything
	/// </summary>
	RenderController * _renderController;

	/// <summary>
	/// init the base classes and structs
	/// </summary>
	void Init();

	/// <summary>
	/// Pulse our actual controllers, This function is not time controlled!
	/// </summary>
	void PulseControllers();
};

