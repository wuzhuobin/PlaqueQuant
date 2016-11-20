#ifndef OVERLAY_H
#define OVERLAY_H

#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include <vtkLookupTable.h>

#include <QObject>
#include <qmap.h>

#include <itkImage.h>

#include "MeasurementFor3D.h"
#include "MeasurementFor2D.h"
#include "MaximumWallThickness.h"

typedef itk::Image<float, 3> ImageType;

/**
 * @class Overlay
 * @author WUZHUOBIN
 * @version
 * @since
 * A very important part of element for using other InteractorStyle
 * the vtkImageData scalar type now is set to float
 */
class Overlay : public QObject
{
	Q_OBJECT

public:

	static const int NUMBER_OF_COLOR = 7;
	const int* m_overlayColor[NUMBER_OF_COLOR];
	const int zeros[4] = {0,0,0,0};
	const int lumen[4] = { 255, 0, 0, 255 };
	const int vesselWall[4] = { 0, 0, 255, 255 };
	const int calcification[4] = { 0,255,0,255 };
	const int hemorrhage[4] = { 255, 255, 0,255 };
	const int lrnc[4] = { 0, 255, 255 ,255 };
	const int lm[4] = { 255, 0, 255 ,255 };

	// 2D parts data
	struct MeasurementFor2DIntegrated
	{
		vtkSmartPointer<MeasurementFor2D> m2d =
			vtkSmartPointer<MeasurementFor2D>::New();
		vtkSmartPointer<MaximumWallThickness> mwt =
			vtkSmartPointer<MaximumWallThickness>::New();
	};

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
	 * don't use it
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
	void ReplacePixels(int* extent, vtkImageData* image);

	void vtkShallowCopyImage(vtkImageData* image);


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
	/**
	* Calculate the 3D measurements of the overlay, which are the volume information
	*/
	void Measure3D();
	/**
	* Calculate the 2D measurements of the overlay, which are the area information and
	* distance information
	*/
	void Measure2D();
	void Measure2D(int slice);

	/***/
	MeasurementFor3D GetMeasurementFor3D();
	double* Get3DMeasurementsAsDouble();
	QStringList Get3DMeasurementsStrings();

	// 2D parts data
	MeasurementFor2DIntegrated GetMeasurementFor2D(int slice);
	QStringList Get2DMeasurementsStrings(int slice);

	// Get contour PolyData*
	QMap<int, QList<vtkSmartPointer<vtkPolyData>>*>* GetVesselWallPolyData();
	QMap<int, QList<vtkSmartPointer<vtkPolyData>>*>* GetLumenPolyData();
	std::map<int, QList<vtkSmartPointer<vtkPolyData>>*>* GetVesselWallPolyDataSTD();
	std::map<int, QList<vtkSmartPointer<vtkPolyData>>*>* GetLumenPolyDataSTD();


signals:
	void signalOverlayUpdated();

private:

	// ImageData
	vtkSmartPointer<vtkImageData> m_vtkOverlay;
	ImageType::Pointer m_itkOverlay;

	vtkSmartPointer<vtkLookupTable> m_lookupTable;

	int DisplayExtent[6];

	// 3D parts data
	// volumeofTotalPlaque, volumeOfLumen, volumeOfVesselWall, volumeOfCalcification
	// volumeOfHemorrhage, volumeOfLRNC, volumeOfLM
	MeasurementFor3D m_measurementFor3D;

	// 2D parts data
	// using QMap for the case that the extent do not begin at 0
	// lumen area, vessel area, NMI, distance, distance error imfo
	QMap<int, MeasurementFor2DIntegrated*> m_measurementFor2D;
	QMap<int, QStringList> m_2DMeasurements;
	QStringList m_volumeStrings;

	// contour parts
	QMap<int, QList<vtkSmartPointer<vtkPolyData>>*> m_vesselWallPolyData;
	QMap<int, QList<vtkSmartPointer<vtkPolyData>>*> m_lumenPolyData;

};

#endif

