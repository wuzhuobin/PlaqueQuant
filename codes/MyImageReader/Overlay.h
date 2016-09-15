#ifndef OVERLAY_H
#define OVERLAY_H

#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include <vtkLookupTable.h>

#include <QObject>

#include <itkImage.h>
typedef itk::Image<float, 3> ImageType;


class Overlay : public QObject
{
	Q_OBJECT

public:

	const int* m_overlayColor[7];
	const int zeros[4] = {0,0,0,0};
	const int lumen[4] = { 255, 0, 0, 255 };
	const int vesselWall[4] = { 0, 0, 255, 255 };
	const int calcification[4] = { 0,255,0,255 };
	const int hemorrhage[4] = { 255, 255, 0,255 };
	const int lrnc[4] = { 0, 255, 255 ,255 };
	const int lm[4] = { 255, 0, 255 ,255 };

	Overlay(QObject* parent = NULL);
	~Overlay();
	bool Update();
	/**
	 * It is supposed to use the first vtkImageData to initialize the overlay
	 * it will deepcopy the img and set all voxels of the copy to 0
	 * @param	img for deepcopy
	 */
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
	/**
	 *
	 */
	void SetPixels(std::list<int[3]> positions, std::list<int> values);


	vtkImageData* GetOutput();
	/**
	 * @deprecated
	 * the same as GetOutput()
	 */
	vtkImageData* GetVTKOutput();
	/**
	 * convert the m_vtkOverlay to itk::ImageData and save it in m_itkOverlay
	 * then return it with the same spacing, origin and direction as the format image
	 * @param	format set the output image direction the same as the format image
	 * @return	m_itkOverlay
	 */
	ImageType::Pointer GetITKOutput(ImageType::Pointer format);
	/**
	 * Get the lookupTable of this overlay which is generate by the constant
	 * @return	m_lookupTable
	 */
	vtkLookupTable* GetLookupTable();

private:

	vtkSmartPointer<vtkImageData> m_vtkOverlay;
	ImageType::Pointer m_itkOverlay;
	vtkSmartPointer<vtkLookupTable> m_lookupTable;
	int DisplayExtent[6];
};

#endif

