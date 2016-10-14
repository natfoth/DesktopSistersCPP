#include "stdafx.h"
#include "BaseNightClouds.h"
#include "ImageLoader.h"
#include <chrono>
#include <fstream>
#include "ImgProc.h"


BaseNightClouds::BaseNightClouds(Configuration * newConfig, ImageLoader * newImageController)
	: Event(newConfig, newImageController)
{

}


BaseNightClouds::~BaseNightClouds()
{
}

string BaseNightClouds::GetEventName() const
{
	return "BaseNightClouds";
}

vector<EventTags> BaseNightClouds::Tags()
{
	vector<EventTags> tags;

	tags.push_back(EventTags::Clouds);

	return tags;
}

bool BaseNightClouds::IsBaseEvent()
{
	return true;
}

double BaseNightClouds::Chance()
{
	return 100;
}

int BaseNightClouds::Length()
{
	return _config->GetUpdateTime();
}

int BaseNightClouds::ZIndex()
{
	return 5;
}

bool BaseNightClouds::CanRun(TimeController* timeController)
{
	return timeController->IsNightTime();
}

void BaseNightClouds::Render(DesktopSisterRgbaBytes* wallpaper, TimeController* timeController)
{
	if (!timeController)
		return;

	auto cloud1 = _imageController->loadBaseNightImage("NightCloud1.png");
	auto cloud2 = _imageController->loadBaseNightImage("NightCloud2.png");
	auto cloud3 = _imageController->loadBaseNightImage("NightCloud3.png");

	auto ResW = _config->GetScreenWidth();

	auto x = int(ResW - (double(1.0 - timeController->dayRatio) * double(ResW) * 0.5));
	auto y = 20;
	wallpaper->Merge(cloud1, SisterPoint(x, y));

	x = int(ResW - (double(1.0 - timeController->dayRatio) * double(ResW) * 0.5) - 200);
	wallpaper->Merge(cloud2, SisterPoint(x, y));

	x = int(ResW - (double(1.0 - timeController->dayRatio) * double(ResW) * 0.5) - 320);
	wallpaper->Merge(cloud3, SisterPoint(x, y));
}

Event* BaseNightClouds::clone()
{
	return new BaseNightClouds(_config, _imageController);
}