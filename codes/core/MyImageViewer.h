#ifndef __MYIMAGEVIEWER_H__
#define __MYIMAGEVIEWER_H__

#include <vtkImageViewer2.h>
#include <vtkTextActor.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkLogLookupTable.h>
#include <vtkImageActor.h>
#include <vtkCursor3D.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkImageData.h>
#include <vtkExtractVOI.h>

#include <QObject>
#include <QString>

#include "Overlay.h"

/**
 * 
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
	/**
	 * @override
	 * @param	os,
	 * @param	indent
	 */
	void PrintSelf(ostream& os, vtkIndent indent);
	/**
	 * set the header by the MRI sequence name 
	 * @param	file, the MRI sequence name
	 */
	virtual void InitializeHeader(std::string file);
	virtual void InitializeHeader(QString file);
	/**
	 * Get the current focal point in image coordinate(extent), or in world coordinate
	 * @param	coordinate, focal point's coordinate
	 */
	virtual void GetFocalPointWithImageCoordinate(int* coordinate);
	virtual void GetFocalPointWithWorldCoordinate(double* coordinate);
	/**
	 * Get the current focal point in world coordinate
	 * @return	world coordinate of the focal point, don't modify it 
	 */
	virtual double* GetFocalPointWithWorldCoordinate();
	/**
	 * Get the Cursor bound of the Cursor 3d in world coordinate, don't modify it 
	 * @return	world coordinate of the cursor bound
	 */
	virtual double* GetCursorBoundWithWorldCoordinate();
	/**
	 * return the all black flag
	 * @return	true, if in all black
	 *			false, if not
	 */
	virtual bool GetAllBlack();

	/**
	 * @override
	 * Render method
	 */
	virtual void Render(void);
	virtual void ResetCamera(void);

	/**
	 * Set/Get the input image to the viewer.
	 * the GetInput() method return image after VOI extraction
	 * @param	in, input image
	 */
	virtual void SetInputData(vtkImageData * in);
	/**
	 * Set/Get the input image to the viewer.
	 * @return	return image after VOI extraction 
	 */
	virtual vtkImageData* GetInput();
	/**
	 * Get original input vtkImageData image before vtkExtractVOI filter
	 * @return	original input
	 */
	virtual vtkImageData* GetOriginalInput();
	// Set/Get Input Layer which is supposed to be the output of overlay
	virtual void SetInputDataLayer(vtkImageData *in);
	virtual vtkImageData *GetInputLayer();
	// Set/Get Overlay
	virtual void SetOverlay(Overlay* overlay);
	virtual Overlay* GetOverlay();



	virtual void SetImageVOI(int* extent);
	virtual void ResetImageVOI();

	virtual void SetOverlayVOI(int* extent);
	virtual void ResetOverlayVOI();

	//// Description:
	//// return DefaultWindowLevel
	//virtual double* GetDefaultWindowLevel();
	
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
	 * Get/Set method of LookupTable
	 * @return	lookupTable
	 */
	virtual vtkLookupTable* GetLookupTable();
	/**
	 * Get/Set method of LookupTable
	 * @param	LookupTable
	 */
	virtual void SetLookupTable(vtkLookupTable* LookupTable);

	/**
	 * Attach an interactor for the internal render window.
	 * @param	arg, interactor
	 */
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
	 * @deprecated
	 * @slot
	 * set the window level and window width of the image
	 * or the same saying image contrast
	 * @param	level window level
	 */
	virtual void SetColorLevel(double level);
	/**
     * @deprecated
	 * @slot
	 * set the window level and window width of the image
	 * or the same saying image contrast
	 * @param	window window width
	 */
	virtual void SetColorWindow(double window);
	/**
	 * @slot
	 * Setting and Getting all visibility of all actor, which means it will somehow look 
	 * like turn off, it will invoke Render()
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
	MyImageViewer(QObject* parent = nullptr);
	~MyImageViewer();

	// Text Method
	virtual void ResizeHeaderAndOrientationText();
	virtual void InitializeIntensityText(QString IntText);
	virtual void InitializeOrientationText();
	// Cursor method
	virtual void InitializeCursorBoundary();

	virtual void InstallPipeline();
	virtual void UnInstallPipeline();
	virtual void UpdateOrientation();

	vtkExtractVOI* ImageExtractVOI;
	vtkExtractVOI* OverlayExtractVOI;


	//OrientationText
	vtkTextActor*	OrientationTextActor[4] = { nullptr };

	//Header
	vtkTextActor* HeaderActor = nullptr;

	// IntensityText
	vtkTextActor* IntTextActor = nullptr;

	// Overlay
	Overlay* SegmentationOverlay = nullptr;
	vtkImageMapToWindowLevelColors* OverlayWindowLevel = nullptr;
	vtkImageActor* OverlayActor = nullptr;

	//Cursor
	vtkCursor3D*		 Cursor3D = nullptr;
	vtkPolyDataMapper* CursorMapper = nullptr;
	vtkActor*			 CursorActor = nullptr;

	//Label and Annotation
	vtkRenderer* AnnotationRenderer = nullptr;

	// LookupTable for OverlayActor
	vtkLookupTable* LookupTable = nullptr;


	////Parameter
	//double DefaultWindowLevel[2] = { 0 };

	// All Black flag
	bool AllBlackFlag = false;

private:
	MyImageViewer(const MyImageViewer&);  // Not implemented.
	void operator=(const MyImageViewer&);  // Not implemented.
	inline vtkAlgorithm* GetInputAlgorithm() {
		return vtkImageViewer2::GetInputAlgorithm(); }
	inline vtkInformation* GetInputInformation() { 
		return vtkImageViewer2::GetInputInformation(); }
	inline void SetInputConnection(vtkAlgorithmOutput* input) {
		return vtkImageViewer2::SetInputConnection(input);
	}


};
#endif // !__MYIMAGEVIEWER_H__
