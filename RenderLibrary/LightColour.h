#ifndef LIGHTCOLOUR_H
#define LIGHTCOLOUR_H

namespace RenderLibrary
{
	namespace Light
	{
		class LightColour
		{
		public:
			LightColour();
			LightColour(int red, int green, int blue, int alpha);
			~LightColour();

			int _red, _green, _blue, _alpha;
		};
	}
}

#endif // LIGHTCOLOUR_H