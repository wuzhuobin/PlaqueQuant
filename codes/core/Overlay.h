#ifndef OVERLAY_H
#define OVERLAY_H

#include <vtkExtractVOI.h>
#include <vtkImageData.h>
//#include <vtkVersion.h>
#include <QObject>
#include "Define.h"
typedef itk::Image<float, 3> FloatImageType;

class Overlay : public QObject
{
	Q_OBJECT

public:
	Overlay(QObject* parent = NULL);
	~Overlay();
	bool Update();
	
	void Initialize(vtkImageData* img);
	/**
	 * @deprecated
	 */
	void Initialize(ImageType::Pointer, int dim[3], double spacing[3], double origin[3], int type);

	void SetDisplayExtent(int*);
	int* GetDisplayExtent();
	void GetDisplayExtent(int*);

	void DisplayExtentOn();
	void DisplayExtentOff();
	/**
	 * set input image data of overlay m_vtkOverlay using itk::image
	 * convert itk::image to vtkImage 
	 * @param	imageData the new input image
	 */
	void SetInputImageData(FloatImageType::Pointer imageData);
	void SetInputImageData(vtkImageData* imageData);
	void SetInputImageData(QString fileName);
	void SetInputImageData(const char* fileName);

	void SetPixel(int pos[3], double value);
	void SetPixel(int pos[3], unsigned char value);
	void SetVisibleImageNo(int);

	vtkImageData* GetOutput();
	/**
	 * @deprecated
	 */
	vtkImageData* GetVTKImageData();
	/**
	 * convert the m_vtkOverlay to itk::ImageData and save it in m_itkOverlay
	 * then return it with the same spacing, origin and direction as the format image
	 * @param	format set the output image direction the same as the format image
	 * @return	m_itkOverlay
	 */
	FloatImageType::Pointer GetITKOutput(ImageType::Pointer format);

private:

	vtkImageData* m_vtkOverlay;
	vtkImageData* m_vtkOverlayHolder;
	ImageType::Pointer m_itkOverlay;
	DuplicatorType::Pointer m_duplicator;
	//itk::VTKImageToImageFilter<FloatImageType>::Pointer vtkImageToImageFilter;
	//vtkSmartPointer<vtkImageCast> imageCastFilter;
	int m_visible_image_no;

	int DisplayExtent[6];
};

#endif

