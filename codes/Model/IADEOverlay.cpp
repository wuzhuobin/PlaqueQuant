#include "IADEOverlay.h"

#include <vtkLookupTable.h>

#include <vtkImageAccumulate.h>
#include <vtkExtractVOI.h>
#include <vtkPolyData.h>
IADEOverlay::IADEOverlay(QObject * parent)
	:Overlay(parent)
{
	// using m_overlayColor to build the lookupTable
	m_overlayColor[0] = zeros;
	m_overlayColor[1] = vesselWall;
	m_overlayColor[2] = lumen;

	m_lookupTable = vtkSmartPointer<vtkLookupTable>::New();
	m_lookupTable->SetNumberOfTableValues(NUMBER_OF_COLOR);
	m_lookupTable->SetTableRange(0.0, NUMBER_OF_COLOR - 1);
	for (int i = 0; i < NUMBER_OF_COLOR; ++i) {
		double rgba[4];
		for (int j = 0; j < 4; ++j) {
			rgba[j] = m_overlayColor[i][j] / 255;
		}
		m_lookupTable->SetTableValue(i, rgba);
	}
	m_lookupTable->Build();
	
	m_centerline = vtkSmartPointer<vtkPolyData>::New();
}

IADEOverlay::IADEOverlay(OverlayImageData::itkImageType::Pointer data, QObject * parent)
	:IADEOverlay(parent)
{
	m_data->Graft(data);
}

IADEOverlay::IADEOverlay(OverlayImageData * data, QObject * parent)
	:IADEOverlay(parent)
{
	m_data->ShallowCopy(data);
}

vtkPolyData * IADEOverlay::getCenterLine()
{
	return this->m_centerline;
}

void IADEOverlay::updatedOverlay()
{
	updateMeasurement3D();
	updateMeasurement2D(currentSlice);
	Overlay::updatedOverlay();
}

//void IADEOverlay::updateMeasurement3D()
//{
//	// calculate the volume of a single voxel
//	double voxelSize = 1;
//	for (int i = 0; i < 3; ++i) {
//		voxelSize *= m_data->GetSpacing()[i];
//	}
//	vtkSmartPointer<vtkImageAccumulate> imageAccumulate =
//		vtkSmartPointer<vtkImageAccumulate>::New();
//	imageAccumulate->SetInputData(m_data);
//	imageAccumulate->SetComponentExtent(0, NUMBER_OF_COLOR - 1, 0, 0, 0, 0); // #LookupTable
//	imageAccumulate->SetComponentOrigin(0, 0, 0);
//	imageAccumulate->SetComponentSpacing(1, 0, 0);
//	imageAccumulate->Update();
//	for (int i = 1; i < NUMBER_OF_COLOR; ++i) {
//		Measurements3D[i] = *static_cast<int*>(
//			imageAccumulate->GetOutput()->GetScalarPointer(i, 0, 0)) * voxelSize;
//		if (i == 1) {
//			// initialize total plaque
//			Measurements3D[0] = 0;
//		}
//		else if (i > 2) {
//			// total plaque
//			Measurements3D[0] += Measurements3D[i];
//		}
//	}
//	cerr << "Measurements3D" << endl;
//	for (int i = 0; i < NUMBER_OF_COLOR; ++i) {
//		cerr << Measurements3D[i] << ' ';
//	}
//	cerr << endl;
//}

void IADEOverlay::setCurrentSlice(int slice)
{
	this->currentSlice = slice;
	updateMeasurement2D(this->currentSlice);
	Overlay::updatedOverlay();
}

int IADEOverlay::getCurrentSlice()
{
	return this->currentSlice;
}

//void IADEOverlay::updateMeasurement2D(int slice)
//{
//	// calculate the volume of a single voxel
//	double pixelSize = 1;
//	for (int i = 0; i < 2; ++i) {
//		pixelSize *= m_data->GetSpacing()[i];
//	}
//	if (slice > m_data->GetExtent()[5] || slice < m_data->GetExtent()[4]) {
//		//qDebug() << "error";
//		//qDebug() << "out of extent";
//		return;
//	}
//	vtkSmartPointer<vtkExtractVOI> extractVOI =
//		vtkSmartPointer<vtkExtractVOI>::New();
//	extractVOI->SetInputData(m_data);
//	extractVOI->SetVOI(
//		m_data->GetExtent()[0],
//		m_data->GetExtent()[1],
//		m_data->GetExtent()[2],
//		m_data->GetExtent()[3],
//		slice,
//		slice);
//
//	//extractVOI->Update();
//	vtkSmartPointer<vtkImageAccumulate> imageAccumulate =
//		vtkSmartPointer<vtkImageAccumulate>::New();
//	imageAccumulate->SetInputConnection(extractVOI->GetOutputPort());
//	//imageAccumulate->SetInputData(m_data);
//	imageAccumulate->SetComponentExtent(0, 1, 0, 0, 0, 0); // #LookupTable
//	imageAccumulate->SetComponentOrigin(1, 0, 0);
//	imageAccumulate->SetComponentSpacing(1, 0, 0);
//	imageAccumulate->Update();
//	Measurements2D.insert(slice, QSharedPointer<double>(new double[3], std::default_delete<double[]>()));
//	for (int i = 0; i < 2; ++i) {
//		Measurements2D[slice].data()[i] =  *static_cast<int*>(
//			imageAccumulate->GetOutput()->GetScalarPointer(i, 0, 0)) * pixelSize;
//	}
//	// NMI calculation
//	Measurements2D[slice].data()[2] = Measurements2D[slice].data()[0] / 
//		(Measurements2D[slice].data()[0] + Measurements2D[slice].data()[1]);
//	cerr << "Measurements2D" << endl;
//	for (int i = 0; i < 3; ++i) {
//		cerr << Measurements2D[slice].data()[i] << ' ';
//	}
//	cerr << endl;
//}
