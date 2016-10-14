#include "stdafx.h"
#include "BaseNightBackground.h"
#include "ImageLoader.h"
#include <chrono>
#include <fstream>
#include "ImgProc.h"


BaseNightBackground::BaseNightBackground(Configuration * newConfig, ImageLoader * newImageController)
	: Event(newConfig, newImageController)
{

}


BaseNightBackground::~BaseNightBackground()
{
}

string BaseNightBackground::GetEventName() const
{
	return "BaseNightBackground";
}

vector<EventTags> BaseNightBackground::Tags()
{
	vector<EventTags> tags;

	tags.push_back(EventTags::Background);

	return tags;
}

bool BaseNightBackground::IsBaseEvent()
{
	return true;
}

double BaseNightBackground::Chance()
{
	return 100;
}

int BaseNightBackground::Length()
{
	return _config->GetUpdateTime();
}

int BaseNightBackground::ZIndex()
{
	return 0;
}

bool BaseNightBackground::CanRun(TimeController* timeController)
{
	return timeController->IsNightTime();
}

void BaseNightBackground::Render(DesktopSisterRgbaBytes* wallpaper, TimeController* timeController)
{
	if (!timeController)
		return;	

	auto innerCircle = 1000.0*_config->GetScreenWidth()*1.0 / 1680.0;

	SisterBytes horizonColor(77, 145, 157, 255);
	SisterBytes nightColor(114, 49, 187, 255);
	SisterBytes bleedColor(52, 50, 101, 255);
	SisterBytes backgroundColor(80, 77, 157, 255);

	auto moonX = timeController->moonX;
	auto moonY = _config->GetScreenHeight() - timeController->moonY;

	if(timeController->IsTwilight())
	{
		backgroundColor = ImgProc::BlendColor(SisterBytes(204, 102, 102, 255), backgroundColor, moonX / -6);
		horizonColor = ImgProc::BlendColor(SisterBytes(255, 200, 178, 255), horizonColor, moonX / -6);
	}

	auto bleedWidth = _config->GetScreenWidth() * 2 / 5;

	
	for (int y = 0; y < wallpaper->height-1; y++)
	{
		for (int x = 0; x < wallpaper->width - 1; x++)
		{
			auto src = wallpaper->GetPixel(x, y);
			auto dist = sqrt((moonX - x)*(moonX - x) + (moonY - y)*(moonY - y));

			auto Base = ImgProc::BlendColor(backgroundColor, horizonColor, float(y) / float(_config->GetScreenHeight()));
			auto Night = ImgProc::BlendColor(bleedColor, nightColor, double(max(x - _config->GetScreenWidth() + bleedWidth, 0)) / double(bleedWidth));
			auto BG = ImgProc::BlendColor(Base, Night, Night.red / 255.0f);

			auto color = BG;
			if (dist < innerCircle)
			{
				auto alphaToUse = dist / innerCircle;
				alphaToUse = (1.0 - alphaToUse)*255.0;

				color = ImgProc::BlendColor(BG, SisterBytes(255, 255, 255, 255), (alphaToUse / 255) * 0.2);
			}


			src.red = color.red;
			src.green = color.green;
			src.blue = color.blue;
			src.alpha = 255;

			wallpaper->PutPixel(src, x, y);
		}
	}
}

Event* BaseNightBackground::clone()
{
	return new BaseNightBackground(_config, _imageController);
}