#ifndef LUMEN_EXTRACTION_H
#define LUMEN_EXTRACTION_H

#include "vtkImageData.h"
#include "vtkSmartPointer.h"
#include "vtkMarchingCubes.h"
#include "vtkPolyData.h"
#include "vtkPolyDataConnectivityFilter.h"
#include "vtkPoints.h"
#include "vtkCellArray.h"

#include "itkImage.h"
#include "itkLabelObject.h"
#include "itkLabelMap.h"
#include "itkDefaultDynamicMeshTraits.h"
#include "itkMesh.h"
#include "itkTriangleCell.h"
#include "itkTriangleMeshToBinaryImageFilter.h"
#include <itkVTKImageToImageFilter.h>		
#include <itkCastImageFilter.h>

#include <itkIndex.h>

#include <itkImageFileWriter.h>
#include <QList>
#include "Define.h"

using namespace std;

#ifndef vtkDoubleType
#define vtkDoubleType double
#endif


class LumenExtraction
{
public:
	LumenExtraction();
	~LumenExtraction();

	void SetInputData(vtkImageData* image);
	void SetDilationValue(double dilationVal);
	void AddSeed(double* seed);
	void SetInitialNeighborhoodRadius(double radius);
	void SetMultiplier(double multiplier);
	void SetNumberOfIterations(double itr);
	void GetInputItkImage(ImageType::Pointer itkImage);
	void GetLumenItkImage(ImageType::Pointer itkLumenImage);
	void GetLumenVtkImage(vtkImageData* vtkLumenImage);
	void GetDilatedItkImage(ImageType::Pointer dilatedImage);
	void GetDilatedVtkImage(vtkImageData* dilatedImage);
	void Update();

private:
	vtkImageData* m_input;
	double m_isoVal;
	vtkPolyData* m_lumenSurface;
	vtkPolyData* m_dilatedSurface;
	double m_dilationVal;
	ImageType::Pointer m_itkLumenImage;
	ImageType::Pointer m_inputItkImage;
	ImageType::Pointer m_dilatedItkImage;
	vtkImageData* m_dilatedVtkImage;
	vtkImageData* m_vtkLumenImage;
	QList<double*> m_seedList;
	double m_initialNeighborhoodRadius;
	double m_mulitplier;
	double m_iterations;

	void LabelDilation();
};

#endif