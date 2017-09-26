#include "PlaqueQuantMeasurement.h"

#include "MaximumWallThickness.h"
#include "InteractorStyleSurfaceCenterLineStenosis.h"
#include "ImageSliceBySliceAccumulate.h"

#include <vtkLookupTable.h>
#include <vtkImageAccumulate.h>
#include <vtkExtractVOI.h>
#include <vtkImageData.h>
#include <vtkPolyData.h>
#include <vtkFieldData.h>
PlaqueQuantMeasurement::PlaqueQuantMeasurement(QObject * parent)
	: Measurement(parent)
{
}

PlaqueQuantMeasurement::~PlaqueQuantMeasurement()
{
}

void PlaqueQuantMeasurement::update()
{
	updateMeasurement3D();
	updateMeasurement2D();
	updateMaximumWallThickness(m_currentSlice);

	Measurement::update();
}

void PlaqueQuantMeasurement::updateMeasurement2D()
{
	// calculate the volume of a single voxel
	double pixelSize = 1;
	for (int i = 0; i < 2; ++i) {
		pixelSize *= (m_overlay)->GetSpacing()[i];
	}
	vtkSmartPointer<ImageSliceBySliceAccumulate> imageSliceBySliceAccumulate =
		vtkSmartPointer<ImageSliceBySliceAccumulate>::New();
	imageSliceBySliceAccumulate->SetInputData(m_overlay);
	imageSliceBySliceAccumulate->SetExtent(0, 1, 0, 0, 0, 0); // #LookupTable
	imageSliceBySliceAccumulate->SetOrigin(1, 0, 0);
	imageSliceBySliceAccumulate->SetSpacing(1, 0, 0);
	imageSliceBySliceAccumulate->Update();


	for (int j = m_overlay->GetExtent()[4];
		j <= m_overlay->GetExtent()[5]; j++)
	{
		m_measurements2DMap.insert(j, QSharedPointer<double>(new double[4]{0,0,0,0}, [](double* obj) {delete[] obj; }));
		for (int i = 0; i < 2; ++i) {
			m_measurements2DMap[j].data()[i] = *static_cast<int*>(
				imageSliceBySliceAccumulate->GetOutput()->GetScalarPointer(i, j, 0)) * pixelSize;
		}
		// NMI calculation
		m_measurements2DMap[j].data()[2] = m_measurements2DMap[j].data()[0] /
			(m_measurements2DMap[j].data()[0] + m_measurements2DMap[j].data()[1]);
	}
}

void PlaqueQuantMeasurement::updateMaximumWallThickness(int slice)
{
	if (m_measurements2DMap.contains(slice))
	{
		m_currentSlice = slice;
		vtkSmartPointer<vtkExtractVOI> extractVOI =
			vtkSmartPointer<vtkExtractVOI>::New();
		extractVOI->SetInputData(m_overlay);
		extractVOI->SetVOI(
			m_overlay->GetExtent()[0],
			m_overlay->GetExtent()[1],
			m_overlay->GetExtent()[2],
			m_overlay->GetExtent()[3],
			slice,
			slice);
		extractVOI->Update();

		try
		{
			vtkSmartPointer<MaximumWallThickness> mwt =
				vtkSmartPointer<MaximumWallThickness>::New();
			mwt->SetSliceImage(extractVOI->GetOutput());
			mwt->SetLumemIntensity(2);
			mwt->SetVesselIntensity(1);
			mwt->Update();
			m_measurements2DMap[slice].data()[3] = mwt->GetDistanceLoopPairVect().at(0).Distance;
		}
		catch (...)
		{
			m_measurements2DMap[slice].data()[3] = 0;
		}
		emit signalMeasurement2D(m_measurements2DMap[slice].data());
	}
}

void PlaqueQuantMeasurement::updateStenosis(double stenosis)
{
	m_stenosis = stenosis;
	emit signalStenosis(stenosis);
}

void PlaqueQuantMeasurement::updateMeasurement3D()
{
	// calculate the volume of a single voxel
	double voxelSize = 1;
	for (int i = 0; i < 3; ++i) {
		voxelSize *= m_overlay->GetSpacing()[i];
	}
	vtkSmartPointer<vtkImageAccumulate> imageAccumulate =
		vtkSmartPointer<vtkImageAccumulate>::New();
	imageAccumulate->SetInputData(m_overlay);
	imageAccumulate->SetComponentExtent(0, 7 - 1, 0, 0, 0, 0); // #LookupTable
	imageAccumulate->SetComponentOrigin(0, 0, 0);
	imageAccumulate->SetComponentSpacing(1, 0, 0);
	imageAccumulate->Update();
	for (int i = 1; i < 7; ++i) {
		m_measurements3D[i] = *static_cast<int*>(
			imageAccumulate->GetOutput()->GetScalarPointer(i, 0, 0)) * voxelSize;
		if (i == 1) {
			// initialize total plaque
			m_measurements3D[0] = 0;
		}
		else if (i > 2) {
			// total plaque
			m_measurements3D[0] += m_measurements3D[i];
		}
	}
	emit signalMeasurement3D(m_measurements3D);
	//cerr << "Measurements3D" << endl;
	//for (int i = 0; i < PlaqueQuantOverlay::NUMBER_OF_COLOR; ++i) {
	//	cerr << Measurements3D[i] << ' ';
	//}
	//cerr << endl;
}
