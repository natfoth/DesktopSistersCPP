#include "stdafx.h"
#include "EventController.h"

#include <random>
#include <filesystem>

#include "BaseDayLandscape.h"
#include "BaseDayBackground.h"
#include "BaseDaySun.h"
#include "BaseDayClouds.h"
#include "BaseCelestia.h"

#include "BaseNightBackground.h"
#include "BaseNightLandscape.h"
#include "BaseNightMoon.h"
#include "BaseFallingStar.h"
#include "BaseLuna.h"
#include "BaseNightClouds.h"
#include "BaseNightStars.h"
#include "BaseNightTriangle.h"

/// <summary>
/// Handles tracking of the events, generating the events based on time, and randomizing the events to be rendered
/// </summary>
EventController::EventController(Configuration * newConfig, ImageLoader * newImageController)
{
	config = newConfig;
	imageController = newImageController;

	events.push_back(new BaseDayLandscape(config, imageController));
	events.push_back(new BaseDayBackground(config, imageController));
	events.push_back(new BaseDaySun(config, imageController));
	events.push_back(new BaseDayClouds(config, imageController));
	events.push_back(new BaseCelestia(config, imageController));

	events.push_back(new BaseNightBackground(config, imageController));
	events.push_back(new BaseNightLandscape(config, imageController));
	events.push_back(new BaseNightMoon(config, imageController));
	events.push_back(new BaseFallingStar(config, imageController));
	events.push_back(new BaseLuna(config, imageController));
	events.push_back(new BaseNightClouds(config, imageController));
	events.push_back(new BaseNightStars(config, imageController));
	events.push_back(new BaseNightTriangle(config, imageController));

	//GenerateEvents();
}


EventController::~EventController()
{

}

/// <summary>
/// Renders all of the activate events to the passed in image
/// </summary>
/// <param name="wallpaper">Which image to render the events to</param>
/// <param name="timeController">The time controller for the time</param>
void EventController::RenderEvents(DesktopSisterRgbaBytes * wallpaper, TimeController * timeController)
{
	auto eventsForTime = EventsForTime(timeController);

	sort(eventsForTime.begin(), eventsForTime.end(), [](Event*& lhs, Event*& rhs)
	{
		return lhs->ZIndex() < rhs->ZIndex();
	});

	for (auto&& event : eventsForTime)
	{
		typedef std::chrono::high_resolution_clock Clock;
		auto t1 = Clock::now();

		event->Render(wallpaper, timeController);

		//Timer
		auto t2 = Clock::now();
		auto duration = (t2 - t1);
		auto countDur = duration.count();
		cout << "Rendered : " << event->GetEventName() << " - in : " << to_string(countDur / 1000000.0) << " milliseconds\n";
	}
}

/// <summary>
/// Returns a randomized event list for the specified time
/// </summary>
/// <param name="timeController">The TimeController for the time</param>
/// <returns></returns>

vector<Event*> EventController::GetRandomEvent(TimeController * timeController)
{
	vector<Event*> list;



	for (auto&& dynamicEvent : events)
	{
		if (!dynamicEvent->CanRun(timeController))
			continue;

		auto rand = RandomFloat(100);
		if (rand <= dynamicEvent->Chance())
			list.push_back(dynamicEvent);
	}

	return list;
}

/// <summary>
/// Returns all the currently active or previously active events for the time. Does not generate new events if there is existing events for the time.
/// </summary>
/// <param name="timeController">The TimeController for the time</param>
/// <returns></returns>
vector<Event*> EventController::EventsForTime(TimeController * timeController)
{
	vector<Event*> list;

	RemoveOldActiveEvents(timeController);

	for (auto&& event : activeEvents)
	{
		list.push_back(event);
	}

	auto randomEvents = GetRandomEvent(timeController);

	for (auto&& event : randomEvents)
	{
		auto newEvent = event->clone();
		newEvent->Init(timeController);
		newEvent->UpdateTimes(timeController);

		auto countOfSameEvent = CountOfSameEvents(newEvent, activeEvents);
		if (countOfSameEvent > newEvent->MaxEvents())
		{
			delete newEvent;
			newEvent = nullptr;
			continue;
		}

		if (!newEvent->AllowDuplicateTags())
		{
			auto overridableTagsCount = CountOfSameNonOverridableTags(newEvent, activeEvents);
			if (overridableTagsCount > 0)
				continue;

			auto sameTagsList = ListOfSameTag(newEvent, activeEvents);

			if (sameTagsList.size() > 0 && newEvent->IsBaseEvent())
				continue;

			for (auto&& sameTagEvent : sameTagsList)
			{
				activeEvents.erase(std::remove(activeEvents.begin(), activeEvents.end(), sameTagEvent), activeEvents.end());
			}
		}

		bool shouldContinue = false;
		for (auto&& listCont : list) // continue if we already have one in the list like this
		{
			if (listCont->GetEventName() == newEvent->GetEventName())
				shouldContinue = true;
		}

		if (shouldContinue)
			continue;

		if (newEvent->CanRun(timeController) && newEvent->IsCurrentlyActive(timeController))
		{
			activeEvents.push_back(newEvent);
			list.push_back(newEvent);
		}
	}

	return list;
}

/// <summary>
/// Returns the number of events that match the passed in event
/// </summary>
/// <param name="forEvent">The event you wish to match</param>
/// <param name="eventsForTime">The active list of events to compare to</param>
/// <returns></returns>
int EventController::CountOfSameEvents(Event* forEvent, vector<Event*> eventsForTime)
{
	vector<Event*> list;

	for (auto&& evt : eventsForTime)
	{
		if (evt == forEvent)
			list.push_back(evt);
	}

	return list.size();
}

/// <summary>
/// Returns the number of events that match the passed in event based on tags
/// </summary>
/// <param name="forEvent">The event you wish to match</param>
/// <param name="eventsForTime">The active list of events to compare to</param>
/// <returns></returns>
int EventController::CountOfSameNonOverridableTags(Event* forEvent, vector<Event*> eventsForTime)
{
	vector<Event*> list;

	for (auto&& evt : eventsForTime)
	{
		if (!evt->CanBeOverRidden())
		{
			auto eventTags = evt->Tags();
			auto forEventTags = forEvent->Tags();

			for (auto&& evtTag : eventTags)
			{
				for (auto&& forTag : forEventTags)
				{
					if (EventTags::Tags(forTag) == EventTags::Tags(evtTag))
					{
						list.push_back(evt);
						continue;
					}
				}
			}
		}
	}

	return list.size();
}

/// <summary>
/// Returns all of the events that contain a conflicting tag
/// </summary>
/// <param name="forEvent">the event to match to</param>
/// <param name="eventsForTime">The active list of events to compare to</param>
/// <returns></returns>
vector<Event*> EventController::ListOfSameTag(Event* forEvent, vector<Event*> eventsForTime)
{
	vector<Event*> list;

	for (auto&& evt : eventsForTime)
	{
		if (evt->CanBeOverRidden())
		{
			auto eventTags = evt->Tags();
			auto forEventTags = forEvent->Tags();

			for (auto&& evtTag : eventTags)
			{
				for (auto&& forTag : forEventTags)
				{
					if (EventTags::Tags(forTag) == EventTags::Tags(evtTag))
					{
						list.push_back(evt);
						continue;
					}
				}
			}
		}
	}

	return list;
}

/// <summary>
/// Removes the currently active events from the list that have expired
/// </summary>
/// <param name="timeController">The TimeController to compare all of the active events to</param>
void EventController::RemoveOldActiveEvents(TimeController * timeController)
{
	vector<Event*> stillActiveList;
	for (auto&& event : activeEvents)
	{
		if (event->CanRun(timeController) && event->IsCurrentlyActive(timeController))
		{
			stillActiveList.push_back(event);
		}
		else
		{
			// Deletes old events to save on memory
			delete event;
			event = nullptr;
		}
	}

	activeEvents = stillActiveList;
}

/// <summary>
/// Randomly returns a float between 0 and your max
/// </summary>
/// <param name="max">the highest number you wish to get</param>
/// <returns></returns>
float EventController::RandomFloat(int max)
{
	const double lower_bound = 0;
	uniform_real_distribution<double> unif(lower_bound, max);

	mt19937 rand_engine(unsigned int(chrono::system_clock::now().time_since_epoch().count())); // mt19937 is a good pseudo-random number 
										  // generator.

	return float(unif(rand_engine));
}