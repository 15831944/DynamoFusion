#pragma once

// This is the Dynamo Addin project in Fusion360
#include <Core/CoreAll.h>
#include <Fusion/FusionAll.h>
#include <CAM/CAMAll.h>

using namespace adsk::core;
using namespace adsk::fusion;
using namespace adsk::cam;

/*
#include "DynamoWrapper.h"
#include <sstream>

struct Commands
{
	std::string Id;
	std::string Name;
	std::string Description;
	std::string Resources;
};

/// Create the command definition.
Ptr<CommandDefinition> createCommandDefinition(Commands cmd);


class CommandExecutedHandler : public adsk::core::CommandEventHandler
{
public:
	void notify(const Ptr<CommandEventArgs>& eventArgs);
};

class CommandCreatedHandler : public adsk::core::CommandCreatedEventHandler
{
public:
	void notify(const Ptr<CommandCreatedEventArgs>& eventArgs);

private:
	CommandExecutedHandler onCommandExecuted_;
} onCommandCreated;


*/