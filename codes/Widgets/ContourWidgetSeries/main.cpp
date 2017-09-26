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
#include <vtkSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <ContourWidgetSeries.h>
#include <ContourWidgetSeriesRepresentation.h>
 
int main( int argc, char *argv[] )
{
	//vtkSmartPointer<vtkContourWidget> contourWidget =
	//	vtkSmartPointer<vtkContourWidget>::New();

	//// Override the default representation for the contour widget to customize its look
	//vtkSmartPointer<vtkOrientedGlyphContourRepresentation> contourRepresentation =
	//	vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New();
	//contourRepresentation->GetLinesProperty()->SetColor(1, 0, 0); // Set color to red
	//contourWidget->SetRepresentation(contourRepresentation);



    // Override the default representation for the contour widget to customize its look
	vtkSmartPointer<vtkOrientedGlyphContourRepresentation> contourRepresentation =
		vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New();
 //   vtkSmartPointer<ContourWidgetSeriesRepresentation> seriesContourRepresentation =
 //       vtkSmartPointer<ContourWidgetSeriesRepresentation>::New();
	//seriesContourRepresentation->SetContourRepresentation(contourRepresentation);
	// Create the contour widget
	vtkSmartPointer<ContourWidgetSeries> contourWidget =
		vtkSmartPointer<ContourWidgetSeries>::New();
	contourWidget->CreateDefaultRepresentation();
	//contourWidget->GetContourWidgetSeriesRepresentation()->SetContourRepresentation(contourRepresentation);
 //   contourWidget->SetRepresentation(seriesContourRepresentation);
 
    // Generate a set of points arranged in a circle
    int numPts = 10;
    vtkSmartPointer<vtkPoints> points = vtkSmartPointer<vtkPoints>::New();
    for (int i = 0; i < numPts; i++)
    {
        // Create numPts points evenly spread around a circumference of radius 0.1
        const double angle = 2.0*vtkMath::Pi()*i/numPts;
        points->InsertPoint(static_cast<vtkIdType>(i), 0.1*cos(angle), 0.1*sin(angle), 0.0 );
    }
 
    // Create a cell array to connect the points into meaningful geometry
    vtkIdType* vertexIndices = new vtkIdType[numPts+1];
    for (int i = 0; i < numPts; i++) { vertexIndices[i] = static_cast<vtkIdType>(i); }
    // Set the last vertex to 0; this means the last line segment will join the 19th point (vertices[19])
    // with the first one (vertices[0]), thus closing the circle.
    vertexIndices[numPts] = 0;
    vtkSmartPointer<vtkCellArray> lines = vtkSmartPointer<vtkCellArray>::New();
    lines->InsertNextCell(numPts+1, vertexIndices);
 
    // Create polydata to hold the geometry just created, and populate it
    vtkSmartPointer<vtkPolyData> polydata = vtkSmartPointer<vtkPolyData>::New();
    polydata->SetPoints(points);
    polydata->SetLines(lines);

	// Create a sphere
	vtkSmartPointer<vtkSphereSource> sphereSource =
		vtkSmartPointer<vtkSphereSource>::New();
	sphereSource->SetCenter(0.0, 0.0, 0.0);
	sphereSource->SetRadius(5.0);

	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(sphereSource->GetOutputPort());

	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);
 
    // Create the renderer to visualize the scene
    vtkSmartPointer<vtkRenderer> renderer =
        vtkSmartPointer<vtkRenderer>::New();
	renderer->AddActor(actor);
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
    //contourWidget->Initialize(polydata);
    renderer->ResetCamera(); // Reposition camera to fit the scene elements
 
    // Start the interaction
    interactor->Start();
 
    return EXIT_SUCCESS;
}