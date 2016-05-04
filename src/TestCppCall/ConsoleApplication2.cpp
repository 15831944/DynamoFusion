// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DynamoWrapper.h"
//#include "DynamoFusionWrapper"
//#using <DynamoFusionWrapper.dll>

[System::STAThread]
int main()
{
	DynamoWrapper* test = new DynamoWrapper();
	test->LoadDynamo();
	return 0;
}

