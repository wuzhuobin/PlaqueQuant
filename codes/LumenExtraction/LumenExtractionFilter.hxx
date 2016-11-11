#ifndef __LUMEN_EXTRACTION_FILTER_HXX__
#define __LUMEN_EXTRACTION_FILTER_HXX__

#include "LumenExtractionFilter.h"
#include "itkObjectFactory.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"

#include <itkVotingBinaryIterativeHoleFillingImageFilter.h>


namespace itk
{
	template<typename TImage>
	inline void LumenExtractionFilter<TImage>::ClearSeeds()
	{
		m_confidenceConnectedFilter->ClearSeeds();
	}

	template<typename TImage>
	inline void LumenExtractionFilter<TImage>::SetInitialNeighborhoodRadius(unsigned int radius)
	{
		m_confidenceConnectedFilter->SetInitialNeighborhoodRadius(radius);
	}
	template<typename TImage>
	inline void LumenExtractionFilter<TImage>::SetMultiplier(double multiplier)
	{
		m_confidenceConnectedFilter->SetMultiplier(multiplier);
	}
	/**
	 * Constructor
	 */
	template<typename TImage>
	inline LumenExtractionFilter<TImage>::LumenExtractionFilter()
	{
		m_confidenceConnectedFilter
			= ConfidenceConnectedImageFilter::New();
	}
	template< typename TImage >
	void LumenExtractionFilter< TImage >
		::GenerateData()
	{
		typedef VotingBinaryIterativeHoleFillingImageFilter<TImage> FillHoleFilterType;


		typename TImage::ConstPointer input = this->GetInput();
		typename TImage::Pointer output = this->GetOutput();
		m_confidenceConnectedFilter->SetInput(input);
		m_confidenceConnectedFilter->Update();

		FillHoleFilterType::Pointer fillHoleFilter = FillHoleFilterType::New();
		fillHoleFilter->SetInput(m_confidenceConnectedFilter->GetOutput());
		fillHoleFilter->SetForegroundValue(1);
		fillHoleFilter->Update();

		output->Graft(fillHoleFilter->GetOutput());
	}

}// end namespace


#endif //!__LUMEN_EXTRACTION_FILTER_HXX__