#ifndef __IMAGE_VIEWER_H__
#define __IMAGE_VIEWER_H__

#include <vtkImageViewer2.h>

class vtkLookupTable;
class vtkTextActor;
class vtkImageMapToColors;
class vtkImageActor;
class vtkCursor3D;
class vtkPolyDataMapper;
class vtkActor;
class vtkResizeHeaderAndOrientationTextCallback;

/**
 * 
 * @class	ImageViewer
 * @author	WUZHUOBIN
 * @version	
 * @since	
 */

class ImageViewer: public vtkImageViewer2
{
public:
	static ImageViewer *New();
	vtkTypeMacro(ImageViewer, vtkImageViewer2);
	/**
	 * @override
	 * @param	os,
	 * @param	indent
	 */
	void PrintSelf(ostream& os, vtkIndent indent);
	/**
	 * @override
	 * Render method
	 */
	virtual void Render(void);
	/**
	 * set the header by the MRI sequence name 
	 * @param	file, the MRI sequence name
	 */
	virtual void InitializeHeader(std::string file);
	/**
	 * @brief re-locate the annotation text 
	 */
	virtual void ResizeHeaderAndOrientationText();
	/**
	 * Get the current focal point in image coordinate(extent)
	 * @param	coordinate, focal point's coordinate
	 */
	virtual void GetFocalPointWithImageCoordinate(int* coordinate);
	/**
	* Get the current focal point in world coordinate
	* @param	coordinate, focal point's coordinate
	*/
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
	* the following methods of setting focal point will also change current slice and
	* the cursor3D focal point with world coordinate or image coordinate
	* @param	x	world coordinate
	* @param	y
	* @param	z
	*/
	virtual void SetFocalPointWithWorldCoordinate(double x, double y, double z);
	/**
	* the following methods of setting focal point will also change current slice and
	* the cursor3D focal point with world coordinate or image coordinate
	* @param	i	image coordinate
	* @param	j
	* @param	k
	*/
	virtual void SetFocalPointWithImageCoordinate(int i, int j, int k);

	/**
	 * return the all black flag
	 * @return	true, if in all black
	 *			false, if not
	 */
	virtual bool GetAllBlack();

	/**
	 * Set/Get the input image to the viewer.
	 * @param	in, input image
	 */
	virtual void SetInputData(vtkImageData * in);

	// Set/Get Input Layer which is supposed to be the output of overlay
	virtual void SetOverlay(vtkImageData *in);
	virtual vtkImageData *GetOverlay();

	virtual void SetSliceOrientation(int orientation);

	// Description:
	// Get the internal render window, renderer, image actor, and
	// image map instances.
	vtkGetObjectMacro(OverlayActor, vtkImageActor);
	vtkGetObjectMacro(OverlayImageMapToColors, vtkImageMapToColors);

	/**
	 * Get/Set method of LookupTable
	 * @param	LookupTable
	 * @return	lookupTable
	 */
	vtkGetObjectMacro(LookupTable, vtkLookupTable);
	virtual void SetLookupTable(vtkLookupTable* LookupTable);

	vtkBooleanMacro(EnableDepthPeeling, bool);
	virtual void SetEnableDepthPeeling(bool flag);

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
	vtkGetVector6Macro(DisplayExtent, int);
	virtual void SetDisplayExtent(int* displayExtent);
	virtual void SetDisplayExtent(int displayExtent1, int displayExtent2, int displayExtent3,
		int displayExtent4, int displayExtent5, int displayExtent6);
	virtual void UpdateDisplayExtent();
	virtual void ResetDisplayExtent();
	virtual void GetSliceRange(int &min, int &max);
	virtual int* GetSliceRange();
	/**
	* set the window level and window width of the image
	* or the same saying image contrast
	* @param	level window level
	*/
	virtual void SetColorLevel(double level);
	/**
	* set the window level and window width of the image
	* or the same saying image contrast
	* @param	window window width
	*/
	virtual void SetColorWindow(double window);

	/**
	 * Setting and Getting all visibility of all actor, which means it will somehow look 
	 * like turn off, it will invoke Render()
	 * it will also set AllBlackFlag = flag
	 * @param	flag 
	 */
	virtual void SetAllBlack(bool flag);


protected:
	ImageViewer();
	~ImageViewer();


	// Cursor method
	virtual void InitializeCursorBoundary();

	virtual void InstallPipeline();
	virtual void UnInstallPipeline();
	virtual void UpdateOrientation();

	//OrientationText
	vtkTextActor*	OrientationTextActor[4] = { nullptr };

	//Header
	vtkTextActor* HeaderActor = nullptr;

	// IntensityText
	vtkTextActor* IntTextActor = nullptr;

	// Overlay
	vtkImageMapToColors* OverlayImageMapToColors = nullptr;
	vtkImageActor* OverlayActor = nullptr;

	//Cursor
	vtkCursor3D*		 Cursor3D = nullptr;
	vtkPolyDataMapper* CursorMapper = nullptr;
	vtkActor*			 CursorActor = nullptr;

	// LookupTable for OverlayActor
	vtkLookupTable* LookupTable = nullptr;

	// All Black flag
	bool AllBlackFlag = false;

	int DisplayExtent[6] = { -1 };

	// MaxNoOfPeels maximum number of depth peels(multi - pass rendering)
	int MaxNoOfPeels = 0;
	// OcclusionRatio the occlusion ration(0.0 means a perfect image,
	// >0.0 means a non - perfect image which in general results in faster rendering)
	double OcclusionRatio = 0.0;

	//friend class vtkResizeHeaderAndOrientationTextCallback;

private:
	ImageViewer(const ImageViewer&);  // Not implemented.
	void operator=(const ImageViewer&);  // Not implemented.
	void SetInputConnection(vtkAlgorithmOutput* input) {} // Not implemented.
};
#endif // !__IMAGE_VIEWER_H__
