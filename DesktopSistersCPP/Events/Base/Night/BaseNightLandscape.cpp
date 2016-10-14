#include "stdafx.h"
#include "BaseNightLandscape.h"
#include "ImageLoader.h"
#include <chrono>
#include <fstream>


BaseNightLandscape::BaseNightLandscape(Configuration * newConfig, ImageLoader * newImageController)
	: Event(newConfig, newImageController)
{

}


BaseNightLandscape::~BaseNightLandscape()
{
}

string BaseNightLandscape::GetEventName() const
{
	return "BaseNightLandscape";
}

vector<EventTags> BaseNightLandscape::Tags()
{
	vector<EventTags> tags;

	tags.push_back(EventTags::Landscape);

	return tags;
}

bool BaseNightLandscape::IsBaseEvent()
{
	return true;
}

double BaseNightLandscape::Chance()
{
	return 100;
}

int BaseNightLandscape::Length()
{
	return _config->GetUpdateTime();
}

int BaseNightLandscape::ZIndex()
{
	return 15;
}

bool BaseNightLandscape::CanRun(TimeController* timeController)
{
	return timeController->IsNightTime();
}

void BaseNightLandscape::Render(DesktopSisterRgbaBytes* wallpaper, TimeController* timeController)
{
	if (!timeController)
		return;

	auto landscapeImage = _imageController->loadBaseNightImage("LandscapeNight.png");

	auto height = max(wallpaper->height - 1, landscapeImage->height - 1);
	wallpaper->Blit(landscapeImage, SisterRect(0, 0, landscapeImage->width - 1, landscapeImage->height - 1), SisterRect(0, 0, wallpaper->width - 1, height), ScNearestNeighbor, 100);
}

Event* BaseNightLandscape::clone()
{
	return new BaseNightLandscape(_config, _imageController);
}