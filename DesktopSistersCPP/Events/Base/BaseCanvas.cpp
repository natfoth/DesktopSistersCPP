#include "stdafx.h"
#include "BaseCanvas.h"
#include "ImageLoader.h"
#include <chrono>
#include <fstream>


BaseCanvas::BaseCanvas(Configuration * newConfig, ImageLoader * newImageController)
	: Event(newConfig, newImageController)
{

}


BaseCanvas::~BaseCanvas()
{
}

vector<EventTags> BaseCanvas::Tags()
{
	vector<EventTags> tags;

	tags.push_back(EventTags::Filter);

	return tags;
}

bool BaseCanvas::IsBaseEvent()
{
	return true;
}

double BaseCanvas::Chance()
{
	return 100;
}

int BaseCanvas::Length()
{
	return _config->GetUpdateTime();
}

int BaseCanvas::ZIndex()
{
	return 31;
}

void BaseCanvas::Render(DesktopSisterRgbaBytes* wallpaper, TimeController* timeController)
{
	if (!timeController)
		return;

	auto landscapeImage = _imageController->loadBaseDayImage("Landscape.png");

	auto height = max(wallpaper->height - 1, landscapeImage->height - 1);
	wallpaper->Blit(landscapeImage, SisterRect(0, 0, landscapeImage->width - 1, landscapeImage->height - 1), SisterRect(0, 0, wallpaper->width - 1, height), ScNearestNeighbor, 100);
}

Event* BaseCanvas::clone()
{
	return new BaseCanvas(_config, _imageController);
}