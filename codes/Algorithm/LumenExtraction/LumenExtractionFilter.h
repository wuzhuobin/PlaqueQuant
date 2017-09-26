#ifndef __LUMEN_EXTRACTION_FILTER_H__
#define __LUMEN_EXTRACTION_FILTER_H__

//#include "itkImage.h"
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
		typedef typename TImage::IndexType IndexType;


		void AddSeed(const typename IndexType& seed);
		void ClearSeeds();

		void SetInitialNeighborhoodRadius(unsigned int radius);
		void SetMultiplier(double multiplier);
		void SetNumberOfIterations(unsigned int num);
		void SetMaximumNumberOfIterations(unsigned int value);

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
		unsigned int MaximumNumberOfIterationsOfHoleFilling = 2;
		

	};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "LumenExtractionFilter.hxx"
#endif

#include <vtkImageAlgorithm.h>

class LumenExtractionFilter: public vtkImageAlgorithm
{
public:
	typedef itk::Image<float, 3> InputImageType;
	typedef itk::Image<unsigned char, 3> OutputImageType;
	//typedef itk::LumenExtractionFilter<OutputImageType> LEFilter;
	typedef itk::LumenExtractionFilter<InputImageType> LEFilter;
	LEFilter::Pointer CoreFilter = nullptr;

	static LumenExtractionFilter* New();
	vtkTypeMacro(LumenExtractionFilter, vtkImageAlgorithm);
	void PrintSelf(ostream& os, vtkIndent indent);

	// Description:
	// Specify i-j-k (min,max) pairs to extract. The resulting structured points
	// dataset can be of any topological dimension (i.e., point, line, image,
	// or volume).


protected:

	virtual int RequestUpdateExtent(vtkInformation* request,
		vtkInformationVector** inputVector,
		vtkInformationVector* outputVector);
	virtual int RequestInformation(vtkInformation* request,
		vtkInformationVector** inputVector,
		vtkInformationVector* outputVector);
	virtual int RequestData(vtkInformation* request,
		vtkInformationVector** inputVector,
		vtkInformationVector* outputVector);

	LumenExtractionFilter();
	~LumenExtractionFilter();


};






#endif // !__LUMEN_EXTRACTION_FILTER_H__
