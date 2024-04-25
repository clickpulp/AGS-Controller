#include "GameControllerModule.h"

Controller dummyController2;

GameControllerModule::GameControllerModule(IAGSEngine* engine)
	: ControllerModule(engine)
{
	m_sdlJoystick = nullptr;
	m_sdlGameController = nullptr;
	m_scriptManagedObject = new ControllerScriptManagedObject();
	m_managedObjectReader = new ControllerManagedObjectReader(engine, &m_controllerInAGS, m_scriptManagedObject);

	SDL_InitSubSystem(SDL_INIT_GAMECONTROLLER);

	engine->AddManagedObjectReader(m_scriptManagedObject->GetType(), m_managedObjectReader);
}

int GameControllerModule::ControllerCount()
{
	return SDL_NumJoysticks();
}

void GameControllerModule::Update()
{
	if (m_sdlGameController != NULL)
	{
		int up = SDL_GameControllerGetButton(m_sdlGameController, SDL_CONTROLLER_BUTTON_DPAD_UP);
		int down = SDL_GameControllerGetButton(m_sdlGameController, SDL_CONTROLLER_BUTTON_DPAD_DOWN);
		int left = SDL_GameControllerGetButton(m_sdlGameController, SDL_CONTROLLER_BUTTON_DPAD_LEFT);
		int right = SDL_GameControllerGetButton(m_sdlGameController, SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

		if (up == 1)
		{
			if (left == 1) m_controllerInAGS.pov = 0x2 ^ 11;
			else if (right == 1) m_controllerInAGS.pov = 0x2 ^ 1;
			else m_controllerInAGS.pov = 0x2 ^ 3;
		}
		else if (down == 1)
		{
			if (left == 1) m_controllerInAGS.pov = 0x2 ^ 14;
			else if (right == 1) m_controllerInAGS.pov = 0x2 ^ 4;
			else m_controllerInAGS.pov = 0x2 ^ 6;
		}
		else
		{
			if (left == 1) m_controllerInAGS.pov = 0x2 ^ 10;
			else if (right == 1) m_controllerInAGS.pov = 0x2 ^ 0;
			else m_controllerInAGS.pov = 0;
		}

		bool LeftTrigger = !(SDL_GameControllerGetAxis(m_sdlGameController, SDL_CONTROLLER_AXIS_TRIGGERLEFT) < (abs(32768) - 1000));
		bool RightTrigger = !(SDL_GameControllerGetAxis(m_sdlGameController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) < (abs(32768) - 1000));

		int getButtons = m_controllerInAGS.button_count - 1;

		m_controllerInAGS.buttstate[getButtons + 1] = LeftTrigger;
		m_controllerInAGS.buttstate[getButtons + 2] = RightTrigger;
	}
}

Controller* GameControllerModule::Open(int num)
{
	Controller* con;
	int ax;
	int b;

	if (num == -1)
	{
		con = &dummyController2;
	}
	else
	{
		m_sdlJoystick = SDL_JoystickOpen(num);

		if (SDL_IsGameController(num))
		{
			m_sdlGameController = SDL_GameControllerOpen(num);
			m_isGamepad = true;
		}

		m_controllerInAGS.button_count = SDL_JoystickNumButtons(m_sdlJoystick);
		m_controllerInAGS.axes_count = SDL_JoystickNumAxes(m_sdlJoystick);

		for (ax = 0; ax < 16; ax = ax + 1)
		{
			m_controllerInAGS.axes[ax] = 0;
		}

		for (b = 0; b < 32; b = b + 1)
		{
			m_controllerInAGS.buttstate[b] = SDL_RELEASED;
		}

		int AMAXINT = 0;//131072;

		m_controllerInAGS.id = num;
		m_controllerInAGS.pov = AMAXINT;

		con = &m_controllerInAGS;
	}
	m_engine->RegisterManagedObject(con, m_scriptManagedObject);


	return con;
}

void GameControllerModule::Close(Controller* controller)
{
	if (m_sdlGameController)
	{
		SDL_GameControllerClose(m_sdlGameController);
	}
	if (m_sdlJoystick) {
		SDL_JoystickClose(m_sdlJoystick);
	}
}

int GameControllerModule::Plugged(Controller* controller)
{
	SDL_GameControllerUpdate();
	if (!m_sdlGameController)
	{
		return 0;
	}
	else
	{
		return SDL_GameControllerGetAttached(m_sdlGameController);
	}
}

int GameControllerModule::GetAxis(Controller* controller, int axis)
{
	if (axis > controller->axes_count || axis < 0 || !m_sdlGameController) { return 0; }
	SDL_GameControllerUpdate();
	m_controllerInAGS.axes[axis] = SDL_GameControllerGetAxis(m_sdlGameController, static_cast<SDL_GameControllerAxis>(axis));
	
	return m_controllerInAGS.axes[axis];
}

int GameControllerModule::GetPOV(Controller* controller)
{
	if (!m_sdlJoystick)
	{
		return -1;
	}

	SDL_JoystickUpdate();
	int setHat = SDL_JoystickGetHat(m_sdlJoystick, 0);

	if (setHat == SDL_HAT_CENTERED)	m_controllerInAGS.pov = 0;
	else if (setHat == SDL_HAT_DOWN) m_controllerInAGS.pov = 0x2 ^ 6;
	else if (setHat == SDL_HAT_LEFT) m_controllerInAGS.pov = 0x2 ^ 10;
	else if (setHat == SDL_HAT_RIGHT) m_controllerInAGS.pov = 0x2 ^ 0;
	else if (setHat == SDL_HAT_UP) m_controllerInAGS.pov = 0x2 ^ 3;
	else if (setHat == SDL_HAT_LEFTDOWN)m_controllerInAGS.pov = 0x2 ^ 14;
	else if (setHat == SDL_HAT_RIGHTDOWN)m_controllerInAGS.pov = 0x2 ^ 4;
	else if (setHat == SDL_HAT_LEFTUP)m_controllerInAGS.pov = 0x2 ^ 11;
	else if (setHat == SDL_HAT_RIGHTUP)m_controllerInAGS.pov = 0x2 ^ 1;

	return m_controllerInAGS.pov;
}

int GameControllerModule::IsButtonDown(Controller* controller, int button)
{
	if (button > SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_MAX ||
		button <= SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_INVALID ||
		!m_sdlGameController)
	{
		return 0;
	}

	SDL_GameControllerUpdate();

	if (button < m_controllerInAGS.button_count)
	{
		m_controllerInAGS.buttstate[button] = SDL_GameControllerGetButton(m_sdlGameController, static_cast<SDL_GameControllerButton>(button));

	}

	return m_controllerInAGS.buttstate[button];
}

const char* GameControllerModule::GetName(Controller* controller)
{
	if (!Plugged(controller)) {
		return m_engine->CreateScriptString("disconnected");
	}
	else if (m_isGamepad && SDL_GameControllerName(m_sdlGameController) != NULL) {
		return m_engine->CreateScriptString(SDL_GameControllerName(m_sdlGameController));
	}
	else {
		return m_engine->CreateScriptString("");
	}
}

void GameControllerModule::Rumble(Controller* controller, int left, int right, int duration)
{
	if (m_sdlJoystick)
	{
		duration = (duration / 40) * 1000; // TODO: Depends on the game speed
		int maxFq = 65535; // TODO: Should be const or definition
		SDL_GameControllerRumble(m_sdlGameController, clamp(left, 0, maxFq), clamp(right, 0, maxFq), duration);
	}
}

int GameControllerModule::IsButtonDownOnce(Controller* controller, int button)
{
	if (button > SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_MAX ||
		button <= SDL_GameControllerButton::SDL_CONTROLLER_BUTTON_INVALID ||
		!m_sdlGameController)
	{
		return 0;
	}

	SDL_GameControllerUpdate();
	if (m_controllerInAGS.isHeld[button])
	{
	}
	else
	{
		if (button < m_controllerInAGS.button_count)
		{
			m_controllerInAGS.buttstate[button] = SDL_GameControllerGetButton(m_sdlGameController, static_cast<SDL_GameControllerButton>(button));
		}
		if (m_controllerInAGS.buttstate[button]) m_controllerInAGS.isHeld[button] = true;
		//else ControllerInAGS.isHeld[butt]=false;

		return m_controllerInAGS.buttstate[button];
	}

	return 0;
}

int GameControllerModule::BatteryStatus(Controller* controller)
{
	if (!m_sdlJoystick)
	{
		return 0;
	}
	SDL_JoystickUpdate();

	return SDL_JoystickCurrentPowerLevel(m_sdlJoystick);
}

int GameControllerModule::PressAnyKey(Controller* controller)
{
	//SDL_JoystickUpdate();
	int button = 0;
	while (button < 32)//ControllerInAGS.button_count)
	{
		int getButtonState = IsButtonDown(controller, button);//SDL_JoystickGetButton(sdlController,butt);
		if (getButtonState == 1)
		{
			m_controllerInAGS.buttstate[button] = getButtonState;
			return button;
		}
		button++;
	}
	return -1;
}

void GameControllerModule::ClickMouse(int button)
{
	m_engine->SimulateMouseClick(button);
}
