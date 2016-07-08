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

#include <vtkObject.h>
#include <vtkVersion.h>
#include <vtkStreamingDemandDrivenPipeline.h>
#include <vtkInformation.h>
#include <vtkImageViewer2.h>
#include <vtkAlgorithmOutput.h>
#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCursor3D.h>
#include <vtkPolyData.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkPlane.h>
#include <vtkDataSet.h>
#include <vtkLookupTable.h>
#include <vtkTextActor.h>
#include <vtkDistanceWidget.h>
#include <vtkAngleWidget.h>
#include <vtkContourWidget.h>
#include <vtkObjectFactory.h>
#include <vtkImageActor.h>
#include <vtkPointHandleRepresentation2D.h>
#include <vtkTextProperty.h>
#include <vtkImageProperty.h>
#include <vtkDistanceRepresentation2D.h>
#include <vtkAngleRepresentation2D.h>
#include <vtkLeaderActor2D.h>
#include <vtkAxisActor2D.h>
#include <vtkProperty2D.h>

#include <QString>


class MyImageViewer : public vtkImageViewer2
{
public:
  static MyImageViewer *New();
  vtkTypeMacro(MyImageViewer,vtkObject);
    virtual void InitializeHeader(QString);
	virtual void InitializeIntensityText(QString);
  
   virtual void RemoveInput();
   // Set Input Layer
    virtual void SetInputDataLayer(vtkImageData *in);
    virtual vtkImageData *GetInputLayer();
    virtual void RemoveInputLayer();
    
	virtual void AddPolyData(vtkPolyData* polydata,vtkProperty* property);
  
	// Description:
	// Set/get the slice orientation
	//BTX
	//ETX
	virtual void SetSliceOrientation(int orientation);
	virtual void SetSliceOrientationToXY()
	{
		this->SetSliceOrientation(vtkImageViewer2::SLICE_ORIENTATION_XY);
	};
	virtual void SetSliceOrientationToYZ()
	{
		this->SetSliceOrientation(vtkImageViewer2::SLICE_ORIENTATION_YZ);
	};
	virtual void SetSliceOrientationToXZ()
	{
		this->SetSliceOrientation(vtkImageViewer2::SLICE_ORIENTATION_XZ);
	};
  
  // Description:
  // Set window and level for mapping pixels to colors.
    
  virtual double* GetDefaultWindowLevel();

  virtual double  GetColorWindowLayer();
  virtual double  GetColorLevelLayer();
  virtual void	  SetColorWindowLayer(double s);
  virtual void	  SetColorLevelLayer(double s);
  virtual void SetDefaultWindowLevel(double* defaultWindowLevel);


  virtual void SetBound(double* b);
  virtual double* GetBound();
  virtual double* GetFocalPoint();
  
  // Description:
  // Get the internal render window, renderer, image actor, and
  // image map instances.
  vtkGetObjectMacro(ImageActorLayer, vtkImageActor);
  vtkGetObjectMacro(WindowLevelLayer, vtkImageMapToWindowLevelColors);
  vtkGetObjectMacro(SliceImplicitPlane, vtkPlane);

  //Cursor
  virtual void SetFocalPoint(double x, double y, double z);

  //Orientation Text
  virtual void ResizeOrientationText();
 
  //Ruler
  virtual void SetRulerEnabled(bool b);
  virtual void SetProtractorEnabled(bool b);


  //// Description:
  //// @deprecated Replaced by MyImageViewer::GetSliceMin() as of VTK 5.0.
  //VTK_LEGACY(int  GetWholeZMin());
  //VTK_LEGACY(int  GetWholeZMax());
  //VTK_LEGACY(int  GetZSlice());
  //VTK_LEGACY(void SetZSlice(int));

protected:
  MyImageViewer();
  ~MyImageViewer();

  virtual void SetCursorBoundary();
  virtual void InitializeOrientationText();
    

  //Rendering 
  vtkRenderWindow           *RenderWindow;
  vtkRenderer               *Renderer;
  vtkRenderWindowInteractor *Interactor;
  vtkInteractorStyleImage   *InteractorStyle;

  //OrientationText
  vtkTextActor*	TextActor[4];
    
  //Header
    vtkTextActor* HeaderActor;

	// IntensityText
	vtkTextActor* IntTextActor;

  //Cursor
  vtkCursor3D*		 Cursor3D;
  vtkPolyDataMapper* Cursormapper;	
  vtkActor*			 CursorActor;

  //BackGround
  vtkImageMapToWindowLevelColors* WindowLevel;
  double Bound[6];

  //Label
   vtkImageMapToWindowLevelColors* WindowLevelLayer;
  vtkImageActor* ImageActorLayer;
   vtkLookupTable* LookUpTableLayer;
  
  //Clip polyData
  vtkActor*	ClipActor;

  //Widget
  vtkDistanceWidget* DistanceWidget;

  vtkAngleWidget*	 AngleWidget;
   vtkContourWidget*  ContourWidget;
  //vtkSmartPointer<vtkOrientedGlyphContourRepresentation> ContourRep;
  //Parameter
  int SliceOrientation;
  int FirstRender;
  int Slice;
  vtkPlane* SliceImplicitPlane;
  double DefaultWindowLevel[2];
    

private:
  MyImageViewer(const MyImageViewer&);  // Not implemented.
  void operator=(const MyImageViewer&);  // Not implemented.
};

#endif


