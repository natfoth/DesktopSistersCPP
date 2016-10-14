#pragma once
#include "Event.h"

class ImageLoader;
class Configuration;

class BaseDayBackground : public Event
{
public:
	BaseDayBackground(Configuration * newConfig, ImageLoader * newImageController);
	~BaseDayBackground();

	
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

