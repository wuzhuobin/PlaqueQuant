#ifndef IMAGE_H
#define IMAGE_H

#include <QObject>
#include <QDebug>
#include <QStringList>
#include <vtkLookupTable.h>
#include <vtkImageData.h>
#include "Define.h"
#include "AbstractData.h"

class Image : public QObject, public AbstractData
{
    Q_OBJECT
public:
    explicit Image(QObject *parent = 0);
	~Image();

	virtual void SetImage(vtkImageData*);
	void SetLUT(vtkLookupTable* lut);

	vtkImageData*			GetImageData();
	int						GetLUT();
	vtkLookupTable*			GetLookUpTable();
	//double*					GetSpacing();
	//void					GetPosition(double* pos);
	


public slots:

private:
    void InstallPipeline();

	vtkImageData*						m_imageData;
	vtkLookupTable*						m_lut;

	int		m_lutType;
};


#endif // IMAGE_H
