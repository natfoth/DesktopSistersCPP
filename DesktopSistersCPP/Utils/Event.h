#pragma once

/// <summary>
/// The abstract base event class that controls the rendering and timing of all of our events
/// </summary>

#include <iostream>
#include <iomanip>
#include <ctime>
#include "TimeController.h"
#include "Imager.h"
#include "Configuration.h"
#include "ImageLoader.h"

using namespace std;

class Event
{
public:
	Event(Configuration * newConfig, ImageLoader * newImageController);
	virtual ~Event();

	/// <summary>
	/// The Time that our event starts
	/// </summary>
	struct tm * startTime;

	/// <summary>
	/// The Time that our event ends
	/// </summary>
	struct tm * endTime;


	//Functions//

	/// <summary>
	/// Gets the Width of our current screen
	/// </summary>
	static int GetScreenWidth();

	/// <summary>
	/// Gets the Height of our current screen
	/// </summary>
	static int getScreenHeight();

	/// <summary>
	/// Init the event with the associated time controller
	/// </summary>
	/// <param name="timeController"></param>
	virtual void Init(TimeController * timeController);

	/// <summary>
	/// Returns a unique name for the event. MUST BE UNIQUE!!
	/// </summary>
	/// <returns></returns>
	virtual string GetEventName() const;

	/// <summary>
	/// The chance per tick that we can spawn. Between 0 - 100
	/// </summary>
	/// <returns></returns>
	virtual double Chance();

	/// <summary>
	/// Returns a list of the tags for this event
	/// </summary>
	/// <returns>Returns a list of the tags for this event</returns>
	virtual vector<EventTags> Tags();

	/// <summary>
	/// Returns the length of the event in minutes
	/// </summary>
	/// <returns></returns>
	virtual int Length();

	/// <summary>
	/// The Index to render the event at, a higher value means it will render further away
	/// </summary>
	/// <returns>The Index to render the event at, a higher value means it will render further away</returns>
	virtual int ZIndex();

	/// <summary>
	/// Returns if the event lasts the calender day
	/// </summary>
	/// <returns></returns>
	virtual bool IsAllDay();

	/// <summary>
	/// Decides if this event can appear with another event that has a similar tag
	/// </summary>
	/// <returns>Decides if this event can appear with another event that has a similar tag</returns>
	virtual bool AllowDuplicateTags();

	/// <summary>
	/// Returns if we are a base event. A Base event should be critical events that need to happen at all times.
	/// </summary>
	/// <returns></returns>
	virtual bool IsBaseEvent();

	/// <summary>
	/// If another event wants to spawn that shares a tag with this, should we be deleted and allow the new one to enter?
	/// </summary>
	/// <returns>returns if we would like to delete ourself to allow the next even to start</returns>
	virtual bool CanBeOverRidden();

	/// <summary>
	/// Updates our Start Time and our End Time based on duration and current time
	/// </summary>
	/// <param name="timeController">The time controller used to determine the times</param>
	virtual void UpdateTimes(TimeController * timeController);

	/// <summary>
	/// Draws our event onto the frame
	/// </summary>
	virtual void Render(DesktopSisterRgbaBytes * wallpaper, TimeController * timeController);

	/// <summary>
	/// Determines if we can use our event
	/// </summary>
	/// <param name="timeController">The time controller</param>
	/// <returns>returns if the event can run or not</returns>
	virtual bool CanRun(TimeController * timeController);

	/// <summary>
	/// Determines if we are currently running based on the time
	/// </summary>
	/// <param name="timeController"></param>
	/// <returns></returns>
	virtual bool IsCurrentlyActive(TimeController * timeController);

	/// <summary>
	/// The Maximum number of times this event can be duplicated on the screen at once
	/// </summary>
	/// <returns></returns>
	virtual int MaxEvents();

	/// <summary>
	/// Calculates the percent of the used time so far for the event
	/// </summary>
	/// <param name="currentTime"></param>
	/// <returns>returns a value between 0 and 1.0f</returns>
	virtual double Ratio(struct tm * currentTime);

	/// <summary>
	/// Performs a DeepClone on the event
	/// </summary>
	/// <returns>returns a new deep clone copy of this event</returns>
	virtual Event * clone();

	/// <summary>
	/// Equals Operator that just checks based on the strong type
	/// </summary>
	/// <param name="other"></param>
	/// <returns></returns>
	bool operator==(const Event *other) const
	{
		return GetEventName() == other->GetEventName();
	}

protected:
	TimeController * _timeController;

	Configuration * _config;
	ImageLoader * _imageController;
	
};

