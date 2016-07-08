#ifndef VESSELSEGMENTATION_H
#define VESSELSEGMENTATION_H



#include <vtkPolyData.h>
#include <vtkSmartPointer.h>

#include <SurfaceCreator.h>
#include <Define.h>
using namespace std;
using namespace itk;




class VesselSegmentation
{
public:
	VesselSegmentation();
	~VesselSegmentation();
	void					SetT1(FloatImageType::Pointer T1);
	void					SetMPRAGE(FloatImageType::Pointer MPRAGE);
	void					SetInputSegmentationImage(FloatImageType::Pointer inputSegmentation); //check if the input image data has scalar with unsigned short type
	void					GetOutputSegmentationImage(FloatImageType::Pointer outputSegmentation);
	void					GetVesselWallSurface(vtkPolyData* wallSurface);
	void					GetVesselLumenSurface(vtkPolyData* lumenSurface);
	void					GetPlaqueSurface(vtkPolyData* plaqueSurface);
	void 					GetOutputSegmentationLabelMap(LabelMapType::Pointer outputLabelMap);
	void					Update();


private:
	FloatImageType::Pointer			m_T1 = NULL;
	FloatImageType::Pointer			m_MPRAGE = NULL;
	FloatImageType::Pointer			m_inputSegmentation = NULL;
	FloatImageType::Pointer			m_outputSegmentation = NULL;
	FloatImageType::Pointer			m_maskedT1 = NULL;
	FloatImageType::Pointer			m_maskedMPRAGE = NULL;
	FloatImageType::Pointer			m_maskedT1_invert = NULL;
	FloatImageType::Pointer			m_wallImage = NULL;
	FloatImageType::Pointer			m_lumenImage = NULL;
	FloatImageType::Pointer			m_plaqueImage = NULL;
	LabelMapType::Pointer			m_wallLabelMap = NULL;
	LabelMapType::Pointer			m_lumenLabelMap = NULL;
	LabelMapType::Pointer			m_plaqueLabelMap = NULL;
	LabelMapType::Pointer			m_labelMap = NULL;
	bool							m_hasT1 = 0;
	bool							m_hasMPRAGE = 0;
	bool							m_hasInputSegmentation = 0;
	vtkSmartPointer<vtkPolyData>	m_wallSurface;
	vtkSmartPointer<vtkPolyData>	m_lumenSurface;
	vtkSmartPointer<vtkPolyData>	m_plaqueSurface;
};

#endif