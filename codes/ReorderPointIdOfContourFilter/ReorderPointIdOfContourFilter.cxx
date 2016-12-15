#include "ReorderPointIdOfContourFilter.h"

#include <vtkObjectFactory.h>
#include <vtkSmartPointer.h>
#include <vtkInformation.h>
#include <vtkInformationVector.h>
#include <vtkPolyLine.h>
#include <vtkPolygon.h>
#include <vtkCellArray.h>

#include <list>
#include <memory>

vtkStandardNewMacro(ReorderPointIdOfContourFilter);

ReorderPointIdOfContourFilter::ReorderPointIdOfContourFilter()
{
	this->SetNumberOfInputPorts(1);
	this->SetNumberOfOutputPorts(1);
}

ReorderPointIdOfContourFilter::~ReorderPointIdOfContourFilter()
{

}

int ReorderPointIdOfContourFilter::RequestData(vtkInformation *vtkNotUsed(request),
	vtkInformationVector **inputVector,
	vtkInformationVector *outputVector)
{
	using namespace std;
	// get the input and output
	// get the info objects
	vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
	vtkInformation *outInfo = outputVector->GetInformationObject(0);

	// get the input and output
	vtkPolyData *input = vtkPolyData::SafeDownCast(
		inInfo->Get(vtkDataObject::DATA_OBJECT()));
	vtkPolyData *output = vtkPolyData::SafeDownCast(
		outInfo->Get(vtkDataObject::DATA_OBJECT()));
	if (!input) {
		vtkErrorMacro("input is nullptr.");
		return 0;
	}
	if (input->GetNumberOfCells() < 1) {
		vtkErrorMacro("input has no cell.");
		return 0;
	}
	if (input->GetNumberOfPoints() < 1) {
		vtkErrorMacro("input has no point.");
		return 0;
	}
	if (!output) {
		vtkErrorMacro(<< "output is nullptr");
		return 0;
	}
	list<shared_ptr<vtkIdType>> pointPairList;
	list<shared_ptr<vtkIdType>> reorderPointPairList;


	// pushing a pair of point of every cell of the input to the list
	for (vtkIdType i = 0; i < input->GetNumberOfCells(); ++i) {
		vtkIdType numOfPoints;
		vtkIdType* points;
		input->BuildLinks();
		input->GetCellPoints(i, numOfPoints, points);
		// All cells should have 2 points
		if (numOfPoints != 2) {
			vtkErrorMacro(<< "Cell " << i << " has " << numOfPoints << " points");
			return 0;
		}
		// when using array with shared_ptr, 
		// destructor needed to be specified
		pointPairList.push_back(shared_ptr<vtkIdType>(new vtkIdType[2],
			[](vtkIdType* ptr)->void {delete[] ptr; }));
		pointPairList.back().get()[0] = points[0];
		pointPairList.back().get()[1] = points[1];
	}
	// push the last pair of points into reorderPointPairList
	reorderPointPairList.push_back((pointPairList.back()));
	
	// loop until there only one pair left, because one have already push before
	while (pointPairList.size() > 1) {
		vtkDebugMacro(<< pointPairList.size() << endl);
		for (list<shared_ptr<vtkIdType>>::const_iterator cit = pointPairList.cbegin();
			cit != pointPairList.cend(); ++cit) {
			// the first cell's first point pair to a cell's last point
			if (reorderPointPairList.front().get()[0] == cit->get()[1]) {
				reorderPointPairList.push_front(*cit);
				cit = pointPairList.erase(cit);
				break;
			}
			// the last cell's last point pair to a cell's first point
			else if (reorderPointPairList.back().get()[1] == cit->get()[0]) {
				reorderPointPairList.push_back(*cit);
				cit = pointPairList.erase(cit);
				break;
			}
			else if(cit == --pointPairList.cend()) {
				// all points should be shared by just 2 cells
				vtkErrorMacro(<< "A cell which has pointIds are " << 
					to_string(cit->get()[0]) << " and " << 
					to_string(cit->get()[1]) << 
					" has a point which is not shared by other cell.");
				return 0;
			}
		}
	}
	// push the reorder points into vtkPoints
	// push vtkPoints into vtkPolyData
	vtkSmartPointer<vtkPoints> _points =
		vtkSmartPointer<vtkPoints>::New();
	for (list<shared_ptr<vtkIdType>>::const_iterator cit = reorderPointPairList.cbegin();
		cit != reorderPointPairList.cend(); ++cit) {
		const double* coordinate;
		if (cit == reorderPointPairList.cbegin()) {
			coordinate =
				input->GetPoint(cit->get()[0]);
			_points->InsertNextPoint(coordinate);
		}
		coordinate =
			input->GetPoint(cit->get()[1]);
		_points->InsertNextPoint(coordinate);
	}
	output->SetPoints(_points);

	// push the reorder points into vtkCell
	// push vtkCell into vtkPolyData
	vtkSmartPointer<vtkCell> cell;
	vtkSmartPointer<vtkCellArray> cellArray =
		vtkSmartPointer<vtkCellArray>::New();
	switch (OutputCellType)
	{
	case VTK_POLYGON:
		cell =
			vtkSmartPointer<vtkPolygon>::New();
		cell->GetPointIds()->SetNumberOfIds(_points->GetNumberOfPoints());
		for (int i = 0; i < _points->GetNumberOfPoints(); ++i) {
			cell->GetPointIds()->SetId(i, i);
		}
		cellArray->InsertNextCell(cell);
		output->SetPolys(cellArray);
		break;

	case VTK_POLY_LINE:
	default:
		cell =
			vtkSmartPointer<vtkPolyLine>::New();
		cell->GetPointIds()->SetNumberOfIds(_points->GetNumberOfPoints());
		for (int i = 0; i < _points->GetNumberOfPoints(); ++i) {
			cell->GetPointIds()->SetId(i, i);
		}
		cellArray->InsertNextCell(cell);
		output->SetLines(cellArray);
		break;
	}


	return 1;
}


//----------------------------------------------------------------------------
void ReorderPointIdOfContourFilter::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);
}