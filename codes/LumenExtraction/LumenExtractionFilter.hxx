#ifndef __LUMEN_EXTRACTION_FILTER_HXX__
#define __LUMEN_EXTRACTION_FILTER_HXX__

#include "LumenExtractionFilter.h"
#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

namespace itk
{

	template< class TImage>
	void LumenExtractionFilter< TImage>
		::GenerateData()
	{
		typename TImage::ConstPointer input = this->GetInput();
		typename TImage::Pointer output = this->GetOutput();

		itk::Index<2> cornerPixel = input->GetLargestPossibleRegion().GetIndex();
		typename TImage::PixelType newValue = 3;

		this->AllocateOutputs();

		ImageAlgorithm::Copy(input.GetPointer(), output.GetPointer(), output->GetRequestedRegion(),
			output->GetRequestedRegion());

		output->SetPixel(cornerPixel, newValue);
	}

}// end namespace


#endif //!__LUMEN_EXTRACTION_FILTER_HXX__