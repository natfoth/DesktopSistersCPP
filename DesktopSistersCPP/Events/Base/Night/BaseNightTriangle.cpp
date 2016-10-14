#include "stdafx.h"
#include "BaseNightTriangle.h"
#include "ImageLoader.h"
#include <chrono>
#include <fstream>
#include "ImgProc.h"


BaseNightTriangle::BaseNightTriangle(Configuration * newConfig, ImageLoader * newImageController)
	: Event(newConfig, newImageController)
{

}


BaseNightTriangle::~BaseNightTriangle()
{
}

string BaseNightTriangle::GetEventName() const
{
	return "BaseNightTriangle";
}

vector<EventTags> BaseNightTriangle::Tags()
{
	vector<EventTags> tags;

	tags.push_back(EventTags::SkyEvent);

	return tags;
}

bool BaseNightTriangle::AllowDuplicateTags()
{
	return true;
}

bool BaseNightTriangle::IsBaseEvent()
{
	return true;
}

double BaseNightTriangle::Chance()
{
	return 100;
}

int BaseNightTriangle::Length()
{
	return _config->GetUpdateTime();
}

int BaseNightTriangle::ZIndex()
{
	return 1;
}

bool BaseNightTriangle::CanRun(TimeController* timeController)
{
	return timeController->IsNightTime();
}

void BaseNightTriangle::Render(DesktopSisterRgbaBytes* wallpaper, TimeController* timeController)
{
	auto starImage = _imageController->loadBaseNightImage("Triangle.png");

	auto x = int(double(timeController->nightRatio) * double(_config->GetScreenWidth()));
	wallpaper->Merge(starImage, SisterPoint(float(x), float(_config->GetScreenHeight() - 150)));
}

Event* BaseNightTriangle::clone()
{
	auto newEvent = new BaseNightTriangle(_config, _imageController);
	return newEvent;
}