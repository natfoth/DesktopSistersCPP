#pragma once
#include "Event.h"

class ImageLoader;
class Configuration;

class BaseNightBackground : public Event
{
public:
	BaseNightBackground(Configuration * newConfig, ImageLoader * newImageController);
	~BaseNightBackground();

	
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

