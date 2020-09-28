#pragma once

#include <AmberEngine/API/Export.h>

#define FORCE_DEDICATED_GPU \
extern "C" \
{ \
	API_AMBERENGINE unsigned long NvOptimusEnablement = 0x00000001;\
}