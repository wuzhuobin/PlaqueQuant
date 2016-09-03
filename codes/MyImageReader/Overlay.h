#ifndef OVERLAY_H
#define OVERLAY_H

#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include <QObject>

#include <itkImage.h>
typedef itk::Image<float, 3> ImageType;


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
	void SetInputImageData(ImageType::Pointer imageData);
	void SetInputImageData(vtkImageData* imageData);
	void SetInputImageData(QString fileName);
	void SetInputImageData(const char* fileName);

	void SetPixel(int pos[3], double value);
	void SetPixel(int pos[3], unsigned char value);

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
	ImageType::Pointer GetITKOutput(ImageType::Pointer format);

private:

	vtkSmartPointer<vtkImageData> m_vtkOverlay;
	ImageType::Pointer m_itkOverlay;
	int m_visible_image_no;

	int DisplayExtent[6];
};

#endif

