#ifndef __MEASUREMENT_H__
#define __MEASUREMENT_H__


#include <qobject.h>
#include <qsharedpointer.h>

class vtkImageData;
class vtkCallbackCommand;

class Measurement : public QObject
{
	Q_OBJECT;
public:
	Measurement(QObject* parent = nullptr);
	virtual ~Measurement();

	virtual void setOverlay(vtkImageData* overlay);
	virtual vtkImageData* getOverlay();
public slots:
	virtual void update();
//signals:
//	void updated();

protected:
	vtkImageData* m_overlay = nullptr;
	vtkCallbackCommand* m_callback = nullptr;


};


#endif // !__MEASUREMENT_H__