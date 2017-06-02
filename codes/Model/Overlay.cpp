#include "Overlay.h"

#include <itkVTKImageToImageFilter.h>
#include <itkImageToVTKImageFilter.h>
#include <vtkLookupTable.h>
#include <vtkObjectFactory.h>

#include <qdebug.h>

vtkStandardNewMacro(OverlayImageData);
vtkStandardNewMacro(OverlayUpdatedObserver);

Overlay::Overlay(QObject * parent)
	:QObject(parent)
{
	m_data = vtkSmartPointer<OverlayImageData>::New();

	m_updatedObserver = vtkSmartPointer<OverlayUpdatedObserver>::New();
	m_updatedObserver->overlay = this;
	m_data->AddObserver(vtkCommand::UpdateDataEvent, m_updatedObserver);

	m_lookupTable = vtkSmartPointer<vtkLookupTable>::New();
	m_lookupTable->SetNumberOfTableValues(1);
	m_lookupTable->Build();
	m_lookupTable->SetTableValue(0, 0, 0, 0, 0);
}

Overlay::Overlay(OverlayImageData::itkImageType::Pointer data, QObject * parent)
	:Overlay(parent)
{
	m_data->Graft(data);
}

Overlay::Overlay(OverlayImageData * data, QObject * parent)
	: Overlay(parent)
{
	m_data->ShallowCopy(data);
}

Overlay::~Overlay()
{
	m_data->RemoveAllObservers();
}

OverlayImageData * Overlay::getData() const
{
	return m_data;
}

vtkLookupTable * Overlay::getLookupTable() const
{
	return m_lookupTable;
}

void Overlay::slotRequestOpacity(int color)
{
	double* value = m_lookupTable->GetTableValue(color);
	emit signalGetRequestedOpacity(value[3]);
}

void Overlay::slotSetOpacity(int color, double opacity)
{
	double* value = m_lookupTable->GetTableValue(color);
	value[3] = opacity;
	m_lookupTable->SetTableValue(color, value);
	m_lookupTable->Build();
	m_data->Modified();
}

double Overlay::getOpacity(int color)
{
	double* value = m_lookupTable->GetTableValue(color);
	return value[3];
}

void Overlay::updatedOverlay()
{
	qDebug() << "Modified";
	emit signalUpdatedOverlay();
}

void OverlayImageData::PrintSelf(ostream & os, vtkIndent indent)
{
	os << indent << "VTK Image information: " << std::endl;
	vtkImageData::PrintSelf(os, indent);
	os << indent << "ITK Image information:" << std::endl;
	m_itkImage->Print(os, 0);
}

void OverlayImageData::ShallowCopy(vtkDataObject * dataObject)
{
	OverlayImageData* overlayImageData = OverlayImageData::SafeDownCast(dataObject);
	vtkImageData *imageData = vtkImageData::SafeDownCast(dataObject);

	if (imageData)
	{
		vtkImageData::ShallowCopy(imageData);
		if (overlayImageData) {
			m_itkImage->Graft(overlayImageData->GetItkImage());
		}
		else {
			updateITKImage();
		}
		InvokeEvent(vtkCommand::UpdateDataEvent, nullptr);
	}

}

void OverlayImageData::DeepCopy(vtkDataObject * dataObject)
{

	OverlayImageData* overlayImageData = OverlayImageData::SafeDownCast(dataObject);
	vtkImageData *imageData = vtkImageData::SafeDownCast(dataObject);
	if (imageData)
	{
		vtkImageData::DeepCopy(imageData);
		updateITKImage();
		InvokeEvent(vtkCommand::UpdateDataEvent, nullptr);
	}
}

void OverlayImageData::Graft(itkImageType::Pointer dataObject)
{
	m_itkImage->Graft(dataObject);

	updateVTKImage();
	InvokeEvent(vtkCommand::UpdateDataEvent, nullptr);
}

OverlayImageData::itkImageType::Pointer OverlayImageData::GetItkImage()
{
	return m_itkImage;
}

OverlayImageData::OverlayImageData()
{
	m_itkImage = (itkImageType::New());

	updateVTKImage();
}

OverlayImageData::~OverlayImageData()
{
}

void OverlayImageData::updateITKImage()
{
	typedef itk::VTKImageToImageFilter<itkImageType> VTKImageToImageType;

	VTKImageToImageType::Pointer vtkImageToImageFilter = VTKImageToImageType::New();
	vtkImageToImageFilter->SetInput(this);
	vtkImageToImageFilter->Update();
	vtkImageToImageFilter->GetOutput()->SetDirection(m_itkImage->GetDirection());
	m_itkImage->Graft(vtkImageToImageFilter->GetOutput());
}

void OverlayImageData::updateVTKImage()
{
	typedef itk::ImageToVTKImageFilter<itkImageType> ImageToVTKImageFilter;

	ImageToVTKImageFilter::Pointer imageToVTKImageFilter =
		ImageToVTKImageFilter::New();
	imageToVTKImageFilter->SetInput(m_itkImage);
	imageToVTKImageFilter->Update();
	vtkImageData::ShallowCopy(imageToVTKImageFilter->GetOutput());
}

void OverlayUpdatedObserver::Execute(vtkObject * caller, unsigned long eventId, void * callData)
{
	overlay->updatedOverlay();
}
