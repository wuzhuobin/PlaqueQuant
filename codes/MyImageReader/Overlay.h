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
	/**
	 * @deprecated
	 */
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
	/**
	 * set the overlay with value in pos[3]
	 * @param	the coordinate of the pixel to be set to value
	 * @param	the value of the pixel to be set
	 */
	void SetPixel(int pos[3], double value);
	/**
	 * @deprecated
	 */
	void SetPixel(int pos[3], unsigned char value);
	/**
	 * replace all the pixels in the extent of the overlay
	 * the extent is supposed be a slice of image, so there should be 2 same extent
	 * when the replacing is finished, it emits signalOverlayUpdated()
	 * @param	extent 2 exents should be the same to determine the direction
	 * @param	image the image to replace the overlay in the extent
	 */
	void SetPixels(int* extent, vtkImageData* image);
	/**
	 * replace all the pixels specified by the @param points with the @param label
	 */
	void SetPixels(vtkPoints* points, int label);
	/***/
	void Measure3D();
	void Measure2D();
	void Measure2D(int slice);

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
	/***/
	double* Get3DMeasurements();
	QStringList Get3DMeasurementsStrings();
	QStringList Get2DMeasurementsStrings(int slice);

signals:
	void signalOverlayUpdated();

private:

	vtkSmartPointer<vtkImageData> m_vtkOverlay;
	ImageType::Pointer m_itkOverlay;
	vtkSmartPointer<vtkLookupTable> m_lookupTable;

	int DisplayExtent[6];
	double* m_3DMeasurements = nullptr;
	// lumen area, vessel area, NMI, distance, distance error imfo
	QList<QStringList> m_2DMeasurements;
	QStringList m_volumeStrings;

};

#endif

