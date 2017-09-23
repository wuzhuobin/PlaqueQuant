#ifndef __YOLK_3D_SERIES_H__
#define __YOLK_3D_SERIES_H__

#include <QWidget>
#include <QMap>

//#include <itkImage.h>

// vtk
#include <vtkSmartPointer.h>


class IVtkImageData;

class vtkImageViewer2;
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
class vtkMatrix4x4;
class vtkSphereSource;
namespace Ui { class Yolk3DSeries; }

class Yolk3DSeries : public QWidget
{
	Q_OBJECT;
public:
	explicit Yolk3DSeries(QWidget* parent = nullptr);
	~Yolk3DSeries();

	//typedef itk::Image<float, 3> ImageType;

	//void connectViewer(MyImageViewer*);
	//void disconnectViewer(MyImageViewer*);


	void set3DSeries(QStringList);
	void setSlice(int sliceNum);
	void setImageDirection(vtkMatrix4x4* direction);
	void drawLineByPlane(const double* normal, const double* pos);

public slots:


	void on_pushButtonLoad_clicked();
	void SetWorldCoordinate(double x, double y, double z, unsigned int i);
	//void slotUpdate();
	void on_spinBoxSlice_valueChanged(int value);

protected:

	vtkSmartPointer<vtkImageViewer2> imageViewer;
	QHash<int, vtkSmartPointer<IVtkImageData>> imageSlice;
	QHash<int, vtkSmartPointer<vtkMatrix4x4>> imageMatrixes;

	QStringList fileNames;

	/* Line actors */
	vtkSmartPointer<vtkPlaneSource> planeSource;
	vtkSmartPointer<vtkActor> planeActor;
	vtkSmartPointer<vtkTransformPolyDataFilter> planeTransform;

	//void updateByViewer();
	void updateBy3DSeries();

	void readSeries(QStringList filenames);


	vtkPolyDataMapper*	m_mapper;
	vtkActor*			m_lineActor;
	vtkCutter*			m_cutter;
	vtkPlane*			m_cutfunction;

	/* DEBUG */
	//vtkSphereSource*	m_debugSphereSource;
	//vtkActor*			m_debugSphereActor;


	/* Image set */
	QMap<int, vtkImageData*> m_3dimageList;
	QMap<int, double*> m_matrixList;
	vtkMatrix4x4* m_imageDirection;
	double m_imageOrigin[3];

	/* vtk actors */
	double m_normalByExtent[4];
	double m_viewUpByExtent[4];
	vtkImageActor* m_imageActor;


	/* UI */
	//itk::GDCMImageIO::Pointer m_gdcmio;
	vtkRenderer* m_ren;
	vtkRenderer* m_frontRen;
	vtkRenderWindow* m_renwin;

	Ui::Yolk3DSeries* ui;

};


#endif // __YOLK_3D_SERIES_H__