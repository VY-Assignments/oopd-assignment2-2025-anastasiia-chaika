#pragma once
#include "GameEngine.h"

class IRenderer {
public:
	virtual ~IRenderer() = default;
	virtual void mainLoop() = 0;
};
