// This is the main DLL file.

#include "stdafx.h"

#include "FusionManagedWrapper.h"

using namespace FusionManagedWrapper;

#pragma region Fusion Selection

cli::array<FusionEntity^>^ FusionSelection::StartSelection()
{
	// (1) Create & Execute Command

	// (2) Get Bodies
	auto bodies = FusionCore::getSelectedEntities();
	int size = bodies.size();
	cli::array<FusionEntity^>^ bodyArray = gcnew cli::array<FusionEntity^>(size);

	for (int i = 0; i < size; i++)
	{
		bodyArray[i] = gcnew FusionSolid(bodies[i].detach());
	}
	return bodyArray;
}

FusionEntity::~FusionEntity()
{
	//m_pEntity->deleteMe();
}

#pragma endregion

#pragma region Fusion Entity

cli::array<FusionEntity^>^ FusionEntity::getSelectedEntities()
{
	auto bodies = FusionCore::getSelectedEntities();
	int size = bodies.size();
	cli::array<FusionEntity^>^ bodyArray = gcnew cli::array<FusionEntity^>(size);

	for (int i = 0; i < size; i++)
	{
		bodyArray[i] = gcnew FusionSolid(bodies[i].detach());
	}
	return bodyArray;
}

FusionEntity::~FusionEntity()
{
	//m_pEntity->deleteMe();
}

#pragma endregion

#pragma region Fusion Solids

FusionSolid::FusionSolid(BRepBody* pSolid)
{
	m_pSolid = pSolid;
}

FusionSolid::~FusionSolid()
{
	m_pSolid->deleteMe();
}

#pragma endregion

#pragma region Fusion Sketches

FusionCurve::FusionCurve(SketchCircle* pCurve)
{
	m_pCurve = pCurve;
}

FusionCurve^ FusionCurve::createCircle(double x, double y, double z, double r)
{
	Ptr<Point3D> point = Point3D::create(x, y, z);
	Ptr<SketchCircle> circle = FusionCore::circleByPointRadius(point, r);

	return gcnew FusionCurve(circle.detach());
}

FusionCurve::~FusionCurve()
{
	m_pCurve->deleteMe();
}

#pragma endregion