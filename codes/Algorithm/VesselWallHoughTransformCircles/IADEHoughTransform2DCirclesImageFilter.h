/*=========================================================================
 *
 *  Copyright Insight Software ConsortiumHoughTransform2DCirclesImageFilter
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
#ifndef __IADE_HOUGH_TRANSFORM_2D_CIRCLIES_IMAGE_FILTER_H__
#define __IADE_HOUGH_TRANSFORM_2D_CIRCLIES_IMAGE_FILTER_H__


#include "itkImageToImageFilter.h"
#include "itkEllipseSpatialObject.h"

namespace itk
{
/**
 * \class IADEHoughTransform2DCirclesImageFilter
 * \brief Performs the Hough Transform to find circles in a 2D image.
 *
 * This filter derives from the base class ImageToImageFilter.
 * The input is an image, and all pixels above some threshold are those
 * we want to consider during the process.
 *
 * This filter produces two output:
 *   1) The accumulator array, which represents probability of centers.
 *   2) The array or radii, which has the radius value at each coordinate point.
 *
 *  When the filter finds a "correct" point, it computes the gradient at this
 * point and draws a regular narrow-banded circle using the minimum and maximum
 * radius given by the user, and fills in the array of radii.
 * The SweepAngle value can be adjusted to improve the segmentation.
 *
 * \ingroup ImageFeatureExtraction
 *
 * \ingroup ITKImageFeature
 *
 * \wiki
 * \wikiexample{Conversions/IADEHoughTransform2DCirclesImageFilter,IADEHoughTransform2DCirclesImageFilter}
 * \endwiki
 */

template< typename TInputPixelType, typename TOutputPixelType, typename TMaskPixelType >
class IADEHoughTransform2DCirclesImageFilter:
  public ImageToImageFilter< Image< TInputPixelType, 2 >, Image< TOutputPixelType, 2 > >
{
public:

  /** Standard "Self" typedef. */
  typedef IADEHoughTransform2DCirclesImageFilter Self;

  /** Input Image typedef */
  typedef Image< TInputPixelType, 2 >           InputImageType;
  typedef typename InputImageType::Pointer      InputImagePointer;
  typedef typename InputImageType::ConstPointer InputImageConstPointer;

  /** Output Image typedef */
  typedef Image< TOutputPixelType, 2 >      OutputImageType;
  typedef typename OutputImageType::Pointer OutputImagePointer;

  /** Standard "Superclass" typedef. */
  typedef ImageToImageFilter<
    Image< TInputPixelType, 2 >,
    Image< TOutputPixelType, 2 > >  Superclass;

  /** Smart pointer typedef support. */
  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;

  /** Image index typedef */
  typedef typename InputImageType::IndexType IndexType;

  /** Image pixel value typedef */
  typedef typename InputImageType::PixelType PixelType;

  /** Typedef to describe the output image region type. */
  typedef typename InputImageType::RegionType OutputImageRegionType;

  /** Circle typedef */
  typedef EllipseSpatialObject< 2 >    CircleType;
  typedef typename CircleType::Pointer CirclePointer;
  typedef std::list< CirclePointer >   CirclesListType;

  typedef typename CirclesListType::size_type CirclesListSizeType;

  /** mask*/
  typedef Image<TMaskPixelType, 2> MaskImageType;
  typedef typename MaskImageType::Pointer MaskImagePointer;

  /** Run-time type information (and related methods). */
  itkTypeMacro(IADEHoughTransform2DCirclesImageFilter, ImageToImageFilter);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Method for evaluating the implicit function over the image. */
  void GenerateData() ITK_OVERRIDE;

  /** Set both Minimum and Maximum radius values */
  void SetRadius(double radius);

  /** Set the minimum radiu value the filter should look for */
  itkSetMacro(MinimumRadius, double);
  itkGetConstMacro(MinimumRadius, double);

  /** Set the maximum radius value the filter should look for */
  itkSetMacro(MaximumRadius, double);
  itkGetConstMacro(MaximumRadius, double);

  /** Set the threshold above which the filter should consider
      the point as a valid point */
  itkSetMacro(Threshold, double);

  /** Get the threshold value */
  itkGetConstMacro(Threshold, double);

  /** Get the radius image */
  itkGetModifiableObjectMacro(RadiusImage, OutputImageType);

  /** Set the scale of the derivative function (using DoG) */
  itkSetMacro(SigmaGradient, double);

  /** Get the scale value */
  itkGetConstMacro(SigmaGradient, double);

  /** Get the list of circles. This recomputes the circles */
  CirclesListType & GetCircles(unsigned int n = 0);

  /** Set/Get the number of circles to extract */
  itkSetMacro(NumberOfCircles, unsigned int);
  itkGetConstMacro(NumberOfCircles, unsigned int);

  /** Set/Get the radius of the disc to remove from the accumulator
   *  for each circle found */
  itkSetMacro(DiscRadiusRatio, float);
  itkGetConstMacro(DiscRadiusRatio, float);

  /** Set the variance of the gaussian bluring for the accumulator */
  itkSetMacro(Variance, float);
  itkGetConstMacro(Variance, float);

  /** Set the sweep angle */
  itkSetMacro(SweepAngle, float);
  itkGetConstMacro(SweepAngle, float);

  /** Set the MaskImagePointer angle */
  itkSetMacro(MaskImage, MaskImagePointer);
  itkGetConstMacro(MaskImage, MaskImagePointer);

#ifdef ITK_USE_CONCEPT_CHECKING
  // Begin concept checking
  itkConceptMacro( IntConvertibleToOutputCheck,
                   ( Concept::Convertible< int, TOutputPixelType > ) );
  itkConceptMacro( InputGreaterThanDoubleCheck,
                   ( Concept::GreaterThanComparable< PixelType, double > ) );
  itkConceptMacro( OutputPlusIntCheck,
                   ( Concept::AdditiveOperators< TOutputPixelType, int > ) );
  itkConceptMacro( OutputDividedByIntCheck,
                   ( Concept::DivisionOperators< TOutputPixelType, int > ) );
  // End concept checking
#endif

protected:

  IADEHoughTransform2DCirclesImageFilter();
  virtual ~IADEHoughTransform2DCirclesImageFilter() {}

  void PrintSelf(std::ostream & os, Indent indent) const ITK_OVERRIDE;

  /** IADEHoughTransform2DCirclesImageFilter needs the entire input. Therefore
   * it must provide an implementation GenerateInputRequestedRegion().
   * \sa ProcessObject::GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion() ITK_OVERRIDE;

  /** IADEHoughTransform2DCirclesImageFilter's produces all the output.
   * Therefore, it must provide an implementation of
   * EnlargeOutputRequestedRegion.
   * \sa ProcessObject::EnlargeOutputRequestedRegion() */
  void EnlargeOutputRequestedRegion( DataObject *itkNotUsed(output) ) ITK_OVERRIDE;

private:

  IADEHoughTransform2DCirclesImageFilter(const Self &);
  void operator=(const Self &);

  float  m_SweepAngle;
  double m_MinimumRadius;
  double m_MaximumRadius;
  double m_Threshold;
  double m_SigmaGradient;

  OutputImagePointer    m_RadiusImage;
  CirclesListType       m_CirclesList;
  CirclesListSizeType   m_NumberOfCircles;
  float                 m_DiscRadiusRatio;
  float                 m_Variance;
  ModifiedTimeType      m_OldModifiedTime;
  MaskImagePointer m_MaskImage;

  CirclesListSizeType m_OldNumberOfCircles;
};
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "IADEHoughTransform2DCirclesImageFilter.hxx"
#endif

#endif
