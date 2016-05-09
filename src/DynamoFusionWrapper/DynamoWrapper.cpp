/*
#include "DynamoWrapper.h"

using namespace System;

DynamoWrapper::DynamoWrapper()
{
	dynamoAPI = new DynamoWrapperPrivate();
	dynamoAPI->dynamoWrapper = gcnew DynamoFusionApp::DynamoFusionApp();
}

DynamoWrapper* DynamoWrapper::GetInstance()
{
	if (wrapper == nullptr)
	{
		wrapper = new DynamoWrapper();
	}
	return wrapper;
}

[STAThreadAttribute]
void DynamoWrapper::LoadDynamo()
{
	String^ asmLocation =
		gcnew String("C:\\Users\\t_shansh\\AppData\\Local\\Autodesk\\webdeploy\\production\\8761ad41e24127c30f608621af9f94797e223b67");
	dynamoAPI->dynamoWrapper->Run(asmLocation);
}

void DynamoWrapper::CreateSelectionNode()
{
	dynamoAPI->dynamoWrapper->CreateSelectionNode();
}

*/



// This is the main DLL file.

#using<DynamoFusionApp.dll>


using namespace System;

#include <msclr\auto_gcroot.h>

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DYNAMO_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DYNAMO_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef DYNAMO_EXPORTS
#define DYNAMO_API __declspec(dllexport)
#else
#define DYNAMO_API __declspec(dllimport)
#endif


class DynamoWrapperPrivate
{
public: msclr::auto_gcroot<DynamoFusionApp::DynamoFusionApp^> dynamoWrapper;
};

class DYNAMO_API DynamoWrapper
{
private:
	DynamoWrapper()
	{
		dynamoAPI = new DynamoWrapperPrivate();
		dynamoAPI->dynamoWrapper = gcnew DynamoFusionApp::DynamoFusionApp();
	}

	static DynamoWrapper* wrapper;
	DynamoWrapperPrivate* dynamoAPI;

public:
	static DynamoWrapper* GetInstance()
	{
		if (wrapper == nullptr)
		{
			wrapper = new DynamoWrapper();
		}
		return wrapper;
	}

	[STAThreadAttribute]
	void LoadDynamo()
	{
		String^ asmLocation =
			gcnew String("C:\\Users\\t_shansh\\AppData\\Local\\Autodesk\\webdeploy\\production\\8761ad41e24127c30f608621af9f94797e223b67");
		dynamoAPI->dynamoWrapper->Run(asmLocation);
	}

	void CreateSelectionNode()
	{
		dynamoAPI->dynamoWrapper->CreateSelectionNode();
	}
};


DynamoWrapper* DynamoWrapper::wrapper = nullptr;
