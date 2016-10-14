#include "stdafx.h"
#include "BaseFallingStar.h"
#include "ImageLoader.h"
#include <chrono>
#include <fstream>
#include "ImgProc.h"


BaseFallingStar::BaseFallingStar(Configuration * newConfig, ImageLoader * newImageController)
	: Event(newConfig, newImageController)
{

}


BaseFallingStar::~BaseFallingStar()
{
}

string BaseFallingStar::GetEventName() const
{
	return "BaseFallingStar";
}

vector<EventTags> BaseFallingStar::Tags()
{
	vector<EventTags> tags;

	tags.push_back(EventTags::SkyEvent);

	return tags;
}

bool BaseFallingStar::AllowDuplicateTags()
{
	return true;
}

bool BaseFallingStar::IsBaseEvent()
{
	return true;
}

double BaseFallingStar::Chance()
{
	return 100;
}

int BaseFallingStar::Length()
{
	return _config->GetUpdateTime();
}

int BaseFallingStar::ZIndex()
{
	return 1;
}

bool BaseFallingStar::CanRun(TimeController* timeController)
{
	return timeController->IsNightTime();
}

void BaseFallingStar::Render(DesktopSisterRgbaBytes* wallpaper, TimeController* timeController)
{
	auto starImage = _imageController->loadBaseNightImage("FallingStar.png");

	auto x = int(double(timeController->nightRatio) * double(_config->GetScreenWidth())) + 200;
	wallpaper->Merge(starImage, SisterPoint(x, _config->GetScreenHeight() - 200));
}

Event* BaseFallingStar::clone()
{
	return new BaseFallingStar(_config, _imageController);
}