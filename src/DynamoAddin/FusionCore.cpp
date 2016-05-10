

//#pragma unmanaged
#include "FusionCore.h"
#include <sstream>
Ptr<SketchPoint> FusionCore::pointByCoordinates(double x, double y, double z)
{
	Ptr<Design> design = app->activeProduct();
	Ptr<Component> rootComponent = design->rootComponent();

	Ptr<Sketches> sketches = rootComponent->sketches();
	Ptr<ConstructionPlane> xyPlane = rootComponent->xYConstructionPlane();
	Ptr<Sketch> sketch = sketches->add(xyPlane);

	return sketch->sketchPoints()->add(Point3D::create(x, y, z));
}

Ptr<SketchCircle> FusionCore::circleByPointRadius(Ptr<Point3D> point, double r, int idNumber)
{
	Ptr<Design> design = app->activeProduct();
	Ptr<Component> rootComponent = design->rootComponent();

	Ptr<Sketches> sketches = rootComponent->sketches();
	
	Ptr<Sketch> sketch = sketches->item(0);
	if (!sketch) {
		Ptr<ConstructionPlane> xyPlane = rootComponent->xYConstructionPlane();
		sketch = sketches->add(xyPlane);
	}
	//
	// http://fusion360.autodesk.com/learning/learning.html?guid=GUID-BAF017FE-10B8-4612-BDE2-0EF5D4C6F800
	//
	Ptr<SketchCircles> circles = sketch->sketchCurves()->sketchCircles();
	Ptr<SketchCircle> circle;
	size_t i = 0;
	size_t sketchCount = circles->count();

	std::string id = "id" + std::to_string(idNumber);

	std::vector<Ptr<Attribute>> attributes = design->findAttributes("Dynamo-SketchCircle", id);
	if (idNumber == 0 || attributes.size()==0)
	{
		std::stringstream ss;
		ss.precision(6);
		ss << std::fixed;	

		circle = circles->addByCenterRadius(
			Point3D::create(point->x(), point->y(), point->z()), r);

		std::string id = "id" + std::to_string(attributes.size() + 1);
		circle->attributes()->add("Dynamo-SketchCircle", id, "1");
		/*
		ss << "Suppose to create Circle at Point = " << point->x() << "," << point->y() << "," << point->z() << "\n";
		Ptr<Point3D> pt = circle->geometry()->center();
		ss << "Created Circle at Point = " << pt->x() << "," << pt->y() << "," << pt->z() << "\n";
		ss << "Added ID = " << id << "\n";
		attributes = design->findAttributes("Dynamo-SketchCircle", id);
		ss << "Attributes size = " << attributes.size() << "\n";*/
		//ui->messageBox(ss.str());
	}
	else {
		while (i < sketchCount) {

			circle = circles->item(i);

			if (circle && circle->attributes()->itemByName("Dynamo-SketchCircle", id)) {
				//ui->messageBox("Found ID = " + id);
				/*
				std::stringstream ss;
				ss.precision(6);
				ss << std::fixed;

				Ptr<Point3D> pt = circle->geometry()->center();
				ss << "Centre BEFORE: " << pt->x() << ", " << pt->y() << ", " << pt->z() << "\n";
				circle->centerSketchPoint()->move(point->asVector());
				pt = circle->geometry()->center();
				ss << "Centre AFTER: " << pt->x() << ", " << pt->y() << ", " << pt->z() << "\n";
				if (pt->asVector() == point->asVector()) {
				ss << "I can COMPARE POINTS through vector!"<< "\n";
				}
				*/
				//ui->messageBox(ss.str());

				circle->radius(r);

				break;

			}

			i++;

		}
	}
	return circle;
}

std::vector<Ptr<BRepBody>> FusionCore::getSelectedEntities()
{
	//
	// Create array for storing objects
	//
	std::vector<Ptr<BRepBody>> objects;

	if (selectionInput == nullptr)
		return objects;

	int selCount = selectionInput->selectionCount();

	//
	// Store the objects in the array
	//
	for (size_t i = 0; i < selCount; i++)
	{
		objects.push_back(selectionInput->selection(i)->entity());

		//
		// Get Geometry Info
		//
		//BRepBodyGeometryInfo(ui, objects[i]);
	}
	return objects;
}
//#pragma managed