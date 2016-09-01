#ifndef IO_MANAGER_H
#define IO_MANAGER_H

#include <QObject>
#include <QString>

#include "ImageData.h"
#include "Overlay.h"

class IOManager
{
	Q_OBJECT
public:
	IOManager();
	~IOManager();

public slots:
	virtual void slotOpenImage();
	virtual void slotOpenImage(QString);

	virtual void slotOpenSegmentation();
	virtual void slotOpenSegmentation(QString);

	virtual void slotSaveImage(QString uniqueName, QString path);
	virtual void slotSaveImage(int index, QString path);

	virtual void slotSaveSemgentation(QString uniqueName, QString path);
	virtual void slotSaveSemgentation(int index, QString path);

protected:
	virtual Image* LoadImageData();
	virtual bool SaveImageData(Image*, QString);

	virtual Overlay* LoadSegmentation();
	virtual bool SaveSegmentation(Overlay*, QString);

};

#endif // !IO_MANAGER_H
