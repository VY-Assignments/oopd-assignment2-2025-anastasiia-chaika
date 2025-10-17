#pragma once
#include "Object.h"

class Tank : public Object {
public:
	int get_hp();
protected:
	int hp=100;
};