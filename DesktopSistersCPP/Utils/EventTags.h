#pragma once

/// <summary>
/// This precompiled struct contains our event tags.
/// We do it like this to avoid namespace bloat so we can call it like ( EventTags::Moon )
/// </summary>
struct EventTags
{
	enum Tags
	{
		Filter,

		//Background
		Background,
		Landscape,
		Sun,
		Moon,
		Stars,
		Clouds,
		SkyEvent,

		//Ponies
		Celestia,
		Luna,
		Twilight,
		Fluttershy,
		RainbowDash,
		Applejack,
		Rarity,
		PinkiePie,


		Alicorn,
	};
	Tags t_;
	EventTags(Tags t) : t_(t) {}
	operator Tags () const { return t_; }

	//prevent automatic conversion for any other built-in types such as bool, int, etc
	template<typename T>
	operator T () const;
};
