// ConsoleApplication2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "DynamoWrapper.h"

[System::STAThread]
int main()
{
	DynamoWrapper* test = new DynamoWrapper();
	test->Initialize1();
	return 0;
}

