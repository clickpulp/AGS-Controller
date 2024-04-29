#ifndef GAMECONTROLLERMODULE_H
#define GAMECONTROLLERMODULE_H

#include "ControllerModule.h"


class GameControllerModule : public ControllerModule
{
	Controller m_controllerInAGS;
	SDL_Joystick* m_sdlJoystick;
	SDL_GameController* m_sdlGameController;
	ControllerManagedObjectReader* m_managedObjectReader;
	ControllerScriptManagedObject* m_scriptManagedObject;
	bool m_isGamepad = false;
	bool m_supportsHat = false;

public:
	GameControllerModule(IAGSEngine* engine);

	void Update() override;
	int ControllerCount() override;
	Controller* Open(int num) override;
	int Plugged(Controller* controller) override;
	int GetAxis(Controller* controller, int axis) override;
	int GetPOV(Controller* controller) override;
	int IsButtonDown(Controller* controller, int button) override;
	void Close(Controller* controller) override;
	const char* GetName(Controller* controller) override;
	void Rumble(Controller* controller, int left, int right, int duration) override;
	int IsButtonDownOnce(Controller* controller, int button) override;
	int BatteryStatus(Controller* controller) override;
	int PressAnyKey(Controller* controller) override;
	void ClickMouse(int button) override;
};

#endif