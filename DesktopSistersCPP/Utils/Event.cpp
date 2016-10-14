#include "stdafx.h"
#include "Event.h"


Event::Event(Configuration * newConfig, ImageLoader * newImageController)
	: startTime(nullptr), 
	endTime(nullptr), 
	_config(newConfig), 
	_imageController(newImageController)
{
}


Event::~Event()
{
	delete startTime;
	delete endTime;
	delete _timeController;
}

/// <summary>
/// Gets the Width of our current screen
/// </summary>
int Event::GetScreenWidth()
{
	int resW;
	int resH;
	Util::GetDesktopResolution(resW, resH);
	return resW;
}

/// <summary>
/// Gets the Height of our current screen
/// </summary>
int Event::getScreenHeight()
{
	int resW;
	int resH;
	Util::GetDesktopResolution(resW, resH);
	return resH;
}

/// <summary>
/// Init the event with the associated time controller
/// </summary>
/// <param name="timeController"></param>
void Event::Init(TimeController * timeController)
{
	_timeController = new TimeController(Util::duplicateTime(timeController->dateTime), _config->GetCoords());
	_timeController->Update();
}

string Event::GetEventName() const
{
	return "Event";
}

/// <summary>
/// The chance per tick that we can spawn. Between 0 - 100
/// </summary>
/// <returns></returns>
double Event::Chance()
{
	return 100;
}

/// <summary>
/// Returns a list of the tags for this event
/// </summary>
/// <returns>Returns a list of the tags for this event</returns>
vector<EventTags> Event::Tags()
{
	vector<EventTags> tags;
	return tags;
}

/// <summary>
/// Returns the length of the event in minutes
/// </summary>
/// <returns></returns>
int Event::Length()
{
	return 5;
}

/// <summary>
/// The Index to render the event at, a higher value means it will render further away
/// </summary>
/// <returns>The Index to render the event at, a higher value means it will render further away</returns>
int Event::ZIndex()
{
	return 0;
}

/// <summary>
/// Returns if the event lasts the calender day
/// </summary>
/// <returns></returns>
bool Event::IsAllDay()
{
	return false;
}

/// <summary>
/// Decides if this event can appear with another event that has a similar tag
/// </summary>
/// <returns>Decides if this event can appear with another event that has a similar tag</returns>
bool Event::AllowDuplicateTags()
{
	return false;
}

/// <summary>
/// Returns if we are a base event. A Base event should be critical events that need to happen at all times.
/// </summary>
/// <returns></returns>
bool Event::IsBaseEvent()
{
	return false;
}

/// <summary>
/// If another event wants to spawn that shares a tag with this, should we be deleted and allow the new one to enter?
/// </summary>
/// <returns>returns if we would like to delete ourself to allow the next even to start</returns>
bool Event::CanBeOverRidden()
{
	return IsBaseEvent();
}

/// <summary>
/// Updates our Start Time and our End Time based on duration and current time
/// </summary>
/// <param name="timeController">The time controller used to determine the times</param>
void Event::UpdateTimes(TimeController * timeController)
{
	delete startTime;
	startTime = Util::duplicateTime(timeController->dateTime);

	delete endTime;
	endTime = Util::duplicateTime(startTime);
	endTime->tm_min += Length();

	if(IsAllDay())
	{
		startTime = Util::getDateTimeWithNewHour(startTime, 0, 0, true);

		endTime = Util::getDateTimeWithNewHour(endTime, 23, 59, true);
	}

	//Rollover the minutes into hours
	while(endTime->tm_min >= 60)
	{
		endTime->tm_hour += 1;
		endTime->tm_min -= 60;
	}

	//Rollover the hours into days
	while (endTime->tm_hour >= 24)
	{
		endTime->tm_mday += 1;
		endTime->tm_hour -= 24;
	}

	//This is just a super basic function, it will act weird on the end of months and new years!!!!
	//TODO : Fix the time rollover to take into account month lengths and year lengths
}

/// <summary>
/// Renders our Event onto the canvas
/// </summary>
/// <param name="wallpaper">The canvas we want to render our event to</param>
/// <param name="timeController">Our time controller</param>
void Event::Render(DesktopSisterRgbaBytes* wallpaper, TimeController* timeController)
{
	if (wallpaper && timeController)
		return;
}

/// <summary>
/// Determines if we can use our event
/// </summary>
/// <param name="timeController">The time controller</param>
/// <returns>returns if the event can run or not</returns>
bool Event::CanRun(TimeController * timeController)
{
	if (!timeController)
		return false;

	return true;
}

/// <summary>
/// Determines if we are currently running based on the time
/// </summary>
/// <param name="timeController"></param>
/// <returns></returns>
bool Event::IsCurrentlyActive(TimeController* timeController)
{
	auto anchoredTime = Util::getAnchoredDateTime();
	auto diffStartTime = difftime(Util::makeTime(startTime), Util::makeTime(anchoredTime));
	auto diffEndTime = difftime(Util::makeTime(endTime), Util::makeTime(anchoredTime));
	auto diffCurrentTime = difftime(Util::makeTime(timeController->dateTime), Util::makeTime(anchoredTime));

	delete anchoredTime;
	anchoredTime = nullptr;

	if (diffCurrentTime >= diffStartTime && diffCurrentTime < diffEndTime)
		return true;

	return false;
}

/// <summary>
/// The Maximum number of times this event can be duplicated on the screen at once
/// </summary>
/// <returns></returns>
int Event::MaxEvents()
{
	return 1;
}

/// <summary>
/// Calculates the percent of the used time so far for the event
/// </summary>
/// <param name="currentTime"></param>
/// <returns>returns a value between 0 and 1.0f</returns>
double Event::Ratio(struct tm * currentTime)
{
	auto anchoredTime = Util::getAnchoredDateTime();
	auto diffStartTime = difftime(Util::makeTime(startTime), Util::makeTime(anchoredTime));
	auto diffEndTime = difftime(Util::makeTime(endTime), Util::makeTime(anchoredTime));
	auto diffCurrentTime = difftime(Util::makeTime(currentTime), Util::makeTime(anchoredTime));

	auto ratio = float(diffCurrentTime - diffStartTime) / float(diffEndTime - diffStartTime);

	return ratio;
}

/// <summary>
/// Performs a DeepClone on the event
/// </summary>
/// <returns>returns a new deep clone copy of this event</returns>
Event * Event::clone()
{
	return new Event(_config, _imageController);
}
