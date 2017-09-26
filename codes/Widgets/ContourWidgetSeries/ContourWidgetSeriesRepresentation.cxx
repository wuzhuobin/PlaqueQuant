#include "ContourWidgetSeriesRepresentation.h"

#include <vtkObjectFactory.h>
#include <vtkContourRepresentation.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>
#include <vtkMath.h>
#include <vtkLine.h>

#include <iterator>
#include <list>

vtkStandardNewMacro(ContourWidgetSeriesRepresentation);
vtkCxxSetObjectMacro(ContourWidgetSeriesRepresentation, ContourRepresentation, vtkContourRepresentation);

// The vtkHandleList is a PIMPLed list<T>.
class vtkContourList : public std::list<vtkContourRepresentation*> {};
typedef std::list<vtkContourRepresentation*>::iterator vtkContourListIterator;

void ContourWidgetSeriesRepresentation::PrintSelf(ostream & os, vtkIndent indent)
{
}

int ContourWidgetSeriesRepresentation::GetNumberOfContours()
{
	return static_cast<int>(this->Contours->size());
}

vtkContourRepresentation * ContourWidgetSeriesRepresentation::GetContourRepresentation(unsigned int num)
{
	if (num < this->Contours->size())
	{
		vtkContourListIterator iter = this->Contours->begin();
		std::advance(iter, num);
		return (*iter);
	}
	else //create one
	{
		if (this->ContourRepresentation == NULL)
		{
			vtkErrorMacro("GetHandleRepresentation " << num << ", no handle representation has been set yet, cannot create a new handle.");
			return NULL;
		}
		vtkContourRepresentation *rep = this->ContourRepresentation->NewInstance();
		//rep->DeepCopy(this->ContourRepresentation);
		// no #DeepCopy(), do it manually
		rep->SetPixelTolerance(this->ContourRepresentation->GetPixelTolerance());
		rep->SetWorldTolerance(this->ContourRepresentation->GetWorldTolerance());
		rep->SetCurrentOperation(this->ContourRepresentation->GetCurrentOperation());
		rep->SetPointPlacer(this->ContourRepresentation->GetPointPlacer());
		rep->SetLineInterpolator(this->ContourRepresentation->GetLineInterpolator());
		rep->SetShowSelectedNodes(this->ContourRepresentation->GetShowSelectedNodes());
		rep->SetClosedLoop(this->ContourRepresentation->GetClosedLoop());
		this->Contours->push_back(rep);
		return rep;
	}
}

int ContourWidgetSeriesRepresentation::GetActiveContour()
{
	return this->ActiveContour;
}

int ContourWidgetSeriesRepresentation::CreateContour(/*double e[2]*/)
{
	//double pos[3];
	//pos[0] = e[0];
	//pos[1] = e[1];
	//pos[2] = 0.0;

	vtkContourRepresentation *rep = this->GetContourRepresentation(
		static_cast<int>(this->Contours->size()));
	if (rep == NULL)
	{
		vtkErrorMacro("CreateHandle: no handle representation set yet! Cannot create a new handle.");
		return -1;
	}
	//rep->SetDisplayPosition(pos);
	//rep->SetTolerance(this->Tolerance); //needed to ensure that picking is consistent
	this->ActiveContour = static_cast<int>(this->Contours->size()) - 1;
	return this->ActiveContour;
}

void ContourWidgetSeriesRepresentation::RemoveLastContour()
{
	if (this->Contours->size() < 1)
	{
		return;
	}

	// Delete last handle
	this->Contours->back()->Delete();
	this->Contours->pop_back();
}

void ContourWidgetSeriesRepresentation::RemoveActiveContour()
{
	if (this->Contours->size() < 1)
	{
		return;
	}
	if (this->ActiveContour >= 0 && this->ActiveContour < static_cast<int>(this->Contours->size()))
	{
		vtkContourListIterator iter = this->Contours->begin();
		std::advance(iter, this->ActiveContour);
		vtkContourRepresentation *cr = *iter;
		this->Contours->erase(iter);
		cr->Delete();
		this->ActiveContour = -1;
	}
}

void ContourWidgetSeriesRepresentation::RemoveContour(int n)
{
	// Remove nth handle

	if (n == this->ActiveContour)
	{
		this->RemoveActiveContour();
		return;
	}

	if (static_cast<int>(this->Contours->size()) <= n)
	{
		return;
	}

	vtkContourListIterator iter = this->Contours->begin();
	std::advance(iter, n);
	vtkContourRepresentation *cr = *iter;
	this->Contours->erase(iter);
	cr->Delete();
}

void ContourWidgetSeriesRepresentation::BuildRepresentation()
{
	if (this->ActiveContour >= 0 && this->ActiveContour < static_cast<int>(this->Contours->size()))
	{
		vtkContourRepresentation *rep = this->GetContourRepresentation(this->ActiveContour);
		if (rep)
		{
			rep->BuildRepresentation();
		}
	}
}

int ContourWidgetSeriesRepresentation::ComputeInteractionState(int X, int Y, int vtkNotUsed(modify))
{
	// Loop over all the contours to see if the point is close to any of them.
	int i;
	vtkContourListIterator iter;
	for (i = 0, iter = this->Contours->begin(); iter != this->Contours->end(); ++iter, ++i)
	{
		if (*iter != NULL && (*iter)->GetNumberOfNodes() > 1)
		{
			if ((*iter)->ActivateNode(X, Y)) {
				this->ActiveContour = i;
				this->InteractionState = ContourWidgetSeriesRepresentation::NEAR_NODE;
				return this->InteractionState;
			}
			else if (PointOnContour((*iter), X, Y))
			{
				this->ActiveContour = i;
				this->InteractionState = ContourWidgetSeriesRepresentation::NEAR_CONTOUR;
				return this->InteractionState;
			}
		}
	}

	// Nothing found, so it's outside
	this->InteractionState = ContourWidgetSeriesRepresentation::OUTSIDE;
	return this->InteractionState;
}

bool ContourWidgetSeriesRepresentation::PointOnContour(vtkContourRepresentation * contourRep, int X, int Y)
{
	// Make a line out of this viewing ray
	double p1[4], p2[4], *p3 = NULL, *p4 = NULL;

	double tmp1[4], tmp2[4];
	tmp1[0] = X;
	tmp1[1] = Y;
	tmp1[2] = 0.0;
	contourRep->GetRenderer()->SetDisplayPoint(tmp1);
	contourRep->GetRenderer()->DisplayToWorld();
	contourRep->GetRenderer()->GetWorldPoint(p1);

	tmp1[2] = 1.0;
	contourRep->GetRenderer()->SetDisplayPoint(tmp1);
	contourRep->GetRenderer()->DisplayToWorld();
	contourRep->GetRenderer()->GetWorldPoint(p2);

	double closestDistance2 = VTK_DOUBLE_MAX;
	int closestNode = 0;

	// compute a world tolerance based on pixel
	// tolerance on the focal plane
	double fp[4];
	contourRep->GetRenderer()->GetActiveCamera()->GetFocalPoint(fp);
	fp[3] = 1.0;
	contourRep->GetRenderer()->SetWorldPoint(fp);
	contourRep->GetRenderer()->WorldToDisplay();
	contourRep->GetRenderer()->GetDisplayPoint(tmp1);

	tmp1[0] = 0;
	tmp1[1] = 0;
	contourRep->GetRenderer()->SetDisplayPoint(tmp1);
	contourRep->GetRenderer()->DisplayToWorld();
	contourRep->GetRenderer()->GetWorldPoint(tmp2);

	tmp1[0] = contourRep->GetPixelTolerance();
	contourRep->GetRenderer()->SetDisplayPoint(tmp1);
	contourRep->GetRenderer()->DisplayToWorld();
	contourRep->GetRenderer()->GetWorldPoint(tmp1);

	double wt2 = vtkMath::Distance2BetweenPoints(tmp1, tmp2);

	// Now loop through all lines and look for closest one within tolerance
	for (int i = 0; i<contourRep->GetNumberOfNodes(); i++)
	{
		for (int j = 0; j <= contourRep->GetNthNode(i)->Points.size(); j++)
		{
			if (j == 0)
			{
				p3 = contourRep->GetNthNode(i)->WorldPosition;
				if (contourRep->GetNthNode(i)->Points.size())
				{
					p4 = contourRep->GetNthNode(i)->Points[j]->WorldPosition;
				}
				else
				{
					if (i < contourRep->GetNumberOfNodes() - 1)
					{
						p4 = contourRep->GetNthNode(i + 1)->WorldPosition;
					}
					else if (contourRep->GetClosedLoop())
					{
						p4 = contourRep->GetNthNode(0)->WorldPosition;
					}
				}
			}
			else if (j == contourRep->GetNthNode(i)->Points.size())
			{
				p3 = contourRep->GetNthNode(i)->Points[j - 1]->WorldPosition;
				if (i < contourRep->GetNumberOfNodes() - 1)
				{
					p4 = contourRep->GetNthNode(i + 1)->WorldPosition;
				}
				else if (contourRep->GetClosedLoop())
				{
					p4 = contourRep->GetNthNode(0)->WorldPosition;
				}
				else
				{
					// Shouldn't be able to get here (only if we don't have
					// a closed loop but we do have intermediate points after
					// the last node - contradictary conditions)
					continue;
				}
			}
			else
			{
				p3 = contourRep->GetNthNode(i)->Points[j - 1]->WorldPosition;
				p4 = contourRep->GetNthNode(i)->Points[j]->WorldPosition;
			}

			// Now we have the four points - check closest intersection
			double u, v;

			if (vtkLine::Intersection(p1, p2, p3, p4, u, v))
			{
				double p5[3], p6[3];
				p5[0] = p1[0] + u*(p2[0] - p1[0]);
				p5[1] = p1[1] + u*(p2[1] - p1[1]);
				p5[2] = p1[2] + u*(p2[2] - p1[2]);

				p6[0] = p3[0] + v*(p4[0] - p3[0]);
				p6[1] = p3[1] + v*(p4[1] - p3[1]);
				p6[2] = p3[2] + v*(p4[2] - p3[2]);

				double d = vtkMath::Distance2BetweenPoints(p5, p6);

				if (d < wt2 && d < closestDistance2)
				{
					closestDistance2 = d;
					closestNode = static_cast<int>(i);
				}
			}
			else
			{
				double d = vtkLine::DistanceToLine(p3, p1, p2);
				if (d < wt2 && d < closestDistance2)
				{
					closestDistance2 = d;
					closestNode = static_cast<int>(i);
				}

				d = vtkLine::DistanceToLine(p4, p1, p2);
				if (d < wt2 && d < closestDistance2)
				{
					closestDistance2 = d;
					closestNode = static_cast<int>(i);
				}
			}
		}
	}

	if (closestDistance2 < VTK_DOUBLE_MAX)
	{
		if (closestNode < contourRep->GetNumberOfNodes() - 1)
		{
			return true;
		}
		else if (contourRep->GetClosedLoop())
		{
			return true;
		}
	}

	return false;
}

ContourWidgetSeriesRepresentation::ContourWidgetSeriesRepresentation()
{
	this->ContourRepresentation = NULL;

	// The representation for the seed handles
	this->Contours = new vtkContourList;
	this->ActiveContour = -1;

	this->Tolerance = 5;
}

ContourWidgetSeriesRepresentation::~ContourWidgetSeriesRepresentation()
{
	if (this->ContourRepresentation)
	{
		this->ContourRepresentation->Delete();
	}

	// Loop over all handles releasing their observes and deleting them
	vtkContourListIterator iter;
	for (iter = this->Contours->begin(); iter != this->Contours->end(); ++iter)
	{
		(*iter)->Delete();
	}
	delete this->Contours;
}
