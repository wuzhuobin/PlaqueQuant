#ifndef __LUMEN_EXTRACTION_FILTER_H__
#define __LUMEN_EXTRACTION_FILTER_H__

#include "itkImageToImageFilter.h"

namespace itk
{
	template< class TImage>
	class LumenExtractionFilter :public ImageToImageFilter< TImage, TImage >
	{
	public:
		/** Standard class typedefs. */
		typedef LumenExtractionFilter             Self;
		typedef ImageToImageFilter< TImage, TImage > Superclass;
		typedef SmartPointer< Self >        Pointer;

		/** Method for creation through the object factory. */
		itkNewMacro(Self);

		/** Run-time type information (and related methods). */
		itkTypeMacro(LumenExtractionFilter, ImageToImageFilter);

	protected:
		LumenExtractionFilter() {}
		~LumenExtractionFilter() {}

		/** Does the real work. */
		virtual void GenerateData();

	private:
		LumenExtractionFilter(const Self &); //purposely not implemented
		void operator=(const Self &);  //purposely not implemented

	};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "LumenExtractionFilter.hxx"
#endif


#endif // !__LUMEN_EXTRACTION_FILTER_H__