#include "LumenExtraction.h"


LumenExtraction::LumenExtraction()
{
	m_input = NULL;
	m_isoVal = 0;
	m_lumenSurface = vtkPolyData::New();
	m_itkLumenImage = ImageType::New();
	m_vtkLumenImage = vtkImageData::New();
	m_dilatedSurface = vtkPolyData::New();
	m_dilationVal = 0;
	m_inputItkImage = ImageType::New();
	m_dilatedItkImage = ImageType::New();
	m_dilatedVtkImage = vtkImageData::New();
	m_initialNeighborhoodRadius = 3;
	m_mulitplier = 2.5; //cover 99% of confidence interval
	m_iterations = 50;
}

LumenExtraction::~LumenExtraction()
{
	m_lumenSurface->Delete();
	m_dilatedSurface->Delete();
	m_dilatedVtkImage->Delete();
	m_itkLumenImage->Delete();
}

void LumenExtraction::SetInputData(vtkImageData * image)
{
	m_input = image;
}

void LumenExtraction::GetInputItkImage(ImageType::Pointer itkImage)
{
	itkImage->Graft(m_inputItkImage);
}

void LumenExtraction::AddSeed(double * seed)
{
	m_seedList.append(seed);
}

void LumenExtraction::SetInitialNeighborhoodRadius(double radius)
{
	m_initialNeighborhoodRadius = radius;
}

void LumenExtraction::SetMultiplier(double multiplier)
{
	m_mulitplier = multiplier;
}

void LumenExtraction::SetNumberOfIterations(double itr)
{
	m_iterations = itr;
}

void LumenExtraction::GetLumenItkImage(ImageType::Pointer itkLumenImage)
{
	itkLumenImage->Graft(m_itkLumenImage);
}

void LumenExtraction::GetLumenVtkImage(vtkImageData* vtkLumenImage)
{
	vtkLumenImage->DeepCopy(m_vtkLumenImage);
}

void LumenExtraction::SetDilationValue(double dilationVal)
{
	m_dilationVal = dilationVal;
}

void LumenExtraction::GetDilatedItkImage(ImageType::Pointer dilatedImage)
{
	dilatedImage->Graft(m_dilatedItkImage);
}

void LumenExtraction::GetDilatedVtkImage(vtkImageData * dilatedImage)
{
	dilatedImage->DeepCopy(m_dilatedVtkImage);
}

void LumenExtraction::Update()
{
	if (m_input == nullptr)
		return;

	// convert the vtk image to itk image
	VTKImageToImageType::Pointer vtkImageToImageFilter = VTKImageToImageType::New();
	vtkImageToImageFilter->SetInput(m_input);
	vtkImageToImageFilter->Update();

	m_inputItkImage->Graft(vtkImageToImageFilter->GetOutput());

	// lumen segmentation


	// confidence connected filtering
	ConfidenceConnectedFilterType::Pointer confidenceConnectedFilter = ConfidenceConnectedFilterType::New();
	confidenceConnectedFilter->SetInitialNeighborhoodRadius(m_initialNeighborhoodRadius);
	confidenceConnectedFilter->SetMultiplier(m_mulitplier);
	confidenceConnectedFilter->SetNumberOfIterations(m_iterations);
	confidenceConnectedFilter->SetReplaceValue(1);
	confidenceConnectedFilter->SetInput(m_inputItkImage);

	// Set seed
	for (int i = 0; i < m_seedList.length(); i++)
	{
		itk::Index<3> index;
		index.SetElement(0,m_seedList.at(i)[0]);
		index.SetElement(1, m_seedList.at(i)[1]);
		index.SetElement(2, m_seedList.at(i)[2]);

		confidenceConnectedFilter->AddSeed(index);
	}

	if (m_seedList.length() > 0)
	{
		confidenceConnectedFilter->Update();
	}
	else
		return;

	m_itkLumenImage->Graft(confidenceConnectedFilter->GetOutput());

	// create lumen vtk image
	ConnectorType::Pointer connector1 = ConnectorType::New();
	connector1->SetInput(m_itkLumenImage);
	connector1->Update();
	m_vtkLumenImage->DeepCopy(connector1->GetOutput());
	
	//// paint the seed point
	//for (int i = 0; i < m_seedList.length(); i++)
	//{
	//	itk::Index<3> index;
	//	index.SetElement(0, m_seedList.at(i)[0]);
	//	index.SetElement(1, m_seedList.at(i)[1]);
	//	index.SetElement(2, m_seedList.at(i)[2]);

	//	m_itkLumenImage->SetPixel(index, 4);
	//}

	this->LabelDilation();
}

void LumenExtraction::LabelDilation()
{
	typedef itk::BinaryBallStructuringElement<ImageType::PixelType, 3>                  StructuringElementType;
	StructuringElementType structuringElement;
	structuringElement.SetRadius(m_dilationVal);
	structuringElement.CreateStructuringElement();

	typedef itk::BinaryDilateImageFilter <ImageType, ImageType, StructuringElementType> BinaryDilateImageFilterType;
	BinaryDilateImageFilterType::Pointer dilateFilter = BinaryDilateImageFilterType::New();
	dilateFilter->SetInput(m_itkLumenImage);
	dilateFilter->SetKernel(structuringElement);
	dilateFilter->SetDilateValue(1);
	dilateFilter->Update();

	m_dilatedItkImage->Graft(dilateFilter->GetOutput());

	// convert dilated itk image to vtk image
	ConnectorType::Pointer connector = ConnectorType::New();
	connector->SetInput(m_dilatedItkImage);
	connector->Update();
	m_dilatedVtkImage->DeepCopy(connector->GetOutput());
}

