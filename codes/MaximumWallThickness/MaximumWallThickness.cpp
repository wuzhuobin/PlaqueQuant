#include "MaximumWallThickness.h"


MaximumWallThickness::MaximumWallThickness(vtkImageData * image, int internalEdgeValue, int externalEdgeValue)
	:image(image)
{
	edgeValue[0] = (internalEdgeValue);
	edgeValue[1] = externalEdgeValue;
	this->extent = image->GetExtent();
	
	for (int num = 0; num < EDGENUM; ++num) {
		edgePoints[num] = new std::list<EdgePoint>[extent[5] - extent[4] + 1];
		centers[num] = new EdgePoint[extent[5] - extent[4] + 1];
	}


	
}

MaximumWallThickness::~MaximumWallThickness()
{
	for (int num = 0; num < EDGENUM; ++num) {
		delete[] edgePoints[num];
		delete[] centers[num];
	}
}

bool MaximumWallThickness::valueTransform()
{
	vtkSmartPointer<vtkImageShiftScale> shiftScaleFilter =
		vtkSmartPointer<vtkImageShiftScale>::New();
	shiftScaleFilter->SetInputData(image);
	shiftScaleFilter->SetOutputScalarType(VTK_INT);
	shiftScaleFilter->Update();


	return true;
}

bool MaximumWallThickness::extractVOI()
{
	for (int num = 0; num < EDGENUM; ++num) {
		edgeImage[num] = vtkSmartPointer<vtkImageData>::New();
		edgeImage[num]->DeepCopy(image);
		vtkSmartPointer<vtkExtractVOI> extractFilter =
			vtkSmartPointer<vtkExtractVOI>::New();
		extractFilter->SetInputData(edgeImage[num]);
		extractFilter->SetVOI(extent);
		extractFilter->Update();
		edgeImage[num] = extractFilter->GetOutput();

	}

	return true;
}

bool MaximumWallThickness::thresholdImage()
{
	for (int num = 0; num < EDGENUM; ++num) {
		vtkSmartPointer<vtkImageThreshold> thresholdFilter =
			vtkSmartPointer<vtkImageThreshold>::New();
		thresholdFilter->SetInputData(edgeImage[num]);
		thresholdFilter->ThresholdBetween(edgeValue[num], edgeValue[num]);
		thresholdFilter->SetInValue(image->GetScalarTypeMax());
		thresholdFilter->SetOutValue(image->GetScalarTypeMin());
		thresholdFilter->Update();
		edgeImage[num] = thresholdFilter->GetOutput();
		
	}
	return true;
}

bool MaximumWallThickness::edgeDetection()
{
	// sobel edge detector with Non-Maximum-Suppression
	for (int num = 0; num < EDGENUM; ++num) {

		vtkSmartPointer<vtkImageSobel2D> sobelFilter =
			vtkSmartPointer<vtkImageSobel2D>::New();
		sobelFilter->SetInputData(edgeImage[num]);
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

		edgeImage[num] = nonMaximumSuppressionFilter->GetOutput();

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

bool MaximumWallThickness::thicknessCal()
{
	using namespace std;


	for (int num = 0; num < EDGENUM; num++) {

		list<EdgePoint>* edgePointer = edgePoints[num];
		EdgePoint* centersPointer = centers[num];
		double* spacing = edgeImage[num]->GetSpacing();
		//cout << spacing[0] << '\t' << spacing[1] << '\t' << spacing[2] << endl;
		for (int k = extent[4]; k <= extent[5]; k++) {

			edgePointer->clear();
			
			int centersX = 0;
			int centersY = 0;
			for (int j = extent[2]; j <= extent[3]; j++) {
				for (int i = extent[0]; i <= extent[1]; i++) {
					int* pointer = static_cast<int*>(edgeImage[num]->GetScalarPointer(i, j, k));
					if (*pointer != image->GetScalarTypeMin()) {
						EdgePoint e;
						e.x = i * spacing[0];
						e.y = j * spacing[1];
						centersX += i * spacing[0];
						centersY += j * spacing[1];
						edgePointer->push_back(e);
					}
					
				}
			}
			//cout << "edgePointers number:" << edgePointer->size() << endl;
			EdgePoint e;
			//if(edgePointer->size() != 0){
				e.x = (double)centersX / edgePointer->size();
				e.y = (double)centersY / edgePointer->size();
			//}
			*centersPointer = e;
			++centersPointer;
			++edgePointer;

		}

	}


	list<EdgePoint>* internalEdgePointer = edgePoints[0];
	list<EdgePoint>* externalEdgePointer = edgePoints[1];
	EdgePoint* internalCenterPointer = centers[0];

	maximumWallThickness.clear();

	
	for (int k = extent[4]; k <= extent[5]; k++) {

		list<pair<double, pair<EdgePoint, EdgePoint>>> wallThickness;
		for (list<EdgePoint>::const_iterator internalEdgeIt = internalEdgePointer->cbegin(); 
			internalEdgeIt != internalEdgePointer->cend(); ++internalEdgeIt) {
			double kRL = (internalEdgeIt->y - internalCenterPointer->y) / 
				(internalEdgeIt->x - internalCenterPointer->x);
			double bRL = (internalEdgeIt->x * internalCenterPointer->y - 
				internalEdgeIt->y * internalCenterPointer->x) / (internalEdgeIt->x - internalCenterPointer->x);
			
			list<pair<double, EdgePoint>> distancePL;
			for (list<EdgePoint>::const_iterator externalEdgeIt = externalEdgePointer->cbegin();
				externalEdgeIt != externalEdgePointer->cend(); ++externalEdgeIt) {
				double temp = abs(kRL * externalEdgeIt->x - externalEdgeIt->y + bRL);
				temp = temp / sqrt(pow(kRL, 2) + 1);
				
				distancePL.push_back(pair<double, EdgePoint>(temp, *externalEdgeIt));


			}
			distancePL.sort();

			for (list<pair<double, EdgePoint>>::const_iterator distancePLIt = distancePL.cbegin();
				distancePLIt != distancePL.cend();++distancePLIt) {
				if ((internalEdgeIt->x > internalCenterPointer->x) == (distancePLIt->second.x > internalCenterPointer->x) &&
					(internalEdgeIt->y > internalCenterPointer->y) == (distancePLIt->second.y > internalCenterPointer->y)) {
					double temp;
					temp = pow((internalEdgeIt->x - distancePLIt->second.x), 2);
					temp += pow((internalEdgeIt->y - distancePLIt->second.y), 2);
					temp = sqrt(temp);
					pair<double, pair<EdgePoint, EdgePoint>> p1(temp,
						pair<EdgePoint, EdgePoint>(*internalEdgeIt, distancePLIt->second));
					wallThickness.push_back(p1);
					break;
				}
			}
		}	
		wallThickness.sort();
		maximumWallThickness.push_back(wallThickness.back());
		

		//cout << "slice " << k << " " << wallThickness.back().first << endl;
		//cout << "internal" << wallThickness.back().second.first.x << '\t' << wallThickness.back().second.first.y << endl;
		//cout << "external" << wallThickness.back().second.second.x << '\t' << wallThickness.back().second.second.y << endl;
		wallThickness.clear();

		++internalEdgePointer;
		++externalEdgePointer;
		++internalCenterPointer;
	}



	return true;
}

bool MaximumWallThickness::thicknessCal2()
{



	
	return true;
}

bool MaximumWallThickness::output()
{
	using namespace std;
	int k = extent[4];
	list<EdgePoint>** edgePointer = edgePoints;
	for (list<pair<double, pair<EdgePoint, EdgePoint>>>::const_iterator maximumWallThicknessIt = 
		maximumWallThickness.cbegin(); maximumWallThicknessIt != maximumWallThickness.cend(); ++maximumWallThicknessIt) {
		cout << "slice" << k << endl;
		cout << "internal edge points:" << edgePointer[0][k - extent[4]].size() << endl;
		cout << "external edge points:" << edgePointer[1][k - extent[4]].size() << endl;
		cout << "distance: " << maximumWallThicknessIt->first << endl;
		cout << "internal: " << maximumWallThicknessIt->second.first.x << '\t' <<
			maximumWallThicknessIt->second.first.y << endl;
		cout << "external: " << maximumWallThicknessIt->second.second.x << '\t' <<
			maximumWallThicknessIt->second.second.y << endl;
		++k;
	}

	return true;
}

bool MaximumWallThickness::setExtent(int* extent)
{

	this->extent = extent;
	return true;
}

const int * MaximumWallThickness::getExtent()
{
	return extent;
}

