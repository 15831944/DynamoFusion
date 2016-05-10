#pragma once

// This is the Dynamo Addin project in Fusion360
#include <Core/CoreAll.h>
#include <Fusion/FusionAll.h>
#include <CAM/CAMAll.h>

using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

struct Commands
{
	std::string Id;
	std::string Name;
	std::string Description;
	std::string Resources;
};

/// Create the command definition.
Ptr<CommandDefinition> createCommandDefinition(Commands cmd)
{
	Ptr<CommandDefinitions> commandDefinitions = ui->commandDefinitions();
	if (!commandDefinitions)
		return nullptr;

	// Be fault tolerant in case the command is already added.
	Ptr<CommandDefinition> cmDef = commandDefinitions->itemById(cmd.Id);
	if (!cmDef)
	{
		cmDef = commandDefinitions->addButtonDefinition(
			cmd.Id,
			cmd.Name,
			cmd.Description,
			cmd.Resources
			);
	}

	return cmDef;
}

class CommandExecutedHandler : public adsk::core::CommandEventHandler
{
public:
	void notify(const Ptr<CommandEventArgs>& eventArgs) override
	{
		Ptr<Event> firingEvent = eventArgs->firingEvent();
		if (!firingEvent)
			return;

		Ptr<Command> command = firingEvent->sender();
		if (!command)
			return;

		Ptr<CommandDefinition> parentDefinition = command->parentCommandDefinition();
		if (!parentDefinition)
			return;

		Ptr<CommandInputs> inputs = command->commandInputs();
		if (!inputs)
			return;

		if (parentDefinition->id() == "FusionSelectionFromFusion") {
			//
			// Get Bodies here
			// http://adndevblog.typepad.com/manufacturing/2016/02/array-of-fusion-objects-in-c.html
			//
			selectionInput = inputs->itemById("selectEnt");
			auto dynamo_api = DynamoWrapper::GetInstance();
			dynamo_api->CreateSelectionNode();
		}
		if (parentDefinition->id() == "FusionSelectionFromDynamo") {
			//
			// Get Bodies here
			// http://adndevblog.typepad.com/manufacturing/2016/02/array-of-fusion-objects-in-c.html
			//
			selectionInput = inputs->itemById("selectEnt");
			auto dynamo_api = DynamoWrapper::GetInstance();
			dynamo_api->CreateSelectionNode();
		}
		//----------------------------------------------------------------------------------------

		//
		// Only loads Dynamo if the correct ID; Not sure if this is the right way to do it but it works.
		//
		if (parentDefinition->id() == "DynamoLaunch1") {
			auto dynamo_api = DynamoWrapper::GetInstance();
			dynamo_api->LoadDynamo();
		}

		//----------------------------------------------------------------------------------------

		if (ui) {
			std::stringstream ss;
			ss << "command: " + parentDefinition->id() << " executed successfully";
			ui->messageBox(ss.str());
		}
	}
};

class CommandCreatedHandler : public adsk::core::CommandCreatedEventHandler
{
public:
	void notify(const Ptr<CommandCreatedEventArgs>& eventArgs) override
	{
		if (eventArgs)
		{
			Ptr<Command> command = eventArgs->command();
			if (!command)
				return;

			Ptr<CommandEvent> exec = command->execute();
			if (!exec)
				return;
			exec->add(&onCommandExecuted_);

			Ptr<CommandDefinition> parentDefinition = command->parentCommandDefinition();
			if (!parentDefinition)
				return;

			Ptr<CommandInputs> inputs = command->commandInputs();
			if (!inputs)
				return;

			// 
			// Create selection input
			//
			if (parentDefinition->id() == "FusionSelectionFromFusion" || parentDefinition->id() == "FusionSelectionFromDynamo") {
				Ptr<SelectionCommandInput> selectionInput = inputs->addSelectionInput("selectEnt", "Bodies", "Select 1 or more bodies.");
				if (!selectionInput)
					return;
				selectionInput->addSelectionFilter("Bodies");
				selectionInput->setSelectionLimits(1, 0);
			}

			//----------------------------------------------------------------------------------------

			if (ui) {
				std::stringstream ss;
				ss << "command: " + parentDefinition->id() << " created successfully";
				ui->messageBox(ss.str());
			}
		}
	}
private:
	CommandExecutedHandler onCommandExecuted_;
} onCommandCreated;
