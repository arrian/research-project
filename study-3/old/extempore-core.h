#pragma once

#include "PolyCocoaCore.h"

namespace Polycode
{

	class ExtemporeCore : public CocoaCore
	{
	public:
		ExtemporeCore(PolycodeView* view, int xRes = 640, int yRes = 480, bool fullScreen = false, bool vSync = false, int aaLevel = 0, int anisotropyLevel = 0, int frameRate = 60, int monitorIndex = -1, bool retinaSupport = false);


	};
}