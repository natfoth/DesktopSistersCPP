#include "stdafx.h"
#include "ImageLoader.h"
#include "Imager.h"

#undef _UNICODE
#include "il.h"
#include <codecvt>
#include <algorithm>
#include <chrono>
#include <fstream>

#define _UNICODE
#define _X86_

#include <ppl.h> 
#include <filesystem>
#include <fileapi.h>


//#include <png.h>

/// <summary>
/// Loads in images from a resource using the DevIL library and converts it to our own format
/// </summary>
ImageLoader::ImageLoader()
{
}


ImageLoader::~ImageLoader()
{
}

/// <summary>
/// Saves the image to a file
/// </summary>
void ImageLoader::saveImage(DesktopSisterRgbaBytes * imageToSave, string filename)
{
	ILuint ImageName;
	ilGenImages(1, &ImageName);
	

	ilBindImage(ImageName);

	ILuint width, height;
	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);

	ilTexImage(imageToSave->width, imageToSave->height, 8, 4, IL_BGRA, IL_BYTE, nullptr);

	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);

	ILubyte * bytes = ilGetData();
	auto tgtPixel = &bytes[0];


	for (int y = 0; y < int(height); ++y)
	{
		auto srcPixel = imageToSave->GetRowPtr(y);


		for (int x = 0; x < int(width); ++x)
		{
			tgtPixel[0] = srcPixel->blue;
			tgtPixel[1] = srcPixel->green;
			tgtPixel[2] = srcPixel->red;
			tgtPixel[3] = srcPixel->alpha;

			tgtPixel += 4;
			++srcPixel;
		}
	}

	std::string s = filename;

	ilEnable(IL_FILE_OVERWRITE);
	ilSaveImage(s.c_str());

	ilDeleteImages(1, &ImageName);
}

/// <summary>
/// Merges 2 images together using an opacity
/// </summary>
/// <param name="dest">The destination image</param>
/// <param name="src">the source image</param>
/// <param name="opacity">the opacity to merge with between 0 - 1.0f</param>
/// <returns></returns>
SisterBytes ImageLoader::merge(SisterBytes dest, SisterBytes src, float opacity)
{
	int srcAlpha = static_cast<int>(src.red * opacity);

	if (srcAlpha <= 0)
		return dest;
	else if (srcAlpha == 255)
	{
		return src;
	}

	double newalpha = srcAlpha + (((255 - srcAlpha) * dest.red) / 255.0);// / 255.0;
	int alpha255 = static_cast<int>((dest.red * (255 - srcAlpha)) / 255.0);//  /255.0;

	newalpha = 1.0 / newalpha;

	auto blue = ((src.blue * srcAlpha + (dest.blue * alpha255)) * newalpha);
	auto green = ((src.green * srcAlpha + (dest.green * alpha255)) * newalpha);
	auto red = ((src.red * srcAlpha + (dest.red * alpha255)) * newalpha);

	SisterBytes newColor;
	newColor.blue = unsigned char(blue);
	newColor.green = unsigned char(green);
	newColor.red = unsigned char(red);
	newColor.alpha = 255;

	return newColor;
}

/// <summary>
/// Loads in an image from the Base Day Directory allowing it to switch between art styles
/// </summary>
/// <param name="filename">Filename of the file you want to load without a path</param>
/// <returns>returns the loaded image in our format</returns>
DesktopSisterRgbaBytes* ImageLoader::loadBaseDayImage(string filename)
{ 
	auto path = "Resources\\Day\\Traditional\\" + filename;

	auto cachedImage = getCachedImage(path);
	if (cachedImage != nullptr)
	{
		//printf(("loaded image from the cache : " + filename + "\n").c_str());
		return cachedImage->image;
	}

	
	auto loadedImage = loadImage(path);

	cacheList.push_back(new CachedImage(loadedImage, path));

	return loadedImage;
}

/// <summary>
/// Loads in an image from the Base Night Directory allowing it to switch between art styles
/// </summary>
/// <param name="filename">Filename of the file you want to load without a path</param>
/// <returns>returns the loaded image in our format</returns>
DesktopSisterRgbaBytes* ImageLoader::loadBaseNightImage(string filename)
{
	auto path = "Resources\\Night\\Traditional\\" + filename;

	auto cachedImage = getCachedImage(path);
	if (cachedImage != nullptr)
	{
		//printf(("loaded image from the cache : " + filename + "\n").c_str());
		return cachedImage->image;
	}

	
	auto loadedImage = loadImage(path);

	cacheList.push_back(new CachedImage(loadedImage, path));

	return loadedImage;
}

/// <summary>
/// Loads in a dynamic image from the other directories
/// </summary>
/// <param name="filename">Filename of the file you want to load without a path</param>
/// <returns>returns the loaded image in our format</returns>
DesktopSisterRgbaBytes* ImageLoader::loadImage(string filename)
{
	ilInit();
	
	// Loading an image
	ILboolean result = ilLoadImage(filename.c_str());

	if (result == 1)
	{
		//printf(("the image loaded successfully : " + filename + "\n").c_str());
	}
	else
	{
		printf(("the image failed to load : " + filename + "\n").c_str());

		ILenum err = ilGetError();
		printf("the error %d\n", err);
		printf("string is %s\n", ilGetString(err));
	}

	ILuint width, height;
	width = ilGetInteger(IL_IMAGE_WIDTH);
	height = ilGetInteger(IL_IMAGE_HEIGHT);

	DesktopSisterRgbaBytes * sistersImage = new DesktopSisterRgbaBytes(width, height);

	ILubyte * bytes = ilGetData();
	auto srcPixel = &bytes[0];

	

	for (int y = 0; y < int(height); ++y)
	{
		auto tgtPixel = sistersImage->GetRowPtr(y);
		

		for (int x = 0; x < int(width); ++x)
		{
			/*auto red = srcPixel[0];
			auto green = srcPixel[1];
			auto blue = srcPixel[2];*/
			auto alpha = srcPixel[3];


			if (alpha == 0)
			{
				tgtPixel->red = tgtPixel->green = tgtPixel->blue = tgtPixel->alpha = 0;
			}
			else if (alpha == 255)
			{
				tgtPixel->red = srcPixel[0];
				tgtPixel->green = srcPixel[1];
				tgtPixel->blue = srcPixel[2];
				tgtPixel->alpha = alpha;

			}
			else
			{
				tgtPixel->red = min(0xff, (srcPixel[0] * 0xff) / alpha);
				tgtPixel->green = min(0xff, (srcPixel[1] * 0xff) / alpha);
				tgtPixel->blue = min(0xff, (srcPixel[2] * 0xff) / alpha);
				tgtPixel->alpha = srcPixel[3];
			}

			++tgtPixel;
			srcPixel += 4;
		}
	}

	auto rotatedImage = sistersImage->Rotate180();
	rotatedImage->Flip(true);

	delete sistersImage;
	sistersImage = nullptr;

	ilShutDown();

	return rotatedImage;
}

/// <summary>
/// Attempts to find and return a cached version of the image we are looking for. Will return nullptr if not found.
/// </summary>
/// <param name="byFilename">Filename we are seeking in the cache</param>
/// <returns>Returns the image if it was found or nullptr if its not found</returns>
CachedImage* ImageLoader::getCachedImage(string byFilename)
{
	for (auto&& cachcedItem : cacheList)
	{
		if (cachcedItem->filename == byFilename)
			return cachcedItem;
	}

	return nullptr;
}