// DynamoWrapper.h
/*
#pragma once

#include <msclr\auto_gcroot.h>

#using "DynamoFusionApp.dll"

// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the DESIGNSCRIPTRUNNER_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// DESIGNSCRIPTRUNNER_API functions as being imported from a DLL, whereas this DLL sees symbols
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
	DynamoWrapper();

	static DynamoWrapper* wrapper;
	DynamoWrapperPrivate* dynamoAPI;

//public: ~DynamoWrapper();

public: 
	static DynamoWrapper* GetInstance();
	void LoadDynamo();
	void CreateSelectionNode();
};

DynamoWrapper* DynamoWrapper::wrapper = nullptr;

*/


// DynamoManagedWrapper.h

#pragma once


class DynamoWrapper
{

public:

	static DynamoWrapper* GetInstance();

	void LoadDynamo();
	void CreateSelectionNode();

};

