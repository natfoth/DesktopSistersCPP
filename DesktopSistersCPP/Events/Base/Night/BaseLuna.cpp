#include "stdafx.h"
#include "BaseLuna.h"
#include "ImageLoader.h"
#include <chrono>
#include <fstream>
#include "ImgProc.h"


BaseLuna::BaseLuna(Configuration * newConfig, ImageLoader * newImageController)
	: Event(newConfig, newImageController)
{

}


BaseLuna::~BaseLuna()
{
}

string BaseLuna::GetEventName() const
{
	return "BaseLuna";
}

vector<EventTags> BaseLuna::Tags()
{
	vector<EventTags> tags;

	tags.push_back(EventTags::Luna);
	tags.push_back(EventTags::Alicorn);

	return tags;
}

bool BaseLuna::IsBaseEvent()
{
	return true;
}

double BaseLuna::Chance()
{
	return 100;
}

int BaseLuna::Length()
{
	return _config->GetUpdateTime();
}

int BaseLuna::ZIndex()
{
	return 25;
}

bool BaseLuna::CanRun(TimeController* timeController)
{
	return timeController->IsNightTime();
}

void BaseLuna::Render(DesktopSisterRgbaBytes* wallpaper, TimeController* timeController)
{
	if (!timeController)
		return;

	
	auto lunaImage = _imageController->loadBaseNightImage("Luna.png");

	auto lunaHeight = min(lunaImage->height, 350);
	auto ratio = double(lunaHeight) / lunaImage->height;
	auto lunaWidth = int(lunaImage->width * ratio);

	wallpaper->Blit(lunaImage, SisterRect(0, 0, lunaImage->width - 1, lunaImage->height-1), SisterRect(0, 30, lunaWidth, lunaHeight), ScNearestNeighbor, 100);
}

Event* BaseLuna::clone()
{
	return new BaseLuna(_config, _imageController);
}