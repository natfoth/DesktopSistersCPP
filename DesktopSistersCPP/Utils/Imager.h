#pragma once
#include "stdafx.h"

struct SisterRect;

enum ScImageResizeType
{
	ScNearestNeighbor = 0,
	ScBilinear = 1,
	ScBicubic = 2,
	ScSupersampled = 3,
	ScAuto = 4,	//takes into account if it is up it will do bilinear and down will be supersampled
	ScC2 = 5
};

struct  SisterPoint
{
	float x;
	float y;

	SisterPoint(): x(0), y(0)
	{
	}

	SisterPoint(int xIn, int yIn) : x(float(xIn)), y(float(yIn)) {}
	SisterPoint(float xIn, float yIn) : x(xIn), y(yIn) {}
	SisterPoint(double xIn, double yIn) : x(float(xIn)), y(float(yIn)) {}
	SisterPoint(const SisterPoint &pt) : x(float(pt.x)), y(float(pt.y)) {}
};

struct SisterBytes
{
	unsigned char alpha;
	unsigned char red;
	unsigned char green;
	unsigned char blue;

	SisterBytes()
	{
		alpha = red = green = blue = 0;
	}

	SisterBytes(float redIn, float greenIn, float blueIn, float alphaIn)
	{
		red = unsigned char(redIn * 255);
		green = unsigned char(greenIn * 255);
		blue = unsigned char(blueIn * 255);
		alpha = unsigned char(alphaIn * 255);
	}

	SisterBytes(unsigned char redIn, unsigned char greenIn, unsigned char blueIn, unsigned char alphaIn)
	{
		red = redIn;
		green = greenIn;
		blue = blueIn;
		alpha = alphaIn;

	}

	SisterBytes(int redIn, int greenIn, int blueIn, int alphaIn)
	{
		red = unsigned char(redIn);
		green = unsigned char(greenIn);
		blue = unsigned char(blueIn);
		alpha = unsigned char(alphaIn);

	}

	void Blend(SisterBytes from, float amount)
	{

		float ratio = amount;
		float revRatio = 1.0f - ratio;

		red = Limit255(red*revRatio + from.red*ratio);
		green = Limit255(green*revRatio + from.green*ratio);
		blue = Limit255(blue*revRatio + from.blue*ratio);
		alpha = Limit255(alpha*revRatio + from.alpha*ratio);

	}

	static inline unsigned char Limit255(int val1)
	{

		if (val1 < 0)
			return 0;
		if (val1 > 255)
			return 255;

		return static_cast<unsigned char>(val1);
	}

	static inline unsigned char Limit255(float val1)
	{

		if (val1 < 0)
			return 0;
		if (val1 > 255.0f)
			return unsigned char(255.0f);

		return unsigned char(val1);
	}


};


class DesktopSisterRgbaBytes
{
public:
	DesktopSisterRgbaBytes(int withWidth, int withHeight);
	~DesktopSisterRgbaBytes();

	SisterBytes* mImageData;
	//actual pixels..will just point to mBits...

	int width;
	int height;

	SisterBytes* DesktopSisterRgbaBytes::GetRowPtr(int y)
	{
		SisterBytes* src = mImageData;
		src += (y * width);

		return src;
	}

	SisterBytes GetPixel(int x, int y) const;
	void GetPixel(SisterBytes & pixel, int x, int y) const;
	SisterBytes GetInterpolatedPixel(float x, float y, ScImageResizeType tHow);
	void PutPixel(SisterBytes srcPixel, int x, int y) const;

	void Merge(DesktopSisterRgbaBytes * from, SisterPoint toIn) const;
	void Blit(DesktopSisterRgbaBytes * from, SisterRect fromRect, SisterPoint toIn);
	void Blit(DesktopSisterRgbaBytes * from, SisterRect fromRect, SisterRect toRect, ScImageResizeType tHow, int alphaMin) const;
	void Copy(DesktopSisterRgbaBytes  * from) const;

	DesktopSisterRgbaBytes * Rotate180() const;

	void Flip(bool horizontal)
	{
		if (horizontal)
		{
			int mid = width / 2;


			for (int y = 0; y < height; ++y)
			{
				SisterBytes * tgtPixel = GetRowPtr(y);
				SisterBytes * srcPixel = GetRowPtr(y) + (width - 1);

				for (int x = 0; x < mid; ++x)
				{
					SisterBytes temp = *tgtPixel;
					*tgtPixel = *srcPixel;
					*srcPixel = temp;

					++tgtPixel;
					--srcPixel;
				}
			}
		}
		else
		{
			int mid = height / 2;

			for (int y = 0; y < mid; ++y)
			{
				SisterBytes * tgtPixel = GetRowPtr(y);
				SisterBytes * srcPixel = GetRowPtr((height - 1) - y);

				for (int x = 0; x < width; ++x)
				{
					SisterBytes temp = *tgtPixel;
					*tgtPixel = *srcPixel;
					*srcPixel = temp;

					++tgtPixel;
					++srcPixel;
				}
			}


		}
	}

	inline int Limit(int val1, int val2, int val3)
	{

		if (val1 < val2)
			return val2;
		if (val1 > val3)
			return val3;

		return val1;
	}


	inline unsigned char Limit255(int val1)
	{

		if (val1 < 0)
			return 0;
		if (val1 > 255)
			return 255;

		return static_cast<unsigned char>(val1);
	}

	inline float FLimit(float val1, float val2, float val3)
	{

		if (val1 < val2)
			return val2;
		if (val1 > val3)
			return val3;

		return val1;
	}
};



struct SisterRect
{
	float left;
	float top;
	float right;
	float bottom;

	static const SisterRect emptyRect;

	SisterRect()
	{
		left = top = 50000;
		right = bottom = -50000;


	}

	SisterRect(float withLeft, float withTop, float withRight, float withBottom)
	{
		left = withLeft;
		right = withRight;
		top = withTop;
		bottom = withBottom;
	}

	SisterRect(int withLeft, int withTop, int withRight, int withBottom)
	{
		left = float(withLeft);
		right = float(withRight);
		top = float(withTop);
		bottom = float(withBottom);
	}

	SisterRect(const SisterRect& inRect)
	{
		left = inRect.left;
		right = inRect.right;
		top = inRect.top;
		bottom = inRect.bottom;
	}

	bool operator==(const SisterRect& other)
	{
		if (left == other.left && right == other.right && top == other.top && bottom == other.bottom)
			return true;


		return false;

	}

	bool operator!=(const SisterRect& other)
	{
		return !(*this == other);
	}

	SisterRect&	operator /= (const SisterRect& inRect)
	{
		left /= inRect.left;
		top /= inRect.top;
		right /= inRect.right;
		bottom /= inRect.bottom;

		return(*this);
	}

	SisterRect&	operator /= (const float divisor)
	{
		left /= divisor;
		top /= divisor;
		right /= divisor;
		bottom /= divisor;

		return(*this);
	}

	SisterRect& operator *= (const SisterRect& inRect)
	{
		left *= inRect.left;
		top *= inRect.top;
		right *= inRect.right;
		bottom *= inRect.bottom;

		return(*this);
	}

	SisterRect& operator *= (const float multiplier)
	{
		left *= multiplier;
		top *= multiplier;
		right *= multiplier;
		bottom *= multiplier;

		return(*this);
	}


	float FMin(float a, float b)
	{
		return a < b ? a : b;
	}

	float FMax(float a, float b)
	{
		return a > b ? a : b;
	}


	static int IMax(int a, int b)
	{
		return a > b ? a : b;
	}


	static int IMin(int a, int b)
	{
		return a > b ? b : a;
	}

	static int IMax(float a, float b)
	{
		return a > b ? int(a) : int(b);
	}


	static int IMin(float a, float b)
	{
		return a > b ? int(b) : int(a);
	}


	bool Contains(SisterRect withRect)
	{
		if (IsEmpty())
			return false;


		return(withRect.left >= left && withRect.right <= right &&
			withRect.top >= top && withRect.bottom <= bottom);


	}

	bool Overlaps(SisterRect &inRect)
	{
		if (IsEmpty())
			return false;

		if (right >= left && bottom >= top && inRect.right >= inRect.left && inRect.bottom >= inRect.top)
		{
			if ((IMax(top, inRect.top) <= IMin(bottom, inRect.bottom)) &&
				(IMax(left, inRect.left) <= IMin(right, inRect.right)))
			{
				return true;
			}
		}
		return false;
	}



	bool IsValid()
	{
		if (right < left)
			return false;
		if (bottom < top)
			return false;

		return true;
	}



	bool IsEmpty()
	{
		if (*this == SisterRect::emptyRect || (*this == SisterRect(0, 0, 0, 0)))
			return true;

		return false;
	}

	void ForceInt()
	{
		left = float(static_cast<int>(left));
		right = float(static_cast<int>(right));
		top = float(static_cast<int>(top));
		bottom = float(static_cast<int>(bottom));
	}

	void Constrain(SisterRect constrainToRect)
	{
		if (IsEmpty())
		{
			*this = constrainToRect;
			return;
		}


		left = FMax(left, constrainToRect.left);
		left = FMin(left, constrainToRect.right);

		right = FMin(right, constrainToRect.right);
		right = FMax(right, constrainToRect.left);

		top = FMax(top, constrainToRect.top);
		top = FMin(top, constrainToRect.bottom);

		bottom = FMin(bottom, constrainToRect.bottom);
		bottom = FMax(bottom, constrainToRect.top);
	}

	void Expand(float amount)
	{
		if (IsEmpty())
			return;

		left -= amount;
		right += amount;
		top -= amount;
		bottom += amount;
	}

	void Retract(float amount)
	{
		if (IsEmpty())
			return;

		left += amount;
		right -= amount;
		top += amount;
		bottom -= amount;
	}

	void Offset(float xoffset, float yoffset)
	{
		if (IsEmpty())
			return;

		left += xoffset;
		right += xoffset;
		top += yoffset;
		bottom += yoffset;



	}


	void Include(SisterRect includeRect)
	{
		if (left == 50000 && top == 50000 && right == -50000 && bottom == -50000)
		{
			*this = includeRect;
		}
		else {
			left = FMin(includeRect.left, left);
			right = FMax(includeRect.right, right);
			top = FMin(includeRect.top, top);
			bottom = FMax(includeRect.bottom, bottom);
		}
	}

	int Width() const
	{
		return int((right - left) + 1);
		//our rect is inclusive of the right and bottom
	}

	int Height() const
	{
		return int((bottom - top) + 1);
		//our rect is inclusive of the right and bottom
	}

	//resizes us to fit inside tgt and retains aspect ratio
	void FitToAspect(SisterRect tgt)
	{
		int w1 = int(right - left);
		int h1 = int(bottom - top);
		int w2 = int(tgt.right - tgt.left);
		int h2 = int(tgt.bottom - tgt.top);

		if (w1 == 0 || h1 == 0)
			return;

		int vfit = w2 * h1 / w1;
		int hfit = w2;
		if (vfit > h2)
		{
			// Vertical fit
			vfit = h2;
			hfit = h2 * w1 / h1;
		}

		left = float((w2 - hfit) / 2);
		right = float(left + hfit);
		top = float((h2 - vfit) / 2);
		bottom = float(top + vfit);



	}

	void FitToAspectFloat(SisterRect tgt)
	{
		float w1 = right - left;
		float h1 = bottom - top;
		float w2 = tgt.right - tgt.left;
		float h2 = tgt.bottom - tgt.top;

		if (w1 == 0 || h1 == 0)
			return;

		float vfit = w2 * h1 / w1;
		float hfit = w2;
		if (vfit > h2)
		{
			// Vertical fit
			vfit = h2;
			hfit = h2 * w1 / h1;
		}
		left = (w2 - hfit) / 2;
		right = left + hfit;
		top = (h2 - vfit) / 2;
		bottom = top + vfit;



	}


	//scales us to fill the tgt and then we are clipped for areas outside the tgt
	void ScaleToFill(SisterRect tgt)
	{

		double w1 = Width();
		double h1 = Height();
		double w2 = tgt.Width();
		double h2 = tgt.Height();

		if (w1 == 0 || h1 == 0)
			return;

		double srcAsp = w1 / h1;
		double newWidth, newHeight;

		newWidth = w2;
		newHeight = w2 / srcAsp;

		if (newWidth < w2 || newHeight < h2)
		{
			newHeight = h2;
			newWidth = h2 * srcAsp;
		}



		double difWidth = (newWidth - tgt.Width()) * 0.5f;
		double difHeight = (newHeight - tgt.Height()) * 0.5f;

		SisterRect newRect = SisterRect(float(tgt.left - difWidth), float(tgt.top - difHeight), float(tgt.left - difWidth + newWidth - 1), float(tgt.top - difHeight + newHeight - 1));

		*this = newRect;

	}

};