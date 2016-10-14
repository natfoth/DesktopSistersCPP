#pragma once
#include "Event.h"

class ImageLoader;
class Configuration;

class BaseCanvas : public Event
{
public:
	BaseCanvas(Configuration * newConfig, ImageLoader * newImageController);
	~BaseCanvas();

	

	virtual vector<EventTags> Tags() override;
	virtual bool IsBaseEvent() override;
	virtual double Chance() override;
	virtual int Length() override;
	virtual int ZIndex() override;

	virtual void Render(DesktopSisterRgbaBytes * wallpaper, TimeController * timeManager) override;

	virtual Event* clone() override;
};

