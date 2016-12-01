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
#include "ReorderPointIdOfContourFilter.h"
#include <vtkXMLPolyDataWriter.h>
#include <vtkCallbackCommand.h>

int main(int argc, char *argv[])
{
	// Create the contour widget
	vtkSmartPointer<vtkContourWidget> contourWidget =
		vtkSmartPointer<vtkContourWidget>::New();

	// Override the default representation for the contour widget to customize its look
	vtkSmartPointer<vtkOrientedGlyphContourRepresentation> contourRepresentation =
		vtkSmartPointer<vtkOrientedGlyphContourRepresentation>::New();
	contourRepresentation->GetLinesProperty()->SetColor(1, 0, 0); // Set color to red
	contourWidget->SetRepresentation(contourRepresentation);

	vtkSmartPointer<vtkPolyData> polydata;

	vtkSmartPointer<vtkXMLPolyDataReader> reader =
		vtkSmartPointer<vtkXMLPolyDataReader>::New();
	reader->SetFileName("C:/Users/jieji/Desktop/reorder/test3.vtp");
	reader->Update();

	vtkSmartPointer<vtkCleanPolyData> clean =
		vtkSmartPointer<vtkCleanPolyData>::New();
	clean->PointMergingOn();
	clean->SetInputConnection(reader->GetOutputPort());
	clean->Update();

	// vtkError handling
	vtkSmartPointer<vtkCallbackCommand> errorCatch =
		vtkSmartPointer<vtkCallbackCommand>::New();
	// lambda function for handling error
	errorCatch->SetCallback([](vtkObject *caller, unsigned long eid,
		void *clientdata, void *calldata)->void{
		// error catch and error display
		char* ErrorMessage = static_cast<char *>(calldata);
		vtkOutputWindowDisplayErrorText(ErrorMessage);
		ReorderPointIdOfContourFilter* reorder =
			ReorderPointIdOfContourFilter::SafeDownCast(caller);
		// if error happened, skip this filer
		reorder->SetOutput(reorder->GetInput());
	});

	vtkSmartPointer<ReorderPointIdOfContourFilter> reorder =
		vtkSmartPointer<ReorderPointIdOfContourFilter>::New();
	reorder->SetInputConnection(clean->GetOutputPort());
	reorder->AddObserver(vtkCommand::ErrorEvent, errorCatch);
	reorder->Update();

	vtkSmartPointer<vtkCleanPolyData> clean1 =
		vtkSmartPointer<vtkCleanPolyData>::New();
	clean1->SetInputConnection(reorder->GetOutputPort());
	clean1->PointMergingOn();
	clean1->SetTolerance(0.05);
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
	contourWidget->CloseLoop();
	renderer->ResetCamera(); // Reposition camera to fit the scene elements

							 // Start the interaction
	interactor->Start();

	return EXIT_SUCCESS;
}
