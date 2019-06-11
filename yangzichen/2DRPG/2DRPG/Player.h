#pragma once
#include "node.h"
#include "DInput.h"


class Player : public Node, public InputEventListener
{
public:
	Player() {}
	~Player() {}

	virtual void update(float dt);
	virtual void draw();

	virtual void InputEvent(DInput *n, float dt);
};

