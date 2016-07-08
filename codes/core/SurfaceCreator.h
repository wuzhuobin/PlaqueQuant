#ifndef SURFACECREATOR_H
#define SURFACECREATOR_H

#include <QObject>
#include <vtkMath.h>
#include <vtkImageData.h>
#include <vtkImageResample.h>
#include <vtkDiscreteMarchingCubes.h>
#include <vtkMarchingCubes.h>
#include <vtkPolyData.h>
#include <vtkPolyDataConnectivityFilter.h>
#include <vtkSmartPointer.h>
#include <vtkWindowedSincPolyDataFilter.h>

class SurfaceCreator : public QObject
{
 	Q_OBJECT

public:
    SurfaceCreator(QObject* parent = NULL );
	~SurfaceCreator();

	void SetResamplingFactor(double factor);
    void SetInput(vtkImageData*input);
    void SetValue(int value); 
	void SetDiscrete(bool b);
	void SetLargestConnected(bool b);
    bool Update();
    vtkPolyData* GetOutput();

private:
	//Parameter
	double	m_factor; 
    int		m_value;
	bool	m_discrete;
	bool	m_largest;

	//I/O
    vtkImageData*	m_input;
    vtkPolyData*	m_output;

	//Filter
	vtkImageResample*				m_resample;
	vtkMarchingCubes*				m_surface;
	vtkPolyDataConnectivityFilter*	m_connectivityFilter;
	vtkWindowedSincPolyDataFilter*	m_smoother;
};

#endif