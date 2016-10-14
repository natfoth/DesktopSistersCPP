#pragma once
#include "Imager.h"

/// <summary>
/// Contains Basic inline data for Image Processing Functions
/// </summary>

namespace ImgProc
{
	/// <summary>
	/// Blends 2 Colors Together using a ratio
	/// </summary>
	/// <param name="firstColor">The first color</param>
	/// <param name="secondColor">The Second Color</param>
	/// <param name="ratio">The Ratio between the 2 colors. 0 is all the first color, 1.0f is all the second color, 0.5f is both colors evenly</param>
	/// <returns></returns>
	inline SisterBytes BlendColor(SisterBytes firstColor, SisterBytes secondColor, double ratio)
	{
		double revRatio = 1.0f - ratio;

		auto red = byte(firstColor.red * revRatio + secondColor.red * ratio);
		auto green = byte(firstColor.green * revRatio + secondColor.green * ratio);
		auto blue = byte(firstColor.blue * revRatio + secondColor.blue * ratio);
		auto alpha = byte(firstColor.alpha * revRatio + secondColor.alpha * ratio);

		return SisterBytes(red, green, blue, alpha);
	}
}

