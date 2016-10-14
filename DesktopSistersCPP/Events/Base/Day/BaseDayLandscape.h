#pragma once
#include "Event.h"

class ImageLoader;
class Configuration;

class BaseDayLandscape : public Event
{
public:
	BaseDayLandscape(Configuration * newConfig, ImageLoader * newImageController);
	~BaseDayLandscape();

	virtual string GetEventName() const override;
	virtual vector<EventTags> Tags() override;
	virtual bool IsBaseEvent() override;
	virtual double Chance() override;
	virtual int Length() override;
	virtual int ZIndex() override;
	virtual bool CanRun(TimeController * timeController) override;

	virtual void Render(DesktopSisterRgbaBytes * wallpaper, TimeController * timeManager);

	virtual Event* clone();
};

