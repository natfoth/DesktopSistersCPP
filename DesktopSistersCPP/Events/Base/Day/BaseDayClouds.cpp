#include "stdafx.h"
#include "BaseDayClouds.h"
#include "ImageLoader.h"
#include <chrono>
#include <fstream>
#include "ImgProc.h"


BaseDayClouds::BaseDayClouds(Configuration * newConfig, ImageLoader * newImageController)
	: Event(newConfig, newImageController)
{

}


BaseDayClouds::~BaseDayClouds()
{
}

string BaseDayClouds::GetEventName() const
{
	return "BaseDayClouds";
}

vector<EventTags> BaseDayClouds::Tags()
{
	vector<EventTags> tags;

	tags.push_back(EventTags::Clouds);

	return tags;
}

bool BaseDayClouds::IsBaseEvent()
{
	return true;
}

double BaseDayClouds::Chance()
{
	return 100;
}

int BaseDayClouds::Length()
{
	return _config->GetUpdateTime();
}

int BaseDayClouds::ZIndex()
{
	return 5;
}

bool BaseDayClouds::CanRun(TimeController* timeController)
{
	return timeController->IsDayTime();
}

void BaseDayClouds::Render(DesktopSisterRgbaBytes* wallpaper, TimeController* timeController)
{
	if (!timeController)
		return;
	
	auto cloud1 = _imageController->loadBaseDayImage("DayCloud1.png");
	auto cloud2 = _imageController->loadBaseDayImage("DayCloud2.png");
	auto cloud3 = _imageController->loadBaseDayImage("DayCloud3.png");

	auto ResW = _config->GetScreenWidth();

	auto x = int(ResW - (double(1.0 - timeController->dayRatio) * double(ResW) * 0.5));
	auto y = 20;
	wallpaper->Merge(cloud1, SisterPoint(x, y));

	x = int(ResW - (double(1.0 - timeController->dayRatio) * double(ResW) * 0.5) - 200);
	wallpaper->Merge(cloud2, SisterPoint(x, y));

	x = int(ResW - (double(1.0 - timeController->dayRatio) * double(ResW) * 0.5) - 320);
	wallpaper->Merge(cloud3, SisterPoint(x, y));
}

Event* BaseDayClouds::clone()
{
	return new BaseDayClouds(_config, _imageController);
}