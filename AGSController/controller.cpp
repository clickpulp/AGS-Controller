////*

#ifdef WIN32
#define WINDOWS_VERSION
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma warning(disable : 4244)
#endif

#if !defined(BUILTIN_PLUGINS)
#define THIS_IS_THE_PLUGIN
#endif

#include <stdio.h>
#include <SDL.h>
#include <SDL_joystick.h>

#if defined(PSP_VERSION)
#include <pspsdk.h>
#include <pspmath.h>
#include <pspdisplay.h>
#define sin(x) vfpu_sinf(x)
#endif

#include "plugin/agsplugin.h"
#include "ControllerModule.h"
#include "AGSController_script_header.h"
#include "JoystickControllerModule.h"

#if defined(BUILTIN_PLUGINS)
namespace agscontroller {
#endif

#if defined(__GNUC__)
inline unsigned long _blender_alpha16_bgr(unsigned long y) __attribute__((always_inline));
inline void calc_x_n(unsigned long bla) __attribute__((always_inline));
#endif

/*
const unsigned int Magic = 0xACAB0000;
const unsigned int Version = 1;
const unsigned int SaveMagic = Magic + Version;
const float PI = 3.14159265f;
*/

#pragma region Controller Module Wrappers

ControllerModule* g_controllerModule;


int Controller_ControllerCount() { return g_controllerModule->ControllerCount(); }

Controller* Controller_Open(int num) { return g_controllerModule->Open(num); }

void Controller_Close(Controller* controller) { g_controllerModule->Close(controller); }
int Controller_Plugged(Controller* controller) { return g_controllerModule->Plugged(controller); }
int Controller_GetAxis(Controller* controller, int axis) { return g_controllerModule->GetAxis(controller, axis); }
int Controller_GetPOV(Controller* controller) { return g_controllerModule->GetPOV(controller); }
int Controller_IsButtonDown(Controller* controller, int button) { return g_controllerModule->IsButtonDown(controller, button); }
const char* Controller_GetName(Controller* controller) { return g_controllerModule->GetName(controller); }
void Controller_Rumble(Controller* controller, int left, int right, int duration) { g_controllerModule->Rumble(controller, left, right, duration); }
int Controller_IsButtonDownOnce(Controller* controller, int button) { return g_controllerModule->IsButtonDownOnce(controller, button); }
int Controller_BatteryStatus(Controller* controller) { return g_controllerModule->BatteryStatus(controller); }
int Controller_PressAnyKey(Controller* controller) { return g_controllerModule->PressAnyKey(controller); }
void Controller_ClickMouse(int button) { return g_controllerModule->ClickMouse(button); }


#pragma endregion // Controller Module Wrappers

#pragma region Engine Stuff

void AGS_EngineStartup(IAGSEngine *engine)
{
	
	g_controllerModule = new JoystickControllerModule(engine);

	engine->RegisterScriptFunction("ControllerCount", (void*)&Controller_ControllerCount);
	engine->RegisterScriptFunction("Controller::Open", (void*)&Controller_Open);
	engine->RegisterScriptFunction("Controller::Plugged", (void*)&Controller_Plugged);
	engine->RegisterScriptFunction("Controller::GetAxis", (void*)&Controller_GetAxis);
	engine->RegisterScriptFunction("Controller::GetPOV",(void*)&Controller_GetPOV);
	engine->RegisterScriptFunction("Controller::IsButtonDown", (void*)&Controller_IsButtonDown);
	engine->RegisterScriptFunction("Controller::Close", (void*)&Controller_Close);
	engine->RegisterScriptFunction("Controller::GetName^0", reinterpret_cast<void *>(Controller_GetName));
	engine->RegisterScriptFunction("Controller::Rumble", (void*)&Controller_Rumble);
	engine->RegisterScriptFunction("Controller::IsButtonDownOnce",(void*)&Controller_IsButtonDownOnce);
	engine->RegisterScriptFunction("Controller::PressAnyKey",(void*)&Controller_PressAnyKey);
    engine->RegisterScriptFunction("Controller::BatteryStatus",(void*)&Controller_BatteryStatus);
	engine->RegisterScriptFunction("ClickMouse",(void*)&Controller_ClickMouse);
	
	engine->RequestEventHook(AGSE_PREGUIDRAW);

	SDL_GameControllerAddMappingsFromFile("gamecontrollerdb.txt");
}

void AGS_EngineInitGfx(const char *driverID, void *data)
{
}

void AGS_EngineShutdown()
{
	SDL_Quit();
}

int AGS_EngineOnEvent(int event, int data)
{
  if (event == AGSE_PREGUIDRAW)
  {
	  if (g_controllerModule != nullptr) g_controllerModule->Update();
  }  
  else if (event == AGSE_RESTOREGAME)
  {
	//engine->FRead(&SFX[j].repeat,sizeof(int),data);
	
  }
  else if (event == AGSE_SAVEGAME)
  {
	  //engine->FWrite(&SFX[j].repeat,sizeof(int),data);
  }
  else if (event == AGSE_PRESCREENDRAW)
  {
    // Get screen size once here.   
	//engine->UnrequestEventHook(AGSE_SAVEGAME);
	//engine->UnrequestEventHook(AGSE_RESTOREGAME);
	
  }
  else if (event ==AGSE_POSTSCREENDRAW)
  {
	  
  }
  return 0;
}

int AGS_EngineDebugHook(const char *scriptName, int lineNum, int reserved)
{
  return 0;
}

#pragma endregion Editor Stuff


#if defined(WINDOWS_VERSION) && !defined(BUILTIN_PLUGINS)

// ********************************************
// ***********  Editor Interface  *************
// ********************************************


IAGSEditor* editor;


LPCSTR AGS_GetPluginName(void)
{
  // Return the plugin description
  return "AGSController";
}

int  AGS_EditorStartup(IAGSEditor* lpEditor)
{
  // User has checked the plugin to use it in their game

  // If it's an earlier version than what we need, abort.
  if (lpEditor->version < 1)
    return -1;

  editor = lpEditor;
  editor->RegisterScriptHeader(g_scriptHeader);

  // Return 0 to indicate success
  return 0;
}

void AGS_EditorShutdown()
{
  // User has un-checked the plugin from their game
  editor->UnregisterScriptHeader(g_scriptHeader);
}

void AGS_EditorProperties(HWND parent)
{
  // User has chosen to view the Properties of the plugin
  // We could load up an options dialog or something here instead
  MessageBoxA(parent, "AGSController", "About", MB_OK | MB_ICONINFORMATION);
}

int AGS_EditorSaveGame(char* buffer, int bufsize)
{
  // We don't want to save any persistent data
  return 0;
}

void AGS_EditorLoadGame(char* buffer, int bufsize)
{
  // Nothing to load for this plugin
}

#endif


#if defined(BUILTIN_PLUGINS)
} // namespace agscontroller
#endif
