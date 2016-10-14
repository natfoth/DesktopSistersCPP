#include "stdafx.h"
#include "BaseDayBackground.h"
#include "ImageLoader.h"
#include <chrono>
#include <fstream>
#include "ImgProc.h"
#include <ppl.h>

using namespace concurrency;

BaseDayBackground::BaseDayBackground(Configuration * newConfig, ImageLoader * newImageController)
	: Event(newConfig, newImageController)
{

}


BaseDayBackground::~BaseDayBackground()
{
}

string BaseDayBackground::GetEventName() const
{
	return "BaseDayBackground";
}

vector<EventTags> BaseDayBackground::Tags()
{
	vector<EventTags> tags;

	tags.push_back(EventTags::Background);

	return tags;
}

bool BaseDayBackground::IsBaseEvent()
{
	return true;
}

double BaseDayBackground::Chance()
{
	return 100;
}

int BaseDayBackground::Length()
{
	return _config->GetUpdateTime();
}

int BaseDayBackground::ZIndex()
{
	return 0;
}

bool BaseDayBackground::CanRun(TimeController* timeController)
{
	return timeController->IsDayTime();
}

void BaseDayBackground::Render(DesktopSisterRgbaBytes* wallpaper, TimeController* timeController)
{
	if (!timeController)
		return;
	

	auto innerCircle = 1000.0*_config->GetScreenWidth()*1.0 / 1680.0;

	auto SunX = timeController->sunX;
	auto SunY = timeController->sunY;

	SisterBytes startColor(255, 243, 102, 255);
	SisterBytes innerColor(255, 166, 124, 255);

	parallel_for(size_t(0), size_t(wallpaper->height - 1), [&](size_t y)
	{
		parallel_for(size_t(0), size_t(wallpaper->width - 1), [&](size_t x)
		{
			auto src = wallpaper->GetPixel(x, y);
			auto dist = sqrt((SunX - x)*(SunX - x) + (SunY - y)*(SunY - y));

			if (dist < innerCircle)
			{
				auto alphaToUse = dist / innerCircle;
				alphaToUse = (1.0 - alphaToUse);

				// BlendColor(inner_color, start_color, 1.0);
				auto color = ImgProc::BlendColor(innerColor, startColor, alphaToUse);

				src.red = color.red;
				src.green = color.green;
				src.blue = color.blue;
				src.alpha = 255;
			}
			else
			{
				src.red = innerColor.red;
				src.green = innerColor.green;
				src.blue = innerColor.blue;
				src.alpha = 255;
			}

			wallpaper->PutPixel(src, x, y);
		});
	});
}

Event* BaseDayBackground::clone()
{
	return new BaseDayBackground(_config, _imageController);
}