#include <vtkCenterOfMass.h>
#include <vtkConnectivityFilter.h>
#include <vtkImageCast.h>
#include <vtkNIFTIImageWriter.h>
#include <vtkMath.h>
#include <vtkGeometryFilter.h>
#include <vtkSplineFilter.h>
#include <vtkParametricSpline.h>
#include <vtkObjectFactory.h>


//#include "MainWindow.h"


#include "MaximumWallThickness.h"
#include "vtkKdTree.h"

vtkStandardNewMacro(MaximumWallThickness);

MaximumWallThickness::MaximumWallThickness()
{
	this->m_image = NULL;

	this->m_contourFilter = vtkSmartPointer<vtkContourFilter>::New();
	this->m_sliceImage = NULL;

}

MaximumWallThickness::~MaximumWallThickness()
{
}



std::vector<MaximumWallThickness::DistanceLoopPair> MaximumWallThickness::GetDistanceLoopPairVect()
{
	return this->m_distanceVect;
}

void MaximumWallThickness::SetLumemIntensity(int i)
{
	m_lumenIntensity = i;
}

void MaximumWallThickness::SetVesselIntensity(int i)
{
	m_vesselIntensity = i;
}

void MaximumWallThickness::SetImage(vtkImageData *im)
{
	this->m_image = im;
	this->m_image->GetExtent(this->m_extent);
}

void MaximumWallThickness::SetSliceNumber(int i)
{
	this->m_sliceNumber = i;
}

void MaximumWallThickness::SetSliceImage(vtkImageData *im)
{
	this->m_sliceImage = im;

	// check extent
	this->m_sliceImage->GetExtent(this->m_extent);
	if (this->m_extent[4] != this->m_extent[5]) {
		throw ERROR_INPUT_NOT_A_SLICE;
	}
	else {
		this->m_sliceNumber = this->m_extent[4];
	}
}

void MaximumWallThickness::Update()
{
	m_loopPairVect.clear();
	m_distanceVect.clear();
	//if (!this->ExtractSlice()) {
	//	throw ERROR_EXTRACT_SLICE;
	//	return;
	//}

	//if (!this->ValueTransform()) {
	//	throw ERROR_VALUE_TRANSFORM;
	//	return;
	//}

	//if (!this->ExtractLoops()) {
	//	throw ERROR_EXTRACT_LOOP;
	//	return;
	//}

	//if (!this->ThicknessCal()) {
	//	throw ERROR_THICKNESS_CAL;
	//	return;
	//}

	try {
		this->ExtractSlice();
		this->ValueTransform();
		this->ExtractLoops();
		this->ThicknessCal();
	}
	catch (ERROR_CODE e) {
		throw e;
	}

}

int MaximumWallThickness::CheckNumberOfBranches()
{
	vtkSmartPointer<vtkImageThreshold> thres = vtkSmartPointer<vtkImageThreshold>::New();
	thres->ThresholdBetween(m_lumenIntensity - 0.1, m_lumenIntensity+ 0.1);
	thres->SetInputData(this->m_sliceImage);
	thres->SetOutValue(0);
	thres->SetInValue(1);
	thres->ReplaceInOn();
	thres->Update();

	this->m_contourFilter->SetInputData(thres->GetOutput());
	this->m_contourFilter->SetValue(0, 0.1);
	this->m_contourFilter->SetNumberOfContours(1);
	this->m_contourFilter->Update();

	//// Debug
	//cout << "Range: " << this->m_sliceImage->GetScalarTypeAsString() << " " << this->m_sliceImage->GetScalarRange()[0] << " " << this->m_sliceImage->GetScalarRange()[1];
	//vtkSmartPointer<vtkImageActor> actor = vtkSmartPointer<vtkImageActor>::New();
	//actor->SetInputData(thres->GetOutput());
	//actor->Update();
	////std::vector<vtkActor*> renme({ GetActor(contourFilter->GetOutput()) });
	////ActorRenderers* ren = new ActorRenderers;
	////ren->SetRenderList(renme);
	////ren->Render();


	vtkSmartPointer<vtkConnectivityFilter> connectivity = vtkSmartPointer<vtkConnectivityFilter>::New();
	connectivity->SetInputConnection(this->m_contourFilter->GetOutputPort());
	connectivity->SetExtractionModeToAllRegions();
	connectivity->Update();

	//// Debug
	//ActorRenderers* ren = new ActorRenderers;
	//ren->AddActor(GetActor(this->m_contourFilter->GetOutput()));
	//ren->AddViewProp(actor);
	//ren->GetInteractor()->SetInteractorStyle(vtkInteractorStyleImage::New());
	//ren->Render();
	//delete ren;

	return connectivity->GetNumberOfExtractedRegions();
}

bool MaximumWallThickness::ValueTransform()
{
	vtkSmartPointer<vtkImageCast> castFilter =
		vtkSmartPointer<vtkImageCast>::New();
	castFilter->SetInputData(this->m_sliceImage);
	castFilter->SetOutputScalarType(VTK_DOUBLE);
	castFilter->Update();

	this->m_sliceImage = castFilter->GetOutput();

	return true;
}

/* Extract the slice from the input image specified by m_sliceNumber */
bool MaximumWallThickness::ExtractSlice()
{
	// check if the variable is declared
	if (m_sliceImage == NULL)
		this->m_sliceImage = vtkImageData::New();
	else {
		// if slice image is manually set, skip the remaining
		return true;
	}

	//MainWindow* mainwnd = MainWindow::GetMainWindow();
	//mainwnd->GetOverlay()->GetDisplayExtent(m_extent); // Extent is obtained from distplayed extent of overlay

													   // Set the extent to the slice being handled only
	this->m_extent[4] = this->m_sliceNumber;
	this->m_extent[5] = this->m_sliceNumber;

	vtkSmartPointer<vtkExtractVOI> extractVOIFilter = vtkSmartPointer<vtkExtractVOI>::New();
	extractVOIFilter->SetInputData(this->m_image);
	extractVOIFilter->SetVOI(this->m_extent);
	extractVOIFilter->Update();

	// copy the extracted image
	this->m_sliceImage->DeepCopy(extractVOIFilter->GetOutput());

	return true;
}

bool MaximumWallThickness::ExtractLoops()
{
	int numOfLoops = this->CheckNumberOfBranches();
	if (numOfLoops == 0) {
		throw ERROR_NO_SEGMENTATION_FOUND;
	}

	//int vesselWallLabel = Core::LABEL_CALCIFICATION;

	vtkSmartPointer<vtkImageThreshold> thres = vtkSmartPointer<vtkImageThreshold>::New();
	thres->ThresholdBetween(m_lumenIntensity - 0.1, m_lumenIntensity + 0.1);
	thres->SetInputData(this->m_sliceImage);
	thres->SetOutValue(0);
	thres->SetInValue(1);
	thres->ReplaceInOn();
	thres->Update();

	/// Setup contour filter 
	this->m_contourFilter->SetInputData(thres->GetOutput());
	this->m_contourFilter->SetNumberOfContours(1);
	this->m_contourFilter->SetValue(0, 0.1);
	this->m_contourFilter->ComputeScalarsOn();
	this->m_contourFilter->ComputeNormalsOn();
	this->m_contourFilter->Update();

	/// Extract loops
	// extract loops representing vessel wall
	vtkSmartPointer<vtkPolyData> vesselWallLoops = vtkSmartPointer<vtkPolyData>::New();
	vesselWallLoops->DeepCopy(m_contourFilter->GetOutput());

	// extract loops representing lumen
	thres->ThresholdByLower(0.1);
	thres->Update();
	this->m_contourFilter->SetInputData(thres->GetOutput());
	this->m_contourFilter->Update();

	vtkSmartPointer<vtkPolyData> lumenWallLoops = vtkSmartPointer<vtkPolyData>::New();
	lumenWallLoops->DeepCopy(this->m_contourFilter->GetOutput());

	/// Pair loops
	// push each loops into the vector
	std::vector<vtkPolyData*> l_loopsVesselVect, l_loopsLumenVect;
	if (numOfLoops > 1) {
		vtkSmartPointer<vtkThreshold> l_thresFilter = vtkSmartPointer<vtkThreshold>::New();
		vtkSmartPointer<vtkConnectivityFilter>l_connectivityFilter = vtkSmartPointer<vtkConnectivityFilter>::New();
		vtkSmartPointer<vtkGeometryFilter> l_geoFilter = vtkSmartPointer<vtkGeometryFilter>::New();

		// For vessel wall, seperate each loops from the original polydata
		l_connectivityFilter->SetInputData(vesselWallLoops);
		l_connectivityFilter->SetExtractionModeToAllRegions();
		l_connectivityFilter->ColorRegionsOn();
		l_connectivityFilter->Update();
		l_thresFilter->SetInputConnection(l_connectivityFilter->GetOutputPort());
		for (int i = 0; i < l_connectivityFilter->GetNumberOfExtractedRegions(); i++)
		{
			l_thresFilter->ThresholdBetween(double(i) - 0.1, double(i) + 0.1);
			l_thresFilter->Update();

			l_geoFilter->SetInputConnection(l_thresFilter->GetOutputPort());
			l_geoFilter->Update();

			vtkPolyData* l_newPD = vtkPolyData::New();
			l_newPD->DeepCopy(l_geoFilter->GetOutput());
			l_loopsVesselVect.push_back(l_newPD);
		}

		// For lumen
		l_connectivityFilter->SetInputData(lumenWallLoops);
		l_thresFilter->SetInputConnection(l_connectivityFilter->GetOutputPort());
		for (int i = 0; i < l_connectivityFilter->GetNumberOfExtractedRegions(); i++)
		{
			l_thresFilter->ThresholdBetween(double(i) - 0.1, double(i) + 0.1);
			l_thresFilter->Update();

			l_geoFilter->SetInputConnection(l_thresFilter->GetOutputPort());
			l_geoFilter->Update();

			vtkPolyData* l_newPD = vtkPolyData::New();
			l_newPD->DeepCopy(l_geoFilter->GetOutput());
			l_loopsLumenVect.push_back(l_newPD);
		}

		// if there are different number of loops, the branch cannot be defined, throw error
		if (l_loopsLumenVect.size() != l_loopsVesselVect.size()) {
			throw ERROR_UNDEFINED_BRANCH;
			return false;
		}

		// match loops by comparing center distance
		vtkSmartPointer<vtkCenterOfMass> com = vtkSmartPointer<vtkCenterOfMass>::New();
		for (int i = 0; i < l_loopsVesselVect.size(); i++)
		{
			int l_partner;
			double l_d = VTK_DOUBLE_MAX;
			LoopPair* loopPair = new LoopPair;
			vtkPolyData* l1 = vtkPolyData::New(), *l2 = vtkPolyData::New();
			l1->DeepCopy(l_loopsVesselVect.at(i));

			double com1[3];
			com->SetInputData(l_loopsVesselVect.at(i));
			com->SetUseScalarsAsWeights(false);
			com->Update();
			memcpy(com1, com->GetCenter(), sizeof(double) * 3);
			for (int j = 0; j < l_loopsLumenVect.size(); j++)
			{
				double com2[3];
				com->SetInputData(l_loopsLumenVect.at(j));
				com->SetUseScalarsAsWeights(false);
				com->Update();
				memcpy(com2, com->GetCenter(), sizeof(double) * 3);

				double l_l_d = vtkMath::Distance2BetweenPoints(com1, com2);
				if (l_l_d < l_d) {
					l_partner = j;
					l_d = l_l_d;
				}
			}

			l2->DeepCopy(l_loopsLumenVect.at(l_partner));

			loopPair->first = l1;
			loopPair->second = l2;
			this->m_loopPairVect.push_back(*loopPair);
		}
	}
	else {
		LoopPair* loopPair = new LoopPair;
		vtkPolyData* l1 = vtkPolyData::New(), *l2 = vtkPolyData::New();
		l1->DeepCopy(vesselWallLoops);
		l2->DeepCopy(lumenWallLoops);
		loopPair->first = l1;
		loopPair->second = l2;

		this->m_loopPairVect.push_back(*loopPair);
	}

	return true;
}

bool MaximumWallThickness::EdgeDetection()
{
	// sobel edge detector with Non-Maximum-Suppression
	for (int num = 0; num < EDGENUM; ++num) {

		vtkSmartPointer<vtkImageSobel2D> sobelFilter =
			vtkSmartPointer<vtkImageSobel2D>::New();
		sobelFilter->SetInputData(m_edgeImage[num]);
		sobelFilter->Update();
		//Euclidean distance
		// using imageMathematics
		/*vtkSmartPointer<vtkImageExtractComponents> extractXFilter =
		vtkSmartPointer<vtkImageExtractComponents>::New();
		extractXFilter->SetComponents(0);
		extractXFilter->SetInputConnection(sobelFilter->GetOutputPort());
		extractXFilter->Update();

		vtkSmartPointer<vtkImageMathematics> xImageSquare =
		vtkSmartPointer<vtkImageMathematics>::New();
		xImageSquare->SetOperationToSquare();
		xImageSquare->SetInputConnection(extractXFilter->GetOutputPort());
		xImageSquare->Update();

		vtkSmartPointer<vtkImageExtractComponents> extractYFilter =
		vtkSmartPointer<vtkImageExtractComponents>::New();
		extractYFilter->SetComponents(1);
		extractYFilter->SetInputConnection(sobelFilter->GetOutputPort());
		extractYFilter->Update();

		vtkSmartPointer<vtkImageMathematics> yImageSquare =
		vtkSmartPointer<vtkImageMathematics>::New();
		yImageSquare->SetOperationToSquare();
		yImageSquare->SetInputConnection(extractYFilter->GetOutputPort());
		yImageSquare->Update();

		vtkSmartPointer<vtkImageMathematics> imageAdd =
		vtkSmartPointer<vtkImageMathematics>::New();
		imageAdd->SetOperationToAdd();
		imageAdd->SetInput1Data(xImageSquare->GetOutput());
		imageAdd->SetInput2Data(yImageSquare->GetOutput());
		imageAdd->Update();

		vtkSmartPointer<vtkImageMathematics> imageSqrt =
		vtkSmartPointer<vtkImageMathematics>::New();
		imageSqrt->SetOperationToSquareRoot();
		imageSqrt->SetInputConnection(imageAdd->GetOutputPort());
		imageSqrt->Update();*/

		// using Image Magnitude
		vtkSmartPointer<vtkImageMagnitude> magnitudeFilter =
			vtkSmartPointer<vtkImageMagnitude>::New();
		magnitudeFilter->SetInputConnection(sobelFilter->GetOutputPort());
		magnitudeFilter->Update();

		vtkSmartPointer<vtkImageNonMaximumSuppression> nonMaximumSuppressionFilter =
			vtkSmartPointer<vtkImageNonMaximumSuppression>::New();
		nonMaximumSuppressionFilter->SetDimensionality(2);
		nonMaximumSuppressionFilter->SetMagnitudeInputData(magnitudeFilter->GetOutput());
		nonMaximumSuppressionFilter->SetVectorInputData(sobelFilter->GetOutput());
		nonMaximumSuppressionFilter->Update();

		vtkSmartPointer<vtkImageCast> castfilter = vtkSmartPointer<vtkImageCast>::New();
		castfilter->SetInputConnection(nonMaximumSuppressionFilter->GetOutputPort());
		castfilter->SetOutputScalarTypeToInt();
		castfilter->Update();

		m_edgeImage[num]->DeepCopy(castfilter->GetOutput());

		/// OUTPUT the image for debug
		//char filename[256];
		//sprintf(filename, "../../sobleOutput%i.nii.gz", num);
		//vtkSmartPointer<vtkNIFTIImageWriter> writer = vtkSmartPointer<vtkNIFTIImageWriter>::New();
		//writer->SetInputData(m_edgeImage[num]);
		//writer->SetFileName(filename);
		//writer->Update();
		//writer->Write();

	}

	// canny detector
	/*for (int num = 0; num < EDGENUM; ++num) {
	vtkSmartPointer<vtkImageGaussianSmooth> gaussianFilter =
	vtkSmartPointer<vtkImageGaussianSmooth>::New();
	gaussianFilter->SetInputData(edgeImage[num]);
	gaussianFilter->SetRadiusFactors(1, 1, 0);
	gaussianFilter->Update();

	vtkSmartPointer<vtkImageGradient> gradientFilter =
	vtkSmartPointer<vtkImageGradient>::New();
	gradientFilter->SetInputConnection(gaussianFilter->GetOutputPort());
	gradientFilter->Update();

	vtkSmartPointer<vtkImageMagnitude> magnitudeFilter =
	vtkSmartPointer<vtkImageMagnitude>::New();
	magnitudeFilter->SetInputConnection(gradientFilter->GetOutputPort());
	magnitudeFilter->Update();

	vtkSmartPointer<vtkImageNonMaximumSuppression> nonMaximumSuppressionFilter =
	vtkSmartPointer<vtkImageNonMaximumSuppression>::New();
	nonMaximumSuppressionFilter->SetMagnitudeInputData(magnitudeFilter->GetOutput());
	nonMaximumSuppressionFilter->SetVectorInputData(gradientFilter->GetOutput());
	nonMaximumSuppressionFilter->Update();

	vtkSmartPointer<vtkImageConstantPad> constantPadFilter =
	vtkSmartPointer<vtkImageConstantPad>::New();
	constantPadFilter->SetInputConnection(gradientFilter->GetOutputPort());
	constantPadFilter->SetOutputNumberOfScalarComponents(3);
	constantPadFilter->SetConstant(0);
	constantPadFilter->Update();

	vtkSmartPointer<vtkImageToStructuredPoints> i2sp1 =
	vtkSmartPointer<vtkImageToStructuredPoints>::New();
	i2sp1->SetInputConnection(nonMaximumSuppressionFilter->GetOutputPort());
	i2sp1->SetVectorInputData(constantPadFilter->GetOutput());
	i2sp1->Update();

	vtkSmartPointer<vtkLinkEdgels> imgLink =
	vtkSmartPointer<vtkLinkEdgels>::New();
	imgLink->SetInputConnection(i2sp1->GetOutputPort());
	imgLink->SetGradientThreshold(2);
	imgLink->Update();

	vtkSmartPointer<vtkThreshold> thresholdEdgels =
	vtkSmartPointer<vtkThreshold>::New();
	thresholdEdgels->SetInputConnection(imgLink->GetOutputPort());
	thresholdEdgels->ThresholdByUpper(10);
	thresholdEdgels->AllScalarsOff();
	thresholdEdgels->Update();

	vtkSmartPointer<vtkGeometryFilter> gf =
	vtkSmartPointer<vtkGeometryFilter>::New();
	gf->SetInputConnection(thresholdEdgels->GetOutputPort());
	gf->Update();

	vtkSmartPointer<vtkImageToStructuredPoints> i2sp =
	vtkSmartPointer<vtkImageToStructuredPoints>::New();
	i2sp->SetInputConnection(magnitudeFilter->GetOutputPort());
	i2sp->SetVectorInputData(constantPadFilter->GetOutput());
	i2sp->Update();

	vtkSmartPointer<vtkSubPixelPositionEdgels> spe =
	vtkSmartPointer<vtkSubPixelPositionEdgels>::New();
	spe->SetInputConnection(gf->GetOutputPort());
	spe->SetGradMapsData(i2sp->GetStructuredPointsOutput());
	spe->Update();

	vtkSmartPointer<vtkStripper> strip =
	vtkSmartPointer<vtkStripper>::New();
	strip->SetInputConnection(spe->GetOutputPort());

	}*/

	return true;
}

bool MaximumWallThickness::ThicknessCal()
{
	if (this->m_loopPairVect.size() == 0)
		return false;

	// clean the output vector
	this->m_distanceVect.clear();

	vtkSmartPointer<vtkParametricSpline> spline = vtkSmartPointer<vtkParametricSpline>::New();

	vtkSmartPointer<vtkSplineFilter> splineFilter = vtkSmartPointer<vtkSplineFilter>::New();
	for (int i = 0; i < this->m_loopPairVect.size(); i++)
	{
		std::pair<int, int> pids;
		vtkSmartPointer<vtkPolyData> vesselLoop, lumenLoop;
		vesselLoop = this->m_loopPairVect.at(i).first;
		lumenLoop = this->m_loopPairVect.at(i).second;

		// Up-sample the lines for vessel wall
		spline->SetPoints(vesselLoop->GetPoints());
		splineFilter->SetInputData(vesselLoop);
		splineFilter->SetNumberOfSubdivisions(5);
		splineFilter->Update();
		vesselLoop->DeepCopy(splineFilter->GetOutput());
		cout << "Vessel: " << vesselLoop->GetNumberOfPoints() << endl;

		// for lumen
		spline->SetPoints(lumenLoop->GetPoints());
		splineFilter->SetInputData(lumenLoop);
		splineFilter->SetNumberOfSubdivisions(5);
		splineFilter->Update();
		lumenLoop->DeepCopy(splineFilter->GetOutput());
		cout << "Lumen: " << lumenLoop->GetNumberOfPoints() << endl;

		// Build kd tree
		double d = VTK_DOUBLE_MIN;
		vtkSmartPointer<vtkKdTree> kdtree = vtkSmartPointer<vtkKdTree>::New();
		kdtree->BuildLocatorFromPoints(vesselLoop->GetPoints());
		for (int j = 0; j < lumenLoop->GetNumberOfPoints(); j++)
		{
			double l_d;
			int index = kdtree->FindClosestPoint(lumenLoop->GetPoint(j), l_d);
			if (l_d > d) {
				d = l_d;
				pids.first = index;
				pids.second = j;
			}
		}

		DistanceLoopPair dlp;
		dlp.Distance = sqrt(d);
		dlp.LoopPair = this->m_loopPairVect.at(i);
		dlp.PIDs = pids;

		this->m_distanceVect.push_back(dlp);
	}


	return true;
}
