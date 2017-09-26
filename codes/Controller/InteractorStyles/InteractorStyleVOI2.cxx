#include "InteractorStyleVOI2.h"

#include "ImageViewer.h"

#include <vtkImageCroppingRegionsWidget.h>
#include <vtkObjectFactory.h>
#include <vtkImageActor.h>

#include <vtkSmartVolumeMapper.h>
//#include <vtkSmartPointer.h>




vtkStandardNewMacro(InteractorStyleVOI2);
void InteractorStyleVOI2::PrintSelf(ostream & os, vtkIndent indent)
{
	InteractorStyleNavigation::PrintSelf(os, indent);
}
void InteractorStyleVOI2::SetCustomEnabled(bool flag)
{
	InteractorStyleNavigation::SetCustomEnabled(flag);

	if (flag) {
		m_croppingRegionWidget = vtkImageCroppingRegionsWidget::New();
		vtkSmartVolumeMapper* volumeMapper =
			vtkSmartVolumeMapper::New();
		volumeMapper->SetInputData(GetImageViewer()->GetInput());
		volumeMapper->SetBlendModeToAdditive();
		volumeMapper->Update();

		m_croppingRegionWidget->SetInteractor(this->Interactor);
		m_croppingRegionWidget->SetVolumeMapper(volumeMapper);
		m_croppingRegionWidget->PlaceWidget(GetImageViewer()->GetImageActor()->GetBounds());
		m_croppingRegionWidget->SetEnabled(flag);
		GetImageViewer()->Render();
	}
	else {
		m_croppingRegionWidget->SetEnabled(flag);
		m_croppingRegionWidget->Delete();
		m_croppingRegionWidget = nullptr;
	}
}

InteractorStyleVOI2::InteractorStyleVOI2()
{
}

InteractorStyleVOI2::~InteractorStyleVOI2()
{
}
