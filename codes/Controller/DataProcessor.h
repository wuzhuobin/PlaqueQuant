#ifndef __DATA_PROCESSOR_H__
#define __DATA_PROCESSOR_H__

#include <qobject.h>

class StyleSwitch;
class StyleSwitch3D;
class PlaqueQuantImageManager;

class DataProcessor: public QObject
{
	Q_OBJECT;
public:

	DataProcessor(QObject* parent = nullptr);
	~DataProcessor();

	StyleSwitch** imageInteractorStyle = nullptr;
	StyleSwitch3D* surfaceInteractorStyle = nullptr;
	PlaqueQuantImageManager* imageManager = nullptr;

	public slots:

	void initializeCurved();


protected:



private:

};


#endif // !__DATA_PROCESSOR_H__
