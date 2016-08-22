#ifndef OVERLAY_H
#define OVERLAY_H

#include <vtkImageData.h>
#include <vtkVersion.h>


#include <QObject>
#include "Define.h"
typedef itk::Image<float, 3> FloatImageType;

class Overlay : public QObject
{
	Q_OBJECT

public:
	Overlay(QObject* parent = NULL);
	~Overlay();
	void SetInputImageData(vtkImageData* imagedata);
	void SetInputImageData(QString);
	bool Update();
	void Initialize(vtkImageData* img);
	void Initialize(ImageType::Pointer, int dim[3], double spacing[3], double origin[3], int type);
	void SetPixel(int pos[3], double value);
	void SetPixel(int pos[3], unsigned char value);
	void SetVisibleImageNo(int);
	vtkImageData* GetOutput();
	/**
	 * convert the m_vtkOverlay to itk::ImageData and save it in m_itkOverlay
	 * then return it with the same spacing, origin and direction as the format image
	 * @param	format set the output image direction the same as the format image
	 * @return	m_itkOverlay
	 */
	FloatImageType::Pointer GetITKOutput(ImageType::Pointer format);

private:
	vtkImageData* m_vtkOverlay;
	ImageType::Pointer m_itkOverlay;
	DuplicatorType::Pointer m_duplicator;
	//itk::VTKImageToImageFilter<FloatImageType>::Pointer vtkImageToImageFilter;
	//vtkSmartPointer<vtkImageCast> imageCastFilter;
	int m_visible_image_no;
};

#endif

