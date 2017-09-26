#ifndef __CONTOUR_WIDGET_SERIES_ORIENTED_GLYPH_REPRESENTATION_H__
#define __CONTOUR_WIDGET_SERIES_ORIENTED_GLYPH_REPRESENTATION_H__

#include <ContourWidgetSeriesRepresentation.h>
class vtkOrientedGlyphContourRepresentation;

class ContourWidgetSeriesOrientedGlyphRepresentation: public ContourWidgetSeriesRepresentation
{
public:
	static ContourWidgetSeriesOrientedGlyphRepresentation* New();

	vtkTypeMacro(ContourWidgetSeriesOrientedGlyphRepresentation, ContourWidgetSeriesRepresentation);
	void PrintSelf(ostream& os, vtkIndent indent);

	// Description:
	// Get the Contour representations used for a particular seed. A side effect of
	// this method is that it will create a Contour representation in the list of
	// representations if one has not yet been created.
	virtual vtkContourRepresentation *GetContourRepresentation(unsigned int num) override;

protected:
	ContourWidgetSeriesOrientedGlyphRepresentation();
	virtual ~ContourWidgetSeriesOrientedGlyphRepresentation();

	//vtkOrientedGlyphContourRepresentation* ContourRepresentation = nullptr;

private:
	ContourWidgetSeriesOrientedGlyphRepresentation(const ContourWidgetSeriesOrientedGlyphRepresentation&);  //Not implemented
	void operator=(const ContourWidgetSeriesOrientedGlyphRepresentation&);  //Not implemented

};


#endif // !__CONTOUR_WIDGET_SERIES_ORIENTED_GLYPH_REPRESENTATION_H__
