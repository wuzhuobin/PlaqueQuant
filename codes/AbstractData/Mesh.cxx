#include "Mesh.h"
#include "vtkSmartPointer.h"
#include "vtkTriangleFilter.h"
#include "vtkSTLReader.h"
#include "vtkSTLWriter.h"
#include "vtkTransformPolyDataFilter.h"
#include "vtkTransform.h"
#include "vtkXMLPolyDataReader.h"
#include "vtkXMLPolyDataWriter.h"
#include <QFileInfo>

Mesh::Mesh(QObject *parent) :QObject(parent)
{
	m_polyData	 = vtkPolyData::New();
	m_referenceImageUniqueName = "";
}

Mesh::~Mesh(void)
{
}

void Mesh::SetPolyData(vtkPolyData* data)
{
	m_polyData->DeepCopy(data);
	InstallPipeline();
}

void Mesh::SetPolyData( QString path)
{
	QFileInfo fileInfo (path);

	if (fileInfo.suffix() == "stl")
	{
		vtkSmartPointer<vtkSTLReader> stlReader = vtkSmartPointer<vtkSTLReader>::New();
		stlReader->SetFileName(path.toStdString().c_str());
		stlReader->Update();		
			
		m_polyData->DeepCopy(stlReader->GetOutput());
	}
	else 
	{
		vtkSmartPointer<vtkXMLPolyDataReader> vtpReader = vtkSmartPointer<vtkXMLPolyDataReader>::New();
		vtpReader->SetFileName(path.toStdString().c_str());
		vtpReader->Update();		

		m_polyData->DeepCopy(vtpReader->GetOutput());
	}

	this->InstallPipeline();
}


QString Mesh::GetReferenceImageUniqueName()
{
	return m_referenceImageUniqueName;
}


void Mesh::SetRepresentation( int r )
{
	m_actor->GetProperty()->SetRepresentation(r);
}

int Mesh::GetRepresentation()
{
	return m_actor->GetProperty()->GetRepresentation();
}

void Mesh::FollowMatrix(vtkMatrix4x4* matrix)
{
	m_userTransform->SetMatrix(matrix);
}

void Mesh::SetReferenceImageUniqueName(QString refName)
{
	this->m_referenceImageUniqueName = refName;
}



