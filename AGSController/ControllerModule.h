#ifndef CONTROLLERMODULE_H
#define CONTROLLERMODULE_H

#include <SDL.h>

#include "plugin/agsplugin.h"

struct Controller
{
	int32 id;
	int32 button_count;
	int32 axes_count;
	int32 pov;
	int buttstate[32];
	int axes[16];
	bool isHeld[32];
};


class ControllerModule
{
protected:
	IAGSEngine* m_engine;
public:
	ControllerModule(IAGSEngine* engine);
	virtual int ControllerCount() = 0;
	virtual Controller* Open(int num) = 0;
	virtual void Close(Controller* controller) = 0;
	virtual void Update() = 0;
	virtual int Plugged(Controller* controller) = 0;
	virtual int GetAxis(Controller* controller, int axis) = 0;
	virtual int GetPOV(Controller* controller) = 0;
	virtual int IsButtonDown(Controller* controller, int button) = 0;
	virtual const char* GetName(Controller* controller) = 0;
	virtual void Rumble(Controller* controller, int left, int right, int duration) = 0;
	virtual int IsButtonDownOnce(Controller* controller, int button) = 0;
	virtual int BatteryStatus(Controller* controller) = 0;
	virtual int PressAnyKey(Controller* controller) = 0;
	virtual void ClickMouse(int button) = 0;
};

#endif