/*=========================================================================
*
*  Copyright Insight Software Consortium
*
*  Licensed under the Apache License, Version 2.0 (the "License");
*  you may not use this file except in compliance with the License.
*  You may obtain a copy of the License at
*
*         http://www.apache.org/licenses/LICENSE-2.0.txt
*
*  Unless required by applicable law or agreed to in writing, software
*  distributed under the License is distributed on an "AS IS" BASIS,
*  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*  See the License for the specific language governing permissions and
*  limitations under the License.
*
*=========================================================================*/
// Software Guide : BeginLatex
//
// This example illustrates the use of the
// \doxygen{HoughTransform2DCirclesImageFilter} to find Circles in a
// 2-dimensional image.
//
// First, we include the header files of the filter.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
#include "itkHoughTransform2DCirclesImageFilter.h"
// Software Guide : EndCodeSnippet
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkImageRegionIterator.h"
#include "itkThresholdImageFilter.h"
#include "itkMinimumMaximumImageCalculator.h"
#include "itkGradientMagnitudeImageFilter.h"
#include "itkDiscreteGaussianImageFilter.h"
#include <list>
#include "itkCastImageFilter.h"
#include "itkMath.h"
#include "itkMaskImageFilter.h"
#include "itkCannyEdgeDetectionImageFilter.h"
#include "itkBinaryBallStructuringElement.h"
#include "itkBinaryDilateImageFilter.h"
#include <itkExtractImageFilter.h>
#include <itkSubtractImageFilter.h>
#include "itkSobelEdgeDetectionImageFilter.h"
#include <itkThresholdImageFilter.h>

#include <vtkSmartPointer.h>
#include <vtkLookupTable.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkPolyDataMapper.h>

#include "IVtkImageData.h"
#include "IADEOverlay.h"
#include "ImageViewer.h"
#include "InteractorStyleNavigation.h"
#include "QuickView.h"

#include "VesselWallHoughTransformCircles.h"

int main(int argc, char *argv[])
{
	itk::ImageFileReader<IVtkImageData::itkImageType>::Pointer inputReader =
		itk::ImageFileReader< IVtkImageData::itkImageType>::New();
	inputReader->SetFileName("C:/Users/jieji/Desktop/grap cut/T2.nii");
	inputReader->Update();


	itk::ImageFileReader<OverlayImageData::itkImageType>::Pointer overlayReader =
		itk::ImageFileReader< OverlayImageData::itkImageType>::New();
	overlayReader->SetFileName("C:/Users/jieji/Desktop/grap cut/c.nii");
	overlayReader->Update();


	vtkSmartPointer<IVtkImageData> input =
		vtkSmartPointer<IVtkImageData>::New();
	input->Graft(inputReader->GetOutput());

	vtkSmartPointer<OverlayImageData> overlay =
		vtkSmartPointer<OverlayImageData>::New();
	overlay->Graft(overlayReader->GetOutput());


	int extent[6];
	input->GetExtent(extent);
	extent[4] = extent[5] = 14;
	vtkSmartPointer<VesselWallHoughTransformCircles> f =
		vtkSmartPointer<VesselWallHoughTransformCircles>::New();
	f->SetInputData(0, input);
	f->SetInputData(1, overlay);
	f->SetImageRange(extent);
	f->Update();

	vtkSmartPointer<vtkPolyData> polyData = f->GetOutput();
	vtkSmartPointer<vtkPolyDataMapper> mapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputData(polyData);

	vtkSmartPointer<vtkActor> actor =
		vtkSmartPointer<vtkActor>::New();
	actor->SetMapper(mapper);

	vtkSmartPointer<vtkLookupTable> lookupTable =
		vtkSmartPointer<vtkLookupTable>::New();
	lookupTable->SetNumberOfTableValues(5);
	lookupTable->SetTableValue(0, 0, 0, 0, 0);
	lookupTable->SetTableValue(1, 0, 0, 1, 1);
	lookupTable->SetTableValue(2, 1, 0, 0, 1);
	lookupTable->SetTableValue(3, 0, 1, 0, 1);
	lookupTable->SetTableValue(4, 1, 1, 0, 1);
	lookupTable->SetTableRange(0, 4);
	lookupTable->Build();




	// Visualize
	vtkSmartPointer<ImageViewer> imageViewer =
		vtkSmartPointer<ImageViewer>::New();
	imageViewer->SetOverlay(overlay);
	imageViewer->SetLookupTable(lookupTable);
	imageViewer->SetInputData(input);
	imageViewer->GetRenderer()->AddActor(actor);


	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	imageViewer->SetupInteractor(renderWindowInteractor);

	vtkSmartPointer<InteractorStyleNavigation> style =
		vtkSmartPointer<InteractorStyleNavigation>::New();
	style->SetViewer(imageViewer);
	renderWindowInteractor->SetInteractorStyle(style);


	imageViewer->Render();
	imageViewer->GetRenderer()->ResetCamera();
	imageViewer->Render();

	style->SetCustomEnabled(true);
	renderWindowInteractor->Start();
}


//int main(int argc, char *argv[])
//{
//	
//	itk::ImageFileReader<IVtkImageData::itkImageType>::Pointer inputReader =
//		itk::ImageFileReader< IVtkImageData::itkImageType>::New();
//	inputReader->SetFileName("C:/Users/jieji/Desktop/grap cut/T2.nii");
//	inputReader->Update();
//
//	
//	itk::ImageFileReader<OverlayImageData::itkImageType>::Pointer overlayReader =
//		itk::ImageFileReader< OverlayImageData::itkImageType>::New();
//	overlayReader->SetFileName("C:/Users/jieji/Desktop/grap cut/c.nii");
//	overlayReader->Update();
//
//	// edge dection 
//	itk::CannyEdgeDetectionImageFilter< IVtkImageData::itkImageType, IVtkImageData::itkImageType >::Pointer cannyEdge =
//		itk::CannyEdgeDetectionImageFilter< IVtkImageData::itkImageType, IVtkImageData::itkImageType >::New();
//	cannyEdge->SetInput(inputReader->GetOutput());
//	cannyEdge->SetVariance(0.1);
//	cannyEdge->SetLowerThreshold(0.01);
//	cannyEdge->SetUpperThreshold(0.02);
//	cannyEdge->Update();
//
//	//itk::SobelEdgeDetectionImageFilter< IVtkImageData::itkImageType, IVtkImageData::itkImageType >::Pointer sobelEdge = 
//	//	itk::SobelEdgeDetectionImageFilter< IVtkImageData::itkImageType, IVtkImageData::itkImageType >::New();
//	//sobelEdge->SetInput(inputReader->GetOutput());
//	//sobelEdge->Update();
//
//	//itk::ThresholdImageFilter<IVtkImageData::itkImageType>::Pointer threshold =
//	//	itk::ThresholdImageFilter<IVtkImageData::itkImageType>::New();
//	//threshold->SetInput(sobelEdge->GetOutput());
//	//threshold->ThresholdBelow(3000);
//	//threshold->SetOutsideValue(0);
//	//threshold->Update();
//
//
//	// using overlay to remove the inner lumen 
//	// dilate the inner lumen to remove the inner lumen edge
//	
//	itk::BinaryBallStructuringElement<OverlayImageData::itkImageType::PixelType, 3>   innerElement;
//	innerElement.SetRadius(1);
//	innerElement.CreateStructuringElement();
//
//
//	itk::BinaryDilateImageFilter <OverlayImageData::itkImageType, OverlayImageData::itkImageType, itk::BinaryBallStructuringElement<OverlayImageData::itkImageType::PixelType, 3>  >::Pointer innerDilate
//		= itk::BinaryDilateImageFilter <OverlayImageData::itkImageType, OverlayImageData::itkImageType, itk::BinaryBallStructuringElement<OverlayImageData::itkImageType::PixelType, 3>  >::New();
//	innerDilate->SetForegroundValue(2);
//	innerDilate->SetInput(overlayReader->GetOutput());
//	innerDilate->SetKernel(innerElement);
//	innerDilate->Update();
//	
//	itk::BinaryBallStructuringElement<OverlayImageData::itkImageType::PixelType, 3>   outerElement;
//	outerElement.SetRadius(5);
//	outerElement.CreateStructuringElement();
//
//
//	itk::BinaryDilateImageFilter <OverlayImageData::itkImageType, OverlayImageData::itkImageType, itk::BinaryBallStructuringElement<OverlayImageData::itkImageType::PixelType, 3>  >::Pointer outerDilate
//		= itk::BinaryDilateImageFilter <OverlayImageData::itkImageType, OverlayImageData::itkImageType, itk::BinaryBallStructuringElement<OverlayImageData::itkImageType::PixelType, 3>  >::New();
//	outerDilate->SetForegroundValue(2);
//	outerDilate->SetInput(overlayReader->GetOutput());
//	outerDilate->SetKernel(outerElement);
//	outerDilate->Update();
//
//	// outerDilate substract innerDilate to get a ring
//	itk::SubtractImageFilter<OverlayImageData::itkImageType>::Pointer substractImage = 
//		itk::SubtractImageFilter<OverlayImageData::itkImageType>::New();
//	substractImage->SetInput1(outerDilate->GetOutput());
//	substractImage->SetInput2(innerDilate->GetOutput());
//	substractImage->Update();
//
//
//
//	itk::MaskImageFilter< IVtkImageData::itkImageType, OverlayImageData::itkImageType >::Pointer maskFilter2
//		= itk::MaskImageFilter< IVtkImageData::itkImageType, OverlayImageData::itkImageType >::New();
//	maskFilter2->SetInput(cannyEdge->GetOutput());
//	maskFilter2->SetMaskImage(substractImage->GetOutput());
//	maskFilter2->SetMaskingValue(0);
//	maskFilter2->Update();
//
//
//	IVtkImageData::itkImageType::IndexType index;
//	index[0] = inputReader->GetOutput()->GetLargestPossibleRegion().GetIndex()[0];
//	index[1] = inputReader->GetOutput()->GetLargestPossibleRegion().GetIndex()[1];
//	index[2] = 2;
//	IVtkImageData::itkImageType::SizeType size;
//	size[0] = inputReader->GetOutput()->GetLargestPossibleRegion().GetSize()[0];
//	size[1] = inputReader->GetOutput()->GetLargestPossibleRegion().GetSize()[1];
//	size[2] = 0;
//
//	IVtkImageData::itkImageType::RegionType desiredRegion(index, size);
//	
//	itk::ExtractImageFilter< IVtkImageData::itkImageType, itk::Image<float, 2>>::Pointer extractImage
//		= itk::ExtractImageFilter< IVtkImageData::itkImageType, itk::Image<float, 2>>::New();
//	extractImage->SetExtractionRegion(desiredRegion);
//	extractImage->SetInput(maskFilter2->GetOutput());
//	extractImage->SetDirectionCollapseToIdentity(); // This is required.
//	extractImage->Update();
//
//	itk::HoughTransform2DCirclesImageFilter<float, float>::Pointer houghFilter
//		= itk::HoughTransform2DCirclesImageFilter<float, float>::New();
//	houghFilter->SetInput(extractImage->GetOutput());
//	houghFilter->SetNumberOfCircles(1);
//	houghFilter->SetThreshold(0.95);
//	//houghFilter->SetSweepAngle(0.2);
//	houghFilter->SetSigmaGradient(1);
//	houghFilter->SetMinimumRadius(10);
//	houghFilter->SetMaximumRadius(25);
//	houghFilter->SetVariance(0.5);
//	houghFilter->Update();
//	itk::HoughTransform2DCirclesImageFilter<float, float>::CirclesListType circles = houghFilter->GetCircles(1);
//	std::cout << "Found " << circles.size() << " circle(s)." << std::endl;
//
//
//	itk::HoughTransform2DCirclesImageFilter<float, float>::CirclesListType::const_iterator itCircles = circles.begin();
//	while (itCircles != circles.end())
//	{
//		std::cout << "Center: ";
//		std::cout << (*itCircles)->GetObjectToParentTransform()->GetOffset()
//			<< std::endl;
//		std::cout << "Radius: " << (*itCircles)->GetRadius()[0] << std::endl;
//
//
//
//		OverlayImageData::itkImageType::IndexType localIndex;
//		for (double angle = 0; angle <= 2 * itk::Math::pi; angle += itk::Math::pi / 60.0)
//		{
//			localIndex[0] =
//				(long int)((*itCircles)->GetObjectToParentTransform()->GetOffset()[0]
//					+ (*itCircles)->GetRadius()[0] * std::cos(angle));
//			localIndex[1] =
//				(long int)((*itCircles)->GetObjectToParentTransform()->GetOffset()[1]
//					+ (*itCircles)->GetRadius()[0] * std::sin(angle));
//			localIndex[2] = 2;
//			OverlayImageData::itkImageType::RegionType outputRegion =
//				overlayReader->GetOutput()->GetLargestPossibleRegion();
//			if (outputRegion.IsInside(localIndex))
//			{
//				overlayReader->GetOutput()->SetPixel(localIndex, 3);
//			}
//		}
//
//		itCircles++;
//	}
//
//	vtkSmartPointer<IVtkImageData> input =
//		vtkSmartPointer<IVtkImageData>::New();
//	input->Graft(maskFilter2->GetOutput());
//
//	vtkSmartPointer<OverlayImageData> overlay =
//		vtkSmartPointer<OverlayImageData>::New();
//	overlay->Graft(overlayReader->GetOutput());
//
//
//	vtkSmartPointer<vtkLookupTable> lookupTable =
//		vtkSmartPointer<vtkLookupTable>::New();
//	lookupTable->SetNumberOfTableValues(5);
//	lookupTable->SetTableValue(0, 0, 0, 0, 0);
//	lookupTable->SetTableValue(1, 0, 0, 1, 1);
//	lookupTable->SetTableValue(2, 1, 0, 0, 0);
//	lookupTable->SetTableValue(3, 0, 1, 0, 1);
//	lookupTable->SetTableValue(4, 1, 1, 0, 1);
//	lookupTable->SetTableRange(0, 4);
//	lookupTable->Build();
//
//
//
//
//	// Visualize
//	vtkSmartPointer<ImageViewer> imageViewer =
//		vtkSmartPointer<ImageViewer>::New();
//	imageViewer->SetOverlay(overlay);
//	imageViewer->SetLookupTable(lookupTable);
//	imageViewer->SetInputData(input);
//	cout << overlay->GetScalarRange()[0] << ' ' << overlay->GetScalarRange()[1] << endl;
//
//
//	vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
//		vtkSmartPointer<vtkRenderWindowInteractor>::New();
//	imageViewer->SetupInteractor(renderWindowInteractor);
//
//	vtkSmartPointer<InteractorStyleNavigation> style =
//		vtkSmartPointer<InteractorStyleNavigation>::New();
//	style->SetViewer(imageViewer);
//	renderWindowInteractor->SetInteractorStyle(style);
//
//
//	imageViewer->Render();
//	imageViewer->GetRenderer()->ResetCamera();
//	imageViewer->Render();
//
//	style->SetCustomEnabled(true);
//	renderWindowInteractor->Start();
//}

