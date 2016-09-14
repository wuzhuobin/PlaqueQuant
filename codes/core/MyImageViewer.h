/*=========================================================================
Program:   Visualization Toolkit
Module:    MyImageViewer.h
Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
All rights reserved.
See Copyright.txt or http://www.kitware.com/Copyright.htm for details.
This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notice for more information.
=========================================================================*/
// .NAME MyImageViewer - Display a 2D image.
// .SECTION Description
// MyImageViewer is a convenience class for displaying a 2D image.  It
// packages up the functionality found in vtkRenderWindow, vtkRenderer,
// vtkImageActor and vtkImageMapToWindowLevelColors into a single easy to use
// class.  This class also creates an image interactor style
// (vtkInteractorStyleImage) that allows zooming and panning of images, and
// supports interactive window/level operations on the image. Note that
// MyImageViewer is simply a wrapper around these classes.
//
// MyImageViewer uses the 3D rendering and texture mapping engine
// to draw an image on a plane.  This allows for rapid rendering,
// zooming, and panning. The image is placed in the 3D scene at a
// depth based on the z-coordinate of the particular image slice. Each
// call to SetSlice() changes the image data (slice) displayed AND
// changes the depth of the displayed slice in the 3D scene. This can
// be controlled by the AutoAdjustCameraClippingRange ivar of the
// InteractorStyle member.
//
// It is possible to mix images and geometry, using the methods:
//
// viewer->SetInput( myImage );
// viewer->GetRenderer()->AddActor( myActor );
//
// This can be used to annotate an image with a PolyData of "edges" or
// or highlight sections of an image or display a 3D isosurface
// with a slice from the volume, etc. Any portions of your geometry
// that are in front of the displayed slice will be visible; any
// portions of your geometry that are behind the displayed slice will
// be obscured. A more general framework (with respect to viewing
// direction) for achieving this effect is provided by the
// vtkImagePlaneWidget .
//
// Note that pressing 'r' will reset the window/level and pressing
// shift+'r' or control+'r' will reset the camera.
//
// .SECTION See Also
// vtkRenderWindow vtkRenderer vtkImageActor vtkImageMapToWindowLevelColors

#ifndef __MyImageViewer_h
#define __MyImageViewer_h

#include <vtkObjectFactory.h>
#include <vtkImageViewer2.h>
#include <vtkAngleWidget.h>
#include <vtkDistanceWidget.h>
#include <vtkAngleWidget.h>
#include <vtkTextActor.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkPlane.h>
#include <vtkLogLookupTable.h>
#include <vtkImageActor.h>
#include <vtkCursor3D.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkImageData.h>
#include <vtkInteractorStyleImage.h>

#include <QObject>
#include <QString>

#include "Overlay.h"

class MyImageViewer : public QObject, public vtkImageViewer2
{
	Q_OBJECT
public:
	static MyImageViewer *New();
	vtkTypeMacro(MyImageViewer, vtkImageViewer2);
	void PrintSelf(ostream& os, vtkIndent indent);
	// Text methods
	virtual void InitializeHeader(QString File);


	// Cursor methods
	virtual void SetBound(double* b);
	virtual double* GetBound();
	virtual void SetFocalPointWithWorldCoordinate(double x, double y, double z);
	virtual void SetFocalPointWithImageCoordinate(int i, int j, int k);
	virtual void GetFocalPointWithImageCoordinate(int* coordinate);
	virtual void GetFocalPointWithWorldCoordinate(double* coordinate);
	virtual double* GetFocalPointWithWorldCoordinate();

	// Description:
	// Render the resulting image.
	virtual void Render(void);

	// Description:
	// Set/Get the input image to the viewer.
	virtual void SetInputData(vtkImageData * in);
	// Set/Get Input Layer which is supposed to be the output of overlay
	virtual void SetInputDataLayer(vtkImageData *in);
	virtual vtkImageData *GetInputLayer();
	// Set/Get Overlay
	virtual void SetOverlay(Overlay* overlay);
	virtual Overlay* GetOverlay();

	// Description:
	// return DefaultWindowLevel
	virtual double* GetDefaultWindowLevel();
	
	// Description:
	// Get the internal render window, renderer, image actor, and
	// image map instances.
	vtkGetObjectMacro(drawActor, vtkImageActor);
	vtkGetObjectMacro(AnnotationWindowLevel, vtkImageMapToWindowLevelColors);
	//vtkGetObjectMacro(SliceImplicitPlane, vtkPlane);
	vtkGetObjectMacro(AnnotationRenderer, vtkRenderer);
	// Description:
	// Set your own Annotation Renderer
	virtual void SetAnnotationRenderer(vtkRenderer *arg);

	// LookupTable
	virtual vtkLookupTable* getLookupTable();
	virtual void SetLookupTable(vtkLookupTable* LookupTable);

	// Description:
	// Attach an interactor for the internal render window.
	virtual void SetupInteractor(vtkRenderWindowInteractor* arg);

	//Ruler
	virtual void SetRulerEnabled(bool b);
	virtual void SetProtractorEnabled(bool b);

	// Description:
	// Update the display extent manually so that the proper slice for the
	// given m_orientation is displayed. It will also try to set a
	// reasonable camera clipping range.
	// This method is called automatically when the Input is changed, but
	// most of the time the input of this class is likely to remain the same,
	// i.e. connected to the output of a filter, or an image reader. When the
	// input of this filter or reader itself is changed, an error message might
	// be displayed since the current display extent is probably outside
	// the new whole extent. Calling this method will ensure that the display
	// extent is reset properly.
	virtual void UpdateDisplayExtent();

public slots:
	// Slice
	virtual void SetSlice(int s);

	virtual void SetColorLevel(double level);

	virtual void SetColorWindow(double window);

signals:
	
	void SliceChanged(int);
	void ColorLevelChanged(double);
	void ColorWindowChanged(double);

protected:
	MyImageViewer(QObject* parent = NULL);
	~MyImageViewer();

	// Text Method
	virtual void ResizeOrientationText();
	virtual void InitializeIntensityText(QString IntText);
	virtual void InitializeOrientationText();

	virtual void InstallPipeline();
	virtual void UnInstallPipeline();
	virtual void UpdateOrientation();

	//OrientationText
	vtkTextActor*	OrientationTextActor[4];

	//Header
	vtkTextActor* HeaderActor;

	// IntensityText
	vtkTextActor* IntTextActor;

	// Overlay
	Overlay* SegmentationOverlay;

	//Cursor
	vtkCursor3D*		 Cursor3D;
	vtkPolyDataMapper* CursorMapper;
	vtkActor*			 CursorActor;
	virtual void SetCursorBoundary();
	//Bound of cursor
	double Bound[6];

	//Label and Annotation
	vtkImageMapToWindowLevelColors* AnnotationWindowLevel;
	vtkImageActor* drawActor;
	vtkRenderer* AnnotationRenderer;

	// LookupTable for drawActor
	vtkLookupTable* LookupTable;


	//Widget
	vtkDistanceWidget* DistanceWidget;

	vtkAngleWidget*	 AngleWidget;
	//Parameter
	//vtkPlane* SliceImplicitPlane;
	double DefaultWindowLevel[2];

private:
	MyImageViewer(const MyImageViewer&);  // Not implemented.
	void operator=(const MyImageViewer&);  // Not implemented.
};

#endif