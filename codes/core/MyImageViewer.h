#ifndef __MYIMAGEVIEWER_H__
#define __MYIMAGEVIEWER_H__

#include <vtkObjectFactory.h>
#include <vtkImageViewer2.h>
#include <vtkDistanceWidget.h>
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

/**
 * @class	MyImageViewer
 * @author	WUZHUOBIN
 * @version	
 * @since	
 */

class MyImageViewer : public QObject, public vtkImageViewer2
{
	Q_OBJECT
public:
	static MyImageViewer *New();
	vtkTypeMacro(MyImageViewer, vtkImageViewer2);
	void PrintSelf(ostream& os, vtkIndent indent);
	// Text methods
	virtual void InitializeHeader(std::string file);
	virtual void InitializeHeader(QString file);

	virtual void GetFocalPointWithImageCoordinate(int* coordinate);
	virtual void GetFocalPointWithWorldCoordinate(double* coordinate);
	virtual double* GetFocalPointWithWorldCoordinate();
	virtual double* GetCursorBoundWithWorldCoordinate();

	virtual bool GetAllBlack();

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
	vtkGetObjectMacro(OverlayActor, vtkImageActor);
	vtkGetObjectMacro(OverlayWindowLevel, vtkImageMapToWindowLevelColors);
	vtkGetObjectMacro(AnnotationRenderer, vtkRenderer);
	// Description:
	// Set your own Annotation Renderer
	virtual void SetAnnotationRenderer(vtkRenderer *arg);

	/**
	 * LookupTable
	 */
	virtual vtkLookupTable* GetLookupTable();
	virtual void SetLookupTable(vtkLookupTable* LookupTable);

	// Description:
	// Attach an interactor for the internal render window.
	virtual void SetupInteractor(vtkRenderWindowInteractor* arg);

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
	/**
	 * the method only change the slice, while focal point of Cursor3D will not change
	 * @param	s slice 
	 */
	virtual void SetSlice(int s);

	/**
	 * @slot
	 * the following methods of setting focal point will also change current slice and 
	 * the cursor3D focal point with world coordinate or image coordinate
	 * @param	x	world coordinate
	 * @param	y
	 * @param	z
	 */
	virtual void SetFocalPointWithWorldCoordinate(double x, double y, double z);
	/**
	 * @slot
	 * the following methods of setting focal point will also change current slice and
	 * the cursor3D focal point with world coordinate or image coordinate
	 * @param	i	image coordinate
	 * @param	j
	 * @param	k
     */
	virtual void SetFocalPointWithImageCoordinate(int i, int j, int k);
	/**
	 * @slot
	 * set the window level and window width of the image
	 * or the same saying image contrast
	 * @param	level window level
	 */
	virtual void SetColorLevel(double level);
	/**
	 * @slot
	 * set the window level and window width of the image
	 * or the same saying image contrast
	 * @param	window window width
	 */
	virtual void SetColorWindow(double window);
	/**
	 * @slot
	 * Setting and Getting all visibility of all actor, which means it will somehow look 
	 * like turn off
	 * it will also set AllBlackFlag = flag
	 * @param	flag 
	 */
	virtual void SetAllBlack(bool flag);

signals:
	
	void FocalPointWithImageCoordinateChanged(int i, int j, int k);
	/**
	 * @deprecated
	 */
	void SliceChanged(int slice);
	void ColorLevelChanged(double colorLevel);
	void ColorWindowChanged(double colorWindow);
	void AllBlackAlready(bool flag);

protected:
	MyImageViewer(QObject* parent = NULL);
	~MyImageViewer();

	// Text Method
	virtual void ResizeHeaderAndOrientationText();
	virtual void InitializeIntensityText(QString IntText);
	virtual void InitializeOrientationText();

	virtual void InstallPipeline();
	virtual void UnInstallPipeline();
	virtual void UpdateOrientation();

	//OrientationText
	vtkTextActor*	OrientationTextActor[4] = { NULL };

	//Header
	vtkTextActor* HeaderActor = NULL;

	// IntensityText
	vtkTextActor* IntTextActor = NULL;

	// Overlay
	Overlay* SegmentationOverlay = NULL;

	//Cursor
	vtkCursor3D*		 Cursor3D = NULL;
	vtkPolyDataMapper* CursorMapper = NULL;
	vtkActor*			 CursorActor = NULL;
	virtual void InitializeCursorBoundary();

	//Label and Annotation
	vtkImageMapToWindowLevelColors* OverlayWindowLevel = NULL;
	vtkImageActor* OverlayActor = NULL;
	vtkRenderer* AnnotationRenderer = NULL;

	// LookupTable for OverlayActor
	vtkLookupTable* LookupTable = NULL;


	//Parameter
	double DefaultWindowLevel[2] = { 0 };

	// All Black flag
	bool AllBlackFlag = false;

private:
	MyImageViewer(const MyImageViewer&);  // Not implemented.
	void operator=(const MyImageViewer&);  // Not implemented.
};
#endif // !__MYIMAGEVIEWER_H__
