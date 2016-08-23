#ifndef OVERLAY_H
#define OVERLAY_H

#include <vtkExtractVOI.h>
#include "vtkImageData.h"
#include <QObject>
#include "Define.h"
#include <vtkVersion.h>
class Overlay : public QObject
{
	Q_OBJECT

public:
	Overlay(QObject* parent = NULL);
	~Overlay();
	bool Update();
	
	void Initialize(vtkImageData* img);
	void Initialize(ImageType::Pointer, int dim[3], double spacing[3], double origin[3], int type);

	void SetDisplayExtent(int*);
	int* GetDisplayExtent();
	void GetDisplayExtent(int*);

	void DisplayExtentOn();
	void DisplayExtentOff();

	void SetInputImageData(vtkImageData* imagedata);
	void SetInputImageData(QString);
	void SetPixel(int pos[3], double value);
	void SetPixel(int pos[3], unsigned char value);
	void SetVisibleImageNo(int);

	vtkImageData* GetOutput();
	vtkImageData* GetVTKImageData();
	ImageType::Pointer GetITKOutput();

private:

	vtkImageData* m_vtkOverlay;
	vtkImageData* m_vtkOverlayHolder;
	ImageType::Pointer m_itkOverlay;
	DuplicatorType::Pointer m_duplicator;
	int m_visible_image_no;

	int DisplayExtent[6];
};

#endif

