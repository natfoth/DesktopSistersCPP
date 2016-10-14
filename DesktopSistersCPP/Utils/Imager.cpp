#include "stdafx.h"
#include "Imager.h"
#include <minwindef.h>


DesktopSisterRgbaBytes::DesktopSisterRgbaBytes(int withWidth, int withHeight)
{
	width = withWidth;
	height = withHeight;

	mImageData = new SisterBytes[width * height];
}


DesktopSisterRgbaBytes::~DesktopSisterRgbaBytes()
{
	delete mImageData;
}

SisterBytes DesktopSisterRgbaBytes::GetPixel(int x, int y) const
{
	if (x >= 0 && x < width && y >= 0 && y < height)
	{

		SisterBytes * src = mImageData;
		src += (x + y * width);
		return *src;
	}

	return SisterBytes(0.0f, 0.0f, 0.0f, 0.0f);

}

void DesktopSisterRgbaBytes::GetPixel(SisterBytes& pixel, int x, int y) const
{
	if (x >= 0 && x < width && y >= 0 && y < height)
	{
		SisterBytes * src = mImageData;
		src += (x + y * width);
		pixel = *src;
	}
}

SisterBytes DesktopSisterRgbaBytes::GetInterpolatedPixel(float x, float y, ScImageResizeType tHow)
{
	x = FLimit(x, 0, float(width - 1));
	y = FLimit(y, 0, float(height - 1));

	if (tHow == ScNearestNeighbor)
	{
		SisterBytes* src = mImageData;
		src += (int(x) + int(y) * width);
		return *src;
	}
	else
	{
		int yi = int(y);
		yi = max(yi, 0);
		yi = min(yi, height - 1);
		float yFrac = y - float(yi);
		SisterBytes* row1 = static_cast<SisterBytes*>(GetRowPtr(yi));
		yi = yi + ((yi < height - 1) ? 1 : 0);
		yi = max(yi, 0);
		yi = min(yi, height - 1);
		SisterBytes* row2 = static_cast<SisterBytes*>(GetRowPtr(yi));

		int x1 = int(x);
		float xFrac = x - float(x1);
		int x2 = x1 + ((x1 < width - 1) ? 1 : 0);

		SisterBytes nativePixel11 = row1[x1];
		SisterBytes nativePixel12 = row1[x2];
		SisterBytes nativePixel21 = row2[x1];
		SisterBytes nativePixel22 = row2[x2];

		nativePixel11.Blend(nativePixel12, xFrac);
		nativePixel21.Blend(nativePixel22, xFrac);
		nativePixel11.Blend(nativePixel21, yFrac);

		return nativePixel11;
	}
}

void DesktopSisterRgbaBytes::PutPixel(SisterBytes srcPixel, int x, int y) const
{
	SisterBytes* src = mImageData;
	src += (x + y * width);
	*src = srcPixel;
}

void DesktopSisterRgbaBytes::Merge(DesktopSisterRgbaBytes* from, SisterPoint toIn) const
{
	for (auto y = 0; y < from->height - 1; y++)
	{
		if (y >= height - 1 || y + toIn.y >= (height - 1)) // dont copy anything if we are past the image
			continue;

		for (auto x = 0; x < from->width - 1; x++)
		{
			if (x >= width - 1 || x + toIn.x >= (width - 1)) // dont copy anything if we are past the image
				continue;

			auto src = from->GetPixel(x, y);
			if(src.alpha > 50)
			{
				auto dst = src;
				PutPixel(dst, int(x + toIn.x), int(y + toIn.y));
			}
		}
	}
}

void DesktopSisterRgbaBytes::Blit(DesktopSisterRgbaBytes* from, SisterRect fromRect, SisterPoint toIn)
{
	SisterPoint to(float(toIn.x), float(toIn.y));
	int xd = int(to.x - fromRect.left);
	int yd = int(to.y - fromRect.top);

	int x1 = int(to.x);
	int y1 = int(to.y);
	int x2 = int(min(min(fromRect.right + xd, this->width - 1), from->width + xd - 1));
	int y2 = int(min(min(fromRect.bottom + yd, this->height - 1), from->height + yd - 1));
	x1 = int(max(max(x1, 0), xd));
	y1 = int(max(max(y1, 0), yd));


	if (from->width == this->width && from->height == this->height &&
		x1 == 0 && y1 == 0 &&
		x2 == from->width - 1 && y2 == from->height - 1 &&
		to.x == 0 && to.y == 0)
	{
		Copy(from);

	}
	else
	{
		for (int y = y1; y <= y2; y++)
		{
			memcpy(GetRowPtr(y) + x1, from->GetRowPtr(y - yd) + x1 - xd, 4 * ((x2 - x1) + 1));
		}
	}
}

void DesktopSisterRgbaBytes::Blit(DesktopSisterRgbaBytes* from, SisterRect fromRect, SisterRect toRect, ScImageResizeType tHow, int alphaMin) const
{
	int xf;
	int yf;


	int x1 = int(max(toRect.left, 0));
	int y1 = int(max(toRect.top, 0));
	int x2 = int(min(toRect.right, this->width - 1));
	int y2 = int(min(toRect.bottom, this->height - 1));

	if (tHow == ScAuto)
	{
		if ((fromRect.right - fromRect.left) < (toRect.right - toRect.left))
			tHow = ScBilinear;
		else
			tHow = ScSupersampled;
	}


	if (tHow == ScNearestNeighbor)
	{
		float xScale = float(fromRect.Width()) / float(toRect.Width());
		float yScale = float(fromRect.Height()) / float(toRect.Height());

		for (int y = y1; y <= y2; y++)
		{
			int yFrom = int(fromRect.top + (y - toRect.top) * (yScale));
			for (int x = x1; x <= x2; x++)
			{
				//if (y == y2 && x==x2)
				//	short temp=2;

				int xFrom = int(fromRect.left + (x - toRect.left) * (xScale));
				SisterBytes rgbaPixel;
				rgbaPixel = from->GetPixel(xFrom, yFrom);
				if (rgbaPixel.alpha > alphaMin)
					this->PutPixel(rgbaPixel, x, y);
			}
		}

	}
	else if (tHow == ScBilinear || tHow == ScBicubic || tHow == ScC2)
	{
		auto xScale = float(fromRect.Width()) / float(toRect.Width());
		auto yScale = float(fromRect.Height()) / float(toRect.Height());

		for (auto y = y1; y <= y2; y++)
		{
			//if (y==y2)
			//	short temp = 2;
			auto yFrom = fromRect.top + (float(y) - toRect.top) * yScale;
			for (auto x = x1; x <= x2; x++)
			{
				auto xFrom = fromRect.left + (float(x) - toRect.left) * xScale;
				SisterBytes rgbaPixel;
				rgbaPixel = from->GetInterpolatedPixel(xFrom, yFrom, ScBilinear);
				if(rgbaPixel.alpha > alphaMin)
					this->PutPixel(rgbaPixel, x, y);
			}
		}

	}
	else if (tHow == ScSupersampled)
	{
		auto xfw = from->width;
		auto yfw = from->height;
		auto xScale = float(fromRect.Width()) / float(toRect.Width());
		auto yScale = float(fromRect.Height()) / float(toRect.Height());

		for (int y = y1; y <= y2; y++)
		{
			float yFrom1 = fromRect.top + float(y - toRect.top) * yScale;
			float yFrom2 = fromRect.top + float(y + 1 - toRect.top) * yScale;
			if (yFrom2 <= yFrom1) yFrom2 = yFrom1 + 1;
			if (yFrom2 > yfw) yFrom2 = float(yfw);
			for (int x = x1; x <= x2; x++)
			{
				float xFrom1 = fromRect.left + float(x - toRect.left) * xScale;
				float xFrom2 = fromRect.left + float(x + 1 - toRect.left) * xScale;
				if (xFrom2 <= xFrom1) xFrom2 = xFrom1 + 1;
				if (xFrom2 > xfw) xFrom2 = float(xfw);

				float redAccum = 0.0f;
				float greenAccum = 0.0f;
				float blueAccum = 0.0f;
				float alphaAccum = 0.0f;
				int count = 0;
				SisterBytes rgba;
				for (yf = int(yFrom1); yf<yFrom2; yf++)
				{
					for (xf = int(xFrom1); xf<xFrom2; xf++)
					{
						rgba = from->GetPixel(xf, yf);
						redAccum += rgba.red;
						greenAccum += rgba.green;
						blueAccum += rgba.blue;
						alphaAccum += rgba.alpha;
						count++;
					}
				}
				float invCount = 1.0f / float(count);
				rgba.red = static_cast<unsigned char>(redAccum * invCount);
				rgba.green = static_cast<unsigned char>(greenAccum * invCount);
				rgba.blue = static_cast<unsigned char>(blueAccum * invCount);
				rgba.alpha = static_cast<unsigned char>(alphaAccum * invCount);
				PutPixel(rgba, x, y);
			}
		}

	}
}

void DesktopSisterRgbaBytes::Copy(DesktopSisterRgbaBytes* from) const
{
	if (from->width == width && from->height == height)
	{
		SisterBytes* src = from->mImageData;
		SisterBytes* dst = mImageData;

		memcpy(dst, src, 4 * width * height);
	}
	else
	{
		for (int x = 0; x < from->width; ++x)
		{
			for (int y = 0; y < from->height; ++y)
			{
				SisterBytes fromPixel;
				fromPixel = from->GetPixel(x, y);
				PutPixel(fromPixel, x, y);


			}
		}
	}
}


DesktopSisterRgbaBytes* DesktopSisterRgbaBytes::Rotate180() const
{
	DesktopSisterRgbaBytes * outImage = new DesktopSisterRgbaBytes(width, height);


	for (int y = 0; y < height; ++y)
	{
		for (int x = 0; x < width; ++x)
		{
			SisterBytes pixel = GetPixel(x, y);

			outImage->PutPixel(pixel, (outImage->width - 1) - x, (outImage->height - 1) - y);

		}

	}

	return outImage;
}
