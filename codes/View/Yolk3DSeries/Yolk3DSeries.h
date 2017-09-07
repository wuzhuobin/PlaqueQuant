#ifndef __YOLK_3D_SERIES_H__
#define __YOLK_3D_SERIES_H__

#include <QWidget>
#include <QMap>

#include <itkImage.h>
#include <itkGDCMImageIO.h>

//#include "MyImageViewer.h"

class vtkRenderer;
class vtkRenderWindow;
class vtkImageData;
class vtkImageActor;
class vtkPlaneSource;
class vtkPolyDataMapper;
class vtkCutter;
class vtkTransformPolyDataFilter;
class vtkPlane;
class vtkActor;
namespace Ui { class Yolk3DSeries; }

class Yolk3DSeries : public QWidget
{
	Q_OBJECT;
public:
	explicit Yolk3DSeries(QWidget* parent = nullptr);
	~Yolk3DSeries();

	typedef itk::Image<float, 3> ImageType;

	//void connectViewer(MyImageViewer*);
	//void disconnectViewer(MyImageViewer*);


	void set3DSeries(QStringList);
	void setSlice(int sliceNum);
	void drawLineByPlane(const double* normal, const double* pos);

public slots:


	void on_pushButtonLoad_clicked();
	void SetWorldCoordinate(double x, double y, double z, unsigned int i);
	//void slotUpdate();
	void on_spinBoxSlice_valueChanged(int);

protected:
	//void updateByViewer();
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
	vtkRenderer* m_ren;
	vtkRenderer* m_frontRen;
	vtkRenderWindow* m_renwin;

	Ui::Yolk3DSeries* ui;

};


#endif // __YOLK_3D_SERIES_H__