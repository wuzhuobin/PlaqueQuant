#include <vtkSmartPointer.h>
#include <vtkNIFTIImageReader.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkImageGaussianSmooth.h>
#include <vtkExtractVOI.h>
#include <vtkImageGradient.h>
#include <vtkDoubleArray.h>
#include <vtkPointData.h>
#include <vtkFieldDataToAttributeDataFilter.h>
#include <vtkMaskPoints.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>
#include <vtkImageData.h>
#include <vtkActor.h>
#include <vtkVersion.h>
#include <vtkSmartPointer.h>
#include <vtkSphere.h>
#include <vtkSampleFunction.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkVolumeProperty.h>
#include <vtkCamera.h>
#include <vtkImageShiftScale.h>
#include <vtkImageData.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkXMLImageDataReader.h>
#include <vtkNIFTIImageReader.h>

#include <vtkArrowSource.h>
#include <vtkGlyph3D.h>





int main(int argc, char** argv)
{



	vtkSmartPointer<vtkNIFTIImageReader> niftiImageReader =
		vtkSmartPointer<vtkNIFTIImageReader>::New();
	niftiImageReader->SetFileName("C:/Users/jieji/Desktop/test/canny.nii");
	niftiImageReader->Update();

	vtkSmartPointer<vtkImageData> imageData =
		niftiImageReader->GetOutput();

	// Use 1% of the points
	int onRatio = imageData->GetPointData()->GetScalars()->GetNumberOfTuples() /
		(imageData->GetPointData()->GetScalars()->GetNumberOfTuples() * .05);
	double scaleFactor = 10.0;

	vtkSmartPointer<vtkImageGaussianSmooth> imageGaussianSmooth =
		vtkSmartPointer<vtkImageGaussianSmooth>::New();
	imageGaussianSmooth->SetInputConnection(niftiImageReader->GetOutputPort());
	imageGaussianSmooth->SetRadiusFactor(100);
	imageGaussianSmooth->Update();

	vtkSmartPointer<vtkImageGradient> gradientFilter =
		vtkSmartPointer<vtkImageGradient>::New();
	gradientFilter->SetInputConnection(imageGaussianSmooth->GetOutputPort());
	gradientFilter->SetDimensionality(2);
	gradientFilter->Update();

	// Create the Glyphs for the gradient
	vtkSmartPointer<vtkArrowSource> arrowSource =
		vtkSmartPointer<vtkArrowSource>::New();

	// The gradient is 2D but Glyph3D needs 3D vectors. Add a 0 z-component
	// Also, ImageGradient generates a 2-component scalar for the
	// gradient, but Glyph3D needs normals or vectors
	vtkSmartPointer<vtkDoubleArray> zeroes =
		vtkSmartPointer<vtkDoubleArray>::New();
	zeroes->SetNumberOfComponents(1);
	zeroes->SetName("Zero");
	zeroes->SetNumberOfTuples(gradientFilter->GetOutput()->GetPointData()->GetScalars()->GetNumberOfTuples());
	zeroes->FillComponent(0, 0.0);
	gradientFilter->GetOutput()->GetPointData()->AddArray(zeroes);

	std::string scalarName =
		gradientFilter->GetOutput()->GetPointData()->GetScalars()->GetName();

	vtkSmartPointer<vtkFieldDataToAttributeDataFilter> scalarsToVectors =
		vtkSmartPointer<vtkFieldDataToAttributeDataFilter>::New();
	scalarsToVectors->SetInputConnection(gradientFilter->GetOutputPort());
	scalarsToVectors->SetInputFieldToPointDataField();
	scalarsToVectors->SetOutputAttributeDataToPointData();
	scalarsToVectors->SetVectorComponent(0, scalarName.c_str(), 0);
	scalarsToVectors->SetVectorComponent(1, scalarName.c_str(), 1);
	scalarsToVectors->SetVectorComponent(2, "Zero", 0);

	// Select a small percentage of the gradients
	vtkSmartPointer<vtkMaskPoints> maskPoints =
		vtkSmartPointer<vtkMaskPoints>::New();
	maskPoints->SetInputConnection(scalarsToVectors->GetOutputPort());
	maskPoints->RandomModeOff();
	maskPoints->SetOnRatio(onRatio);

	vtkSmartPointer<vtkGlyph3D> vectorGradientGlyph =
		vtkSmartPointer<vtkGlyph3D>::New();
	vectorGradientGlyph->SetSourceConnection(arrowSource->GetOutputPort());
	vectorGradientGlyph->SetInputConnection(maskPoints->GetOutputPort());
	vectorGradientGlyph->SetScaleModeToScaleByVector();
	vectorGradientGlyph->SetVectorModeToUseVector();
	vectorGradientGlyph->SetScaleFactor(scaleFactor);

	vtkSmartPointer<vtkPolyDataMapper> vectorGradientMapper =
		vtkSmartPointer<vtkPolyDataMapper>::New();
	vectorGradientMapper->SetInputConnection(vectorGradientGlyph->GetOutputPort());
	vectorGradientMapper->ScalarVisibilityOff();

	vtkSmartPointer<vtkActor> vectorGradientActor =
		vtkSmartPointer<vtkActor>::New();
	vectorGradientActor->SetMapper(vectorGradientMapper);
	vectorGradientActor->GetProperty()->SetColor(1.0000, 0, 0);

	imageData = imageGaussianSmooth->GetOutput();

	vtkSmartPointer<vtkRenderWindow> renWin =
		vtkSmartPointer<vtkRenderWindow>::New();
	vtkSmartPointer<vtkRenderer> ren1 =
		vtkSmartPointer<vtkRenderer>::New();
	ren1->SetBackground(0.1, 0.4, 0.2);
	ren1->AddActor(vectorGradientActor);

	renWin->AddRenderer(ren1);

	renWin->SetSize(301, 300); // intentional odd and NPOT  width/height

	vtkSmartPointer<vtkRenderWindowInteractor> iren =
		vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(renWin);

	renWin->Render(); // make sure we have an OpenGL context.

	vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper =
		vtkSmartPointer<vtkSmartVolumeMapper>::New();
	volumeMapper->SetBlendModeToComposite(); // composite first
#if VTK_MAJOR_VERSION <= 5
	volumeMapper->SetInputConnection(imageData->GetProducerPort());
#else
	volumeMapper->SetInputData(imageData);
#endif  
	vtkSmartPointer<vtkVolumeProperty> volumeProperty =
		vtkSmartPointer<vtkVolumeProperty>::New();
	volumeProperty->ShadeOff();
	volumeProperty->SetInterpolationType(VTK_LINEAR_INTERPOLATION);

	vtkSmartPointer<vtkPiecewiseFunction> compositeOpacity =
		vtkSmartPointer<vtkPiecewiseFunction>::New();
	compositeOpacity->AddPoint(0.0, 0.0);
	compositeOpacity->AddPoint(10.0, 1.0);
	compositeOpacity->AddPoint(2.1, 0.0);
	//compositeOpacity->AddPoint(255.0,0.0);
	volumeProperty->SetScalarOpacity(compositeOpacity); // composite first.

	vtkSmartPointer<vtkColorTransferFunction> color =
		vtkSmartPointer<vtkColorTransferFunction>::New();
	color->AddRGBPoint(0.1, 0.0, 0.0, 1.0);
	//color->AddRGBPoint(40.0  ,1.0,0.0,0.0);
	//color->AddRGBPoint(255.0,1.0,1.0,1.0);
	volumeProperty->SetColor(color);

	vtkSmartPointer<vtkVolume> volume =
		vtkSmartPointer<vtkVolume>::New();
	volume->SetMapper(volumeMapper);
	volume->SetProperty(volumeProperty);
	ren1->AddViewProp(volume);
	ren1->ResetCamera();

	// Render composite. In default mode. For coverage.
	renWin->Render();

	// 3D texture mode. For coverage.
#if !defined(VTK_LEGACY_REMOVE) && !defined(VTK_OPENGL2)
	volumeMapper->SetRequestedRenderModeToRayCastAndTexture();
#endif // VTK_LEGACY_REMOVE
	renWin->Render();

	// Software mode, for coverage. It also makes sure we will get the same
	// regression image on all platforms.
	volumeMapper->SetRequestedRenderModeToRayCast();
	volumeMapper->SetRequestedRenderModeToGPU();
	renWin->Render();

	iren->Start();


}