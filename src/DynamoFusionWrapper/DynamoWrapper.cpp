
#include "DynamoWrapper.h"
#using "DynamoFusionApp.dll"

using namespace System;

[STAThreadAttribute]
void DynamoWrapper::LoadDynamo()
{
	String^ asmLocation =
		gcnew String("C:\\Users\\t_shansh\\AppData\\Local\\Autodesk\\webdeploy\\production\\c076e205c02e6f8f9c6eac7820ccca6953c0c53a");
	DynamoFusionApp::DynamoFusionApp::Run(asmLocation);
}