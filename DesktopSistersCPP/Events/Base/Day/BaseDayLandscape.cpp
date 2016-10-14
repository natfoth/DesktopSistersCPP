#include "stdafx.h"
#include "BaseDayLandscape.h"
#include "ImageLoader.h"
#include <chrono>
#include <fstream>


BaseDayLandscape::BaseDayLandscape(Configuration * newConfig, ImageLoader * newImageController)
	: Event(newConfig, newImageController)
{

}


BaseDayLandscape::~BaseDayLandscape()
{
}

string BaseDayLandscape::GetEventName() const
{
	return "BaseDayLandscape";
}

vector<EventTags> BaseDayLandscape::Tags()
{
	vector<EventTags> tags;

	tags.push_back(EventTags::Landscape);

	return tags;
}

bool BaseDayLandscape::IsBaseEvent()
{
	return true;
}

double BaseDayLandscape::Chance()
{
	return 100;
}

int BaseDayLandscape::Length()
{
	return _config->GetUpdateTime();
}

int BaseDayLandscape::ZIndex()
{
	return 15;
}

bool BaseDayLandscape::CanRun(TimeController* timeController)
{
	return timeController->IsDayTime();
}

void BaseDayLandscape::Render(DesktopSisterRgbaBytes* wallpaper, TimeController* timeController)
{
	if (!timeController)
		return;

	auto landscapeImage = _imageController->loadBaseDayImage("Landscape.png");

	auto height = max(wallpaper->height - 1, landscapeImage->height - 1);
	wallpaper->Blit(landscapeImage, SisterRect(0, 0, landscapeImage->width - 1, landscapeImage->height - 1), SisterRect(0, 0, wallpaper->width - 1, height), ScNearestNeighbor, 100);

}

Event* BaseDayLandscape::clone()
{
	return new BaseDayLandscape(_config, _imageController);
}