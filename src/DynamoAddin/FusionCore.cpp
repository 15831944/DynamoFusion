//#pragma unmanaged
#include "FusionCore.h"
#include <sstream>
/*
void FusionCore::createAndExecuteSelectionCmd()
{
	const struct Commands ObjectSelectCmd = {
		"FusionSelectionFromFusion",
		"Load Objects to Dynamo",
		"Select Objects base on user clicks and loads them to Dynamo",
		"./resources/Dynamo"
	};
	Ptr<CommandDefinition> objectSelectDefinition = createCommandDefinition(ObjectSelectCmd);
	Ptr<CommandCreatedEvent> cmdCreatedEvent2 = objectSelectDefinition->commandCreated();
	if (!cmdCreatedEvent2)
		return;
	cmdCreatedEvent2->add(&onCommandCreated);

	objectSelectDefinition->execute();
}
*/
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
	std::string id = "id" + std::to_string(idNumber);
	Ptr<Design> design = app->activeProduct();
	Ptr<Component> rootComponent = design->rootComponent();
	//
	// Get all exisiting sketches
	//
	Ptr<Sketches> sketches = rootComponent->sketches();

	Ptr<SketchCircle> circle;
	Ptr<SketchCircles> circles;
	Ptr<SketchCurves> sketchCurves;
	//
	// Check if attribute exist in the first place or not.
	//
	std::vector<Ptr<Attribute>> attributes = design->findAttributes("Dynamo-SketchCircle", id);
	if (attributes.size() > 0) {
	
		//
		// Check each sketch in design
		//
		for (size_t j = 0; j < sketches->count();j++) {
			Ptr<Sketch> sketch = sketches->item(j);
			
			sketchCurves = sketch->sketchCurves();
			if (!sketchCurves) continue;

			circles = sketchCurves->sketchCircles();
			if (!circles) continue;

			//
			// Check each circle in each sketch.
			//
			for (size_t i = 0; i < circles->count(); i++) {

				circle = circles->item(i);

				//
				// http://fusion360.autodesk.com/learning/learning.html?guid=GUID-BAF017FE-10B8-4612-BDE2-0EF5D4C6F800
				//
				Ptr<Attributes> attrib = circle->attributes();
				if(!attrib) continue;

				if (attrib->itemByName("Dynamo-SketchCircle", id)) {
					//ui->messageBox("Found ID = " + id);
					//std::stringstream ss;
					//ss.precision(6);
					//ss << std::fixed;
					//Ptr<Point3D> pt = circle->geometry()->center();
					//ss << "Centre BEFORE: " << pt->x() << ", " << pt->y() << ", " << pt->z() << "\n";
					//circle->centerSketchPoint()->move(point->asVector());
					//pt = circle->geometry()->center();
					//ss << "Centre AFTER: " << pt->x() << ", " << pt->y() << ", " << pt->z() << "\n";
					//if (pt->asVector() == point->asVector()) {
					//ss << "I can COMPARE POINTS through vector!"<< "\n";
					//}
					//ui->messageBox(ss.str());

					//
					// Set New Point through translation
					//
					Ptr<Point3D> currPoint = circle->geometry()->center();
					Ptr<Point3D> pt = Point3D::create(point->x()-currPoint->x(), point->y() - currPoint->y(), point->z() - currPoint->z());
					if(!pt->isEqualTo(Point3D::create(0,0,0)))
						circle->centerSketchPoint()->move(pt->asVector());

					//
					// Set New Radius
					//
					circle->radius(r);
					return circle;
				}
			}
			
		}
	}

	//
	// No such attribute exist; create new circle in new sketch.
	//
	Ptr<ConstructionPlane> xyPlane = rootComponent->xYConstructionPlane();
	Ptr<Sketch> sketch = sketches->add(xyPlane);

	sketchCurves = sketch->sketchCurves();
	if (!sketchCurves) return nullptr;

	circles = sketchCurves->sketchCircles();
	if (!circles) nullptr;
	
	circle = circles->addByCenterRadius(
		Point3D::create(point->x(), point->y(), point->z()), r);

	circle->attributes()->add("Dynamo-SketchCircle", id, "1");

	/*
	std::stringstream ss;
	ss.precision(6);
	ss << std::fixed;
	ss << "Suppose to create Circle at Point = " << point->x() << "," << point->y() << "," << point->z() << "\n";
	//Ptr<Point3D> pt = circle->geometry()->center();
	//ss << "Created Circle at Point = " << pt->x() << "," << pt->y() << "," << pt->z() << "\n";
	ss << "Added ID = " << id << "\n";
	attributes = design->findAttributes("Dynamo-SketchCircle", id);
	ss << "Attributes size = " << attributes.size() << "\n";
	ui->messageBox(ss.str());*/

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