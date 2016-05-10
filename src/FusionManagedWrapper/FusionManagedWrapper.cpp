// This is the main DLL file.

#include "stdafx.h"

#include "FusionManagedWrapper.h"

using namespace FusionManagedWrapper;

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

FusionCurve::FusionCurve(SketchCircle* pCurve, int id)
{
	m_pCurve = pCurve;
	identifier = id;
	//entity = gcnew FusionCurve(m_pCurve);
}

//int* FusionCurve::getID(FusionCurve fusionCurve) {
//	return fusionCurve.getid();
//}

FusionCurve^ FusionCurve::createCircle(double x, double y, double z, double r, int id)
{
	Ptr<Point3D> point = Point3D::create(x, y, z);
	Ptr<SketchCircle> circle = FusionCore::circleByPointRadius(point, r, id);

	std::vector<Ptr<adsk::core::Attribute>> attributes = circle->attributes()->itemsByGroup("Dynamo-SketchCircle");
	std::string ID = attributes[0]->name();
	
	return gcnew FusionCurve(circle.detach(), std::stoi(ID.substr(2, ID.length())));

	//return entity;
}

FusionCurve::~FusionCurve()
{
	//m_pCurve->deleteMe();
}

#pragma endregion