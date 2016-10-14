#pragma once
#include "Event.h"

class ImageLoader;
class Configuration;

class BaseFallingStar : public Event
{
public:
	BaseFallingStar(Configuration * newConfig, ImageLoader * newImageController);
	~BaseFallingStar();

	
	virtual string GetEventName() const override;
	virtual vector<EventTags> Tags() override;
	virtual bool AllowDuplicateTags() override;
	virtual bool IsBaseEvent() override;
	virtual double Chance() override;
	virtual int Length() override;
	virtual int ZIndex() override;
	virtual bool CanRun(TimeController * timeController) override;

	virtual void Render(DesktopSisterRgbaBytes * wallpaper, TimeController * timeManager);

	virtual Event* clone();
};

