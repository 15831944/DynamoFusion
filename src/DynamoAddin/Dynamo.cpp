
#include "DynamoWrapper.h"

//#pragma unmanaged

#include "Dynamo.h"

#include <sstream>

Ptr<Application> app;
Ptr<UserInterface> ui;

// Global reference to selected objects in model workspaces
Ptr<SelectionCommandInput> selectionInput;

void BRepBodyGeometryInfo(Ptr<UserInterface> ui, Ptr<BRepBody> body);



extern "C" XI_EXPORT bool run(const char* context)
{
	app = Application::get();
	if (!app)
		return false;

	ui = app->userInterface();
	if (!ui)
		return false;

	// Object Model Reference:
	// http://help.autodesk.com/cloudhelp/ENU/Fusion-360-API/images/Fusion.pdf
	//
	// 1) Create the command definition.
	// 2) Connect to the command created event.
	// 3) Add at least one control for the definition in the UI.
	// 4) Your add-in waits.
	// When user runs the command:
	//	5) Fusion creates a Command object.
	//	6) Fires the command craeted event to the owner.
	// 7) Create command inpus to define the command dialog.
	// 8) Connect to command related events.
	// 9) Fusion display the command dialog.
	// 10) User interacts with the dialog to provide the input.
	// 11) React to events.
	// 12) Create the final result in the command executed event handler.
	//
	// add a command button on Quick Access Toolbar
	//
	///////////////////////////////////////////////////////////////////////////

	const struct Commands launchDynamoCmd = {
		"DynamoLaunch1",
		"Launch Dynamo",
		"Launch Dynamo in-process with Fusion 360",
		"./resources/Dynamo"
	};
	Ptr<CommandDefinition> launchCmdDefinition = createCommandDefinition(launchDynamoCmd);
	Ptr<CommandCreatedEvent> cmdCreatedEvent = launchCmdDefinition->commandCreated();
	if (!cmdCreatedEvent)
		return false;
	cmdCreatedEvent->add(&onCommandCreated);

	Ptr<ToolbarPanel> toolbarPanel = ui->allToolbarPanels()->itemById("SolidScriptsAddinsPanel");
	if (!toolbarPanel)
		return false;
	Ptr<DropDownControl> dropDown = toolbarPanel->controls()->addDropDown("Dynamo", launchDynamoCmd.Resources);

	dropDown->controls()->addCommand(launchCmdDefinition);

	ui->messageBox("A Dynamo command is successfully added to the panel in modeling workspace");

	//----------------------------------------------------------------------------------------

	const struct Commands ObjectSelectCmd = {
		"FusionSelectionFromFusion",
		"Load Objects to Dynamo",
		"Select Objects base on user clicks and loads them to Dynamo",
		"./resources/Dynamo"
	};
	Ptr<CommandDefinition> objectSelectDefinition = createCommandDefinition(ObjectSelectCmd);
	Ptr<CommandCreatedEvent> cmdCreatedEvent2 = objectSelectDefinition->commandCreated();
	if (!cmdCreatedEvent2)
		return false;
	cmdCreatedEvent2->add(&onCommandCreated);

	dropDown->controls()->addCommand(objectSelectDefinition);

	ui->messageBox("A Select Object command is successfully added to the panel in modeling workspace");

	//----------------------------------------------------------------------------------------

	return true;
}

extern "C" XI_EXPORT bool stop(const char* context)
{
	if (ui)
	{
		ui->messageBox("in stop");
		ui = nullptr;
	}

	return true;
}


#ifdef XI_WIN

#include <windows.h>

BOOL APIENTRY DllMain(HMODULE hmodule, DWORD reason, LPVOID reserved)
{
	switch (reason)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

#endif // XI_WIN

//#pragma unmanaged