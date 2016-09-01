#ifndef DATA_MANAGER_H
#define DATA_MANAGER_H

#include <QObject>
#include <QList>

#include "ImageData.h"
#include "Mesh.h"
#include "Overlay.h"

class DataManager
{
	Q_OBJECT
public:
	DataManager();
	~DataManager();

	QList<Image*>	GetImageList();
	Image*			GetImage(int);
	Image*			GetImage(QString);
	Mesh*			GetCenterline();
	Overlay*		GetOverlay();

	void AppendImageData(Image*);
	void SetCenterlineMesh(Mesh*);
	void SetOverlay(Overlay*);

protected:
	QList<Image*>  m_imageList;

	Mesh*		m_centerline;
	Overlay*	m_overlay;

};



#endif // DATA_MANAGER_H
