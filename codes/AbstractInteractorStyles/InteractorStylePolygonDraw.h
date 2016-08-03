///*
//Author:		Wong, Matthew Lun
//Date:		16th, June 2016
//Occupation:	Chinese University of Hong Kong,
//			Department of Imaging and Inteventional Radiology,
//			Junior Research Assistant
//
//Author:		Lok, Ka Hei Jason
//Date:		16th, June 2016
//Occupation:	Chinese University of Hong Kong,
//			Department of Imaging and Inteventional Radiology,
//			M.Phil Student
//
//This class allows interactive segmentation on images with contour widget.
//
//Wong Matthew Lun, Lok Ka Hei
//Copyright (C) 2016
//*/
//
//#pragma once
//
//
//#include <QTime>
//#include <vtkContourWidget.h>
//#include <vtkRenderWindowInteractor.h>
//#include <vtkOrientedGlyphContourRepresentation.h>
//#include "AbstractInteractorStyleImage.h"
//
//
//class InteractorStylePolygonDraw : public AbstractInteractorStyleImage
//{
//public:
//	vtkTypeMacro(InteractorStylePolygonDraw, AbstractInteractorStyleImage);
//	static InteractorStylePolygonDraw* New();
//
//	void SetPolygonModeEnabled(bool b);
//
//	void SetDrawColor(int r, int g, int b);
//	void SetDrawColor(const int* rgb);
//	void SetDrawOpacity(int opacity);
//
//	void SetDrawIsotropicCheckBox(QCheckBox* checkBox);
//
//protected:
//	InteractorStylePolygonDraw();
//	~InteractorStylePolygonDraw();
//
//	virtual void OnLeftButtonDown();
//	virtual void OnRightButtonDown();
//	virtual void OnMouseMove();
//	virtual void OnKeyPress();
//
//
//private:
//	bool CheckDoubleClicked();
//
//	void DisplayPolygon(vtkObject*, long unsigned, void*);
//	void FillPolygon();
//
//	QTime m_timer;
//	int m_firstClickTimeStamp;
//
//	bool DOUBLE_CLICKED_FLAG;
//	bool CONTOUR_IS_ON_FLAG;
//
//	int color_r, color_g, color_b;
//	int opacity;
//
//	//PaintBrush
//	QComboBox*	m_drawValueComboBox;
//
//
//	vtkContourWidget* m_contourWidget;
//	vtkOrientedGlyphContourRepresentation* m_contourRep;
//	vtkSmartPointer<vtkCallbackCommand> m_callbackFunction;
//};