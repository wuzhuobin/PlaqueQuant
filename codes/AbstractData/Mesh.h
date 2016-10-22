/*
 * File: Mesh.h
 * Purpose: 
 * Created on Dec 12, 2015, 16:01 PM
 */

#ifndef __Mesh_h__
#define __Mesh_h__
#include <QObject>
#include <QString>
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkProperty.h"
#include "vtkActor.h"
#include "vtkRenderer.h"
#include "vtkVersion.h"
#include "vtkMatrix4x4.h"
#include "vtkLookUpTable.h"
#include "AbstractPolyData.h"

class Mesh: public QObject, public AbstractPolyData
{
	Q_OBJECT

public:
	Mesh(QObject *parent = 0);
	~Mesh(void);

	void SetPolyData(vtkPolyData* data);
	void SetPolyData(QString path);
	void SetRepresentation(int r);
	
	void SetReferenceImageUniqueName(QString refName);

	QString				GetReferenceImageUniqueName();
	int					GetRepresentation();

	/// Legacy function
	void FollowMatrix(vtkMatrix4x4* matrix);
	
private:
	QString				m_referenceImageUniqueName;
	
};


#endif