#pragma once
#include "TimeController.h"
#include <vector>
#include "Event.h"
#include "Configuration.h"
#include "Imager.h"

class ImageLoader;
using namespace std;
class SceneContainer
{
public:
	SceneContainer(struct tm * forTime, vector<Event*> listOfEventsForTime)
	{
		time = forTime;
		events = listOfEventsForTime;
	}

	~SceneContainer() {}

	struct tm * time;
	vector<Event*> events;
};

/// <summary>
/// Handles tracking of the events, generating the events based on time, and randomizing the events to be rendered
/// </summary>
class EventController
{
public:
	EventController(Configuration * newConfig, ImageLoader * newImageController);
	~EventController();

	
	/// <summary>
	/// Renders all of the activate events to the passed in image
	/// </summary>
	/// <param name="wallpaper">Which image to render the events to</param>
	/// <param name="timeController">The time controller for the time</param>
	void RenderEvents(DesktopSisterRgbaBytes * wallpaper, TimeController * timeController);

	/// <summary>
	/// Returns a randomized event list for the specified time
	/// </summary>
	/// <param name="timeController">The TimeController for the time</param>
	/// <returns></returns>
	vector<Event*> GetRandomEvent(TimeController* timeController);
	
	/// <summary>
	/// Returns all the currently active or previously active events for the time. Does not generate new events if there is existing events for the time.
	/// </summary>
	/// <param name="timeController">The TimeController for the time</param>
	/// <returns></returns>
	vector<Event*> EventsForTime(TimeController * timeController);
	

private:
	/// <summary>
	/// Our current configuration file in order to access our location and update times
	/// </summary>
	Configuration * config;

	/// <summary>
	/// The Image Controller to load in the images for our events
	/// </summary>
	ImageLoader * imageController;

	/// <summary>
	/// All the events we can pull from for our random events for the time
	/// </summary>
	vector<Event*> events;

	/// <summary>
	/// The Specific events that are currently actively running
	/// </summary>
	vector<Event*> activeEvents;

	/// <summary>
	/// Returns the number of events that match the passed in event
	/// </summary>
	/// <param name="forEvent">The event you wish to match</param>
	/// <param name="eventsForTime">The active list of events to compare to</param>
	/// <returns></returns>
	static int CountOfSameEvents(Event* forEvent, vector<Event*> eventsForTime);

	/// <summary>
	/// Returns the number of events that match the passed in event based on tags
	/// </summary>
	/// <param name="forEvent">The event you wish to match</param>
	/// <param name="eventsForTime">The active list of events to compare to</param>
	/// <returns></returns>
	static int CountOfSameNonOverridableTags(Event* forEvent, vector<Event*> eventsForTime);

	/// <summary>
	/// Returns all of the events that contain a conflicting tag
	/// </summary>
	/// <param name="forEvent">the event to match to</param>
	/// <param name="eventsForTime">The active list of events to compare to</param>
	/// <returns></returns>
	static vector<Event*> ListOfSameTag(Event* forEvent, vector<Event*> eventsForTime);

	/// <summary>
	/// Removes the currently active events from the list that have expired
	/// </summary>
	/// <param name="timeController">The TimeController to compare all of the active events to</param>
	void RemoveOldActiveEvents(TimeController * timeController);

	/// <summary>
	/// Randomly returns a float between 0 and your max
	/// </summary>
	/// <param name="max">the highest number you wish to get</param>
	/// <returns></returns>
	static float RandomFloat(int max);
};

