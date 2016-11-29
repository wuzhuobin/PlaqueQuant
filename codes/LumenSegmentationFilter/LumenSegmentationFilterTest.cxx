#include <vtkSmartPointer.h>
// To setup the ContourWidget and its representation:
#include <vtkContourWidget.h>
#include <vtkProperty.h>
#include <vtkOrientedGlyphContourRepresentation.h>
// To create the geometry:
#include <vtkPolyData.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkMath.h>
// Usual VTK pipeline elements:
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTrackballCamera.h>

// My test header
#include <vtkXMLPolyDataReader.h>
#include <vtkCleanPolyData.h>
#include <exception>
#include <list>
#include <map>
#include <memory>
#include <vtkKdTreePointLocator.h>


class ReorderPolyDataException : public std::exception
{
public:
	/**
	 * @override
	 */
	char const* what() const
	{
		using namespace std;
		string errString;
		errString += exception::what();
		errString += string(__FUNCTION__);
		errString += __LINE__;
		errString += "ReorderPolyDataException";
		return errString.c_str();
	}
};

class ReorderPolyDataExceptionNullPointer: public ReorderPolyDataException
{
public:
	/**
	* @override
	*/
	char const* what() const {
		using namespace std;
		string errString;
		errString += ReorderPolyDataException::what();
		errString += string(__FUNCTION__);
		errString += __LINE__;
		errString += "ReorderPolyDataExceptionNullPointer";
		return errString.c_str();
	}
};

class ReorderPolyDataExceptionHasNoCells: public ReorderPolyDataException
{
public:
	/**
	 * @override
	 */
	char const* what() const {
		using namespace std;
		string errString;
		errString += ReorderPolyDataException::what();
		errString += string(__FUNCTION__);
		errString += __LINE__;
		errString += "ReorderPolyDataExceptionHasNoCells";
		return errString.c_str();
	}
};

class ReorderPolyDataExceptionHasNoPoints : public ReorderPolyDataException
{
public:
	/**
	* @override
	*/
	char const* what() const {
		using namespace std;
		string errString;
		errString += ReorderPolyDataException::what();
		errString += string(__FUNCTION__);
		errString += __LINE__;
		errString += "ReorderPolyDataExceptionHasNoPoints";
		return errString.c_str();
	}
};

/**
 * @deprecated
 */
void ReorderPolyDataqqq(vtkPolyData * lumenWallPolyData) {


	if (lumenWallPolyData == nullptr) {
		throw ReorderPolyDataExceptionNullPointer();
	}
	vtkSmartPointer<vtkCleanPolyData> clean =
		vtkSmartPointer<vtkCleanPolyData>::New();
	clean->SetInputData(lumenWallPolyData);
	clean->PointMergingOn();
	clean->Update();

	lumenWallPolyData->ShallowCopy(clean->GetOutput());
	lumenWallPolyData->BuildLinks();

	if (lumenWallPolyData->GetNumberOfCells() < 1) {
		throw ReorderPolyDataExceptionHasNoCells();
	}
	if (lumenWallPolyData->GetNumberOfPoints() < 1) {
		throw ReorderPolyDataExceptionHasNoPoints();
	}
	vtkIdType* points = nullptr;
	std::list<vtkIdType> _pointsList;


	vtkIdType previousPointId = -1;
	vtkIdType previousCellId = -1;
	unsigned short numOfCells = 0;
	vtkIdType* neighborCells = nullptr;
	previousCellId = 0;

	while (_pointsList.size() < lumenWallPolyData->GetNumberOfPoints())
	{
		vtkIdType numOfPoints = 0;

		for (; previousCellId < lumenWallPolyData->GetNumberOfCells();
			++previousCellId) {
			lumenWallPolyData->GetCellPoints(previousCellId, numOfPoints, points);
			if (numOfPoints >= 1) {
				break;
			}
		}
		//while (numOfPoints < 1)
		//{
		//	++previousCellId;
		//	if (previousCellId == lumenWallPolyData->GetNumberOfCells()) {
		//		break;
		//		//throw ReorderPolyDataExceptionHasNoPoints();
		//	}
		//	lumenWallPolyData->GetCellPoints(previousCellId, numOfPoints, points);
		//	cerr << previousCellId << endl;
		//}
		previousPointId = (previousPointId != points[0] ? points[0] : points[1]);
		cerr << "points[0] " << points[0] << "points[1]" << points[1] << endl;
		cerr << "previousPointId " << previousPointId <<  endl;
		_pointsList.push_back(previousPointId);
		lumenWallPolyData->GetPointCells(previousPointId, numOfCells, neighborCells);
		if (numOfCells < 2) {
			std::cerr << "Warning! this point "<< previousPointId << 
				"has no neighbor cell" << endl;
			// find the closet point which is not in the _pointsList
			// if no point is found, exit all loops
			vtkSmartPointer<vtkKdTreePointLocator> kdTreePointLocater =
				vtkSmartPointer<vtkKdTreePointLocator>::New();
			kdTreePointLocater->SetDataSet(lumenWallPolyData);
			kdTreePointLocater->BuildLocator();

			vtkSmartPointer<vtkIdList> closestPoints =
				vtkSmartPointer<vtkIdList>::New();
			kdTreePointLocater->FindClosestNPoints(lumenWallPolyData->GetNumberOfPoints(),
				lumenWallPolyData->GetPoint(previousPointId), closestPoints);
			int id = 0;
			for (int id = 0; id < closestPoints->GetNumberOfIds(); ++id) {
				if (_pointsList.cend() ==
					std::find(_pointsList.cbegin(), _pointsList.cend(),
						closestPoints->GetId(id))) {
					for (std::list<vtkIdType>::const_iterator cit = _pointsList.cbegin();
						cit != _pointsList.cend(); ++cit) {
						cerr << *cit << ' ';
					}
					cerr << "id: " << id << endl;
					cerr << "GetId(id): " << closestPoints->GetId(id) << endl;
					previousPointId = closestPoints->GetId(id);
				}
				if (id == closestPoints->GetNumberOfIds()) {
					break;
					break;
				}
			}
			//while(true){
			//	//for (std::list<vtkIdType>::const_iterator cit = _pointsList.cbegin();
			//	//	cit != _pointsList.cend(); ++cit) {
			//	//	cerr << *cit << ' ';
			//	//}
			//	cerr << endl;
			//	if (_pointsList.cend() ==
			//		std::find(_pointsList.cbegin(), _pointsList.cend(),
			//			closestPoints->GetId(id))) {
			//		cerr << "this is the cend()";
			//		cerr << (*_pointsList.cend()) << endl;
			//		previousPointId = closestPoints->GetId(id);
			//		cerr << id << endl;
			//		cerr << "Find cloest points" << endl;
			//		cerr << previousPointId << endl;

			//		break;
			//	}
			//	++id;
			//	if (id == closestPoints->GetNumberOfIds()) {
			//		break;
			//		break;
			//	}
			//}

		}
		else {
			cerr << previousCellId << endl;
			//cerr << neighborCells[0] << endl;
			//cerr << neighborCells[1] << endl;
			previousCellId = (previousCellId != neighborCells[0] ? neighborCells[0] : neighborCells[1]);
		}
	}

	vtkSmartPointer<vtkPoints> _points =
		vtkSmartPointer<vtkPoints>::New();
	for (std::list<vtkIdType>::const_iterator cit = _pointsList.cbegin();
		cit != _pointsList.cend(); ++cit) {
		const double* coordinate = lumenWallPolyData->GetPoint(*cit);
		_points->InsertNextPoint(coordinate[0], coordinate[1], coordinate[2]);
	}
	lumenWallPolyData->ShallowCopy(vtkSmartPointer<vtkPolyData>::New());
	lumenWallPolyData->SetPoints(_points);
}

void ReorderPolyData(vtkPolyData * lumenWallPolyData) throw(std::exception)
{
	using namespace std;
	//list<vtkCell*> cellsList;
	//list<unique_ptr<vtkIdType[]>> cellsList;
	list<vtkIdType*> cellsList;
	//list<vtkCell*> newCellsList;
	//list<unique_ptr<vtkIdType[]>> newCellsList;
	list<vtkIdType*> newCellsList;
	vtkSmartPointer<vtkPoints> _points =
		vtkSmartPointer<vtkPoints>::New();

	list<vtkIdType> pointIds;
	for (vtkIdType i = 0; i < lumenWallPolyData->GetNumberOfCells(); ++i) {
		vtkCell* _cell = lumenWallPolyData->GetCell(i);
		vtkIdType* _twoPoints = new vtkIdType[2];
		_twoPoints[0] = _cell->GetPointId(0);
		_twoPoints[1] = _cell->GetPointId(1);
		//unique_ptrvtkIdType*> _twoPoints(new vtkIdType[2]);
		cellsList.push_back(_twoPoints);
	}


	newCellsList.push_back(cellsList.back());
	while (cellsList.size() > 1) {
		cerr << cellsList.size() << endl;
		for (list<vtkIdType*>::iterator it = cellsList.begin();
			it != cellsList.end(); ++it) {
			cerr << (*it)[0] << endl;
			cerr << (*it)[1] << endl;

			if (newCellsList.front()[0] == (*it)[1]) {
				newCellsList.push_front(*it);
				it = cellsList.erase(it);
				break;
			}
			else if (newCellsList.back()[1] == (*it)[0]) {
				newCellsList.push_back(*it);
				it = cellsList.erase(it);
			}
			//if (newCellsList.front()->GetPointId(0) == (*cit)->GetPointId(1)) {
			//	newCellsList.push_front(*cit);
			//	cit = cellsList.erase(cit);
			//	break;
			//}
			//else if (newCellsList.back()->GetPointId(1) == (*cit)->GetPointId(0)) {
			//	newCellsList.push_back(*cit);
			//	cit = cellsList.erase(cit);
			//}
		}
	}

	for (list<vtkIdType*>::const_iterator cit = newCellsList.cbegin();
		cit != newCellsList.cend(); ++cit) {
		const double* coordinate;
		if (cit == newCellsList.cbegin()) {

			coordinate =
				lumenWallPolyData->GetPoint((*cit)[0]);
			_points->InsertNextPoint(coordinate);
		}
		coordinate =
			lumenWallPolyData->GetPoint((*cit)[1]);
		_points->InsertNextPoint(coordinate);
	}
	lumenWallPolyData->DeepCopy(vtkSmartPointer<vtkPolyData>::New());
	lumenWallPolyData->SetPoints(_points);
}

int main( int argc, char *argv[] )
{
    // Create the contour widget
    vtkSmartPointer<vtkContourWidget> contourWidget =
        vtkSmartPointer<vtkContourWidget>::New();
 
    // Override the default representation for the contour widget to customize its look
    vtkSmartPointer<vtkOrientedGlyphContourRepresentation> contourRepresentation =
        vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New();
    contourRepresentation->GetLinesProperty()->SetColor(1, 0, 0); // Set color to red
    contourWidget->SetRepresentation(contourRepresentation);
 
    //// Generate a set of points arranged in a circle
    //int numPts = 10;
    //vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    //for (int i = 0; i < numPts; i++)
    //{
    //    // Create numPts points evenly spread around a circumference of radius 0.1
    //    const double angle = 2.0*vtkMath::Pi()*i/numPts;
    //    points->InsertPoint(static_cast<vtkIdType>(i), 0.1*cos(angle), 0.1*sin(angle), 0.0 );
    //}
 
    //// Create a cell array to connect the points into meaningful geometry
    //vtkIdType* vertexIndices = new vtkIdType[numPts+1];
    //for (int i = 0; i < numPts; i++) { vertexIndices[i] = static_cast<vtkIdType>(i); }
    //// Set the last vertex to 0; this means the last line segment will join the 19th point (vertices[19])
    //// with the first one (vertices[0]), thus closing the circle.
    //vertexIndices[numPts] = 0;
    //vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
    //lines->InsertNextCell(numPts+1, vertexIndices);
 
    //// Create polydata to hold the geometry just created, and populate it
    //vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    //polydata->SetPoints(points);
    //polydata->SetLines(lines);

	vtkSmartPointer<vtkPolyData> polydata;

	vtkSmartPointer<vtkXMLPolyDataReader> reader =
		vtkSmartPointer<vtkXMLPolyDataReader>::New();
	reader->SetFileName("C:/Users/user/Desktop/h/test4.vtp");
	reader->Update();

	vtkSmartPointer<vtkCleanPolyData> clean =
		vtkSmartPointer<vtkCleanPolyData>::New();
	clean->PointMergingOn();
	clean->SetInputConnection(reader->GetOutputPort());
	clean->Update();


	polydata = clean->GetOutput();
	ReorderPolyData(polydata);

	vtkSmartPointer<vtkCleanPolyData> clean1 =
		vtkSmartPointer<vtkCleanPolyData>::New();
	clean1->SetInputData(polydata);
	clean1->PointMergingOn();
	//clean1->SetTolerance(0.05);
	clean1->Update();
	polydata = clean1->GetOutput();
 
    // Create the renderer to visualize the scene
    vtkSmartPointer<vtkRenderer> renderer =
        vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground(0.1, 0.2, 0.4); // Set a dark blue background (default is black)
 
    // Create the GUI window to hold the rendered scene
    vtkSmartPointer<vtkRenderWindow> renderWindow =
        vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer(renderer);
 
    // Create the events manager for the renderer window
    vtkSmartPointer<vtkRenderWindowInteractor> interactor =
        vtkSmartPointer<vtkRenderWindowInteractor>::New();
    interactor->SetRenderWindow(renderWindow);
 
    // Use the "trackball camera" interactor style, rather than the default "joystick camera"
    vtkSmartPointer<vtkInteractorStyleTrackballCamera> style =
      vtkSmartPointer<vtkInteractorStyleTrackballCamera>::New();
    interactor->SetInteractorStyle(style);
 
    // Set up the contour widget within the visualization pipeline just assembled
    contourWidget->SetInteractor(interactor);
    contourWidget->On(); // Turn on the interactor observer
    contourWidget->Initialize(polydata);
    renderer->ResetCamera(); // Reposition camera to fit the scene elements
 
    // Start the interaction
    interactor->Start();
 
    return EXIT_SUCCESS;
}


