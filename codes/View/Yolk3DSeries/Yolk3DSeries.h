#ifndef __YOLK_3D_SERIES_H__
#define __YOLK_3D_SERIES_H__

#include <QWidget>


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
namespace Ui { class Yolk3DSeries; }

class Yolk3DSeries : public QWidget
{
	Q_OBJECT;
public:
	explicit Yolk3DSeries(QWidget* parent = nullptr);
	~Yolk3DSeries();

	void setImageDirection(vtkMatrix4x4* direction);

public slots:


	void on_pushButtonLoad_clicked();
	void SetWorldCoordinate(double x, double y, double z, unsigned int i);
	//void slotUpdate();
	void on_spinBoxSlice_valueChanged(int value);

protected:

	vtkSmartPointer<vtkImageViewer2> imageViewer;
	QHash<int, vtkSmartPointer<IVtkImageData>> imageSlice;
	QHash<int, vtkSmartPointer<vtkMatrix4x4>> imageMatrixes;

	/* Line actors */
	vtkSmartPointer<vtkPlaneSource> planeSource;
	//vtkSmartPointer<vtkActor> planeActor;
	//vtkSmartPointer<vtkTransformPolyDataFilter> planeTransform;

	//vtkSmartPointer<vtkPlaneSource> planeSource2;
	//vtkSmartPointer<vtkActor> planeActor2;
	//vtkSmartPointer<vtkTransformPolyDataFilter> planeTransform2;

	vtkSmartPointer<vtkPlane> plane;
	vtkSmartPointer<vtkCutter> cutter;
	vtkSmartPointer<vtkActor> lineActor;



	vtkSmartPointer<vtkMatrix4x4> imageDirection;

	Ui::Yolk3DSeries* ui;

};


#endif // __YOLK_3D_SERIES_H__