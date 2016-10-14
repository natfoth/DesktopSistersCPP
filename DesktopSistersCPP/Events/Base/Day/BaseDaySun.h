#pragma once
#include "Event.h"

class ImageLoader;
class Configuration;

class BaseDaySun : public Event
{
public:
	BaseDaySun(Configuration * newConfig, ImageLoader * newImageController);
	~BaseDaySun();

	
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

