#include "stdafx.h"
#include "BaseDaySun.h"
#include "ImageLoader.h"
#include <chrono>
#include <fstream>
#include "ImgProc.h"


BaseDaySun::BaseDaySun(Configuration * newConfig, ImageLoader * newImageController)
	: Event(newConfig, newImageController)
{

}


BaseDaySun::~BaseDaySun()
{
}

string BaseDaySun::GetEventName() const
{
	return "BaseDaySun";
}

vector<EventTags> BaseDaySun::Tags()
{
	vector<EventTags> tags;

	tags.push_back(EventTags::Sun);

	return tags;
}

bool BaseDaySun::IsBaseEvent()
{
	return true;
}

double BaseDaySun::Chance()
{
	return 100;
}

int BaseDaySun::Length()
{
	return _config->GetUpdateTime();
}

int BaseDaySun::ZIndex()
{
	return 2;
}

bool BaseDaySun::CanRun(TimeController* timeController)
{
	return timeController->IsDayTime();
}

void BaseDaySun::Render(DesktopSisterRgbaBytes* wallpaper, TimeController* timeController)
{
	if (!timeController)
		return;

	auto sunImage = _imageController->loadBaseDayImage("Sun.png");

	auto sunHeight = min(sunImage->height, _config->GetScreenHeight() / 3.4);
	auto ratio = double(sunHeight) / sunImage->height;
	auto sunWidth = int(sunImage->width * ratio);

	sunWidth = sunImage->width;
	sunHeight = sunImage->height;

	auto sunX = _timeController->sunX - double(sunWidth) / 2.0;
	auto sunY = ((_config->GetScreenHeight() - 1) - _timeController->sunY) - double(sunHeight) / 2.0;

	wallpaper->Merge(sunImage, SisterPoint(sunX, sunY));
}

Event* BaseDaySun::clone()
{
	return new BaseDaySun(_config, _imageController);
}