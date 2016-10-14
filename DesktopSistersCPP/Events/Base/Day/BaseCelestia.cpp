#include "stdafx.h"
#include "BaseCelestia.h"
#include "ImageLoader.h"
#include <chrono>
#include <fstream>
#include "ImgProc.h"


BaseCelestia::BaseCelestia(Configuration * newConfig, ImageLoader * newImageController)
	: Event(newConfig, newImageController)
{

}


BaseCelestia::~BaseCelestia()
{
}

string BaseCelestia::GetEventName() const
{
	return "BaseCelestia";
}

vector<EventTags> BaseCelestia::Tags()
{
	vector<EventTags> tags;

	tags.push_back(EventTags::Celestia);
	tags.push_back(EventTags::Alicorn);

	return tags;
}

bool BaseCelestia::IsBaseEvent()
{
	return true;
}

double BaseCelestia::Chance()
{
	return 100;
}

int BaseCelestia::Length()
{
	return _config->GetUpdateTime();
}

int BaseCelestia::ZIndex()
{
	return 25;
}

bool BaseCelestia::CanRun(TimeController* timeController)
{
	return timeController->IsDayTime();
}

void BaseCelestia::Render(DesktopSisterRgbaBytes* wallpaper, TimeController* timeController)
{
	if (!timeController)
		return;
	
	auto celestiaImage = _imageController->loadBaseDayImage("Celestia.png");

	auto celestiaHeight = min(celestiaImage->height, 350);
	auto ratio = double(celestiaHeight) / celestiaImage->height;
	auto celestWidth = int(celestiaImage->width * ratio);

	wallpaper->Blit(celestiaImage, SisterRect(0, 0, celestiaImage->width - 1, celestiaImage->height-1), SisterRect(0, 30, celestWidth, celestiaHeight), ScNearestNeighbor, 100);
}

Event* BaseCelestia::clone()
{
	return new BaseCelestia(_config, _imageController);
}