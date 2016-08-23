#include <VesselSegmentation.h>

//#include "itkImageFileWriter.h"

VesselSegmentation::VesselSegmentation(void)
{
	m_wallSurface = vtkSmartPointer<vtkPolyData>::New();
	m_lumenSurface = vtkSmartPointer<vtkPolyData>::New();
	m_plaqueSurface = vtkSmartPointer<vtkPolyData>::New();
}

VesselSegmentation::~VesselSegmentation(void)
{

}

void VesselSegmentation::SetT1(FloatImageType::Pointer T1)
{	
	m_T1 = T1;
	m_hasT1 = 1;
}

void VesselSegmentation::SetMPRAGE(FloatImageType::Pointer MPRAGE)
{
	m_MPRAGE = MPRAGE;
	m_hasMPRAGE = 1;
}

void VesselSegmentation::SetInputSegmentationImage(FloatImageType::Pointer inputSegmentation)
{
	m_inputSegmentation = inputSegmentation;

	// convert the input segmentation to labelMap type
	LabelImageToLabelMapFilterType::Pointer labelImageToLabelMapFilter = LabelImageToLabelMapFilterType::New();
	labelImageToLabelMapFilter->SetInput(m_inputSegmentation);
	labelImageToLabelMapFilter->Update();
	m_labelMap		= labelImageToLabelMapFilter->GetOutput();
	cout << "Label map conversion success"<<endl;
	
	m_hasInputSegmentation = 1;
}

void VesselSegmentation::Update()
{
	if (!m_hasInputSegmentation)
	{
		cerr << "Segmentation image is not setted." << endl;
	}
	else
	{
		// initialize vessel wall as the input segmentation image
		m_wallImage = m_inputSegmentation;

		// generate vtk vessel wall surface;
		ImageToVTKImageType::Pointer wallConverter = ImageToVTKImageType::New();
		wallConverter->SetInput(m_inputSegmentation);
		wallConverter->Update();
		SurfaceCreator* wallCreator = new SurfaceCreator;
		wallCreator->SetInput(wallConverter->GetOutput());
		wallCreator->SetValue(1);
		wallCreator->SetResamplingFactor(1);
		wallCreator->Update();
		m_wallSurface->DeepCopy(wallCreator->GetOutput());

		if (m_hasT1)
		{
			// invert T1 image
			InvertIntensityImageFilterType::Pointer invertImageFilter = InvertIntensityImageFilterType::New();
			invertImageFilter->SetInput(m_T1);
			invertImageFilter->SetMaximum(256);
			invertImageFilter->Update();
			cout << "T1 image inversion success" << endl;

			// mask the vessel image
			MaskImageFilterType::Pointer T1ImageMaskFilter = MaskImageFilterType::New();			;
			try {
				T1ImageMaskFilter->SetInput(invertImageFilter->GetOutput());
				T1ImageMaskFilter->SetMaskImage(m_inputSegmentation);
				T1ImageMaskFilter->Update();
			}catch(itk::ExceptionObject &e){
				std::cerr << e.what() << endl;
			}
			m_maskedT1_invert = T1ImageMaskFilter->GetOutput();

			// Otsu thresholding
			OtsuThresholdImageFilterType::Pointer T1OtsuFilter = OtsuThresholdImageFilterType::New();
			T1OtsuFilter->SetInput(m_maskedT1_invert) ;
			T1OtsuFilter->SetOutsideValue(1);
			T1OtsuFilter->SetInsideValue(0);
			T1OtsuFilter->Update();
			cout << "T1 Otsu thresholding success" << endl;

			// extract the largest component
			ConnectedComponentImageFilterType::Pointer connectedFilter = ConnectedComponentImageFilterType::New();
			connectedFilter->SetInput(T1OtsuFilter->GetOutput());
			connectedFilter->Update();
			cout << "T1 connected component definition success" << endl;

			LabelShapeKeepNObjectsImageFilterType::Pointer T1LabelShapeKeepNObjectFilterbjectFilter = LabelShapeKeepNObjectsImageFilterType::New();
			T1LabelShapeKeepNObjectFilterbjectFilter->SetInput(connectedFilter->GetOutput());
			T1LabelShapeKeepNObjectFilterbjectFilter->SetBackgroundValue(0);
			T1LabelShapeKeepNObjectFilterbjectFilter->SetNumberOfObjects(1);
			T1LabelShapeKeepNObjectFilterbjectFilter->SetAttribute(LabelShapeKeepNObjectsImageFilterType::LabelObjectType::NUMBER_OF_PIXELS);
			T1LabelShapeKeepNObjectFilterbjectFilter->Update();

			// cast the lumen image to float image type
			CastImageFilter<UnsignedShortImageType, FloatImageType>::Pointer lumenCaster = CastImageFilter<UnsignedShortImageType, FloatImageType>::New();
			lumenCaster->SetInput(T1LabelShapeKeepNObjectFilterbjectFilter->GetOutput());
			lumenCaster->Update();
			m_lumenImage = lumenCaster->GetOutput();
			cout << "Lumen segmentation success." << endl;
				
			// subtract wall image by lumen image to get wall image
			BinarThresholdImageFilterType::Pointer wallBinaryFilter = BinarThresholdImageFilterType::New();
			wallBinaryFilter->SetInsideValue(1);
			wallBinaryFilter->SetOutsideValue(0);
			wallBinaryFilter->SetLowerThreshold(0.5);
			wallBinaryFilter->SetUpperThreshold(999999999999);
			wallBinaryFilter->SetInput(m_wallImage);
			wallBinaryFilter->Update();
			m_wallImage->Graft(wallBinaryFilter->GetOutput());

			BinarThresholdImageFilterType::Pointer lumenBinaryFilter = BinarThresholdImageFilterType::New();
			lumenBinaryFilter->SetInsideValue(1);
			lumenBinaryFilter->SetOutsideValue(0);
			lumenBinaryFilter->SetLowerThreshold(0.5);
			lumenBinaryFilter->SetUpperThreshold(999999999999);
			lumenBinaryFilter->SetInput(m_lumenImage);
			lumenBinaryFilter->Update();
			m_lumenImage->Graft(lumenBinaryFilter->GetOutput());

			SubtractImageFilterType::Pointer lumenSubtractFilter = SubtractImageFilterType::New();
			lumenSubtractFilter->SetInput1(m_wallImage);
			lumenSubtractFilter->SetInput2(m_lumenImage);
			lumenSubtractFilter->Update();
			m_wallImage = lumenSubtractFilter->GetOutput();

			//itk::ImageFileWriter<FloatImageType>::Pointer writer = itk::ImageFileWriter<FloatImageType>::New();
			//writer->SetFileName("C:\\Users\\user\\Desktop\\wall.nii");
			//writer->SetInput(m_wallImage);
			//writer->Write();

			//writer->SetFileName("C:\\Users\\user\\Desktop\\lumen.nii");
			//writer->SetInput(m_lumenImage);
			//writer->Write();

			// convert lumen image to label map
			try {
				LabelImageToLabelMapFilterType::Pointer LumenImageToLabelMapFilter = LabelImageToLabelMapFilterType::New();
				LumenImageToLabelMapFilter->SetInput(m_lumenImage);
				LumenImageToLabelMapFilter->Update();
				m_lumenLabelMap = LumenImageToLabelMapFilter->GetOutput();
				m_lumenLabelMap->GetLabelObject(1)->SetLabel(2);
			}catch(itk::ExceptionObject &e){
				std::cerr << m_lumenLabelMap << endl;
				std::cerr << e.what() << endl;
			}
			
			// generate vtk vessel lumen surface;
			ImageToVTKImageType::Pointer lumenConverter = ImageToVTKImageType::New();
			lumenConverter->SetInput(m_lumenImage);
			lumenConverter->Update();
			SurfaceCreator* lumenCreator = new SurfaceCreator;
			lumenCreator->SetInput(lumenConverter->GetOutput());
			lumenCreator->SetValue(1);
			lumenCreator->SetResamplingFactor(1);
			lumenCreator->Update();
			m_lumenSurface->DeepCopy(lumenCreator->GetOutput());
		}
		else
		{
			cerr << "T1 image is not setted" << endl;
		}


		MaskImageFilterType::Pointer MPRAGEImageMaskFilter = MaskImageFilterType::New();
		if (m_hasMPRAGE)
		{
			// mask the vessel image
			MaskImageFilterType::Pointer MPRAGEImageMaskFilter = MaskImageFilterType::New();;
			MPRAGEImageMaskFilter->SetInput(m_MPRAGE);
			MPRAGEImageMaskFilter->SetMaskImage(m_inputSegmentation);
			MPRAGEImageMaskFilter->Update();
			m_maskedMPRAGE = MPRAGEImageMaskFilter->GetOutput();

			cout << "MPRAGE image masking success" << endl;

			// Otsu thresholding
			OtsuMultipleThresholdsImageFilterType::Pointer MPRAGEOtsuFilter = OtsuMultipleThresholdsImageFilterType::New();
			MPRAGEOtsuFilter->SetInput(m_maskedMPRAGE);
			MPRAGEOtsuFilter->SetLabelOffset(false);
			MPRAGEOtsuFilter->SetNumberOfThresholds(3);
			MPRAGEOtsuFilter->Update();
			cout << "T1 Otsu thresholding success" << endl;

			// keep the label with largest label value
			LabelImageToLabelMapFilter<FloatImageType, LabelMapType>::Pointer labelImageToLabelMapFilter = LabelImageToLabelMapFilter<FloatImageType, LabelMapType>::New();
			labelImageToLabelMapFilter->SetInput(MPRAGEOtsuFilter->GetOutput());
			labelImageToLabelMapFilter->Update();

			std::vector<unsigned long> labelsToRemove;
			// Note: do NOT remove the labels inside the loop! The IDs are stored such that they will change when one is deleted.
			// Instead, mark all of the labels to be removed first and then remove them all together.
			for (unsigned int i = 0; i < labelImageToLabelMapFilter->GetOutput()->GetNumberOfLabelObjects(); i++)
			{
				// Mark every other label to be removed
				if ((i != labelImageToLabelMapFilter->GetOutput()->GetNumberOfLabelObjects()) && (i!=0))
				{
					labelsToRemove.push_back(i);
				}
			}

			// Remove all regions that were marked for removal.
			for (unsigned int i = 0; i < labelsToRemove.size(); ++i)
			{
				labelImageToLabelMapFilter->GetOutput()->RemoveLabel(labelsToRemove[i]);
			}
			m_plaqueLabelMap = labelImageToLabelMapFilter->GetOutput();

			// convert plaque label map back to image
			LabelMapToLabelImageFilter<LabelMapType, FloatImageType>::Pointer plaqueLabelMapToLabelImageFilter = LabelMapToLabelImageFilter<LabelMapType, FloatImageType>::New();
			plaqueLabelMapToLabelImageFilter->SetInput(labelImageToLabelMapFilter->GetOutput());
			plaqueLabelMapToLabelImageFilter->Update();
			ChangeLabelImageFilterType::Pointer plaqueChangeLabelImageFilter = ChangeLabelImageFilterType::New();
			plaqueChangeLabelImageFilter->SetInput(plaqueLabelMapToLabelImageFilter->GetOutput());
			std::map<float, float> plaqueChangeMap;
			plaqueChangeMap.insert(pair<float, float>(3, 1));
			plaqueChangeLabelImageFilter->SetChangeMap(plaqueChangeMap);
			plaqueChangeLabelImageFilter->Update();

			m_plaqueImage = plaqueChangeLabelImageFilter->GetOutput();
			cout << "Plaque segmentation success" << endl;

			// subtract wall image by plaque image
			SubtractImageFilterType::Pointer plaqueSubtractFilter = SubtractImageFilterType::New();
			plaqueSubtractFilter->SetInput1(m_wallImage);
			plaqueSubtractFilter->SetInput2(m_plaqueImage);
			plaqueSubtractFilter->Update();
			m_wallImage = plaqueSubtractFilter->GetOutput();

			// generate vtk plaque surface;
			ImageToVTKImageType::Pointer plaqueConverter = ImageToVTKImageType::New();
			plaqueConverter->SetInput(m_plaqueImage);
			plaqueConverter->Update();
			SurfaceCreator* plaqueCreator = new SurfaceCreator;
			plaqueCreator->SetInput(plaqueConverter->GetOutput());
			plaqueCreator->SetValue(1);
			plaqueCreator->SetResamplingFactor(1);
			plaqueCreator->Update();
			m_plaqueSurface->DeepCopy(plaqueCreator->GetOutput());

		}
		else
		{
			cerr << "MPRAGE image is not setted." << endl;
		}

		// convert wall image to label map
		LabelImageToLabelMapFilterType::Pointer wallImageToLabelMapFilter = LabelImageToLabelMapFilterType::New();
		wallImageToLabelMapFilter->SetInput(m_wallImage);
		wallImageToLabelMapFilter->Update();
		m_wallLabelMap = wallImageToLabelMapFilter->GetOutput();

		// merge lumen to label map
		typedef itk::MergeLabelMapFilter<LabelMapType> MergerType;
		MergerType::Pointer merger = MergerType::New();
		merger->SetInput(0, m_wallLabelMap);
		merger->SetInput(1, m_lumenLabelMap);
		merger->SetInput(2, m_plaqueLabelMap);
		merger->SetMethod(MergerType::PACK);
		merger->Update();
		m_labelMap = merger->GetOutput();

		// convert the merged label map to label image type
		LabelMapToLabelImageFilter<LabelMapType, FloatImageType>::Pointer labelMapToLabelImageFilter = LabelMapToLabelImageFilter<LabelMapType, FloatImageType>::New();
		labelMapToLabelImageFilter->SetInput(m_labelMap);
		labelMapToLabelImageFilter->Update();
		m_outputSegmentation = labelMapToLabelImageFilter->GetOutput();

		//typedef ImageFileWriter<FloatImageType> WriterType;
		//WriterType::Pointer writer = WriterType::New();
		//writer->SetInput(m_outputSegmentation);
		//writer->SetFileName("D:\\vessel_segmentation\\data\\nifti_corrected\\seg.nii");
		//writer->Write();
	}
}

void VesselSegmentation::GetOutputSegmentationLabelMap(LabelMapType::Pointer outputLabelMap)
{
	outputLabelMap = m_labelMap;
}

void VesselSegmentation::GetOutputSegmentationImage(FloatImageType::Pointer outputImage)
{
	outputImage = m_outputSegmentation;
}

void VesselSegmentation::GetVesselWallSurface(vtkPolyData* wallSurface)
{
	wallSurface->DeepCopy(m_wallSurface);
}

void VesselSegmentation::GetVesselLumenSurface(vtkPolyData* lumenSurface)
{
	lumenSurface->DeepCopy(m_lumenSurface);
}

void VesselSegmentation::GetPlaqueSurface(vtkPolyData* plaqueSurface)
{
	plaqueSurface->DeepCopy(m_plaqueSurface);
}