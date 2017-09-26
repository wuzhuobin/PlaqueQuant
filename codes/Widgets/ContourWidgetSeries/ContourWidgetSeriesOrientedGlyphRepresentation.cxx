#include "ContourWidgetSeriesOrientedGlyphRepresentation.h"

#include <vtkOrientedGlyphContourRepresentation.h>
#include <vtkProperty.h>
#include <vtkObjectFactory.h>

#include <list>

vtkStandardNewMacro(ContourWidgetSeriesOrientedGlyphRepresentation);

void ContourWidgetSeriesOrientedGlyphRepresentation::PrintSelf(ostream & os, vtkIndent indent)
{
}

vtkContourRepresentation * ContourWidgetSeriesOrientedGlyphRepresentation::GetContourRepresentation(unsigned int num)
{
	if (num < this->GetNumberOfContours()) {
		return ContourWidgetSeriesRepresentation::GetContourRepresentation(num);
	}
	else // create one
	{
		vtkContourRepresentation* rep =
			ContourWidgetSeriesRepresentation::GetContourRepresentation(num);

		vtkOrientedGlyphContourRepresentation* __rep =
			vtkOrientedGlyphContourRepresentation::SafeDownCast(this->ContourRepresentation);
		vtkOrientedGlyphContourRepresentation* _rep =
			vtkOrientedGlyphContourRepresentation::SafeDownCast(rep);
		//rep->DeepCopy(this->ContourRepresentation);
		// no #DeepCopy(), do it manually
		if (_rep && __rep) {
			_rep->SetCursorShape(__rep->GetCursorShape());
			_rep->SetActiveCursorShape(__rep->GetActiveCursorShape());
			_rep->GetProperty()->DeepCopy(__rep->GetProperty());
			_rep->GetActiveProperty()->DeepCopy(__rep->GetActiveProperty());
			_rep->GetLinesProperty()->DeepCopy(__rep->GetLinesProperty());
		}

		return rep;
	}

}

ContourWidgetSeriesOrientedGlyphRepresentation::ContourWidgetSeriesOrientedGlyphRepresentation()
{
	if (this->ContourRepresentation == NULL) {
		vtkOrientedGlyphContourRepresentation* rep = 
			vtkOrientedGlyphContourRepresentation::New();
		rep->SetLineColor(0, 0, 0);
		SetContourRepresentation(rep);
		rep->Delete();
	}
}

ContourWidgetSeriesOrientedGlyphRepresentation::~ContourWidgetSeriesOrientedGlyphRepresentation()
{
}