/*=========================================================================

  Program:   Visualization Toolkit
  Module:    MyImageViewer.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "MyImageViewer.h"


#include "vtkCamera.h"
#include "vtkCommand.h"
#include "vtkImageActor.h"
#include "vtkImageData.h"
#include "vtkImageMapToWindowLevelColors.h"
#include "vtkInteractorStyleImage.h"
#include "vtkImageMapper3D.h"
#include "vtkObjectFactory.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkRenderer.h"
#include "vtkCursor3D.h"
#include "vtkPolyDataMapper.h"
#include "vtkActor.h"
#include "vtkProperty.h"
#include "vtkCutter.h"
#include "vtkDataSet.h"
#include "vtkPlane.h"
#include "vtkLookupTable.h"
#include "vtkImageProperty.h"
#include "vtkTextActor.h"
#include "vtkTextProperty.h"
#include "vtkDistanceWidget.h"
#include "vtkAngleWidget.h"
#include "vtkContourWidget.h"
#include "vtkPointHandleRepresentation2D.h"
#include "vtkDistanceRepresentation2D.h"

#include "vtkAngleRepresentation2D.h"
#include "vtkSmartPointer.h"

#include "vtkAxisActor2D.h"
#include "vtkProperty2D.h"
#include "vtkLeaderActor2D.h"
#include "vtkTransform.h"
#include "MainWindow.h"

vtkStandardNewMacro(MyImageViewer);

//----------------------------------------------------------------------------
MyImageViewer::MyImageViewer()
{
  this->RenderWindow    = NULL;
  this->Renderer        = NULL;  
  this->Interactor      = NULL;
  this->InteractorStyle = NULL;
  this->DistanceWidget	= vtkDistanceWidget::New();
  this->AngleWidget		= vtkAngleWidget::New();
  this->ContourWidget = vtkContourWidget::New();
  this->ImageActor     = vtkImageActor::New();
  this->ImageActorLayer = vtkImageActor::New();
  this->WindowLevel    = vtkImageMapToWindowLevelColors::New();
  this->WindowLevelLayer = vtkImageMapToWindowLevelColors::New();
  this->LookUpTableLayer	= vtkLookupTable::New();
  this->SliceImplicitPlane = vtkPlane::New();

  //Disable the warning
  this->WindowLevel->SetGlobalWarningDisplay(false);
  this->WindowLevelLayer->SetGlobalWarningDisplay(false);
  //set label Look Up Table
  this->LookUpTableLayer->SetNumberOfTableValues(4);
  this->LookUpTableLayer->SetTableRange(0.0,3);
  //this->LookUpTableLayer->SetValueRange(-1, 1);
  this->LookUpTableLayer->SetTableValue(0,0,0,0,0);
  this->LookUpTableLayer->SetTableValue(1,1,0,0,1);
  this->LookUpTableLayer->SetTableValue(2, 0, 0, 1, 1);
  this->LookUpTableLayer->SetTableValue(3, 0, 1, 0, 1);
  this->LookUpTableLayer->Build();
  this->ImageActorLayer->GetProperty()->SetLookupTable(LookUpTableLayer);

  this->Slice = 0;
  this->FirstRender = 1;
  this->SliceOrientation = MyImageViewer::SLICE_ORIENTATION_XY;
  this->SliceImplicitPlane->SetOrigin (0,0,0);
  this->SliceImplicitPlane->SetNormal (0,0,1);

 //Cursor
  Cursor3D = vtkCursor3D::New();
  Cursor3D->AllOff();
  Cursor3D->AxesOn();
  Cursor3D->SetModelBounds(0,0,0,0,0,0);
  Cursor3D->Update();

  Cursormapper = vtkPolyDataMapper::New();
  Cursormapper->SetInputData(Cursor3D->GetOutput());
  
  CursorActor = vtkActor::New();
  CursorActor->SetMapper(Cursormapper); 
  CursorActor->GetProperty()->SetColor(0,0,1);
  CursorActor->GetProperty()->SetLineStipplePattern(0xf0f0);
  //Image Actor
  ImageActor->GetProperty()->SetInterpolationTypeToNearest();
  ImageActorLayer->GetProperty()->SetInterpolationTypeToNearest();


  //Setup the pipeline
  vtkRenderWindow *renwin = vtkRenderWindow::New();
  this->SetRenderWindow(renwin);
  renwin->Delete();

  vtkRenderer *ren = vtkRenderer::New();
  this->SetRenderer(ren);
  ren->GetActiveCamera()->SetParallelProjection(true);
  ren->Delete();

  this->InstallPipeline();

  this->ImageActor->SetOpacity(1.0);
  this->ImageActorLayer->SetOpacity(1);
  this->ImageActorLayer->SetVisibility(false);
}

//----------------------------------------------------------------------------
MyImageViewer::~MyImageViewer()
{


  if (this->WindowLevel)
    {
    this->WindowLevel->Delete();
    this->WindowLevel = NULL;
    }
  if (this->WindowLevelLayer)
    {
    this->WindowLevelLayer->Delete();
    this->WindowLevelLayer = NULL;
    }
  if (this->ImageActor)
    {
    this->ImageActor->Delete();
    this->ImageActor = NULL;
    }
  
    if (this->ImageActorLayer)
    {
        this->ImageActorLayer->Delete();
        this->ImageActorLayer = NULL;
    }
  if (this->Renderer)
    {
    this->Renderer->Delete();
    this->Renderer = NULL;
    }

  if (this->RenderWindow)
    {
    this->RenderWindow->Delete();
    this->RenderWindow = NULL;
    }

  if (this->Interactor)
    {
    this->Interactor->Delete();
    this->Interactor = NULL;
    }

  if (this->InteractorStyle)
    {
    this->InteractorStyle->Delete();
    this->InteractorStyle = NULL;
    }

  if (this->SliceImplicitPlane)
  {
	  this->SliceImplicitPlane->Delete();
	  this->SliceImplicitPlane = NULL;
  }
}

//----------------------------------------------------------------------------
void MyImageViewer::SetupInteractor(vtkRenderWindowInteractor *arg)
{
  if (this->Interactor == arg)
    {
    return;
    }

  this->UnInstallPipeline();

  if (this->Interactor)
    {
    this->Interactor->UnRegister(this);
    }
    
  this->Interactor = arg;
  
  if (this->Interactor)
    {
    this->Interactor->Register(this);
    }

  this->InstallPipeline();

  if (this->Renderer)
    {
    this->Renderer->GetActiveCamera()->ParallelProjectionOn();
    }
}

//----------------------------------------------------------------------------
void MyImageViewer::SetRenderWindow(vtkRenderWindow *arg)
{
  if (this->RenderWindow == arg)
    {
    return;
    }

  this->UnInstallPipeline();

  if (this->RenderWindow)
    {
    this->RenderWindow->UnRegister(this);
    }
    
  this->RenderWindow = arg;
  
  if (this->RenderWindow)
    {
    this->RenderWindow->Register(this);
    }

  this->InstallPipeline();
}

//----------------------------------------------------------------------------
void MyImageViewer::SetRenderer(vtkRenderer *arg)
{
  if (this->Renderer == arg)
    {
    return;
    }

  this->UnInstallPipeline();

  if (this->Renderer)
    {
    this->Renderer->UnRegister(this);
    }
    
  this->Renderer = arg;
  
  if (this->Renderer)
    {
    this->Renderer->Register(this);
    }

  this->InstallPipeline();
  this->UpdateOrientation();
}

//----------------------------------------------------------------------------
void MyImageViewer::SetSize(int a,int b)
{
  this->RenderWindow->SetSize(a, b);
}

//----------------------------------------------------------------------------
int* MyImageViewer::GetSize()
{
  return this->RenderWindow->GetSize();
}

//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
int* MyImageViewer::GetSliceRange()
{
  //Use Input 1 as base
#if VTK_MAJOR_VERSION <= 5
      vtkImageData *input = this->GetInput();
        if (input)
        {		
            input->UpdateInformation();
            return input->GetWholeExtent() + this->SliceOrientation * 2;
        }

		return NULL;
#else 
	vtkAlgorithm *input = this->GetInputAlgorithm();
	if (input)
	{
    input->UpdateInformation();
    return input->GetOutputInformation(0)->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT()) +
	      this->SliceOrientation * 2;
     }
   return NULL;
#endif
}

//----------------------------------------------------------------------------
int MyImageViewer::GetSliceMin()
{
  int *range = this->GetSliceRange();
  if (range)
    {
    return range[0];
    }
  return 0;
}

//----------------------------------------------------------------------------
int MyImageViewer::GetSliceMax()
{
  int *range = this->GetSliceRange();
  if (range)
    {
    return range[1];
    }
  return 0;
}

//----------------------------------------------------------------------------
void MyImageViewer::SetSlice(int slice)
{
  int *range = this->GetSliceRange();
  if (range)
    {
    if (slice < range[0])
      {
      slice = range[0];
      }
    else if (slice > range[1])
      {
      slice = range[1];
      }
    }

  if (this->Slice == slice)
    {
    return;
    }


  this->Slice = slice;
  this->Modified();

  this->UpdateDisplayExtent();
  //this->Render();    //Comment for rendering outside together
}

//----------------------------------------------------------------------------
void MyImageViewer::SetSliceOrientation(int orientation)
{
  if (orientation < MyImageViewer::SLICE_ORIENTATION_YZ ||
      orientation > MyImageViewer::SLICE_ORIENTATION_XY)
    {
    vtkErrorMacro("Error - invalid slice orientation " << orientation);
    return;
    }
  
  if (this->SliceOrientation == orientation)
    {
    return;
    }

  this->SliceOrientation = orientation;

  switch (orientation)
  {
  case 0:
	  SliceImplicitPlane->SetNormal(1,0,0);
	  SliceImplicitPlane->SetOrigin(0,0,0);
  	break;

  case 1:
	  SliceImplicitPlane->SetNormal(0,1,0);
	  SliceImplicitPlane->SetOrigin(0,0,0);
	  break;

  case 2:
	  SliceImplicitPlane->SetNormal(0,0,1);
	  SliceImplicitPlane->SetOrigin(0,0,0);
	  break;
  }

  // Update the viewer 
  int *range = this->GetSliceRange();
  if (range)
    {
    this->Slice = static_cast<int>((range[0] + range[1]) * 0.5);
    }

  this->UpdateOrientation();
  this->UpdateDisplayExtent();

  if ((this->Renderer && this->GetInput()))
    {
    double scale = this->Renderer->GetActiveCamera()->GetParallelScale();
    this->Renderer->ResetCamera();
    this->Renderer->GetActiveCamera()->SetParallelScale(scale);
    }

  this->Render();
}

//----------------------------------------------------------------------------
void MyImageViewer::UpdateOrientation()
{
  // Set the camera position

  vtkCamera *cam = this->Renderer ? this->Renderer->GetActiveCamera() : NULL;
  if (cam)
    {
    switch (this->SliceOrientation)
      {
      case MyImageViewer::SLICE_ORIENTATION_XY:
        cam->SetFocalPoint(0,0,0);
        cam->SetPosition(0,0,1); // -1 if medical ?
        cam->SetViewUp(0,1,0);
        break;
        
      case MyImageViewer::SLICE_ORIENTATION_XZ:
        cam->SetFocalPoint(0,0,0);
        cam->SetPosition(0,-1,0); // 1 if medical ?
        cam->SetViewUp(0,0,1);
        break;
        
      case MyImageViewer::SLICE_ORIENTATION_YZ:
        cam->SetFocalPoint(0,0,0);
        cam->SetPosition(1,0,0); // -1 if medical ?
        cam->SetViewUp(0,0,1);
        break;
      }
    }
}

//----------------------------------------------------------------------------
void MyImageViewer::UpdateDisplayExtent()
{

	vtkAlgorithm *input = this->GetInputAlgorithm();
	if (!input || !this->ImageActor || !this->ImageActorLayer)
	{
		return;
	}   
	
	input->UpdateInformation();
	vtkInformation* outInfo = input->GetOutputInformation(0);
	int *w_ext = outInfo->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT());
  

  // Is the slice in range ? If not, fix it

  int slice_min = w_ext[this->SliceOrientation * 2];
  int slice_max = w_ext[this->SliceOrientation * 2 + 1];
  if (this->Slice < slice_min || this->Slice > slice_max)
    {
    this->Slice = static_cast<int>((slice_min + slice_max) * 0.5);
    }

  // Set the image actor
    switch (this->SliceOrientation)
    {
            case MyImageViewer::SLICE_ORIENTATION_XY:
                this->ImageActor->SetDisplayExtent(
                                                    w_ext[0], w_ext[1], w_ext[2], w_ext[3], this->Slice, this->Slice);
                break;
                
            case MyImageViewer::SLICE_ORIENTATION_XZ:
                this->ImageActor->SetDisplayExtent(
                                                    w_ext[0], w_ext[1], this->Slice, this->Slice, w_ext[4], w_ext[5]);
                break;
                
            case MyImageViewer::SLICE_ORIENTATION_YZ:
                this->ImageActor->SetDisplayExtent(
                                                    this->Slice, this->Slice, w_ext[2], w_ext[3], w_ext[4], w_ext[5]);
                break;
    }
    if (this->GetInputLayer())
    {
        switch (this->SliceOrientation)
        {
            case MyImageViewer::SLICE_ORIENTATION_XY:
                this->ImageActorLayer->SetDisplayExtent(
                                                    w_ext[0], w_ext[1], w_ext[2], w_ext[3], this->Slice, this->Slice);
                break;
                
            case MyImageViewer::SLICE_ORIENTATION_XZ:
                this->ImageActorLayer->SetDisplayExtent(
                                                    w_ext[0], w_ext[1], this->Slice, this->Slice, w_ext[4], w_ext[5]);
                break;
                
            case MyImageViewer::SLICE_ORIENTATION_YZ:
                this->ImageActorLayer->SetDisplayExtent(
                                                    this->Slice, this->Slice, w_ext[2], w_ext[3], w_ext[4], w_ext[5]);
                break;
        }
    }
  
}
//----------------------------------------------------------------------------
void MyImageViewer::SetPosition(int a,int b)
{
  this->RenderWindow->SetPosition(a, b);
}

//----------------------------------------------------------------------------
int* MyImageViewer::GetPosition()
{
  return this->RenderWindow->GetPosition();
}

//----------------------------------------------------------------------------
void MyImageViewer::SetDisplayId(void *a)
{
  this->RenderWindow->SetDisplayId(a);
}

//----------------------------------------------------------------------------
void MyImageViewer::SetWindowId(void *a)
{
  this->RenderWindow->SetWindowId(a);
}

//----------------------------------------------------------------------------
void MyImageViewer::SetParentId(void *a)
{
  this->RenderWindow->SetParentId(a);
}

//----------------------------------------------------------------------------
double MyImageViewer::GetColorWindow()
{
  return this->WindowLevel->GetWindow();
}

//----------------------------------------------------------------------------
double MyImageViewer::GetColorLevel()
{
  return this->WindowLevel->GetLevel();
}

//----------------------------------------------------------------------------
void MyImageViewer::SetColorWindow(double s)
{
  this->WindowLevel->SetWindow(s);
}

//----------------------------------------------------------------------------
void MyImageViewer::SetColorLevel(double s)
{
  this->WindowLevel->SetLevel(s);
}

//----------------------------------------------------------------------------
double MyImageViewer::GetColorWindowLayer()
{
  return this->WindowLevelLayer->GetWindow();
}

//----------------------------------------------------------------------------
double MyImageViewer::GetColorLevelLayer()
{
  return this->WindowLevelLayer->GetLevel();
}

//----------------------------------------------------------------------------
void MyImageViewer::SetColorWindowLayer(double s)
{
  this->WindowLevelLayer->SetWindow(s);
}

//----------------------------------------------------------------------------
void MyImageViewer::SetColorLevelLayer(double s)
{
  this->WindowLevelLayer->SetLevel(s);
}

class MyImageViewerCallback : public vtkCommand
{
public:
  static MyImageViewerCallback *New() { return new MyImageViewerCallback; }
  
  void Execute(vtkObject *caller, 
               unsigned long event, 
               void *vtkNotUsed(callData))
    {
      if (this->IV->GetInput() == NULL)
        {
        return;
        }

      // Reset

      if (event == vtkCommand::ResetWindowLevelEvent)
        {

			this->IV->GetInputAlgorithm()->UpdateInformation();
			vtkStreamingDemandDrivenPipeline::SetUpdateExtent(this->IV->GetInputInformation(),vtkStreamingDemandDrivenPipeline::GetWholeExtent(this->IV->GetInputInformation()));
			this->IV->GetInputAlgorithm()->Update();
    
        double *range = this->IV->GetInput()->GetScalarRange();
        this->IV->SetColorWindow(range[1] - range[0]);
        this->IV->SetColorLevel(0.5 * (range[1] + range[0]));
        this->IV->SetColorWindowLayer(range[1] - range[0]);
        this->IV->SetColorLevelLayer(0.5 * (range[1] + range[0]));
        this->IV->Render();
        return;
        }

      // Start

      if (event == vtkCommand::StartWindowLevelEvent)
        {
        this->InitialWindow = this->IV->GetColorWindow();
        this->InitialLevel = this->IV->GetColorLevel();
        return;
        }
      
      // Adjust the window level here

      vtkInteractorStyleImage *isi = 
        static_cast<vtkInteractorStyleImage *>(caller);

      int *size = this->IV->GetRenderWindow()->GetSize();
      double window = this->InitialWindow;
      double level = this->InitialLevel;
      
      // Compute normalized delta

      double dx = 4.0 * 
        (isi->GetWindowLevelCurrentPosition()[0] - 
         isi->GetWindowLevelStartPosition()[0]) / size[0];
      double dy = 4.0 * 
        (isi->GetWindowLevelStartPosition()[1] - 
         isi->GetWindowLevelCurrentPosition()[1]) / size[1];
      
      // Scale by current values

      if (fabs(window) > 0.01)
        {
        dx = dx * window;
        }
      else
        {
        dx = dx * (window < 0 ? -0.01 : 0.01);
        }
      if (fabs(level) > 0.01)
        {
        dy = dy * level;
        }
      else
        {
        dy = dy * (level < 0 ? -0.01 : 0.01);
        }
      
      // Abs so that direction does not flip

      if (window < 0.0) 
        {
        dx = -1*dx;
        }
      if (level < 0.0) 
        {
        dy = -1*dy;
        }
      
      // Compute new window level

      double newWindow = dx + window;
      double newLevel;
      newLevel = level - dy;
      
      // Stay away from zero and really

      if (fabs(newWindow) < 0.01)
        {
        newWindow = 0.01*(newWindow < 0 ? -1 : 1);
        }
      if (fabs(newLevel) < 0.01)
        {
        newLevel = 0.01*(newLevel < 0 ? -1 : 1);
        }
      
      this->IV->SetColorWindow(newWindow);
      this->IV->SetColorLevel(newLevel);

      this->IV->Render();
    }
  
  MyImageViewer *IV;
  double InitialWindow;
  double InitialLevel;
    
};

//----------------------------------------------------------------------------
void MyImageViewer::InstallPipeline()
{
  if (this->RenderWindow && this->Renderer)
    {
    this->RenderWindow->AddRenderer(this->Renderer);
    }

  if (this->Interactor)
    {
    if (!this->InteractorStyle)
      {
      this->InteractorStyle = vtkInteractorStyleImage::New();
      MyImageViewerCallback *cbk = MyImageViewerCallback::New();
      cbk->IV = this;
      this->InteractorStyle->AddObserver(
        vtkCommand::WindowLevelEvent, cbk);
      this->InteractorStyle->AddObserver(
        vtkCommand::StartWindowLevelEvent, cbk);
      this->InteractorStyle->AddObserver(
        vtkCommand::ResetWindowLevelEvent, cbk);
      cbk->Delete();
      }
    
    this->Interactor->SetInteractorStyle(this->InteractorStyle);
    this->Interactor->SetRenderWindow(this->RenderWindow);
    }

  if (this->Renderer && this->ImageActor)
    {
    this->Renderer->AddViewProp(this->ImageActor);
    }
    
    if (this->Renderer && this->ImageActorLayer)
    {
        this->Renderer->AddViewProp(this->ImageActorLayer);
    }
  if (this->Renderer && this->CursorActor)
	{
	  this->Renderer->AddViewProp(this->CursorActor);
	}

	if (this->ImageActor && this->WindowLevel)
	{
		this->ImageActor->GetMapper()->SetInputConnection(this->WindowLevel->GetOutputPort());
	 }
	if (this->ImageActorLayer && this->WindowLevelLayer)
	{
		this->ImageActorLayer->GetMapper()->SetInputConnection(this->WindowLevelLayer->GetOutputPort());
	}

}

//----------------------------------------------------------------------------
void MyImageViewer::UnInstallPipeline()
{
	if (this->ImageActor)
	{
	 this->ImageActor->GetMapper()->SetInputConnection(NULL);
	}
	if (this->ImageActorLayer)
	{
		this->ImageActorLayer->GetMapper()->SetInputConnection(NULL);
	}
  if (this->Renderer && this->ImageActor)
    {
    this->Renderer->RemoveViewProp(this->ImageActor);
    }
    
    if (this->Renderer && this->ImageActorLayer)
    {
        this->Renderer->RemoveViewProp(this->ImageActorLayer);
    }

  if (this->Renderer && this->CursorActor)
	{
	  this->Renderer->RemoveViewProp(this->CursorActor);
	}

  if (this->RenderWindow && this->Renderer)
    {
    this->RenderWindow->RemoveRenderer(this->Renderer);
    }

  if (this->Interactor)
    {
    this->Interactor->SetInteractorStyle(NULL);
    this->Interactor->SetRenderWindow(NULL);
    }
}

//----------------------------------------------------------------------------
void MyImageViewer::Render()
{
  if (this->FirstRender)
    {
		this->InitializeOrientationText();
		this->InitializeIntensityText("");
        //this->InitializeHeader();

    // Initialize the size if not set yet
	#if VTK_MAJOR_VERSION <= 5
        vtkImageData *input=this->GetInput();
	#else
		vtkAlgorithm *input = this->GetInputAlgorithm();
	#endif
    if (input)
      {
		  input->UpdateInformation();
	#if VTK_MAJOR_VERSION <= 5
		  int *w_ext = input->GetWholeExtent();
	#else
		  int *w_ext = this->GetInputInformation()->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT());
	#endif
      int xs = 0, ys = 0;
	  double d=0;

      switch (this->SliceOrientation)
        {
        
		//
        case MyImageViewer::SLICE_ORIENTATION_XY:
        default:
          xs = w_ext[1] - w_ext[0] + 1;
          ys = w_ext[3] - w_ext[2] + 1;
		  this->Renderer->GetActiveCamera()->Roll(180);
		  this->Renderer->GetActiveCamera()->Azimuth(180);
          break;

        case MyImageViewer::SLICE_ORIENTATION_XZ:
          xs = w_ext[1] - w_ext[0] + 1;
          ys = w_ext[5] - w_ext[4] + 1;
		  this->Renderer->GetActiveCamera()->Azimuth(180);
		  this->Renderer->GetActiveCamera()->Elevation(180);
          break;

        case MyImageViewer::SLICE_ORIENTATION_YZ:
          xs = w_ext[3] - w_ext[2] + 1;
          ys = w_ext[5] - w_ext[4] + 1;
		  //this->Renderer->GetActiveCamera()->Azimuth(180);
          break;
        }

      // if it would be smaller than 150 by 100 then limit to 150 by 100
      if (this->RenderWindow->GetSize()[0] == 0)
        {
        this->RenderWindow->SetSize(
          xs < 150 ? 150 : xs, ys < 100 ? 100 : ys);
        }

      if (this->Renderer)
        {
        this->Renderer->ResetCamera();
        
        this->Renderer->GetActiveCamera()->SetParallelScale(xs < 150 ? 75 : (xs - 1 ) / 3);
		d = this->Renderer->GetActiveCamera()->GetDistance();

		this->Renderer->GetActiveCamera()->SetParallelProjection(true);
        }
      this->FirstRender = 0; 
      }
    }
   
  if (this->GetInput())
    {
    this->RenderWindow->Render();
    }
}

//----------------------------------------------------------------------------
const char* MyImageViewer::GetWindowName()
{
  return this->RenderWindow->GetWindowName();
}

//----------------------------------------------------------------------------
void MyImageViewer::SetOffScreenRendering(int i)
{
  this->RenderWindow->SetOffScreenRendering(i);
}

//----------------------------------------------------------------------------
int MyImageViewer::GetOffScreenRendering()
{
  return this->RenderWindow->GetOffScreenRendering();
}

//----------------------------------------------------------------------------
void MyImageViewer::SetInputData(vtkImageData *in)
{
  this->WindowLevel->SetInputData(in);
  this->UpdateDisplayExtent();

  //Color Map
  double* range = in->GetScalarRange();
  this->SetColorWindow(range[1]-range[0]);
  this->SetColorLevel((range[1]+range[0])/2.0);
  DefaultWindowLevel[0] = range[1]-range[0];
  DefaultWindowLevel[1] = (range[1]+range[0])/2.0;

  //Cursor
  this->SetCursorBoundary();

}

void MyImageViewer::SetInputDataLayer(vtkImageData *in)
{
    this->WindowLevelLayer->SetInputData(in);
    this->UpdateDisplayExtent();
    this->ImageActorLayer->SetInputData(this->WindowLevelLayer->GetOutput());
    
    double* range = in->GetScalarRange();
    this->SetColorWindowLayer(range[1]-range[0]);
    this->SetColorLevelLayer((range[1]+range[0])/2.0);
}
//----------------------------------------------------------------------------
vtkAlgorithm* MyImageViewer::GetInputAlgorithm()
{
   return this->WindowLevel->GetInputAlgorithm();
}

vtkImageData* MyImageViewer::GetInput()
{ 
  return vtkImageData::SafeDownCast(this->WindowLevel->GetInput());
}
vtkInformation* MyImageViewer::GetInputInformation()
{
	return this->WindowLevel->GetInputInformation();
}
vtkImageData* MyImageViewer::GetInputLayer()
{
    return vtkImageData::SafeDownCast(this->WindowLevelLayer->GetInput());
}
//----------------------------------------------------------------------------
void MyImageViewer::SetInputConnection(vtkAlgorithmOutput* input)
{
    this->WindowLevel->SetInputConnection(input);
    this->UpdateDisplayExtent();
}


//----------------------------------------------------------------------------
#ifndef VTK_LEGACY_REMOVE
int MyImageViewer::GetWholeZMin()
{
  VTK_LEGACY_REPLACED_BODY(MyImageViewer::GetWholeZMin, "VTK 5.0",
                           MyImageViewer::GetSliceMin);
  return this->GetSliceMin();
}
int MyImageViewer::GetWholeZMax()
{
  VTK_LEGACY_REPLACED_BODY(MyImageViewer::GetWholeZMax, "VTK 5.0",
                           MyImageViewer::GetSliceMax);
  return this->GetSliceMax();
}
int MyImageViewer::GetZSlice()
{
  VTK_LEGACY_REPLACED_BODY(MyImageViewer::GetZSlice, "VTK 5.0",
                           MyImageViewer::GetSlice);
  return this->GetSlice();
}
void MyImageViewer::SetZSlice(int s)
{
  VTK_LEGACY_REPLACED_BODY(MyImageViewer::SetZSlice, "VTK 5.0",
                           MyImageViewer::SetSlice);
  this->SetSlice(s);
}
#endif

//----------------------------------------------------------------------------
void MyImageViewer::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);

  os << indent << "RenderWindow:\n";
  this->RenderWindow->PrintSelf(os,indent.GetNextIndent());
  os << indent << "Renderer:\n";
  this->Renderer->PrintSelf(os,indent.GetNextIndent());
  os << indent << "ImageActor:\n";
  this->ImageActor->PrintSelf(os,indent.GetNextIndent());
  os << indent << "ImageActorContour:\n";
  this->ImageActorLayer->PrintSelf(os,indent.GetNextIndent());
  os << indent << "WindowLevel:\n" << endl;
  this->WindowLevel->PrintSelf(os,indent.GetNextIndent());
  os << indent << "Slice: " << this->Slice << endl;
  os << indent << "SliceOrientation: " << this->SliceOrientation << endl;
  os << indent << "InteractorStyle: " << endl;
  if (this->InteractorStyle)
    {
    os << "\n";
    this->InteractorStyle->PrintSelf(os,indent.GetNextIndent());
    }
  else
    {
    os << "None";
    }
}

void MyImageViewer::SetCursorBoundary()
{
	double spacing[3];
	double origin[3];
	int extent[6];
	vtkImageData* image = vtkImageData::SafeDownCast(this->WindowLevel->GetInput());
	image->GetSpacing(spacing);
	image->GetOrigin(origin);
	image->GetExtent(extent);
	

	Bound[0] = origin[0];
	Bound[1] = origin[0] + extent[1]*spacing[0];
	Bound[2] = origin[1];
	Bound[3] = origin[1] + extent[3]*spacing[1];
	Bound[4] = origin[2];
	Bound[5] = origin[2] + extent[5]*spacing[2];

	Cursor3D->SetTranslationMode(false);
	Cursor3D->SetModelBounds(Bound);
	Cursor3D->Update();
}

void MyImageViewer::SetFocalPoint(double x, double y, double z)
{
	Cursor3D->SetFocalPoint(x,y,z);
	Cursor3D->Update();
	SliceImplicitPlane->SetOrigin(x,y,z);
}

void MyImageViewer::AddPolyData( vtkPolyData* polydata,vtkProperty* property )
{
	ClipActor = vtkActor::New();
	//vtkCutter* cutter = vtkCutter::New();
	//cutter->SetInputData (dataset);
	//cutter->SetCutFunction (this->SliceImplicitPlane);
	vtkPolyDataMapper* mapper = vtkPolyDataMapper::New();
	//mapper->SetInputData(cutter->GetOutput());
	mapper->SetInputData(polydata);
	mapper->Update();
	ClipActor = vtkActor::New();
	ClipActor->SetMapper (mapper);
	if (property)
		ClipActor->SetProperty (property);
	ClipActor->SetDragable(false);
	this->Renderer->AddActor(ClipActor);
	//cutter->Delete();
	mapper->Delete();
	//actor->Delete();
}

void MyImageViewer::SetBound(double* b)
{
	for (int i = 0; i < 6; i++)
	{
		Bound[i] = b[i];
	}
	//Cursor
	this->SetCursorBoundary();
	//Image Actor

	//this->ImageActor->Set
}

double* MyImageViewer::GetBound()
{
	return Bound;
}

double* MyImageViewer::GetDefaultWindowLevel()
{
	return DefaultWindowLevel;
}

double* MyImageViewer::GetFocalPoint()
{
	return Cursor3D->GetFocalPoint();
}

void MyImageViewer::RemoveInput()
{

	if (this->WindowLevel)
	{
		this->WindowLevel->Delete();
		this->WindowLevel = NULL;
	}
	if (this->WindowLevelLayer)
	{
		this->WindowLevelLayer->Delete();
		this->WindowLevelLayer = NULL;
	}
	if (this->ImageActor)
	{
		this->ImageActor->Delete();
		this->ImageActor = NULL;
	}
	if (this->Renderer)
	{
		this->Renderer->Delete();
		this->Renderer = NULL;
	}
	/*
	if (this->RenderWindow)
	{
		this->RenderWindow->Delete();
		this->RenderWindow = NULL;
	}
	*/
	if (this->Interactor)
	{
		this->Interactor->Delete();
		this->Interactor = NULL;
	}

	if (this->InteractorStyle)
	{
		this->InteractorStyle->Delete();
		this->InteractorStyle = NULL;
	}

	if (this->SliceImplicitPlane)
	{
		this->SliceImplicitPlane->Delete();
		this->SliceImplicitPlane = NULL;
	}
	
	/*
	//Remove Header
	if (HeaderActor != NULL)
        Renderer->RemoveActor2D(HeaderActor);
	
	//REmove Orientation Text
	for (int i = 0 ; i < 4 ; i++)
	{
		Renderer->RemoveActor2D(TextActor[i]);		
	}

	//Remove cursor bound
	if (CursorActor != NULL)
		Renderer->RemoveActor(CursorActor);
	
	if (this->WindowLevel)
		this->WindowLevel->Delete();
	this->WindowLevel = vtkImageMapToWindowLevelColors::New();


	*/
}

void MyImageViewer::RemoveInputLayer()
{
	if (this->ImageActorLayer)
	{
		this->ImageActorLayer->Delete();
		this->ImageActorLayer = NULL;
	}
	
	if (this->WindowLevelLayer)
		this->WindowLevelLayer->Delete();
	this->WindowLevelLayer = vtkImageMapToWindowLevelColors::New();

}
void MyImageViewer::InitializeHeader(QString File)
{
    int* size =  Renderer->GetSize();
    double margin = 15;
    int coord[2]		= {5,size[1]-(int)margin};
	
    if (HeaderActor != NULL){
        Renderer->RemoveActor2D(HeaderActor);
        //HeaderActor->Delete();
    }
    HeaderActor = vtkTextActor::New();
    HeaderActor->GetTextProperty()->SetFontSize ( 15 );
    HeaderActor->GetTextProperty()->SetColor ( 1.0, 0.7490, 0.0 );
    Renderer->AddActor2D ( HeaderActor );
    
    HeaderActor->SetDisplayPosition(coord[0],coord[1]);
    
    
    QByteArray Fileba = File.toLatin1();
    const char *Filestr = Fileba.data();
    
    
    if (GetInput()!=NULL)
        HeaderActor->SetInput(Filestr);
    else
        cout << "Error in setting text, file not found" << endl;
}

void MyImageViewer::InitializeIntensityText(QString IntText)
{
	int* size =  Renderer->GetSize();
    //double margin = 15;
    int coord[2]		= {5,5};
    
	if (this->FirstRender){
		IntTextActor = vtkTextActor::New(); 
		IntTextActor->GetTextProperty()->SetFontSize ( 15 );
		IntTextActor->GetTextProperty()->SetColor ( 1.0, 0.7490, 0.0 );
		Renderer->AddActor2D ( IntTextActor );
		IntTextActor->SetDisplayPosition(coord[0],coord[1]);
		return;
    }

    QByteArray IntTextba = IntText.toLatin1();
    const char *IntTextstr = IntTextba.data();
    
    
    if (GetInput()!=NULL)
        IntTextActor->SetInput(IntTextstr);
    else
        cout << "Error in setting text, file not found" << endl;
}

void MyImageViewer::InitializeOrientationText()
{
	int* size =  Renderer->GetSize();
	double margin = 15;
	
	int down[2]		= {size[0]/2	,5};
	int up[2]		= {size[0]/2	,size[1]-(int)margin};
	int left[2]		= {5			,size[1]/2};
	int right[2]	= {size[0]-(int)margin	,size[1]/2};

	for (int i=0;i<4;i++)
	{
		TextActor[i] = vtkTextActor::New();
		TextActor[i]->GetTextProperty()->SetFontSize ( 15 );		
		TextActor[i]->GetTextProperty()->SetColor ( 1.0, 0.7490, 0.0 );
		Renderer->AddActor2D ( TextActor[i] );		
	}

	TextActor[0]->SetDisplayPosition(up[0]		,up[1]);
	TextActor[1]->SetDisplayPosition(down[0]	,down[1]);
	TextActor[2]->SetDisplayPosition(left[0]	,left[1]);
	TextActor[3]->SetDisplayPosition(right[0]	,right[1]);

	switch (SliceOrientation)
	{
	case 0:
		TextActor[0]->SetInput ("S");
		TextActor[1]->SetInput ("I");
		TextActor[2]->SetInput ("A");
		TextActor[3]->SetInput ("P");
		break;
	case 1:
		TextActor[0]->SetInput ("S");
		TextActor[1]->SetInput ("I");
		TextActor[2]->SetInput ("R");
		TextActor[3]->SetInput ("L");
		break;
	case 2:
		TextActor[0]->SetInput ("A");
		TextActor[1]->SetInput ("P");
		TextActor[2]->SetInput ("R");
		TextActor[3]->SetInput ("L");
		break;
	}
}

void MyImageViewer::ResizeOrientationText()
{
	int* size =  Renderer->GetSize();

	double margin = 15;
    int coord[2]		= {5,size[1]-(int)margin};
	int down[2]		= {size[0]/2	,5};
	int up[2]		= {size[0]/2	,size[1]-(int)margin};
	int left[2]		= {5			,size[1]/2};
	int right[2]	= {size[0]-(int)margin	,size[1]/2};

	TextActor[0]->SetDisplayPosition(up[0]	,up[1]);
	TextActor[1]->SetDisplayPosition(down[0]	,down[1]);
	TextActor[2]->SetDisplayPosition(left[0]	,left[1]);
	TextActor[3]->SetDisplayPosition(right[0]	,right[1]);
    HeaderActor->SetDisplayPosition(coord[0],coord[1]);
}

void MyImageViewer::SetRulerEnabled( bool b )
{
	if (b)
	{
		if (DistanceWidget)
			DistanceWidget->Delete();

		DistanceWidget = vtkDistanceWidget::New();
		DistanceWidget->SetInteractor(RenderWindow->GetInteractor());
		DistanceWidget->SetPriority(RenderWindow->GetInteractor()->GetInteractorStyle()->GetPriority()+0.1);

		vtkSmartPointer< vtkPointHandleRepresentation2D > rulerHandleRep = vtkSmartPointer< vtkPointHandleRepresentation2D >::New();
		vtkSmartPointer< vtkDistanceRepresentation2D > distanceRep		 = vtkSmartPointer< vtkDistanceRepresentation2D >::New();
		distanceRep->SetHandleRepresentation(rulerHandleRep);
		DistanceWidget->SetRepresentation(distanceRep);
		distanceRep->InstantiateHandleRepresentation();
		distanceRep->SetLabelFormat("%-#11.2f mm");
		distanceRep->GetAxis()->GetProperty()->SetColor(0,1,0);

		DistanceWidget->CreateDefaultRepresentation();
		DistanceWidget->On();
	}
	else
	{
		DistanceWidget->Off();

	}		
	
	RenderWindow->Render();
}



void MyImageViewer::SetProtractorEnabled( bool b )
{
	if (b)
	{
		if (AngleWidget)
			AngleWidget->Delete();
		AngleWidget = vtkAngleWidget::New();
		AngleWidget->SetInteractor(RenderWindow->GetInteractor());
		AngleWidget->SetPriority(RenderWindow->GetInteractor()->GetInteractorStyle()->GetPriority()+0.1);
		vtkSmartPointer< vtkPointHandleRepresentation2D > angleHandleRep = vtkSmartPointer< vtkPointHandleRepresentation2D >::New();
		vtkSmartPointer< vtkAngleRepresentation2D > angleRep			 = vtkSmartPointer< vtkAngleRepresentation2D >::New();
		angleRep->SetHandleRepresentation(angleHandleRep);
		AngleWidget->SetRepresentation(angleRep);
		angleRep->InstantiateHandleRepresentation();
		angleRep->SetLabelFormat("%-#11.2f mm");
		angleRep->GetRay1()->GetProperty()->SetColor(0,1,0);
		angleRep->GetRay2()->GetProperty()->SetColor(0,1,0);

		AngleWidget->CreateDefaultRepresentation();
		AngleWidget->On();
	}
	else
	{
		AngleWidget->Off();

	}		

	RenderWindow->Render();
}
