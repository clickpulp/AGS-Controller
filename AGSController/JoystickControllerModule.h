#ifndef JOYSTICKCONTROLLERMODULE_H
#define JOYSTICKCONTROLLERMODULE_H

#include "ControllerModule.h"

class ControllerScriptManagedObject : public IAGSScriptManagedObject {
public:
	// when a ref count reaches 0, this is called with the address
	// of the object. Return 1 to remove the object from memory, 0 to
	// leave it
	virtual int Dispose(const char* address, bool force);
	// return the type name of the object
	virtual const char* GetType();
	// serialize the object into BUFFER (which is BUFSIZE bytes)
	// return number of bytes used
	virtual int Serialize(const char* address, char* buffer, int bufsize);
};

class ControllerManagedObjectReader : public IAGSManagedObjectReader {
	IAGSEngine* m_engine;
	Controller* m_controller;
	IAGSScriptManagedObject* m_scriptManagedObject;
public:
	ControllerManagedObjectReader(IAGSEngine* engine, Controller* controller, IAGSScriptManagedObject* scriptManagedObject);
	virtual void Unserialize(int key, const char* serializedData, int dataSize);
};

class JoystickControllerModule : public ControllerModule
{
	Controller m_controllerInAGS;
	SDL_Joystick* m_sdlJoystick;
	SDL_GameController* m_sdlGameController;
	ControllerManagedObjectReader* m_managedObjectReader;
	ControllerScriptManagedObject* m_scriptManagedObject;
	bool m_isGamepad = false;

public:
	JoystickControllerModule(IAGSEngine* engine);

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