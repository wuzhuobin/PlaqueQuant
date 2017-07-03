/*=========================================================================

Program:   Visualization Toolkit
Module:    ImageSliceBySliceAccumulate.cxx

Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
All rights reserved.
See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "ImageSliceBySliceAccumulate.h"

#include "vtkImageData.h"
#include "vtkImageStencilData.h"
#include "vtkImageStencilIterator.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkMath.h"
#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"

#include <math.h>


template<typename DType>
class MyImageStencilIterator : public vtkImageStencilIterator<DType>
{
public:
	MyImageStencilIterator() {};
	MyImageStencilIterator(
		vtkImageData *image, vtkImageStencilData *stencil, int extent[6],
		vtkAlgorithm *algorithm = 0, int threadId = 0)
		:vtkImageStencilIterator(image, stencil, extent, algorithm, threadId){}
	//int GetSpanIndexX() { return this->SpanIndexX; }
	//int GetSpanIndexY() { return this->SpanIndexY; }
	//int GetSpanIndexZ() { return this->SpanIndexZ; }
	 int GetSpanIndexX() { return this->GetIndex()[0]; }
	 int GetSpanIndexY() { return this->GetIndex()[1]; }
	 int GetSpanIndexZ() { return this->GetIndex()[2]; }
};

template class MyImageStencilIterator<signed char>;
template class MyImageStencilIterator<char>;
template class MyImageStencilIterator<int>;
template class MyImageStencilIterator<long>;
template class MyImageStencilIterator<short>;
template class MyImageStencilIterator<float>;
template class MyImageStencilIterator<double>;
template class MyImageStencilIterator<unsigned long>;
template class MyImageStencilIterator<unsigned short>;
template class MyImageStencilIterator<unsigned char>;
template class MyImageStencilIterator<unsigned int>;


vtkStandardNewMacro(ImageSliceBySliceAccumulate);

//----------------------------------------------------------------------------
// Constructor sets default values
ImageSliceBySliceAccumulate::ImageSliceBySliceAccumulate()
{
	for (int idx = 0; idx < 3; ++idx)
	{
		this->Spacing[idx] = 1.0;
		this->Origin[idx] = 0.0;
		this->Extent[idx * 2] = 0;
		this->Extent[idx * 2 + 1] = 0;
	}
	this->Extent[1] = 255;

	this->ReverseStencil = 0;

	//this->Min[0] = this->Min[1] = this->Min[2] = 0.0;
	//this->Max[0] = this->Max[1] = this->Max[2] = 0.0;
	//this->Mean[0] = this->Mean[1] = this->Mean[2] = 0.0;
	//this->StandardDeviation[0] = this->StandardDeviation[1] =
	//	this->StandardDeviation[2] = 0.0;
	//this->VoxelCount = 0;
	this->IgnoreZero = 0;

	this->SliceOrientation = SLICE_ORIENTATION_XY;

	// we have the image input and the optional stencil input
	this->SetNumberOfInputPorts(2);
}


//----------------------------------------------------------------------------
ImageSliceBySliceAccumulate::~ImageSliceBySliceAccumulate()
{
}

//----------------------------------------------------------------------------
void ImageSliceBySliceAccumulate::SetExtent(int extent[6])
{
	int idx, modified = 0;

	for (idx = 0; idx < 6; ++idx)
	{
		if (this->Extent[idx] != extent[idx])
		{
			this->Extent[idx] = extent[idx];
			modified = 1;
		}
	}
	if (modified)
	{
		this->Modified();
	}
}


//----------------------------------------------------------------------------
void ImageSliceBySliceAccumulate::SetExtent(
	int minX, int maxX,
	int minY, int maxY,
	int minZ, int maxZ)
{
	int extent[6];

	extent[0] = minX;  extent[1] = maxX;
	extent[2] = minY;  extent[3] = maxY;
	extent[4] = minZ;  extent[5] = maxZ;
	this->SetExtent(extent);
}


//----------------------------------------------------------------------------
void ImageSliceBySliceAccumulate::GetExtent(int extent[6])
{
	for (int idx = 0; idx < 6; ++idx)
	{
		extent[idx] = this->Extent[idx];
	}
}


//----------------------------------------------------------------------------
void ImageSliceBySliceAccumulate::SetStencilData(vtkImageStencilData *stencil)
{
	this->SetInputData(1, stencil);
}


//----------------------------------------------------------------------------
vtkImageStencilData *ImageSliceBySliceAccumulate::GetStencil()
{
	if (this->GetNumberOfInputConnections(1) < 1)
	{
		return 0;
	}
	return vtkImageStencilData::SafeDownCast(
		this->GetExecutive()->GetInputData(1, 0));
}

//----------------------------------------------------------------------------
// This templated function executes the filter for any type of data.
template <class T>
void ImageSliceBySliceAccumulateExecute(ImageSliceBySliceAccumulate *self,
	vtkImageData *inData, T *,
	vtkImageData *outData, vtkIdType *outPtr,
	int orientation,
	//double min[3], double max[3],
	//double mean[3],
	//double standardDeviation[3],
	//vtkIdType *voxelCount,
	int* updateExtent)
{
	// variables used to compute statistics (filter handles max 1 components)
	//double sum[3];
	//sum[0] = sum[1] = sum[2] = 0.0;
	//double sumSqr[3];
	//sumSqr[0] = sumSqr[1] = sumSqr[2] = 0.0;
	//min[0] = min[1] = min[2] = VTK_DOUBLE_MAX;
	//max[0] = max[1] = max[2] = VTK_DOUBLE_MIN;
	//standardDeviation[0] = standardDeviation[1] = standardDeviation[2] = 0.0;
	//*voxelCount = 0;

	// input's number of components is used as output dimensionality
	int numC = inData->GetNumberOfScalarComponents();

	// get information for output data
	int outExtent[6];
	outData->GetExtent(outExtent);
	vtkIdType outIncs[3];
	outData->GetIncrements(outIncs);
	double origin[3];
	outData->GetOrigin(origin);
	double spacing[3];
	outData->GetSpacing(spacing);


	// zero count in every bin
	vtkIdType size = 1;
	size *= (outExtent[1] - outExtent[0] + 1);
	size *= (outExtent[3] - outExtent[2] + 1);
	size *= (outExtent[5] - outExtent[4] + 1);
	for (vtkIdType j = 0; j < size; j++)
	{
		outPtr[j] = 0;
	}

	vtkImageStencilData *stencil = self->GetStencil();
	bool reverseStencil = (self->GetReverseStencil() != 0);
	bool ignoreZero = (self->GetIgnoreZero() != 0);

	MyImageStencilIterator<T> inIter(inData, stencil, updateExtent, self);

	while (!inIter.IsAtEnd())
	{
		//cout << "span" << endl;
		// because it is only capable to get index of a span
		// a span is contigous region over which nothing but X index changes.
		// so X index should be increased with the inPtr
		int currentIndex[3] = {
			inIter.GetSpanIndexX(),
			inIter.GetSpanIndexY(),
			inIter.GetSpanIndexZ() };

		if (inIter.IsInStencil() ^ reverseStencil)
		{
			T *inPtr = inIter.BeginSpan();
			T *spanEndPtr = inIter.EndSpan();

			
			while (inPtr != spanEndPtr)
			{
				// find the bin for this pixel.
				bool outOfBounds = false;
				vtkIdType *outPtrC = outPtr;
				for (int idxC = 0; idxC < numC; ++idxC)
				{

					double v = static_cast<double>(*inPtr++);
					//cout << "X: " << currentIndex[0] << endl;
					//cout << "Y: " << currentIndex[1] << endl;
					//cout << "Z: " << currentIndex[2] << endl;
					if (!ignoreZero || v != 0)
					{
						// gather statistics
						//sum[idxC] += v;
						//sumSqr[idxC] += v*v;
						//if (v > max[idxC])
						//{
						//	max[idxC] = v;
						//}
						//if (v < min[idxC])
						//{
						//	min[idxC] = v;
						//}
						//(*voxelCount)++;
					}

					// compute the index
					int outIdx = vtkMath::Floor((v - origin[0]) / spacing[0]);

					// verify that it is in range
					if (outIdx >= outExtent[0] && outIdx <= outExtent[1])
					{
						outPtrC += (outIdx - outExtent[0]) + currentIndex[orientation] * outIncs[1];
					}
					else
					{
						outOfBounds = true;
					}
				}

				// increment the bin
				if (!outOfBounds)
				{
					++(*outPtrC);
				}
				++currentIndex[0];

			}
		}

		inIter.NextSpan();
	}

	//// initialize the statistics
	//mean[0] = 0;
	//mean[1] = 0;
	//mean[2] = 0;

	//standardDeviation[0] = 0;
	//standardDeviation[1] = 0;
	//standardDeviation[2] = 0;

	//if (*voxelCount != 0) // avoid the div0
	//{
	//	double n = static_cast<double>(*voxelCount);
	//	mean[0] = sum[0] / n;
	//	mean[1] = sum[1] / n;
	//	mean[2] = sum[2] / n;

	//	if (*voxelCount - 1 != 0) // avoid the div0
	//	{
	//		double m = static_cast<double>(*voxelCount - 1);
	//		standardDeviation[0] = sqrt((sumSqr[0] - mean[0] * mean[0] * n) / m);
	//		standardDeviation[1] = sqrt((sumSqr[1] - mean[1] * mean[1] * n) / m);
	//		standardDeviation[2] = sqrt((sumSqr[2] - mean[2] * mean[2] * n) / m);
	//	}
	//}

}


//----------------------------------------------------------------------------
// This method is passed a input and output Data, and executes the filter
// algorithm to fill the output from the input.
// It just executes a switch statement to call the correct function for
// the Datas data types.
int ImageSliceBySliceAccumulate::RequestData(
	vtkInformation* vtkNotUsed(request),
	vtkInformationVector** inputVector,
	vtkInformationVector* outputVector)
{
	void *inPtr;
	void *outPtr;

	// get the input
	vtkInformation* in1Info = inputVector[0]->GetInformationObject(0);
	vtkImageData *inData = vtkImageData::SafeDownCast(
		in1Info->Get(vtkDataObject::DATA_OBJECT()));
	int *uExt = in1Info->Get(vtkStreamingDemandDrivenPipeline::UPDATE_EXTENT());

	// get the output
	vtkInformation *outInfo = outputVector->GetInformationObject(0);
	vtkImageData *outData = vtkImageData::SafeDownCast(
		outInfo->Get(vtkDataObject::DATA_OBJECT()));

	vtkDebugMacro(<< "Executing image accumulate");

	// We need to allocate our own scalars since we are overriding
	// the superclasses "Execute()" method.
	outData->SetExtent(outInfo->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT()));
	outData->AllocateScalars(outInfo);

	vtkDataArray *inArray = this->GetInputArrayToProcess(0, inputVector);
	inPtr = inData->GetArrayPointerForExtent(inArray, uExt);
	outPtr = outData->GetScalarPointer();

	// Components turned into x, y and z
	if (inData->GetNumberOfScalarComponents() > 1)
	{
		vtkErrorMacro("This filter can handle up to 1 components");
		return 1;
	}

	// this filter expects that output is type int.
	if (outData->GetScalarType() != VTK_ID_TYPE)
	{
		vtkErrorMacro(<< "Execute: out ScalarType " << outData->GetScalarType()
			<< " must be vtkIdType\n");
		return 1;
	}

	switch (inData->GetScalarType())
	{
		vtkTemplateMacro(ImageSliceBySliceAccumulateExecute(this,
			inData,
			static_cast<VTK_TT *>(inPtr),
			outData,
			static_cast<vtkIdType *>(outPtr),
			this->SliceOrientation,
			//this->Min, this->Max,
			//this->Mean,
			//this->StandardDeviation,
			//&this->VoxelCount,
			uExt));
	default:
		vtkErrorMacro(<< "Execute: Unknown ScalarType");
		return 1;
	}

	return 1;
}


//----------------------------------------------------------------------------
int ImageSliceBySliceAccumulate::RequestInformation(
	vtkInformation* vtkNotUsed(request),
	vtkInformationVector** inputVector,
	vtkInformationVector* outputVector)
{

	vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
		
	const int* extent = inInfo->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT());
	const double* spacing = inInfo->Get(vtkDataObject::SPACING());
	const double* origin = inInfo->Get(vtkDataObject::ORIGIN());

	this->Extent[2] = extent[this->SliceOrientation * 2];
	this->Extent[3] = extent[this->SliceOrientation * 2 + 1];
	this->Extent[4] = 0;
	this->Extent[5] = 0;
	this->Spacing[1] = spacing[this->SliceOrientation];
	this->Spacing[2] = 0;
	this->Origin[1] = origin[this->SliceOrientation];
	this->Origin[2] = 0;

	// get the info objects
	vtkInformation* outInfo = outputVector->GetInformationObject(0);

	outInfo->Set(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(),
		this->Extent, 6);
	outInfo->Set(vtkDataObject::ORIGIN(), this->Origin, 3);
	outInfo->Set(vtkDataObject::SPACING(), this->Spacing, 3);

	vtkDataObject::SetPointDataActiveScalarInfo(outInfo, VTK_ID_TYPE, 1);
	return 1;
}

//----------------------------------------------------------------------------
// Get ALL of the input.
int ImageSliceBySliceAccumulate::RequestUpdateExtent(
	vtkInformation* vtkNotUsed(request),
	vtkInformationVector** inputVector,
	vtkInformationVector* vtkNotUsed(outputVector))
{
	// get the info objects
	vtkInformation* inInfo = inputVector[0]->GetInformationObject(0);
	vtkInformation* stencilInfo = 0;
	if (inputVector[1]->GetNumberOfInformationObjects() > 0)
	{
		stencilInfo = inputVector[1]->GetInformationObject(0);
	}

	// Use the whole extent of the first input as the update extent for
	// both inputs.  This way the stencil will be the same size as the
	// input.
	int extent[6] = { 0,-1,0,-1,0,-1 };
	inInfo->Get(vtkStreamingDemandDrivenPipeline::WHOLE_EXTENT(), extent);
	inInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_EXTENT(), extent, 6);
	if (stencilInfo)
	{
		stencilInfo->Set(vtkStreamingDemandDrivenPipeline::UPDATE_EXTENT(),
			extent, 6);
	}
	return 1;
}

//----------------------------------------------------------------------------
int ImageSliceBySliceAccumulate::FillInputPortInformation(
	int port, vtkInformation* info)
{
	if (port == 1)
	{
		info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkImageStencilData");
		// the stencil input is optional
		info->Set(vtkAlgorithm::INPUT_IS_OPTIONAL(), 1);
	}
	else
	{
		info->Set(vtkAlgorithm::INPUT_REQUIRED_DATA_TYPE(), "vtkImageData");
	}
	return 1;
}

//----------------------------------------------------------------------------
void ImageSliceBySliceAccumulate::PrintSelf(ostream& os, vtkIndent indent)
{
	this->Superclass::PrintSelf(os, indent);

	//os << indent << "Mean: ("
	//	<< this->Mean[0] << ", "
	//	<< this->Mean[1] << ", "
	//	<< this->Mean[2] << ")\n";
	//os << indent << "Min: ("
	//	<< this->Min[0] << ", "
	//	<< this->Min[1] << ", "
	//	<< this->Min[2] << ")\n";
	//os << indent << "Max: ("
	//	<< this->Max[0] << ", "
	//	<< this->Max[1] << ", "
	//	<< this->Max[2] << ")\n";
	//os << indent << "StandardDeviation: ("
	//	<< this->StandardDeviation[0] << ", "
	//	<< this->StandardDeviation[1] << ", "
	//	<< this->StandardDeviation[2] << ")\n";
	//os << indent << "VoxelCount: " << this->VoxelCount << "\n";
	os << indent << "Stencil: " << this->GetStencil() << "\n";
	os << indent << "ReverseStencil: " << (this->ReverseStencil ?
		"On\n" : "Off\n");
	os << indent << "IgnoreZero: " << (this->IgnoreZero ? "On" : "Off") << "\n";

	os << indent << "Origin: ( "
		<< this->Origin[0] << ", "
		<< this->Origin[1] << ", "
		<< this->Origin[2] << " )\n";

	os << indent << "Spacing: ( "
		<< this->Spacing[0] << ", "
		<< this->Spacing[1] << ", "
		<< this->Spacing[2] << " )\n";

	os << indent << "Extent: ( "
		<< this->Extent[0] << "," << this->Extent[1] << " "
		<< this->Extent[2] << "," << this->Extent[3] << " "
		<< this->Extent[4] << "," << this->Extent[5] << " }\n";

	os << indent << "SliceOrientation: " << this->SliceOrientation << "\n";
}

