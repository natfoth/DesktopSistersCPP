
#pragma once

#include "Imager.h"
#include <vector>

using namespace std;

/// <summary>
/// Class to handle self caching of images
/// </summary>
class CachedImage
{
public:
	CachedImage(DesktopSisterRgbaBytes * newImage, string newFilename)
	{
		image = newImage;
		filename = newFilename;
	}

	~CachedImage()
	{

	}

	string filename;
	DesktopSisterRgbaBytes * image;
};

/// <summary>
/// Loads in images from a resource using the DevIL library and converts it to our own format
/// </summary>
class ImageLoader
{
public:
	ImageLoader();
	~ImageLoader();

	/// <summary>
	/// Our list of cached images
	/// </summary>
	vector<CachedImage*> cacheList;

	/// <summary>
	/// Saves the image to a file
	/// </summary>
	static void saveImage(DesktopSisterRgbaBytes* imageToSave, string filename);

	/// <summary>
	/// Merges 2 images together using an opacity
	/// </summary>
	/// <param name="dest">The destination image</param>
	/// <param name="src">the source image</param>
	/// <param name="opacity">the opacity to merge with between 0 - 1.0f</param>
	/// <returns></returns>
	static SisterBytes merge(SisterBytes dest, SisterBytes src, float opacity);

	/// <summary>
	/// Loads in an image from the Base Day Directory allowing it to switch between art styles
	/// </summary>
	/// <param name="filename">Filename of the file you want to load without a path</param>
	/// <returns>returns the loaded image in our format</returns>
	DesktopSisterRgbaBytes* loadBaseDayImage(string filename);

	/// <summary>
	/// Loads in an image from the Base Night Directory allowing it to switch between art styles
	/// </summary>
	/// <param name="filename">Filename of the file you want to load without a path</param>
	/// <returns>returns the loaded image in our format</returns>
	DesktopSisterRgbaBytes* loadBaseNightImage(string filename);

	/// <summary>
	/// Loads in a dynamic image from the other directories
	/// </summary>
	/// <param name="filename">Filename of the file you want to load without a path</param>
	/// <returns>returns the loaded image in our format</returns>
	DesktopSisterRgbaBytes * loadImage(string filename);

private:
	/// <summary>
	/// Attempts to find and return a cached version of the image we are looking for. Will return nullptr if not found.
	/// </summary>
	/// <param name="byFilename">Filename we are seeking in the cache</param>
	/// <returns>Returns the image if it was found or nullptr if its not found</returns>
	CachedImage* getCachedImage(string byFilename);
};

