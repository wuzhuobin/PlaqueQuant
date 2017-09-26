#include "IVtkImageData.h"

#include <vtkObjectFactory.h>

#include <itkVTKImageToImageFilter.h>
#include <itkImageToVTKImageFilter.h>

vtkStandardNewMacro(IVtkImageData);
typedef itk::Image<float, 3> itkImageType;

void IVtkImageData::PrintSelf(ostream & os, vtkIndent indent)
{
	os << indent << "VTK Image information: " << std::endl;
	vtkImageData::PrintSelf(os, indent);
	os << indent << "ITK Image information:" << std::endl;
	m_itkImage->Print(os, 0);
	//os << indent << "ITK GDCMImageIO information:" << std::endl;
	//m_dicomIO->Print(os, 0);
}

void IVtkImageData::ShallowCopy(vtkDataObject * dataObject)
{
	IVtkImageData* ivtkImageData = IVtkImageData::SafeDownCast(dataObject);
	vtkImageData *imageData = vtkImageData::SafeDownCast(dataObject);

	if (imageData)
	{
		vtkImageData::ShallowCopy(imageData);
		if (ivtkImageData) {
			m_itkImage->Graft(ivtkImageData->GetItkImage());
		}
		else
		{
			updateITKImage();
		}
	}

}

void IVtkImageData::DeepCopy(vtkDataObject * dataObject)
{
	IVtkImageData* ivtkImageData = IVtkImageData::SafeDownCast(dataObject);
	vtkImageData *imageData = vtkImageData::SafeDownCast(dataObject);

	if (imageData)
	{
		vtkImageData::DeepCopy(imageData);
		updateITKImage();
	}
}

void IVtkImageData::Graft(itkImageType::Pointer dataObject)
{
	m_itkImage->Graft(dataObject);
	
	updateVTKImage();

}

itkImageType::Pointer IVtkImageData::GetItkImage()
{
	return m_itkImage;
}

//void IVtkImageData::operator=(vtkImageData * vtkImage)
//{
//	m_vtkImage->ShallowCopy(vtkImage);
//	updateITKImage();
//}

IVtkImageData::IVtkImageData()
{
	//m_vtkImage = (vtkSmartPointer<vtkImageData>::New());
	//m_vtkImage = this;
	m_itkImage = (itkImageType::New());

	updateVTKImage();
}

IVtkImageData::~IVtkImageData()
{
}

void IVtkImageData::updateITKImage()
{
	typedef itk::VTKImageToImageFilter<itkImageType> VTKImageToImageType;

	VTKImageToImageType::Pointer vtkImageToImageFilter = VTKImageToImageType::New();
	vtkImageToImageFilter->SetInput(this);
	vtkImageToImageFilter->Update();
	vtkImageToImageFilter->GetOutput()->SetDirection(m_itkImage->GetDirection());
	m_itkImage->Graft(vtkImageToImageFilter->GetOutput());

}

void IVtkImageData::updateVTKImage()
{
	typedef itk::ImageToVTKImageFilter<itkImageType> ImageToVTKImageFilter;

	ImageToVTKImageFilter::Pointer imageToVTKImageFilter =
		ImageToVTKImageFilter::New();
	imageToVTKImageFilter->SetInput(m_itkImage);
	imageToVTKImageFilter->Update();
	vtkImageData::ShallowCopy(imageToVTKImageFilter->GetOutput());

}
