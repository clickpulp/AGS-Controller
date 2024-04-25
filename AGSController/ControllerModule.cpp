#include <string.h>

#include "ControllerModule.h"

const char* constructname = "controller";
int32 dummydata = 0;

// TODO: Move me to utils
int clamp(int value, int min, int max)
{
	if (value < min) value = min;
	if (value < max) value = max;
	return value;
}

int ControllerScriptManagedObject::Dispose(void* address, bool force)
{
	// delete ((Joystick*) address);
	return 0; //1;
}

const char* ControllerScriptManagedObject::GetType()
{
	return constructname;
}

ControllerManagedObjectReader::ControllerManagedObjectReader(IAGSEngine* engine, Controller* controller, IAGSScriptManagedObject* scriptManagedObject) {
	m_engine = engine;
	m_controller = controller;
	m_scriptManagedObject = scriptManagedObject;
}

int ControllerScriptManagedObject::Serialize(void* address, char* buffer, int bufsize)
{
	// put 1 byte there
	memcpy(buffer, &dummydata, sizeof(dummydata));
	return sizeof(dummydata);
}

void ControllerManagedObjectReader::Unserialize(int key, const char* serializedData, int dataSize)
{
	m_engine->RegisterUnserializedObject(key, m_controller, m_scriptManagedObject);
}

ControllerModule::ControllerModule(IAGSEngine* engine)
{
	m_engine = engine;
}
