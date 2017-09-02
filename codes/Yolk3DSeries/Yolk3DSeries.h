#ifndef __YOLK_3D_SERIES_H__
#define __YOLK_3D_SERIES_H__

#include <QList>
#include <QObject>
#include <QWidget>

#include <itkImage.h>
#include <itkGDCMImageIO.h>

#include "MyImageViewer.h"

class QVTKWidget2;
class vtkRenderer;
class vtkRenderWindow;
class vtkImageData;
class vtkImageActor;
class vtkPlaneSource;
class vtkPolyDataMapper;
class vtkCutter;
class vtkTransformPolyDataFilter;
class vtkPlane;

class Yolk3DSeries : public QWidget
{
	Q_OBJECT
public:
	Yolk3DSeries(QObject* parent = nullptr);
	~Yolk3DSeries();

	typedef itk::Image<float, 3> ImageType;

	void connectViewer(MyImageViewer*);
	void disconnectViewer(MyImageViewer*);

	void set3DSeries(QStringList);
	void setSlice(int sliceNum);
	void drawLineByPlane(const double* normal, const double* pos);

public slots:
	void slotUpdate();
	void slotSetSlice(int);

protected:
	void updateByViewer();
	void updateBy3DSeries();

	void readSeries(QStringList filenames);


	/* Line actors */
	vtkPolyDataMapper*	m_mapper;
	vtkActor*			m_lineActor;
	vtkCutter*			m_cutter;
	vtkPlaneSource*		m_planeSource;
	vtkPlane*			m_cutfunction;
	vtkTransformPolyDataFilter* m_transfilter;


	/* Image set */
	QMap<int, vtkImageData*> m_3dimageList;
	QMap<int, double*> m_matrixList;

	/* vtk actors */
	double m_normalByExtent[4];
	double m_viewUpByExtent[4];
	vtkImageActor* m_imageActor;


	/* UI */
	itk::GDCMImageIO::Pointer m_gdcmio;
	QVTKWidget2* m_qvtkWidget;
	vtkRenderer* m_ren;
	vtkRenderer* m_frontRen;
	vtkRenderWindow* m_renwin;
};


#endif // __YOLK_3D_SERIES_H__