#ifndef __LUMEN_EXTRACTION_FILTER_H__
#define __LUMEN_EXTRACTION_FILTER_H__

#include "itkImage.h"
#include "itkImageToImageFilter.h"
#include <itkConfidenceConnectedImageFilter.h>


namespace itk
{
	template< typename TImage>
	class LumenExtractionFilter :
		public ImageToImageFilter< TImage, TImage >
	{
	public:
		/** Standard class typedefs. */
		typedef LumenExtractionFilter             Self;
		typedef ImageToImageFilter< TImage, TImage > Superclass;
		typedef SmartPointer< Self >        Pointer;
		typedef SmartPointer< const Self >        ConstPointer;
		typedef ConfidenceConnectedImageFilter< TImage, TImage > ConfidenceConnectedImageFilter;


		/** Method for creation through the object factory. */
		itkNewMacro(Self);

		/** Run-time type information (and related methods). */
		itkTypeMacro(LumenExtractionFilter, ImageToImageFilter);

		void AddSeed(const typename TImage::IndexType& seed);
		void ClearSeeds();

		void SetInitialNeighborhoodRadius(unsigned int radius);
		void SetMultiplier(double multiplier);
		void SetNumberOfIterations(unsigned int num);

		void SetDilationValue(int dilationVal);
	protected:
		LumenExtractionFilter();
		~LumenExtractionFilter() {}

		/** Does the real work. */
		virtual void GenerateData();

	private:
		LumenExtractionFilter(const Self &); //purposely not implemented
		void operator=(const Self &);  //purposely not implemented

		typename ConfidenceConnectedImageFilter::Pointer m_confidenceConnectedFilter;
		

	};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "LumenExtractionFilter.hxx"
#endif


#endif // !__LUMEN_EXTRACTION_FILTER_H__