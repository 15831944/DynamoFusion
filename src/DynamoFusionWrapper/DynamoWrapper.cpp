
//#include "stdafx.h"

#using "DynamoFusion.dll"

#include <msclr\auto_gcroot.h>

using namespace System::Runtime::InteropServices; // Marshal

class DynamoWrapperPrivate
{
public: msclr::auto_gcroot<DynamoFusion::DynamoFusionApp^> dynamoFusionApp;
};

class __declspec(dllexport) DynamoWrapper
{
private: DynamoWrapperPrivate* _private;

public: DynamoWrapper()
{
	_private = new DynamoWrapperPrivate();
	_private->dynamoFusionApp = gcnew DynamoFusion::DynamoFusionApp();
}

public:  void Initialize1()
{
	_private->dynamoFusionApp->Run();
}

public: ~DynamoWrapper()
{
	delete _private;
}
};