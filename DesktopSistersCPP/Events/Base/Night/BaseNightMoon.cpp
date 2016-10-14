#include "stdafx.h"
#include "BaseNightMoon.h"
#include "ImageLoader.h"
#include <chrono>
#include <fstream>
#include "ImgProc.h"


BaseNightMoon::BaseNightMoon(Configuration * newConfig, ImageLoader * newImageController)
	: Event(newConfig, newImageController)
{

}


BaseNightMoon::~BaseNightMoon()
{
}

string BaseNightMoon::GetEventName() const
{
	return "BaseNightMoon";
}

vector<EventTags> BaseNightMoon::Tags()
{
	vector<EventTags> tags;

	tags.push_back(EventTags::Moon);

	return tags;
}

bool BaseNightMoon::IsBaseEvent()
{
	return true;
}

double BaseNightMoon::Chance()
{
	return 100;
}

int BaseNightMoon::Length()
{
	return _config->GetUpdateTime();
}

int BaseNightMoon::ZIndex()
{
	return 2;
}

bool BaseNightMoon::CanRun(TimeController* timeController)
{
	return timeController->IsNightTime();
}

void BaseNightMoon::Render(DesktopSisterRgbaBytes* wallpaper, TimeController* timeController)
{
	if (!timeController)
		return;

	auto moonImage = _imageController->loadBaseNightImage("Moon.png");

	auto moonHeight = min(moonImage->height, _config->GetScreenHeight() / 3.4);
	auto ratio = double(moonHeight) / moonImage->height;
	auto moonWidth = int(moonImage->width * ratio);

	moonWidth = moonImage->width;
	moonHeight = moonImage->height;

	auto moonX = _timeController->moonX - double(moonWidth) / 2.0;
	auto moonY = ((_config->GetScreenHeight() - 1) - _timeController->moonY) - double(moonHeight) / 2.0;

	wallpaper->Merge(moonImage, SisterPoint(moonX, moonY));
}

Event* BaseNightMoon::clone()
{
	return new BaseNightMoon(_config, _imageController);
}