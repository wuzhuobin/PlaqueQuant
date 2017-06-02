#include "QInteractorStylePolygonDrawSeries.h"
#include "ui_QInteractorStylePolygonDrawSeries.h"
#include "ui_QAbstractNavigation.h"

#include <vtkObjectFactory.h>
#include <vtkOrientedGlyphContourRepresentation.h>
#include <vtkImageData.h>
#include <vtkContourWidget.h>
#include <vtkImageActorPointPlacer.h>
#include <vtkCommand.h>
#include <vtkProperty.h>
#include <vtkLookupTable.h>
#include <vtkPolyData.h>
#include <vtkCellData.h>
#include <vtkLinearContourLineInterpolator.h>
#include <vtkBezierContourLineInterpolator.h>

#include "MagneticContourLineInterpolator.h"
#include "ContourWidgetSeries.h"
#include "ContourWidgetSeriesOrientedGlyphRepresentation.h"
#include "ImageViewer.h"

vtkStandardNewMacro(QInteractorStylePolygonDrawSeries);
QSETUP_UI_SRC(QInteractorStylePolygonDrawSeries);

QInteractorStylePolygonDrawSeries::ContourMap QInteractorStylePolygonDrawSeries::m_normalXZ;
QInteractorStylePolygonDrawSeries::ContourMap QInteractorStylePolygonDrawSeries::m_normalYZ;
QInteractorStylePolygonDrawSeries::ContourMap QInteractorStylePolygonDrawSeries::m_normalXY;


class QInteractorStylePolygonDrawSeriesCallback : public vtkCommand
{
public:
	vtkTypeMacro(QInteractorStylePolygonDrawSeriesCallback, vtkCommand);
	static QInteractorStylePolygonDrawSeriesCallback* New() {
		return new QInteractorStylePolygonDrawSeriesCallback;
	}
	virtual void Execute(vtkObject *caller, unsigned long eventId, void *callData) override {
		//ContourWidgetSeries* contourSeries = ContourWidgetSeries::SafeDownCast(caller);
		self->SaveContoursToContourMap();
	}
	QInteractorStylePolygonDrawSeries* self = nullptr;

};


QInteractorStylePolygonDrawSeries::QInteractorStylePolygonDrawSeries(int uiType, QWidget * parent)
{
	QNEW_UI();
}

QInteractorStylePolygonDrawSeries::~QInteractorStylePolygonDrawSeries()
{
	QDELETE_UI();
}

void QInteractorStylePolygonDrawSeries::uniqueInitialization()
{


}

void QInteractorStylePolygonDrawSeries::initialization()
{
	m_callback =
		vtkSmartPointer<QInteractorStylePolygonDrawSeriesCallback>::New();
	m_callback->self = this;

	m_contourSeries->AddObserver(vtkCommand::InteractionEvent, m_callback);
	m_contourSeries->AddObserver(vtkCommand::EndInteractionEvent, m_callback);

	connect(ui->cleanAllPushButton, SIGNAL(clicked()),
		this, SLOT(CleanAll()));
	connect(ui->cleanOnePushButton, SIGNAL(clicked()),
		this, SLOT(CleanOne()));
	connect(ui->fillContourPushButton, SIGNAL(clicked()),
		this, SLOT(FillContours()));

	connect(ui->polygonRadionButton, SIGNAL(clicked()),
		this, SLOT(SetLinearInterpolator()));
	connect(ui->smoothCurveRadioButton, SIGNAL(clicked()), 
		this, SLOT(SetBezierInterpolator()));
	connect(ui->radioButtonMagnetic, SIGNAL(clicked()),
		this, SLOT(SetMagneticInterpolator()));
	connect(ui->comboBoxLabel, SIGNAL(currentIndexChanged(int)),
		this, SLOT(SetContourLabel(int)));
	
}

void QInteractorStylePolygonDrawSeries::SaveContoursToContourMap()
{
	if (m_contourSeries->GetEnabled()) {
		switch (this->GetSliceOrientation())
		{
		case ImageViewer::SLICE_ORIENTATION_YZ:
			this->SaveContoursToContourMap(this->GetSlice(), this->m_contourSeries, this->m_normalYZ);
			break;
		case ImageViewer::SLICE_ORIENTATION_XZ:
			this->SaveContoursToContourMap(this->GetSlice(), this->m_contourSeries, this->m_normalXZ);
			break;
		case ImageViewer::SLICE_ORIENTATION_XY:
			this->SaveContoursToContourMap(this->GetSlice(), this->m_contourSeries, this->m_normalXY);
			break;
		default:
			break;
		}
	}
}

void QInteractorStylePolygonDrawSeries::SaveContoursToContourMap(int slice, ContourWidgetSeries * contourWidgetSeries, ContourMap & contourMap)
{
	ContourWidgetSeriesRepresentation* rep = contourWidgetSeries->GetContourWidgetSeriesRepresentation();
	
	ContourMapElementPtr element = ContourMapElementPtr(new PolyDataList);

	for (int i = 0; i < rep->GetNumberOfContours(); ++i) {
		vtkSmartPointer<vtkPolyData> nodePolyData =
			vtkSmartPointer<vtkPolyData>::New();
		rep->GetContourRepresentation(i)->GetNodePolyData(nodePolyData);

		vtkOrientedGlyphContourRepresentation* _rep =
			vtkOrientedGlyphContourRepresentation::SafeDownCast(rep->GetContourRepresentation(i));

		double* colour = 
			_rep->GetLinesProperty()->GetColor();

		vtkSmartPointer<vtkUnsignedCharArray> colourArray =
			vtkSmartPointer<vtkUnsignedCharArray>::New();
		colourArray->SetName(COLOUR);
		colourArray->SetNumberOfComponents(3);
		colourArray->InsertNextTuple3(
			colour[0],
			colour[1],
			colour[2]);

		nodePolyData->GetCellData()->AddArray(colourArray);

		int interpolator = -1;
		const char* interpolatorName = _rep->GetLineInterpolator()->GetClassName();
		if (strcmp("vtkLinearContourLineInterpolator", interpolatorName) == 0) {
			interpolator = 0;
		}
		else if (strcmp("vtkBezierContourLineInterpolator", interpolatorName) == 0) {
			interpolator = 1;
		}
		else if (strcmp("MagneticContourLineInterpolator", interpolatorName) == 0) {
			interpolator = 2;
		}

		vtkSmartPointer<vtkUnsignedCharArray> interpolatorArray =
			vtkSmartPointer<vtkUnsignedCharArray>::New();
		interpolatorArray->SetName(INTERPOLATOR);
		interpolatorArray->SetNumberOfComponents(1);
		interpolatorArray->InsertNextTuple1(interpolator);

		nodePolyData->GetCellData()->AddArray(interpolatorArray);

		(*element) << nodePolyData;

	}
	contourMap[slice] = element;

}

void QInteractorStylePolygonDrawSeries::LoadContoursFromContourMap()
{
	if (this->m_contourSeries->GetEnabled()) {
		int ijk[3];
		GetImageViewer()->GetFocalPointWithImageCoordinate(ijk); 
		switch (this->GetSliceOrientation())
		{
		case ImageViewer::SLICE_ORIENTATION_YZ:
			/*if (i != ijk[0])*/ {
			//m_contourSeries->ResetContours();
			LoadContoursFromContourMap(ijk[0], m_contourSeries, m_normalYZ);
			m_contourSeries->Render();
		}
			break;
		case ImageViewer::SLICE_ORIENTATION_XZ:
			/*if (j != ijk[1]) */ {
			//m_contourSeries->ResetContours();
			LoadContoursFromContourMap(ijk[1], m_contourSeries, m_normalXZ);
			m_contourSeries->Render();
		}
			break;
		case ImageViewer::SLICE_ORIENTATION_XY:
			/*if (k != ijk[2])*/ {
			//m_contourSeries->ResetContours();
			LoadContoursFromContourMap(ijk[2], m_contourSeries, m_normalXY);
			m_contourSeries->Render();

		}
		default:
			break;
		}
	}
}


void QInteractorStylePolygonDrawSeries::LoadContoursFromContourMap(int slice, ContourWidgetSeries * contourWidgetSeries, ContourMap & contourMap)
{
	if (contourMap.contains(slice)) {
		ContourMapElementPtr element = contourMap[slice];

		for (int i = 0; i < element->size(); ++i) {
			vtkPolyData* nodePolyData = element->at(i);
			if (!nodePolyData || nodePolyData->GetNumberOfPoints() < 1) {
				continue;
			}
			vtkContourWidget* _contour = contourWidgetSeries->CreateNewContourWidget();
			vtkOrientedGlyphContourRepresentation* _rep = vtkOrientedGlyphContourRepresentation::SafeDownCast(_contour->GetContourRepresentation());
			
			vtkDataArray* colourArray = nodePolyData->GetCellData()->GetArray(COLOUR);
			if (colourArray) {
				double* colour = colourArray->GetTuple3(0);
				_rep->GetLinesProperty()->SetColor(colour);
			}
			vtkDataArray* interpolatorArray = nodePolyData->GetCellData()->GetArray(INTERPOLATOR);
			if (interpolatorArray) {
				int interpolator = static_cast<int>(interpolatorArray->GetTuple1(0) + 0.5);
				switch (interpolator)
				{
				case LINEAR:
					_rep->SetLineInterpolator(vtkSmartPointer<vtkLinearContourLineInterpolator>::New());
					break;
				case BEZIER:
					_rep->SetLineInterpolator(vtkSmartPointer<vtkBezierContourLineInterpolator>::New());
					break;
				case MAGNETIC:
					_rep->SetLineInterpolator(vtkSmartPointer<MagneticContourLineInterpolator>::New());
					break;
				default:
					break;
				}
			}

			_contour->EnabledOn();
			_contour->Initialize(nodePolyData);

		}

	}
	contourWidgetSeries->SetWidgetState(ContourWidgetSeries::MANIPULATE);
}

void QInteractorStylePolygonDrawSeries::OnEnter()
{
	InteractorStylePolygonDrawSeries::OnEnter();
	QList<QInteractorStylePolygonDrawSeries*> _disable;
	QList<QInteractorStylePolygonDrawSeries*> _enable;
	for (std::list<AbstractInteractorStyleImage*>::const_iterator cit =
		m_imageStyles.cbegin(); cit != m_imageStyles.cend(); ++cit) {
		QInteractorStylePolygonDrawSeries* _style = QInteractorStylePolygonDrawSeries::SafeDownCast(*cit);
		if (_style != nullptr && _style->GetCustomEnabled()) {
			// find out all styles which needs to be disable
			if (_style->m_contourSeries->GetEnabled() &&
				_style->GetSliceOrientation() == this->GetSliceOrientation() &&
				_style != this) {
				_disable << _style;
			}
			// find out all styles which needs to be enable
			else if (!_style->m_contourSeries->GetEnabled() && 
				(_style->GetSliceOrientation() != this->GetSliceOrientation() || _style == this)) {
				_enable << _style;
			}
		}
	}
	foreach(QInteractorStylePolygonDrawSeries* style, _disable) {
		style->m_contourSeries->ResetContours();
		style->m_contourSeries->EnabledOff();
	}
	foreach(QInteractorStylePolygonDrawSeries* style, _enable) {
		style->m_contourSeries->EnabledOn();
		style->LoadContoursFromContourMap();
	}
}


void QInteractorStylePolygonDrawSeries::SetCustomEnabled(bool flag)
{
	InteractorStylePolygonDrawSeries::SetCustomEnabled(flag);
	uniqueInvoke(flag);
}

void QInteractorStylePolygonDrawSeries::SetCurrentFocalPointWithImageCoordinate(int i, int j, int k)
{
	InteractorStylePolygonDrawSeries::SetCurrentFocalPointWithImageCoordinate(i, j, k);
	QAbstractNavigation::SetCurrentFocalPointWithImageCoordinate(i, j, k);
}

void QInteractorStylePolygonDrawSeries::SetCurrentFocalPointWithImageCoordinateByViewer(int i, int j, int k)
{
	int ijk[3];
	GetImageViewer()->GetFocalPointWithImageCoordinate(ijk); 
	InteractorStylePolygonDrawSeries::SetCurrentFocalPointWithImageCoordinateByViewer(i, j, k);
	if (GetSlice() != ijk[GetSliceOrientation()]) {
		m_contourSeries->ResetContours();
		LoadContoursFromContourMap();
	}
}


void QInteractorStylePolygonDrawSeries::SetContourLabel(int contourLabel)
{
	InteractorStylePolygonDrawSeries::SetContourLabel(contourLabel);
}

void QInteractorStylePolygonDrawSeries::CleanOne()
{
	if (m_contourSeries->GetEnabled()) {
		m_contourSeries->DeleteContour(m_contourSeriesRep->GetNumberOfContours() - 1);
		m_contourSeries->InvokeEvent(vtkCommand::InteractionEvent);
		m_contourSeries->Render();
	}
}

void QInteractorStylePolygonDrawSeries::CleanAll()
{
	m_normalXY.clear();
	m_normalXZ.clear();
	m_normalYZ.clear();
	m_contourSeries->ResetContours();
	m_contourSeries->Render();
}

void QInteractorStylePolygonDrawSeries::FillContours()
{
	if (m_contourSeries->GetEnabled()) {
		InteractorStylePolygonDrawSeries::FillContours();
	}
}

void QInteractorStylePolygonDrawSeries::SetFillSliceBegin(int slice)
{
	this->m_fillSliceBegin = slice;
}

void QInteractorStylePolygonDrawSeries::SetFillSliceEnd(int slice)
{
	this->m_fillSliceEnd = slice;
}

void QInteractorStylePolygonDrawSeries::FillSlices()
{
	if (!m_contourSeries->GetEnabled() ||
		GetSliceOrientation() != ImageViewer::SLICE_ORIENTATION_XY) {
		return;
	}
	
	int extent[6];
	GetImageViewer()->GetDisplayExtent(extent);

	extent[4] = std::max(m_fillSliceBegin, extent[4]);
	extent[5] = std::min(m_fillSliceEnd, extent[5]);

	for (int i = extent[4]; i <= extent[5]; ++i) {
		SetCurrentSlice(i);
		FillContours();
	}

}


