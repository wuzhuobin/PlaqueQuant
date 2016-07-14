#include "AbstractInteractorStyle.hxx"

AbstractInteractorStyle::AbstractInteractorStyle() {
	m_rightFunctioning = false;
	m_leftFunctioning = false;
	m_middleFunctioning = false;
}

AbstractInteractorStyle::~AbstractInteractorStyle() {

}

void AbstractInteractorStyle::SetViewers(MyViewer** viewers)
{
	for (int i = 0; i < 4; i++)
	{
		m_viewers[i] = viewers[i];
	}
}

MyViewer** AbstractInteractorStyle::GetViewers()
{
	return m_viewers;
}

MyViewer * AbstractInteractorStyle::GetViewer(int i)
{
	return m_viewers[i];
}

int AbstractInteractorStyle::GetOrientation()
{
	return m_orientation;
}

void AbstractInteractorStyle::SetOrientation(int i)
{
	m_orientation = i;
}

void AbstractInteractorStyle::OnLeftButtonDown()
{
	m_leftFunctioning = true;
}

void AbstractInteractorStyle::OnRightButtonDown()
{
	m_rightFunctioning = true;
}

void AbstractInteractorStyle::OnMiddleButtonDown()
{
	m_middleFunctioning = true;
}

void AbstractInteractorStyle::OnLeftButtonUp()
{
	m_leftFunctioning = false;
}

void AbstractInteractorStyle::OnRightButtonUp()
{
	m_rightFunctioning = false;
}

void AbstractInteractorStyle::OnMiddleButtonUp()
{
	m_middleFunctioning = false;
}

vtkActor * AbstractInteractorStyle::PickActor(int x, int y)
{
	vtkSmartPointer<vtkPropPicker> picker = vtkSmartPointer<vtkPropPicker>::New();
	if (this->m_viewers[m_orientation]->GetRenderer()) {
		picker->Pick(x, y, 0, this->m_viewers[m_orientation]->GetRenderer());
	}
	if (picker->GetActor()) {
		return picker->GetActor();
	}
	else {
		return nullptr;
	}
}