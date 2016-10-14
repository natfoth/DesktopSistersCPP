#include "stdafx.h"
#include "BaseNightStars.h"
#include "ImageLoader.h"
#include <chrono>
#include <fstream>
#include "ImgProc.h"


BaseNightStars::BaseNightStars(Configuration * newConfig, ImageLoader * newImageController)
	: Event(newConfig, newImageController)
{

}


BaseNightStars::~BaseNightStars()
{
}

string BaseNightStars::GetEventName() const
{
	return "BaseNightStars";
}

vector<EventTags> BaseNightStars::Tags()
{
	vector<EventTags> tags;

	tags.push_back(EventTags::Stars);

	return tags;
}

bool BaseNightStars::IsBaseEvent()
{
	return true;
}

double BaseNightStars::Chance()
{
	return 100;
}

int BaseNightStars::Length()
{
	return _config->GetUpdateTime();
}

int BaseNightStars::ZIndex()
{
	return 1;
}

bool BaseNightStars::CanRun(TimeController* timeController)
{
	return timeController->IsNightTime();
}

void BaseNightStars::Render(DesktopSisterRgbaBytes* wallpaper, TimeController* timeController)
{
	if (!timeController)
		return;

	auto starImage = _imageController->loadBaseNightImage("Stars.png");

	wallpaper->Blit(starImage, SisterRect(0, 0, starImage->width - 1, starImage->height-1), SisterRect(0, 0, wallpaper->width - 1, wallpaper->height - 1), ScNearestNeighbor, 50);
}

Event* BaseNightStars::clone()
{
	return new BaseNightStars(_config, _imageController);
}