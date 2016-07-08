#include "MyVtkInteractorStyleImage.h"



vtkStandardNewMacro(MyVtkInteractorStyleImage);

void MyVtkInteractorStyleImage::SetImageViewer(vtkImageViewer2* imageViewer) 
{
	this->imageViewer = imageViewer;
	minSlice	  = imageViewer->GetSliceMin();
	maxSlice	  = imageViewer->GetSliceMax();
	slice		  = imageViewer->GetSlice();
	cout << minSlice << " " << maxSlice << endl;
//	//Contour Widget
//    contourWidget = NULL;
//    contourWidgetA = NULL;
//    contourRep = NULL;
//    contourRepA = NULL;
//	//Paint Brush
//	m_drawValueComboBox = NULL;
//	m_drawBrushShapeComboBox = NULL;
//	m_drawBrushSizeSpinBox = NULL;
//	m_drawVolumetricCheckBox = NULL;
//	m_drawIsotropicCheckBox = NULL;
//	m_borderWidget = NULL;
//	m_retangleRep = NULL;
//	m_brush = NULL;
//	BrushActor = NULL;
//	//Default color: Red
//	color_r = 255;
//	color_g = 0;
//	color_b = 0;
//	m_opacity = 255;
//	//Image Viewer
	orientation = imageViewer->GetSliceOrientation();
//	//Default setting
//    m_mode = NavaigationMode;
//    m_functioning = false;
//    rclick_mode = false;
//    mclick_mode = false;
//    
//	m_imageViewer->GetInput()->GetExtent(extent);
	imageViewer->GetInput()->GetSpacing(spacing);
	imageViewer->GetInput()->GetOrigin(origin);
}
//
//void MyVtkInteractorStyleImage::SetOrientation(MyImageViewer* imageViewer)
//{
//    m_minSlice	  = imageViewer->GetSliceMin();
//    m_maxSlice	  = imageViewer->GetSliceMax();
//    m_slice		  = imageViewer->GetSlice();
//    m_orientation = imageViewer->GetSliceOrientation();
//}
//
//void MyVtkInteractorStyleImage::SetSliceSpinBox(QSpinBox* x, QSpinBox* y, QSpinBox* z)
//{
//	m_sliceSplinBox[0] = x;
//	m_sliceSplinBox[1] = y;
//	m_sliceSplinBox[2] = z;
//}
//
//
//void MyVtkInteractorStyleImage::SetWindowLevelSpinBox(QDoubleSpinBox* w,QDoubleSpinBox* l)
//{
//	m_wlDoubleSpinBox[0] = w;
//	m_wlDoubleSpinBox[1] = l;
//}
//
//void MyVtkInteractorStyleImage::SetDrawBrushSizeSpinBox(QSpinBox* s)
//{
//	m_drawBrushSizeSpinBox = s;
//}
//
//void MyVtkInteractorStyleImage::SetDrawBrushShapeComBox(QComboBox* comboBox)
//{
//	m_drawBrushShapeComboBox = comboBox;
//}
//
//void MyVtkInteractorStyleImage::SetDrawVolumetricCheckBox(QCheckBox* checkBox)
//{
//	m_drawVolumetricCheckBox = checkBox;
//}
//
//void MyVtkInteractorStyleImage::SetDrawIsotropicCheckBox(QCheckBox* checkBox)
//{
//	m_drawIsotropicCheckBox = checkBox;
//}
//void MyVtkInteractorStyleImage::SetDrawColor(int* rgb)
//{
//	color_r = rgb[0];
//	color_g = rgb[1];
//	color_b = rgb[2];
//}
//void MyVtkInteractorStyleImage::SetDrawColor(int r, int g, int b)
//{
//	color_r = r;
//	color_g = g;
//	color_b = b;
//}
//void MyVtkInteractorStyleImage::SetDrawOpacity(int opacity)
//{
//	m_opacity = opacity;
//}
void MyVtkInteractorStyleImage::SetMode(int m) 
{
	if( m>-1 && m<3)
		mode = (Mode)m;
	
	//if (m == 1)
	//	m_mode = NavaigationMode;

	//if (m == 2)
	//	m_mode = WindowLevelMode;

	//if (m == 3)
	//{
	//	m_mode = PaintBrushMode;
	//	//SetPaintBrushModeEnabled(true);
	//}

	//if (m == 4)
	//{
	//	m_mode = PolygonMode;
	//	//SetPolygonModeEnabled(true);
	//}

}

void MyVtkInteractorStyleImage::SetMode(Mode m)
{
	mode = m;
}

//void MyVtkInteractorStyleImage::setController(MultiplanarViewController * controller)
//{
//	this->controller = controller;
//}

MyVtkInteractorStyleImage::MyVtkInteractorStyleImage()
	:vtkInteractorStyleImage()
{
	slice = 0;
	minSlice = 0;
	maxSlice = 0;
	mode = WindowLevelMode;
	rclickDrag = false;
	lclickDrag = false;
	//controller = NULL;
	imageViewer = NULL;
}

MyVtkInteractorStyleImage::~MyVtkInteractorStyleImage()
{
}

void MyVtkInteractorStyleImage::MoveSliceForward() 
{
	//MainWindow* mainWnd = MainWindow::GetMainWindow();
	if(slice < maxSlice) 
	{
		slice += 1;
		imageViewer->SetSlice(slice);
		cout << " slice " << slice << endl;
		cout << " maxslice " << maxSlice << endl;
		cout << " minslice " << minSlice << endl;
		//switch (m_orientation)
		//{
		//case 0:
		//	mainWnd->slotChangeSlice(m_slice, m_sliceSplinBox[1]->value(), m_sliceSplinBox[2]->value());
		//	break;
		//case 1:
		//	mainWnd->slotChangeSlice(m_sliceSplinBox[0]->value(), m_slice, m_sliceSplinBox[2]->value());
		//	break;
		//case 2:
		//	mainWnd->slotChangeSlice(m_sliceSplinBox[0]->value(), m_sliceSplinBox[1]->value(), m_slice);
		//	break;
		//}

	}
}

void MyVtkInteractorStyleImage::MoveSliceBackward() 
{
	//MainWindow* mainWnd = MainWindow::GetMainWindow();
	if(slice > minSlice) 
	{
		slice -= 1;
		imageViewer->SetSlice(slice);
		cout << " slice " << slice << endl;
		cout << " maxslice " << maxSlice << endl;
		cout << " minslice " << minSlice << endl;
		//switch (m_orientation)
		//{
		//case 0:
		//	mainWnd->slotChangeSlice(m_slice, m_sliceSplinBox[1]->value(), m_sliceSplinBox[2]->value());
		//	break;
		//case 1:
		//	mainWnd->slotChangeSlice(m_sliceSplinBox[0]->value(), m_slice, m_sliceSplinBox[2]->value());
		//	break;
		//case 2:
		//	mainWnd->slotChangeSlice(m_sliceSplinBox[0]->value(), m_sliceSplinBox[1]->value(), m_slice);
		//	break;
		//}

	}
}

void MyVtkInteractorStyleImage::OnMouseWheelForward() 
{
	//std::cout << "Scrolled mouse wheel forward." << std::endl;
	if(slice < maxSlice) {
		MoveSliceForward();
	}
	// don't forward events, otherwise the image will be zoomed 
	// in case another interactorstyle is used (e.g. trackballstyle, ...)
	// vtkInteractorStyleImage::OnMouseWheelForward();
}


void MyVtkInteractorStyleImage::OnMouseWheelBackward() 
{
	//std::cout << "Scrolled mouse wheel backward." << std::endl;
	if(slice > minSlice) {
		MoveSliceBackward();
	}
	// don't forward events, otherwise the image will be zoomed 
	// in case another interactorstyle is used (e.g. trackballstyle, ...)
	// vtkInteractorStyleImage::OnMouseWheelBackward();
}

void MyVtkInteractorStyleImage::OnLeftButtonUp()
{
	MainWindow* mainWnd = MainWindow::GetMainWindow();
	lclickDrag = false;
	//if (m_mode == 3)
	//{
	//	if (!isDraw)
	//	{
	//		this->Write2ImageData();
	//		mainWnd->slotOverlayVisibilty(true, m_orientation);
	//		//Clear Layer
	//		m_brush->SetDrawColor(0, 0, 0, 0);
	//		this->FillBox3D();
	//		this->Render();
	//		isDraw = true;
	//	}
	//	
	//}

}

void MyVtkInteractorStyleImage::OnLeftButtonDown()
{
	
	lclickDrag = true;
	//m_functioning = true;
	//isDraw = false;
	//MainWindow* mainWnd = MainWindow::GetMainWindow();
	switch (mode)
	{
	case MyVtkInteractorStyleImage::OtherMode:
		break;
	case MyVtkInteractorStyleImage::NavaigationMode:
		CalculateIndex(index);
		break;
	case MyVtkInteractorStyleImage::WindowLevelMode:
		vtkInteractorStyleImage::OnLeftButtonDown();
		break;
	case MyVtkInteractorStyleImage::PaintBrushMode:
		break;
	case MyVtkInteractorStyleImage::PolygonMode:
		break;
	default:
		vtkInteractorStyleImage::OnLeftButtonDown();
		break;
	}

	//if(m_mode == 1)
	//	CalculateIndex();

	//if(m_mode == 2)
	//{
	//	int x = this->GetInteractor()->GetEventPosition()[0];
	//	int y = this->GetInteractor()->GetEventPosition()[1];

	//	this->WindowLevelStartPosition[0] = x;
	//	this->WindowLevelStartPosition[1] = y;  		
	//	
	//	this->FindPokedRenderer(x, y);
	//}
	//if (m_mode == 3)
 //   {
	//	this->GetInteractor()->GetPicker()->Pick(
	//		this->GetInteractor()->GetEventPosition()[0],
	//		this->GetInteractor()->GetEventPosition()[1],
	//		0,  // always zero.
	//		m_imageViewer->GetRenderer());

	//	double* picked = this->GetInteractor()->GetPicker()->GetPickPosition();

	//	//Check if valid pick
	//	if (picked[0] == 0.0&&picked[1] == 0.0)
	//		return;
	//	double index[3];
	//	if (m_imageViewer->GetInput() != NULL) {
	//		picked[m_orientation] = origin[m_orientation] + m_slice * spacing[m_orientation];
	//		for (int i = 0; i<3; i++)
	//		{
	//			index[i] = (picked[i] - origin[i]) / spacing[i];
	//		}
	//	}
	//	draw_index_old[0] = (int)(index[0] + 0.5);
	//	draw_index_old[1] = (int)(index[1] + 0.5);
	//	draw_index_old[2] = (int)(index[2] + 0.5);
	//	if (rclick_mode == true)
	//	{
	//		rclick_mode = false;
	//	}
	//	else
	//	{
	//		this->ReadfromImageData();
	//		this->Render();
	//	}
	//	mainWnd->slotOverlayVisibilty(false, m_orientation);
	//	Draw(true);
	//	this->Render();
 //   }
	/*
	if (m_mode == 4)
	{
		FillPolygon();
	}
	*/
}

//void MyVtkInteractorStyleImage::OnRightButtonUp()
//{
//	vtkInteractorStyleTrackballCamera::OnRightButtonUp();
//	rclick_mode = false;
//	MainWindow* mainWnd = MainWindow::GetMainWindow();
//	if (m_mode == 3)
//	{
//		if (!isDraw)
//		{
//			this->Write2ImageData();
//			mainWnd->slotOverlayVisibilty(true, m_orientation);
//			//Clear Layer
//			m_brush->SetDrawColor(0, 0, 0, 0);
//			this->FillBox3D();
//			this->Render();
//			isDraw = true;
//		}
//	}
//
//}
//
//void MyVtkInteractorStyleImage::OnRightButtonDown()
//{
//	MainWindow* mainWnd = MainWindow::GetMainWindow();
//	isDraw = false;
//	rclick_mode = true;
//	if (m_mode == 3)
//	{
//		this->GetInteractor()->GetPicker()->Pick(
//			this->GetInteractor()->GetEventPosition()[0],
//			this->GetInteractor()->GetEventPosition()[1],
//			0,  // always zero.
//			m_imageViewer->GetRenderer());
//
//		double* picked = this->GetInteractor()->GetPicker()->GetPickPosition();
//
//		//Check if valid pick
//		if (picked[0] == 0.0&&picked[1] == 0.0)
//			return;
//		double index[3];
//		if (m_imageViewer->GetInput() != NULL) {
//			picked[m_orientation] = origin[m_orientation] + m_slice * spacing[m_orientation];
//			for (int i = 0; i<3; i++)
//			{
//				index[i] = (picked[i] - origin[i]) / spacing[i];
//			}
//		}
//		draw_index_old[0] = (int)(index[0] + 0.5);
//		draw_index_old[1] = (int)(index[1] + 0.5);
//		draw_index_old[2] = (int)(index[2] + 0.5);
//		if (m_functioning == true)
//		{
//			m_functioning = false;
//		}
//		else
//		{
//			this->ReadfromImageData();
//			this->Render();
//		}
//		mainWnd->slotOverlayVisibilty(false, m_orientation);
//		this->Draw(false);
//		this->Render();
//	}
//    vtkInteractorStyleTrackballCamera::OnRightButtonDown();
//
//    
//}
//
//void MyVtkInteractorStyleImage::OnMiddleButtonUp()
//{
//    mclick_mode=false;
//    // Forward events
//    vtkInteractorStyleTrackballCamera::OnMiddleButtonUp();
//}
//
//void MyVtkInteractorStyleImage::OnMiddleButtonDown()
//{
//    mclick_mode=true;
//    vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
//}
void MyVtkInteractorStyleImage::OnMouseMove()
{
	if (lclickDrag) {
		switch (mode)
		{
		case MyVtkInteractorStyleImage::OtherMode:
			break;
		case MyVtkInteractorStyleImage::NavaigationMode:
			CalculateIndex(index);
			break;
		case MyVtkInteractorStyleImage::WindowLevelMode:
			vtkInteractorStyleImage::OnMouseMove();
			break;
		case MyVtkInteractorStyleImage::PaintBrushMode:
			break;
		case MyVtkInteractorStyleImage::PolygonMode:
			break;
		default:
			vtkInteractorStyleImage::OnMouseMove();
			break;
		}
	}
	else {
		vtkInteractorStyleImage::OnMouseMove();

	}
	

	//if (m_mode==1 && m_functioning == true)
	//	CalculateIndex();
	//else if (m_mode==2 && m_functioning == true)
	//	WindowLevel();
	//else if (m_mode == 3 && m_functioning == true)
	//{
	//	Draw(true);
	//	this->UpdateBorderWidgetPosition();
	//	this->Render();
	//}
	//else if (m_mode == 3 && rclick_mode == true)
	//{
	//	Draw(false);
	//	this->UpdateBorderWidgetPosition();
	//	this->Render();
	//}
	//else if (m_mode == 3)
	//{
	//	this->UpdateBorderWidgetPosition();
	//	this->Render();
	//	vtkInteractorStyleImage::OnMouseMove();
	//}
	//else
	//{
	//	vtkInteractorStyleImage::OnMouseMove();
	//}
	///*
 //   if ( rclick_mode == true || mclick_mode == true){
 //       Synchronize();
 //   }
	//*/
}
//
//	
//
//
//void MyVtkInteractorStyleImage::OnChar()
//{
//	vtkRenderWindowInteractor *rwi = this->Interactor;
//	switch (rwi->GetKeyCode()) 
//	{
//	case 'r':
//	case 'R':
//            if (m_imageViewer->GetInput() != NULL){
//                m_wlDoubleSpinBox[0]->setValue(m_imageViewer->GetDefaultWindowLevel()[0]);
//                m_wlDoubleSpinBox[1]->setValue(m_imageViewer->GetDefaultWindowLevel()[1]);
//            }
//		break;
//	}
//}
//
void MyVtkInteractorStyleImage::OnKeyPress() 
{
	// Get the keypress
	vtkRenderWindowInteractor *rwi = this->Interactor;
	std::string key = rwi->GetKeySym();

	// Handle an arrow key
	if(key == "Up")
		MoveSliceForward();
	else if(key == "Down")
		MoveSliceBackward();

	// Forward events
	vtkInteractorStyleImage::OnKeyPress();
}
//void MyVtkInteractorStyleImage::CalculateIndex()
void MyVtkInteractorStyleImage::CalculateIndex(double* index)
{
	//MainWindow* mainWnd = MainWindow::GetMainWindow();
	//Pick
	this->GetInteractor()->GetPicker()->Pick(	
		this->GetInteractor()->GetEventPosition()[0], 
		this->GetInteractor()->GetEventPosition()[1], 
		0,  // always zero.
		imageViewer->GetRenderer());

	double* picked = this->GetInteractor()->GetPicker()->GetPickPosition();

	//Check if valid pick
	if(picked[0]==0.0&&picked[1]==0.0)	
		return;
    if (imageViewer->GetInput() != NULL){
	picked[orientation] = origin[orientation] + slice * spacing[orientation];
        for (int i =0;i<3;i++)
        {
            index[i] = (picked[i]-origin[i])/spacing[i];
        }
		cout << index[0] << " " << index[1] << " " << index[2] << endl;
		//mainWnd->slotChangeSlice((int)(index[0] + 0.5), (int)(index[1] + 0.5), (int)(index[2] + 0.5));
    }
    
	
}
//
//void MyVtkInteractorStyleImage::RemoveContourWidgetAll(){
//    MainWindow* mainwnd = MainWindow::GetMainWindow();
// 
//    for (int i = 0 ; i < mainwnd->GetVisibleImageNo() ; i++){
//        mainwnd->GetMyVtkInteractorStyleImage(i)->RemoveContourWidget();
//        mainwnd->GetMyImageViewer(i)->GetRenderWindow()->Render();
//    }
//    
//
//}
//
//void MyVtkInteractorStyleImage::SetPaintBrushModeEnabled(bool b)
//{
//	MainWindow* mainWnd = MainWindow::GetMainWindow();
//
//	//Overlay Image
//	//if (!mainWnd->GetVtkOverlay())
//	//{
//
//	//	}
//	// Boarder Widget
//	if (m_borderWidget != NULL)
//	{
//		m_retangleRep->Delete();
//		m_borderWidget->Delete();
//		m_borderWidget = NULL;
//	}
//
//	if (b)
//	{
//		m_retangleRep = vtkBorderRepresentation::New();
//		m_borderWidget = vtkBorderWidget::New();
//		m_borderWidget->SetInteractor(this->GetInteractor());
//		m_borderWidget->SetCurrentRenderer(m_imageViewer->GetRenderer());
//		m_borderWidget->SetRepresentation(m_retangleRep);
//		m_borderWidget->SetManagesCursor(true);
//		m_borderWidget->GetBorderRepresentation()->SetMoving(false);
//		m_borderWidget->GetBorderRepresentation()->SetProportionalResize(true);
//		m_borderWidget->SelectableOff();
//		m_borderWidget->ProcessEventsOff();
//		m_borderWidget->Off();
//	}
//
//	//mainWnd->GetViewers(m_orientation)->GetAnnotationRenderer()->ResetCameraClippingRange();
//	this->GetInteractor()->Render();
//
//	if (m_brush)
//	{
//		Renderer2->RemoveActor(BrushActor);
//		Renderer2->Delete();
//		Renderer2 = NULL;
//
//		m_brush->Delete();
//		m_brush = NULL;
//		BrushActor->Delete();
//		BrushActor = NULL;
//		
//	}
//
//	m_brush = vtkImageCanvasSource2D::New();
//	m_brush->SetScalarTypeToUnsignedChar();
//	m_brush->SetNumberOfScalarComponents(4);
//	switch (m_orientation)
//	{
//	case 0:
//	{
//		m_brush->SetExtent(0, 0, extent[2], extent[3], extent[4], extent[5]);
//		break;
//	}
//	case 1:
//	{
//		m_brush->SetExtent(extent[0], extent[1], 0, 0, extent[4], extent[5]);
//		break;
//	}
//	case 2:
//	{
//		m_brush->SetExtent(extent[0], extent[1], extent[2], extent[3], 0, 0);
//		break;
//	}
//	}
//	m_brush->SetDrawColor(0, 0, 0, 0);
//	this->FillBox3D();
//	m_brush->Update();
//	m_brush->GetOutput()->SetOrigin(origin);
//	m_brush->GetOutput()->SetSpacing(spacing);
//
//	BrushActor = vtkImageActor::New();
//	BrushActor->SetInputData(m_brush->GetOutput());
//	BrushActor->GetProperty()->SetInterpolationTypeToNearest();
//	// Set the image actor
//
//	switch (this->m_orientation)
//	{
//	case 0:
//		this->BrushActor->SetDisplayExtent(
//			0, 0, extent[2], extent[3], extent[4], extent[5]);
//		break;
//	case 1:
//		this->BrushActor->SetDisplayExtent(
//			extent[0], extent[1], 0, 0, extent[4], extent[5]);
//		break;
//	case 2:
//		this->BrushActor->SetDisplayExtent(
//			extent[0], extent[1], extent[2], extent[3], 0, 0);
//		break;
//	}
//	Renderer2 = vtkRenderer::New();
//	Renderer2->SetLayer(1);
//	Renderer2->SetActiveCamera(m_imageViewer->GetRenderer()->GetActiveCamera());
//	m_imageViewer->GetRenderer()->SetLayer(0);
//	m_imageViewer->GetRenderWindow()->SetNumberOfLayers(2);
//	m_imageViewer->GetRenderWindow()->AddRenderer(Renderer2);
//
//	Renderer2->AddActor(BrushActor);
//	m_imageViewer->GetRenderWindow()->Render();
//
//}
//	
//void MyVtkInteractorStyleImage::Draw(bool b)
//{
//
//	if (b)
//		m_brush->SetDrawColor(color_r, color_g, color_b, m_opacity);
//	else
//		m_brush->SetDrawColor(0, 0, 0, 0);
//
//	//Pick
//	this->GetInteractor()->GetPicker()->Pick(
//		this->GetInteractor()->GetEventPosition()[0],
//		this->GetInteractor()->GetEventPosition()[1],
//		0,  // always zero.
//		m_imageViewer->GetRenderer());
//
//	double* picked = this->GetInteractor()->GetPicker()->GetPickPosition();
//
//	//Check if valid pick
//	if (picked[0] == 0.0&&picked[1] == 0.0)
//		return;
//	double index[3];
//	if (m_imageViewer->GetInput() != NULL) {
//		picked[m_orientation] = origin[m_orientation] + m_slice * spacing[m_orientation];
//		for (int i = 0; i<3; i++)
//		{
//			index[i] = (picked[i] - origin[i]) / spacing[i];
//		}
//	}
//	int draw_index[3];
//
//	draw_index[0] = (int)(index[0] + 0.5);
//	draw_index[1] = (int)(index[1] + 0.5);
//	draw_index[2] = (int)(index[2] + 0.5);
//
//	int x2Dindex, y2Dindex;
//	switch (m_orientation)
//	{
//	case 0:
//		x2Dindex = 1;
//		y2Dindex = 2;
//		break;
//	case 1:
//		x2Dindex = 0;
//		y2Dindex = 2;
//		break;
//	case 2:
//		x2Dindex = 0;
//		y2Dindex = 1;
//		break;
//	}
//
//	//Determine Brush Size and Position
//
//	//Circle
//	if (m_drawBrushShapeComboBox->currentText() == "Circle") {
//		if (m_drawBrushSizeSpinBox->value() % 2 == 1)
//			DrawCircle(draw_index_old[x2Dindex], draw_index_old[y2Dindex], draw_index[x2Dindex], draw_index[y2Dindex], m_drawBrushSizeSpinBox->value() / 2);
//		else
//		{
//			DrawCircle(draw_index_old[x2Dindex] - 1, draw_index_old[y2Dindex], draw_index[x2Dindex] - 1, draw_index[y2Dindex], m_drawBrushSizeSpinBox->value() / 2 - 1);
//			DrawCircle(draw_index_old[x2Dindex] - 1, draw_index_old[y2Dindex] - 1, draw_index[x2Dindex] - 1, draw_index[y2Dindex] - 1, m_drawBrushSizeSpinBox->value() / 2 - 1);
//			DrawCircle(draw_index_old[x2Dindex], draw_index_old[y2Dindex] - 1, draw_index[x2Dindex], draw_index[y2Dindex] - 1, m_drawBrushSizeSpinBox->value() / 2 - 1);
//			DrawCircle(draw_index_old[x2Dindex], draw_index_old[y2Dindex], draw_index[x2Dindex], draw_index[y2Dindex], m_drawBrushSizeSpinBox->value() / 2 - 1);
//		}
//	}
//	//Default:Square
//	else
//	{
//		if (m_drawBrushSizeSpinBox->value() % 2 == 1)
//		{// Odd Number Brush Size
//			for (int x = -(m_drawBrushSizeSpinBox->value() - 1) / 2; x < (m_drawBrushSizeSpinBox->value() - 1) / 2 + 1; x++)
//			{
//				for (int y = -(m_drawBrushSizeSpinBox->value() - 1) / 2; y < (m_drawBrushSizeSpinBox->value() - 1) / 2 + 1; y++)
//				{
//					this->DrawLine3D(draw_index_old[x2Dindex] + x, draw_index_old[y2Dindex] + y, draw_index[x2Dindex] + x, draw_index[y2Dindex] + y);
//				}
//			}
//		}
//		else
//		{ // Even Number Brush Size
//			for (int x = -(m_drawBrushSizeSpinBox->value()) / 2; x < (m_drawBrushSizeSpinBox->value()) / 2; x++)
//			{
//				for (int y = -(m_drawBrushSizeSpinBox->value()) / 2; y < (m_drawBrushSizeSpinBox->value()) / 2; y++)
//				{
//					this->DrawLine3D(draw_index_old[x2Dindex] + x, draw_index_old[y2Dindex] + y, draw_index[x2Dindex] + x, draw_index[y2Dindex] + y);
//				}
//			}
//
//		}
//	}
//
//	// Reset the old draw index
//	for (int i = 0; i < 3; i++)
//	{
//		draw_index_old[i] = draw_index[i];
//	}
//
//	//m_brush->Update();
//	//m_brush->GetOutput()->SetOrigin(origin);
//	//m_brush->GetOutput()->SetSpacing(spacing);
//
//}
//
//void MyVtkInteractorStyleImage::FillBox3D()
//{
//
//	switch (m_orientation)
//	{
//	case 0:
//		for (int y = extent[2]; y < extent[3]; y++)
//		{
//			m_brush->DrawSegment3D(0, y, extent[4], 0, y, extent[5]);
//		}
//		break;
//	case 1:
//		for (int x = extent[0]; x < extent[1]; x++)
//		{
//			m_brush->DrawSegment3D(x, 0, extent[4], x, 0, extent[5]);
//		}
//		break;
//	case 2:
//		m_brush->FillBox(extent[0], extent[1], extent[2], extent[3]);
//		break;
//	}
//
//}
//
//void MyVtkInteractorStyleImage::DrawLine3D(int x0, int y0, int x1, int y1)
//{
//	int index[6];
//	switch (m_orientation)
//	{
//	case 0:
//		if (x0 < extent[2] || x0 > extent[3] || y0 < extent[4] || y0 > extent[5] || x1 < extent[2] || x1 > extent[3] || y1 < extent[4] || y1 > extent[5])
//			return;
//		index[0] = 0;
//		index[1] = x0;
//		index[2] = y0;
//		index[3] = 0;
//		index[4] = x1;
//		index[5] = y1;
//		break;
//	case 1:
//		if (x0 < extent[0] || x0 > extent[1] || y0 < extent[4] || y0 > extent[5] || x1 < extent[0] || x1 > extent[1] || y1 < extent[4] || y1 > extent[5])
//			return;
//		index[0] = x0;
//		index[1] = 0;
//		index[2] = y0;
//		index[3] = x1;
//		index[4] = 0;
//		index[5] = y1;
//		break;
//	case 2:
//		if (x0 < extent[0] || x0 > extent[1] || y0 < extent[2] || y0 > extent[3] || x1 < extent[0] || x1 > extent[1] || y1 < extent[2] || y1 > extent[3])
//			return;
//		index[0] = x0;
//		index[1] = y0;
//		index[2] = 0;
//		index[3] = x1;
//		index[4] = y1;
//		index[5] = 0;
//		break;
//	}
//	m_brush->DrawSegment3D(index[0], index[1], index[2], index[3], index[4], index[5]);
//}
//
//void MyVtkInteractorStyleImage::DrawCircle(int x0, int y0, int x1, int y1, int radius)
//{
//	int x = 0, y = radius;
//	int d = 1 - radius;
//	while (x <= y) {
//		for (int i = -x; i <= x; i++)
//		{
//			this->DrawLine3D(x0 + i, y0 + y, x1 + i, y1 + y);
//			this->DrawLine3D(x0 + i, y0 - y, x1 + i, y1 - y);
//		}
//
//		for (int j = -y; j <= y; j++)
//		{
//			this->DrawLine3D(x0 + j, y0 + x, x1 + j, y1 + x);
//			this->DrawLine3D(x0 + j, y0 - x, x1 + j, y1 - x);
//		}
//
//		if (d<0)
//			d += (2 * x) + 3;
//		else {
//			d += (2 * (x - y)) + 5;
//			y -= 1;
//		}
//		x++;
//	}
//
//	/*			//MidPoint Circle algorithm //Th shape of the circle is a little bit wierd
//	int x = radius;
//	int y = 0;
//	int err = 0;
//
//	while (x >= y)
//	{
//	for (int i = -x; i <= x; i++)
//	{
//	this->DrawLine3D(x0 + i, y0 + y, x1 + i, y1 + y);
//	this->DrawLine3D(x0 + i, y0 - y, x1 + i, y1 - y);
//	}
//
//	for (int j = -y; j <= y; j++)
//	{
//	this->DrawLine3D(x0 + j, y0 + x, x1 + j, y1 + x);
//	this->DrawLine3D(x0 + j, y0 - x, x1 + j, y1 - x);
//	}
//
//	y += 1;
//	err += 1 + 2 * y;
//	if (2 * (err - x) + 1 > 0)
//	{
//	x -= 1;
//	err += 1 - 2 * x;
//	}
//	}
//	*/
//}
//void MyVtkInteractorStyleImage::UpdateBorderWidgetPosition()
//{
//	MainWindow* mainWnd = MainWindow::GetMainWindow();
//	if (m_borderWidget == NULL)
//		return;
//
//	double size = m_drawBrushSizeSpinBox->value();
//	double color[3] = { (double)color_r / 255.0,(double)color_g / 255.0,(double)color_b / 255.0 };
//
//	//Restrict the box applying on whole pixel
//	this->GetInteractor()->GetPicker()->Pick(
//		this->GetInteractor()->GetEventPosition()[0],
//		this->GetInteractor()->GetEventPosition()[1],
//		0,  // always zero.
//		m_imageViewer->GetRenderer());
//
//	double* picked = this->GetInteractor()->GetPicker()->GetPickPosition();
//
//	//Check if valid pick
//	if (picked[0] == 0.0&&picked[1] == 0.0)
//		return;
//	double index[3];
//	if (m_imageViewer->GetInput() != NULL) {
//		picked[m_orientation] = origin[m_orientation] + m_slice * spacing[m_orientation];
//		for (int i = 0; i<3; i++)
//		{
//			index[i] = (picked[i] - origin[i]) / spacing[i];
//		}
//	}
//
//	int x = this->GetInteractor()->GetEventPosition()[0];
//	int y = this->GetInteractor()->GetEventPosition()[1];
//	//int x = (int)(index[0] + 0.5)*spacing[0] + origin[0];
//	//int y = (int)(index[1] + 0.5)*spacing[1] + origin[1];
//
//
//	double X1 = x;
//	double Y1 = y;
//	double display_origin[3];
//	double display_size[3];
//
//	m_imageViewer->GetRenderer()->SetWorldPoint(0, 0, 0, 1);
//	m_imageViewer->GetRenderer()->WorldToDisplay();
//	m_imageViewer->GetRenderer()->GetDisplayPoint(display_origin);
//	//cout << "World->Display vtkRenderer 00: " << "(" << x << "," << y << ")" << " --> " << "(" << result2[0] << "," << result2[1] << ")\n";
//
//	m_imageViewer->GetRenderer()->SetWorldPoint(size*spacing[0], size*spacing[1], size*spacing[2], 1);
//	m_imageViewer->GetRenderer()->WorldToDisplay();
//	m_imageViewer->GetRenderer()->GetDisplayPoint(display_size);
//	//cout << "World->Display vtkRenderer: "	<< "(" << x << "," << y << ")" << " --> " << "(" << result[0] << "," << result[1] << ")\n";
//
//	double X3 = display_size[0] - display_origin[0];
//	double Y3 = display_size[1] - display_origin[1];
//
//	m_imageViewer->GetRenderer()->DisplayToNormalizedDisplay(X1, Y1);
//	m_imageViewer->GetRenderer()->NormalizedDisplayToViewport(X1, Y1);
//	m_imageViewer->GetRenderer()->ViewportToNormalizedViewport(X1, Y1);
//
//	m_imageViewer->GetRenderer()->DisplayToNormalizedDisplay(X3, Y3);
//	m_imageViewer->GetRenderer()->NormalizedDisplayToViewport(X3, Y3);
//	m_imageViewer->GetRenderer()->ViewportToNormalizedViewport(X3, Y3);
//
//	//if (m_drawBrushShapeComboBox->currentText() == "Square") //Square
//	//{
//	m_borderWidget->SetRepresentation(m_retangleRep);
//	X1 = X1 - X3 / 2.0;
//	Y1 = Y1 - Y3 / 2.0;
//	//	}
//
//	vtkCoordinate* pos1 = m_borderWidget->GetBorderRepresentation()->GetPositionCoordinate();
//	pos1->SetValue(X1, Y1);
//
//	vtkCoordinate* pos2 = m_borderWidget->GetBorderRepresentation()->GetPosition2Coordinate();
//	pos2->SetValue(X3, Y3);
//	m_borderWidget->GetBorderRepresentation()->GetBorderProperty()->SetColor(color);
//	m_borderWidget->GetBorderRepresentation()->GetBorderProperty()->SetLineStipplePattern(0xf0f0);
//	m_borderWidget->GetBorderRepresentation()->Modified();
//	m_borderWidget->Modified();
//	m_borderWidget->On();
//
//	//double rsize[2];
//	//m_borderWidget->GetBorderRepresentation()->GetSize(rsize);
//	//qDebug() << "Size: " << size;
//	//qDebug() << "rSize: " << rsize[0] << ", " << rsize[1];
//	//qDebug() << "Event: " << x << y;
//	//qDebug() << "Pos3:" << X3 << Y3;
//	//qDebug() << "rPos: " << QString::number(m_borderWidget->GetBorderRepresentation()->GetPosition()[0]) << QString::number(m_borderWidget->GetBorderRepresentation()->GetPosition()[1]);
//	//qDebug() << "rPos2: " << QString::number(m_borderWidget->GetBorderRepresentation()->GetPosition2()[0]) << QString::number(m_borderWidget->GetBorderRepresentation()->GetPosition2()[1]);
//
//}
//
//void MyVtkInteractorStyleImage::ReadfromImageData()
//{
//	
//	MainWindow* mainWnd = MainWindow::GetMainWindow();
//	int label = mainWnd->GetImageLayerNo();
//	//Clear Layer
//	m_brush->SetDrawColor(0, 0, 0, 0);
//	this->FillBox3D();
//	//m_brush->Update();
//	
//
//	int pos[3];
//	switch (m_orientation)
//	{
//	case 0:
//		for (int y = extent[2]; y < extent[3]; y++)
//		{
//			for (int z = extent[4]; z < extent[5]; z++)
//			{
//				pos[0] = m_sliceSplinBox[m_orientation]->value();
//				pos[1] = y;
//				pos[2] = z;
//				double* val = static_cast<double *>(mainWnd->GetOverlay()->GetOutput()->GetScalarPointer(pos));
//				if ((int)val[0] == label)
//				{
//					this->SetDrawColor(255, 0, 0);
//					m_brush->DrawSegment3D(0, y, z, 0, y, z);
//				}
//			}
//		}
//		break;
//	case 1:
//		for (int x = extent[0]; x < extent[1]; x++)
//		{
//			for (int z = extent[4]; z < extent[5]; z++)
//			{
//				pos[0] = x;
//				pos[1] = m_sliceSplinBox[m_orientation]->value();
//				pos[2] = z;
//				double* val = static_cast<double *>(mainWnd->GetOverlay()->GetOutput()->GetScalarPointer(pos));
//				if (val[0] > 0)
//					m_brush->DrawSegment3D(x, 0, z, x, 0, z);
//			}
//		}
//		break;
//	case 2:
//		for (int x = extent[0]; x < extent[1]; x++)
//		{
//			for (int y = extent[2]; y < extent[3]; y++)
//			{
//				pos[0] = x;
//				pos[1] = y;
//				pos[2] = m_sliceSplinBox[m_orientation]->value();
//				double * val = static_cast<double *>(mainWnd->GetOverlay()->GetOutput()->GetScalarPointer(pos));
//				if (val[0] > 0)
//					m_brush->DrawPoint(x, y);
//			}
//		}
//		break;
//	}
//
//}
//
//void MyVtkInteractorStyleImage::Render()
//{
//	MainWindow* mainWnd = MainWindow::GetMainWindow();
//	m_brush->Update();
//	m_brush->GetOutput()->SetOrigin(origin);
//	m_brush->GetOutput()->SetSpacing(spacing);
//	mainWnd->RenderAllViewer();
//}
//
//void MyVtkInteractorStyleImage::Write2ImageData()
//{
//	//Update Layer
//	MainWindow* mainWnd = MainWindow::GetMainWindow();
//	int label = mainWnd->GetImageLayerNo();
//	int pos[3];
//	double pixelval;
//	switch (m_orientation)
//	{
//	case 0:
//		;
//		for (int y = extent[2]; y < extent[3]; y++)
//		{
//			for (int z = extent[4]; z < extent[5]; z++)
//			{
//				pos[0] = m_sliceSplinBox[m_orientation]->value();
//				pos[1] = y;
//				pos[2] = z;
//				uchar* val = static_cast<uchar *>(m_brush->GetOutput()->GetScalarPointer(0,y,z));
//				if (val[0] > 0 || val[1] > 0 || val[2] > 0 || val[3] > 0)
//					pixelval = label;
//				else
//					pixelval = 0;
//				mainWnd->GetOverlay()->SetPixel(pos, pixelval);
//			}
//		}
//		break;
//	case 1:
//		for (int x = extent[0]; x < extent[1]; x++)
//		{
//			for (int z = extent[4]; z < extent[5]; z++)
//			{
//				pos[0] = x;
//				pos[1] = m_sliceSplinBox[m_orientation]->value();
//				pos[2] = z;
//				uchar* val = static_cast<uchar *>(m_brush->GetOutput()->GetScalarPointer(x,0,z));
//				if (val[0] > 0 || val[1] > 0 || val[2] > 0 || val[3] > 0)
//					pixelval = label;
//				else
//					pixelval = 0;
//				mainWnd->GetOverlay()->SetPixel(pos, pixelval);
//			}
//		}
//		break;
//	case 2:
//		for (int x = extent[0]; x < extent[1]; x++)
//		{
//			for (int y = extent[2]; y < extent[3]; y++)
//			{
//				pos[0] = x;
//				pos[1] = y;
//				pos[2] = m_sliceSplinBox[m_orientation]->value();
//				uchar * val = static_cast<uchar *>(m_brush->GetOutput()->GetScalarPointer(x,y,0));
//				if (val[0] > 0 || val[1] > 0 || val[2] > 0 || val[3] > 0)
//					pixelval = label;
//				else
//					pixelval = 0;
//				mainWnd->GetOverlay()->SetPixel(pos, pixelval);
//				
//			}
//		}
//		break;
//	}
//	mainWnd->GetOverlay()->GetOutput()->Modified();
//	
//}
//
//void MyVtkInteractorStyleImage::SetPolygonModeEnabled(bool b)
//{
//	    this->RemoveContourWidgetAll();
//        Renderer=m_imageViewer->GetRenderer();
//        RenderWindow=m_imageViewer->GetRenderWindow();
//        this->Synchronize();
//        if (contourWidget){
//            contourWidget->Delete();
//            contourWidget = NULL;
//        }
//        if (contourWidgetA){
//        contourWidgetA->Delete();
//        contourWidgetA = NULL;
//        }
//        Renderer2 = vtkRenderer::New();
//        Renderer2->SetLayer(1);
//        Renderer2->SetActiveCamera(Renderer->GetActiveCamera());
//        Renderer->SetLayer(0);
//        RenderWindow->SetNumberOfLayers(2);
//        RenderWindow->AddRenderer(Renderer2);
//        
//        contourWidget = vtkContourWidget::New();
//        contourWidget->SetInteractor(RenderWindow->GetInteractor());
//        contourWidget->SetCurrentRenderer(Renderer2);
//        
//        contourRep = vtkOrientedGlyphContourRepresentation::New();
//        contourRep->SetRenderer(Renderer2);
//        contourRep->GetLinesProperty()->SetColor(255,255,0);
//        
//        vtkPolyData* cursorpolyData = contourRep->GetActiveCursorShape();
//        vtkSmartPointer<vtkTransform> translation = vtkSmartPointer<vtkTransform>::New();
//        if (m_orientation == 0){
//            translation->RotateX(90);
//            translation->RotateZ(90);
//        }
//        else if (m_orientation == 1){
//            translation->RotateX(90);
//            translation->RotateY(90);
//        }
//        
//        vtkSmartPointer<vtkTransformPolyDataFilter> transformFilter =
//        vtkSmartPointer<vtkTransformPolyDataFilter>::New();
//        transformFilter->SetInputData(cursorpolyData);
//        transformFilter->SetTransform(translation);
//        transformFilter->Update();
//        
//        cursorpolyData->DeepCopy(transformFilter->GetOutput());
//        contourWidget->SetRepresentation(contourRep);
//        contourWidget->FollowCursorOn();
//        contourWidget->CreateDefaultRepresentation();
//        contourWidget->On();
//    contourWidget->EnabledOn();
//    
//    RenderWindow->Render();
//}
//
//
//
//bool MyVtkInteractorStyleImage::FillPolygon()
//    {
//        
//        //Check if region is selected by the user
//        if (contourRep == NULL)
//            return false;
//
//        vtkPolyData* polydata=vtkPolyData::New();
//        polydata=contourRep->GetContourRepresentationAsPolyData();
//        pointsInsideContour=vtkPoints::New();
//        vtkPolygon* polygon = vtkPolygon::New();
//        int numOfPoints = polydata->GetNumberOfPoints();
//        //cout << numOfPoints << endl;
//        // Get the coordinates of the contour data points
//        double origin[3];
//        double spacing[3];
//        m_imageViewer->GetInput()->GetOrigin(origin);
//        m_imageViewer->GetInput()->GetSpacing(spacing);
//        
//        for(vtkIdType i = 0; i < numOfPoints; i++)
//        {
//            double p[3];
//            double s[3];
//            polydata->GetPoint(i,p);
//            //std::cout << "Point " << i << " : (" << p[0] << " " << p[1] << " " << p[2] << ")" << std::endl;
//            //Take one image data 1 to be reference
//            s[0]=(p[0]-origin[0])/spacing[0];
//            s[1]=(p[1]-origin[1])/spacing[1];
//            s[2]=(p[2]-origin[2])/spacing[2];
//            //cout << s[0] << " " << s[1] << " " << s[2] << endl;
//            //Test whether the points are inside the polygon or not
//            if (m_orientation == 0)
//            polygon->GetPoints()->InsertNextPoint(0.0,s[1], s[2]);
//            else if (m_orientation == 1)
//            polygon->GetPoints()->InsertNextPoint(s[0],0.0, s[2]);
//            else if (m_orientation == 2)
//            polygon->GetPoints()->InsertNextPoint(s[0],s[1], 0.0);
//        }
//        
//        double n[3];
//        polygon->ComputeNormal(polygon->GetPoints()->GetNumberOfPoints(),
//                               static_cast<double*>(polygon->GetPoints()->GetData()->GetVoidPointer(0)), n);
//        double bounds[6];
//        int bounds_int[6];
//        
//        polygon->GetPoints()->GetBounds(bounds);
//        
//        bounds_int[0] = ceil(bounds[0]);
//        bounds_int[1] = floor(bounds[1]);
//        bounds_int[2] = ceil(bounds[2]);
//        bounds_int[3] = floor(bounds[3]);
//        bounds_int[4] = ceil(bounds[4]);
//        bounds_int[5] = floor(bounds[5]);
//
//        //cout << "Bounds: " << bounds_int[0] << " " << bounds_int[1] << " " << bounds_int[2] << " " << bounds_int[3] << " " << bounds_int[4] << " " << bounds_int[5] << endl;
//        
//        if (m_orientation == 0)
//        {
//            for ( int y = bounds_int[2] ; y < bounds_int[3] ; y++){
//                for ( int z = bounds_int[4] ; z < bounds_int[5] ; z++){
//                    double p[3];
//                    p[0] = 0.0;
//                    p[1] = (double)y;
//                    p[2] = (double)z;
//                    if (polygon->PointInPolygon(p, polygon->GetPoints()->GetNumberOfPoints(),
//                                                static_cast<double*>(polygon->GetPoints()->GetData()->GetVoidPointer(0)), bounds, n))
//                        pointsInsideContour->InsertNextPoint(p);
//                        
//                }
//            }
//        }
//        else if (m_orientation == 1)
//        {
//        for ( int x = bounds_int[0] ; x < bounds_int[1] ; x++){
//            for ( int z = bounds_int[4] ; z < bounds_int[5] ; z++){
//                double p[3];
//                p[0] = (double)x;
//                p[1] = 0.0;
//                p[2] = (double)z;
//                if (polygon->PointInPolygon(p, polygon->GetPoints()->GetNumberOfPoints(),
//                                            static_cast<double*>(polygon->GetPoints()->GetData()->GetVoidPointer(0)), bounds, n))
//                    pointsInsideContour->InsertNextPoint(p);
//            }
//        }
//        }
//        else if (m_orientation == 2)
//        {
//            for ( int x = bounds_int[0] ; x < bounds_int[1] ; x++){
//                for ( int y = bounds_int[2] ; y < bounds_int[3] ; y++){
//                    double p[3];
//                    p[0] = (double)x;
//                    p[1] = (double)y;
//                    p[2] = 0.0;
//                    if (polygon->PointInPolygon(p, polygon->GetPoints()->GetNumberOfPoints(),
//                                                static_cast<double*>(polygon->GetPoints()->GetData()->GetVoidPointer(0)), bounds, n))
//                        pointsInsideContour->InsertNextPoint(p);
//                }
//            }
//        }
//        
//        
//        if (pointsInsideContour->GetNumberOfPoints() == 0)
//        {
//            return false;
//        }
//        else
//            return true;
//}
//
//
//void MyVtkInteractorStyleImage::DrawSynchronize(vtkPolyData* polydata)
//{
//    Renderer=m_imageViewer->GetRenderer();
//    RenderWindow=m_imageViewer->GetRenderWindow();
//    if (contourWidgetA){
//        contourWidgetA->Delete();
//        contourWidgetA = NULL;
//    }
//    Renderer2 = vtkRenderer::New();
//    Renderer2->SetLayer(1);
//    Renderer2->SetActiveCamera(Renderer->GetActiveCamera());
//    Renderer->SetLayer(0);
//    RenderWindow->SetNumberOfLayers(2);
//    RenderWindow->AddRenderer(Renderer2);
//    
//    contourWidgetA = vtkContourWidget::New();
//    contourWidgetA->SetInteractor(RenderWindow->GetInteractor());
//    contourWidgetA->SetCurrentRenderer(Renderer2);
//    
//    contourRepA = vtkOrientedGlyphContourRepresentation::New();
//    contourRepA->SetRenderer(Renderer2);
//    contourRepA->GetLinesProperty()->SetColor(255,255,255);
//    contourRepA->GetProperty()->SetOpacity(0);
//    contourWidgetA->CreateDefaultRepresentation();
//    contourWidgetA->SetRepresentation(contourRepA);
//    contourWidgetA->Initialize(polydata);
//    contourWidgetA->SetRepresentation(contourRepA);
//    contourWidgetA->On();
//    RenderWindow->Render();
//    
//}
//
//
//void MyVtkInteractorStyleImage::RemoveContourWidget()
//{
//    if (contourWidget)
//    {
//        contourWidget->Delete();
//    }
//    if (contourWidgetA)
//    {
//        contourWidgetA->Delete();
//    }
//
//    contourRep = NULL;
//    contourRepA = NULL;
//    contourWidget = NULL;
//    contourWidgetA = NULL;
//    pointsInsideContour = NULL;
//    
//}
//
//
//void MyVtkInteractorStyleImage::WindowLevel()
//{
//	this->WindowLevelCurrentPosition[0] = this->GetInteractor()->GetEventPosition()[0];
//	this->WindowLevelCurrentPosition[1] = this->GetInteractor()->GetEventPosition()[1];
//
//	int *size = this->GetCurrentRenderer()->GetSize();
//
//    double window=0.0, level=0.0;
//    if (m_imageViewer->GetInput() != NULL){
//        window = m_imageViewer->GetDefaultWindowLevel()[0];
//        level = m_imageViewer->GetDefaultWindowLevel()[1];
//    }
//    
//	// Compute normalized delta
//
//	double dx = (this->WindowLevelCurrentPosition[0] -
//		this->WindowLevelStartPosition[0]) * 4.0 / size[0];
//	double dy = (this->WindowLevelStartPosition[1] -
//		this->WindowLevelCurrentPosition[1]) * 4.0 / size[1];
//
//	// Scale by current values
//
//	if ( fabs( window ) > 0.01 )
//	{
//		dx = dx * window;
//	}
//	else
//	{
//		dx = dx * ( window < 0 ? -0.01 : 0.01 );
//	}
//	if ( fabs( level ) > 0.01 )
//	{
//		dy = dy * level;
//	}
//	else
//	{
//		dy = dy * ( level < 0 ? -0.01 : 0.01 );
//	}
//
//	// Abs so that direction does not flip
//
//	if ( window < 0.0 )
//	{
//		dx = -1 * dx;
//	}
//	if ( level < 0.0 )
//	{
//		dy = -1 * dy;
//	}
//
//	// Compute new window level
//
//	double newWindow = dx + window;
//	double newLevel = level - dy;
//
//	if ( newWindow < 0.01 )
//	{
//		newWindow = 0.01;
//	}
//
//	m_wlDoubleSpinBox[0]->setValue(newWindow);
//	m_wlDoubleSpinBox[1]->setValue(newLevel);
//}
//
// 
// 
//void MyVtkInteractorStyleImage::SetSliceRange( int min, int max )
//{
//	m_minSlice = min;
//	m_maxSlice = max;
//}
//
//void MyVtkInteractorStyleImage::SetCurrentSlice( int slice )
//{
//	m_slice = slice;
//}
//
//void MyVtkInteractorStyleImage::Synchronize()
//{
//    
//    MainWindow* mainwnd = MainWindow::GetMainWindow();
//    vtkCamera* camera = m_imageViewer->GetRenderer()->GetActiveCamera();
//    for (int i=0;i<mainwnd->GetVisibleImageNo();i++)
//    {
//        
//        mainwnd->GetMyImageViewer(i)->GetRenderer()->SetActiveCamera(camera);
//        mainwnd->GetMyImageViewer(i)->Render();
//    }
//}
//
//vtkPoints* MyVtkInteractorStyleImage::GetPointsInsideContour()
//{
//    if (pointsInsideContour != NULL)
//        return pointsInsideContour;
//    else
//        return NULL;
//}
//
//vtkOrientedGlyphContourRepresentation* MyVtkInteractorStyleImage::GetContourRep()
//{
//    if (contourRep != NULL)
//        return contourRep;
//    else
//        return NULL;
//}
//
//void MyVtkInteractorStyleImage::SetContourVisibility(bool b)
//{
//	if (b)
//	{
//    if (contourWidget)
//        contourWidget->SetEnabled(1);
//    if (contourWidgetA)
//        contourWidgetA->SetEnabled(1);
//	}
//	else
//	{
//    if (contourWidget)
//        contourWidget->SetEnabled(0);
//    if (contourWidgetA)
//        contourWidgetA->SetEnabled(0);
//	}
//}