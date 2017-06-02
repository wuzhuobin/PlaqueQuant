//#ifndef __RESLICE_IMAGE_VIEWER_H__
//#define __RESLICE_IMAGE_VIEWER_H__
//
//#include "ImageViewer.h"
//
//class vtkResliceCursorWidget;
//class vtkResliceCursor;
//class vtkScalarsToColors;
//class vtkBoundedPlanePointPlacer;
//class vtkResliceImageViewerMeasurements;
//class vtkResliceImageViewerScrollCallback;
//class vtkPlane;
//
//class ResliceImageViewer: public ImageViewer
//{
//public:
//	// Description:
//	// Standard VTK methods.
//	static ResliceImageViewer *New();
//	vtkTypeMacro(ResliceImageViewer, ImageViewer);
//	void PrintSelf(ostream& os, vtkIndent indent);
//
//	// Description:
//	// Set/get the slice orientation
//	//BTX
//	enum
//	{
//		RESLICE_AXIS_ALIGNED = 0,
//		RESLICE_OBLIQUE = 1
//	};
//	//ETX
//
//	vtkGetMacro(ResliceMode, int);
//	virtual void SetResliceMode(int resliceMode);
//	virtual void SetResliceModeToAxisAligned()
//	{
//		this->SetResliceMode(ResliceImageViewer::RESLICE_AXIS_ALIGNED);
//	};
//	virtual void SetResliceModeToOblique()
//	{
//		this->SetResliceMode(ResliceImageViewer::RESLICE_OBLIQUE);
//	};
//
//protected:
//
//	ResliceImageViewer();
//	~ResliceImageViewer();
//
//private:
//
//};
//
//#endif // !__RESLICE_IMAGE_VIEWER_H__
