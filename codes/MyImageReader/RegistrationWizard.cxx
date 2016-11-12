#include "RegistrationWizard.h"
#include <QWidget>
#include <QLabel>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QSpacerItem>
#include <QProgressBar>
#include <QTreeWidgetItem>
#include <QDirIterator>
#include <QThread>
#include <QMutex>
#include <QTabWidget>
#include <QTableWidget>
#include <QSpinBox>
#include <QDebug>
#include <QStringList>
#include <QHeaderView>
#include <QString>
#include <QTableWidgetItem>
#include <QFileInfo>
#include <QRadioButton>
#include <QGroupBox>

#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkGDCMImageIO.h"

typedef itk::Image< float, 3 >		ImageType;
typedef itk::GDCMSeriesFileNames					NamesGeneratorType;
typedef itk::ImageSeriesReader< ImageType >			ReaderType;
typedef itk::GDCMImageIO							ImageIOType;


RegistrationWizard::RegistrationWizard(QString dir, QWidget *parent) : QWizard(parent)
{
	directoryPage	= new DirectoryPage;
	imagePage		= new ImagePage;
	conclusionPage	= new ConclusionPage;
	fileNameList	= new QList<QStringList*>;

	setMinimumHeight(500);
	setMinimumWidth(600);

    setPage(Page_Directory,  directoryPage);
    setPage(Page_Image,		 imagePage );
    setPage(Page_Conclusion, conclusionPage);
	
    setStartId(Page_Directory);
    setWindowTitle(tr("  Registration Wizard  "));
	setOption(HaveHelpButton,	true);
	setOption(NoCancelButton,	true);
	setOption(IndependentPages,	false);

    connect(this, SIGNAL(helpRequested()), this, SLOT(showHelp()));

	directoryPage->setDirectory(dir);
}

void RegistrationWizard::showHelp()
{
    QString message;

    switch (currentId()) {
    case Page_Directory:
        message = tr("Select the image directory");
        break;
    case Page_Image:
        message = tr("Select the image series");
        break;
    case Page_Conclusion:
        message = tr("Confirm the image information");
        break;
    default:
        message = tr("");
    }

    QMessageBox::information(this, tr("Registration Wizard Help"), message);

}


QStringList* RegistrationWizard::getFileNames1()
{
	int pos = field("image.list1").toInt();
	if (pos== -1)
		return NULL;
	else
		return fileNameList->at(pos);
}


QStringList* RegistrationWizard::getFileNames2()
{
	int pos = field("image.list2").toInt();
	if (pos== -1)
		return NULL;
	else
		return fileNameList->at(pos);
}

QStringList* RegistrationWizard::getFileNames3()
{
	int pos = field("image.list3").toInt();
	if (pos== -1)
		return NULL;
	else
		return fileNameList->at(pos);
}

QStringList* RegistrationWizard::getFileNames4()
{
    int pos = field("image.list4").toInt();
    if (pos== -1)
        return NULL;
    else
        return fileNameList->at(pos);
}

QStringList* RegistrationWizard::getFileNames5()
{
    int pos = field("image.list5").toInt();
    if (pos== -1)
        return NULL;
    else
        return fileNameList->at(pos);
}

int RegistrationWizard::getTotalFileNo()
{
	int num = 0;
	if (this->getFileNames1())
	{
		num += 1;
	}
	if (this->getFileNames2())
	{
		num += 1;
	}
	if (this->getFileNames3())
	{
		num += 1;
	}
	if (this->getFileNames4())
	{
		num += 1;
	}
	if (this->getFileNames5())
	{
		num += 1;
	}
	return num;
}

QString RegistrationWizard::getDirectory()
{
	return field("directory.path").toString();
}

QStringList * RegistrationWizard::getFileNamesN(int n)
{
	if (n < 1 || n>5)
		return NULL;
	int pos = field("image.list" + QString::number(n)).toInt();
	if (pos == -1)
		return NULL;
	else
		return fileNameList->at(pos);
}

DirectoryPage::DirectoryPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Set Image Directory"));

	imageDirectoryLabel		= new QLabel(tr("Image Directory:"));
	imageDirectoryLineEdit	= new QLineEdit;
	imageDirectoryLabel->setBuddy(imageDirectoryLineEdit);

	browseBtn = new QPushButton;
	browseBtn->setText("Browse");

	connect(browseBtn,SIGNAL(clicked()),this,SLOT(browse()),Qt::UniqueConnection);

	registerField("directory.path*", imageDirectoryLineEdit);

	QHBoxLayout *layout = new QHBoxLayout;
	layout->addWidget(imageDirectoryLabel);
	layout->addWidget(imageDirectoryLineEdit);
	layout->addWidget(browseBtn);
	setLayout(layout);
}

void DirectoryPage::browse()
{
	QString path = imageDirectoryLineEdit->text();
	if (path =="")
		path = QDir::currentPath();

	//Dialog Open Directory
	QString dir = QFileDialog::getExistingDirectory(this, 
		tr("Open Image Directory"),path, QFileDialog::ShowDirsOnly|QFileDialog::DontResolveSymlinks);

	if(dir=="") 
		return;

	dir.replace("\\","/");

	imageDirectoryLineEdit->setText(dir);
}

int DirectoryPage::nextId() const
{
    return RegistrationWizard::Page_Image;
}

void DirectoryPage::setDirectory(QString dir)
{
	imageDirectoryLineEdit->setText(dir);
}

ImagePage::ImagePage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Select image series"));

	//Image 1
	image1Label		= new QLabel(tr("CUBE T1:"));
	image1LineEdit	= new QLineEdit;
	image1SetBtn	= new QPushButton(tr("<<"));
	image1RemoveBtn	= new QPushButton(tr("X"));
	image1SpinBox	= new QSpinBox;
	image1Label->setBuddy(image1LineEdit);
	image1LineEdit->setReadOnly(true);
	image1SetBtn->setFixedSize(30,23);
	image1RemoveBtn->setFixedSize(30,23);
	
	QHBoxLayout *image1layout = new QHBoxLayout;
	image1layout->addWidget(image1Label);
	image1layout->addWidget(image1LineEdit);
	image1layout->addWidget(image1SetBtn);
	image1layout->addWidget(image1RemoveBtn);
	
	//Image 2
	image2Label		= new QLabel(tr("CUBE T2:"));
	image2LineEdit	= new QLineEdit;
	image2SetBtn	= new QPushButton(tr("<<"));
	image2RemoveBtn	= new QPushButton(tr("X"));
	image2SpinBox	= new QSpinBox;
	image2Label->setBuddy(image2LineEdit);
	image2LineEdit->setReadOnly(true);
	image2SetBtn->setFixedSize(30,23);
	image2RemoveBtn->setFixedSize(30,23);

	QHBoxLayout *image2layout = new QHBoxLayout;
	image2layout->addWidget(image2Label);
	image2layout->addWidget(image2LineEdit);
	image2layout->addWidget(image2SetBtn);
	image2layout->addWidget(image2RemoveBtn);

	//Image 3
	image3Label		= new QLabel(tr("CUBE T1+C:"));
	image3LineEdit	= new QLineEdit;
	image3SetBtn	= new QPushButton(tr("<<"));
	image3RemoveBtn	= new QPushButton(tr("X"));
	image3SpinBox	= new QSpinBox;
	image3Label->setBuddy(image3LineEdit);
	image3LineEdit->setReadOnly(true);
	image3SetBtn->setFixedSize(30,23);
	image3RemoveBtn->setFixedSize(30,23);

	QHBoxLayout *image3layout = new QHBoxLayout;
	image3layout->addWidget(image3Label);
	image3layout->addWidget(image3LineEdit);
	image3layout->addWidget(image3SetBtn);
	image3layout->addWidget(image3RemoveBtn);
    
    //Image 4
    image4Label		= new QLabel(tr("2D DIR/QIR:"));
    image4LineEdit	= new QLineEdit;
    image4SetBtn	= new QPushButton(tr("<<"));
    image4RemoveBtn	= new QPushButton(tr("X"));
    image4SpinBox	= new QSpinBox;
    image4Label->setBuddy(image4LineEdit);
    image4LineEdit->setReadOnly(true);
    image4SetBtn->setFixedSize(30,23);
    image4RemoveBtn->setFixedSize(30,23);
    
    QHBoxLayout *image4layout = new QHBoxLayout;
    image4layout->addWidget(image4Label);
    image4layout->addWidget(image4LineEdit);
    image4layout->addWidget(image4SetBtn);
    image4layout->addWidget(image4RemoveBtn);

	//Image 5
    image5Label		= new QLabel(tr("MPRAGE:"));
    image5LineEdit	= new QLineEdit;
    image5SetBtn	= new QPushButton(tr("<<"));
    image5RemoveBtn	= new QPushButton(tr("X"));
    image5SpinBox	= new QSpinBox;
    image5Label->setBuddy(image5LineEdit);
    image5LineEdit->setReadOnly(true);
    image5SetBtn->setFixedSize(30,23);
    image5RemoveBtn->setFixedSize(30,23);
    
    QHBoxLayout *image5layout = new QHBoxLayout;
    image5layout->addWidget(image5Label);
    image5layout->addWidget(image5LineEdit);
    image5layout->addWidget(image5SetBtn);
    image5layout->addWidget(image5RemoveBtn);

	//Tree
	QStringList headerList;
	headerList<<"Patient Name"<<"Modality"<<"Series Description"<<"Slice"<<"Date"<<"Series UID"<<"Format";
	
	treeWidget = new QTreeWidget;
	treeWidget->setHeaderLabels(headerList);

	progressBar = new QProgressBar;
	progressBar->setTextVisible(true);
	progressBar->setStyleSheet("QProgressBar {text-align: center;}");

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(treeWidget);	
	layout->addSpacerItem(new QSpacerItem (50,1));
	layout->addLayout(image1layout);
	layout->addLayout(image2layout);
	layout->addLayout(image3layout);
    layout->addLayout(image4layout);
	layout->addLayout(image5layout);
	layout->addWidget(progressBar);
	setLayout(layout);


	//Set Initial Value to -ve value
	image1SpinBox->setMinimum(-1);
	image2SpinBox->setMinimum(-1);
	image3SpinBox->setMinimum(-1);
    image4SpinBox->setMinimum(-1);
    image5SpinBox->setMinimum(-1);

	image1SpinBox->setValue(-1);
	image2SpinBox->setValue(-1);
	image3SpinBox->setValue(-1);
    image4SpinBox->setValue(-1);
	image5SpinBox->setValue(-1);

	//* mean compulsory , otherwise optional 
	registerField("image.set1",	image1LineEdit);
	registerField("image.set2",	image2LineEdit);
	registerField("image.set3",	image3LineEdit);
    registerField("image.set4",    image4LineEdit);
	registerField("image.set5",    image5LineEdit);

	//* mean compulsory , otherwise optional 
	registerField("image.list1",	image1SpinBox);
	registerField("image.list2",	image2SpinBox);
	registerField("image.list3",	image3SpinBox);
    registerField("image.list4",	image4SpinBox);
	registerField("image.list5",	image5SpinBox);
	//Initialize 
	thread		  = new FindImageThread;
	lastDirectory ="";

	connect(image1SetBtn,SIGNAL(clicked()),this,SLOT(setImage1()),Qt::UniqueConnection);
	connect(image2SetBtn,SIGNAL(clicked()),this,SLOT(setImage2()),Qt::UniqueConnection);
	connect(image3SetBtn,SIGNAL(clicked()),this,SLOT(setImage3()),Qt::UniqueConnection);
    connect(image4SetBtn,SIGNAL(clicked()),this,SLOT(setImage4()),Qt::UniqueConnection);
	connect(image5SetBtn,SIGNAL(clicked()),this,SLOT(setImage5()),Qt::UniqueConnection);

	connect(image1RemoveBtn,SIGNAL(clicked()),this,SLOT(removeImage1()),Qt::UniqueConnection);
	connect(image2RemoveBtn,SIGNAL(clicked()),this,SLOT(removeImage2()),Qt::UniqueConnection);
	connect(image3RemoveBtn,SIGNAL(clicked()),this,SLOT(removeImage3()),Qt::UniqueConnection);
    connect(image4RemoveBtn,SIGNAL(clicked()),this,SLOT(removeImage4()),Qt::UniqueConnection);
    connect(image5RemoveBtn,SIGNAL(clicked()),this,SLOT(removeImage5()),Qt::UniqueConnection);

	connect(thread, SIGNAL(updateTree(QTreeWidgetItem*,QStringList*)),this,SLOT(onUpdateTree(QTreeWidgetItem*,QStringList*)),Qt::UniqueConnection);
	connect(thread, SIGNAL(updateProgressBar(int)),this,SLOT(onUpdateProgressBar(int)),Qt::UniqueConnection);
}

void ImagePage::setImage1()
{
	if(treeWidget->currentItem())
	{
		if (treeWidget->currentItem()->text(6)=="DICOM")
			image1LineEdit->setText(treeWidget->currentItem()->text(2));
		else
			image1LineEdit->setText(treeWidget->currentItem()->text(0));

		image1SpinBox->setValue(treeWidget->indexOfTopLevelItem ( treeWidget->currentItem()));
	}
}

void ImagePage::setImage2()
{
	if(treeWidget->currentItem())
	{
		if (treeWidget->currentItem()->text(6)=="DICOM")
			image2LineEdit->setText(treeWidget->currentItem()->text(2));
		else
			image2LineEdit->setText(treeWidget->currentItem()->text(0));
		
		image2SpinBox->setValue(treeWidget->indexOfTopLevelItem ( treeWidget->currentItem()));
	}
}

void ImagePage::setImage3()
{	
	if(treeWidget->currentItem())
	{
		if (treeWidget->currentItem()->text(6)=="DICOM")
			image3LineEdit->setText(treeWidget->currentItem()->text(2));
		else
			image3LineEdit->setText(treeWidget->currentItem()->text(0));
		
		image3SpinBox->setValue(treeWidget->indexOfTopLevelItem ( treeWidget->currentItem()));
	}
}

void ImagePage::setImage4()
{
    if(treeWidget->currentItem())
    {
        if (treeWidget->currentItem()->text(6)=="DICOM")
            image4LineEdit->setText(treeWidget->currentItem()->text(2));
        else
            image4LineEdit->setText(treeWidget->currentItem()->text(0));
    
        image4SpinBox->setValue(treeWidget->indexOfTopLevelItem ( treeWidget->currentItem()));
    }
}

void ImagePage::setImage5()
{
    if(treeWidget->currentItem())
    {
        if (treeWidget->currentItem()->text(6)=="DICOM")
            image5LineEdit->setText(treeWidget->currentItem()->text(2));
        else
            image5LineEdit->setText(treeWidget->currentItem()->text(0));
    
        image5SpinBox->setValue(treeWidget->indexOfTopLevelItem ( treeWidget->currentItem()));
    }
}

void ImagePage::removeImage1()
{
	image1LineEdit->setText("");
	image1SpinBox->setValue(-1);
}

void ImagePage::removeImage2()
{
	image2LineEdit->setText("");
	image2SpinBox->setValue(-1);
}

void ImagePage::removeImage3()
{
	image3LineEdit->setText("");
	image3SpinBox->setValue(-1);
}

void ImagePage::removeImage4()
{
    image4LineEdit->setText("");
    image4SpinBox->setValue(-1);
}

void ImagePage::removeImage5()
{
    image5LineEdit->setText("");
    image5SpinBox->setValue(-1);
}

bool ImagePage::validatePage()
{
	if (image1SpinBox->value() < 0 && image2SpinBox->value() < 0 && image3SpinBox->value() < 0 && image4SpinBox->value() < 0 && image5SpinBox->value() < 0)
	{
		QMessageBox msgBox;
		QString text = "Please select one CT or MRI image.";
		msgBox.setText(text);
		msgBox.exec();
		return false;
	}

	return true;
}

void ImagePage::initializePage()
{
	//Find Images	
	if (lastDirectory != field("directory.path").toString())
	{		
		lastDirectory = field("directory.path").toString();

		disconnect(thread, SIGNAL(updateTree(QTreeWidgetItem*,QStringList*)),this,SLOT(onUpdateTree(QTreeWidgetItem*,QStringList*)));
		disconnect(thread, SIGNAL(updateProgressBar(int)),this,SLOT(onUpdateProgressBar(int)));
		
		treeWidget->clear();
		((RegistrationWizard*)(wizard()))->fileNameList->clear();
		image1LineEdit->setText("");
		image2LineEdit->setText("");
		image3LineEdit->setText("");
        image4LineEdit->setText("");
		image5LineEdit->setText("");
		progressBar->setValue(0);
		progressBar->setRange(0,100);

		while (thread->isRunning())
		{
			thread->forceStop();
			thread->wait(500);
		}

		connect(thread, SIGNAL(updateTree(QTreeWidgetItem*,QStringList*)),this,SLOT(onUpdateTree(QTreeWidgetItem*,QStringList*)),Qt::UniqueConnection);
		connect(thread, SIGNAL(updateProgressBar(int)),this,SLOT(onUpdateProgressBar(int)),Qt::UniqueConnection);
		thread->setDirectory(field("directory.path").toString());
		thread->start();
	}
}

int ImagePage::nextId() const
{
	return RegistrationWizard::Page_Conclusion;
}

void ImagePage::onUpdateTree(QTreeWidgetItem* item,QStringList* list)
{	
	treeWidget->addTopLevelItem(item);

	((RegistrationWizard*)(wizard()))->fileNameList->append(list);

}

void ImagePage::onUpdateProgressBar(int value)
{	
	progressBar->setValue(value);
}

ConclusionPage::ConclusionPage(QWidget *parent)
    : QWizardPage(parent)
{
    setTitle(tr("Image Properties"));
	
	QString text = tr("Please check the image header information.");
    bottomLabel = new QLabel;
    bottomLabel->setWordWrap(true);
	bottomLabel->setText(text);
 
	tabWidget	 = new QTabWidget;
	table1Widget = new QTableWidget;
	table2Widget = new QTableWidget;
	table3Widget = new QTableWidget;
    table4Widget = new QTableWidget;
	table5Widget = new QTableWidget;

	table1Widget->setColumnCount(2);
	table2Widget->setColumnCount(2);
	table3Widget->setColumnCount(2);
    table4Widget->setColumnCount(2);
    table5Widget->setColumnCount(2);

	table1Widget->horizontalHeader()->setStretchLastSection(true);
	table2Widget->horizontalHeader()->setStretchLastSection(true);
	table3Widget->horizontalHeader()->setStretchLastSection(true);
    table4Widget->horizontalHeader()->setStretchLastSection(true);
	table5Widget->horizontalHeader()->setStretchLastSection(true);

	table1Widget->verticalHeader()->setVisible(false);
	table2Widget->verticalHeader()->setVisible(false);
	table3Widget->verticalHeader()->setVisible(false);
    table4Widget->verticalHeader()->setVisible(false);
	table5Widget->verticalHeader()->setVisible(false);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(tabWidget);
    //layout->addWidget(groupBox);
	layout->addWidget(bottomLabel);
    setLayout(layout);
}

int ConclusionPage::nextId() const
{
    return -1;
}

void ConclusionPage::initializePage()
{
	tabWidget->clear();
	table1Widget->clear();
	table2Widget->clear();
	table3Widget->clear();
    table4Widget->clear();
    table5Widget->clear();

	QStringList headerList;
	headerList<<"Tag"<<"Description";

	table1Widget->setHorizontalHeaderLabels(headerList);
	table2Widget->setHorizontalHeaderLabels(headerList);
	table3Widget->setHorizontalHeaderLabels(headerList);
    table4Widget->setHorizontalHeaderLabels(headerList);
	table5Widget->setHorizontalHeaderLabels(headerList);

	if (field("image.set1").toString()!="")
	{
		setTable1();
		tabWidget->addTab(table1Widget,field("image.set1").toString());
	}

	if (field("image.set2").toString()!="")
	{
		setTable2();
		tabWidget->addTab(table2Widget,field("image.set2").toString());
	}

	if (field("image.set3").toString()!="")
	{
		setTable3();
		tabWidget->addTab(table3Widget,field("image.set3").toString());
	}
    if (field("image.set4").toString()!="")
    {
        setTable4();
        tabWidget->addTab(table4Widget,field("image.set4").toString());
    }
	if (field("image.set5").toString()!="")
    {
        setTable5();
        tabWidget->addTab(table5Widget,field("image.set5").toString());
    }

}

void ConclusionPage::setVisible(bool visible)
{
    QWizardPage::setVisible(visible);
}

void ConclusionPage::setTable1()
{
	int pos = field("image.list1").toInt();
	QString path  = ((RegistrationWizard*)(wizard()))->fileNameList->at(pos)->at(0); //Use first DICOM file in the series
	//Dont process Nifti header file
	if (path.contains(".nii"))
		return;

	//Read Header file
	ReaderType::Pointer reader = ReaderType::New();
	ImageIOType::Pointer dicomIO = ImageIOType::New();
	reader->SetImageIO( dicomIO );
	reader->SetFileName( path.toStdString() ); 
	reader->Update();

	typedef itk::MetaDataDictionary   DictionaryType;
	const  DictionaryType & dictionary = dicomIO->GetMetaDataDictionary();
	typedef itk::MetaDataObject< std::string > MetaDataStringType;
	DictionaryType::ConstIterator itr = dictionary.Begin();
	DictionaryType::ConstIterator end = dictionary.End();

	//Display in table
	int tableRow=0;
	while( itr != end )
	{
		itk::MetaDataObjectBase::Pointer  entry = itr->second;

		MetaDataStringType::Pointer entryvalue =
			dynamic_cast<MetaDataStringType *>( entry.GetPointer() );

		if( entryvalue )
		{			
			table1Widget->setRowCount(tableRow+1);

			std::string tagkey   = itr->first;
			std::string labelId;
			bool found =  itk::GDCMImageIO::GetLabelFromTag( tagkey, labelId );

			QTableWidgetItem *tagKeyItem = new QTableWidgetItem();

			if(found)
				tagKeyItem->setText(QString::fromStdString(labelId));
			else
				tagKeyItem->setText(QString::fromStdString(tagkey));

			QTableWidgetItem *tagValueItem = new QTableWidgetItem(QString::fromStdString(entryvalue->GetMetaDataObjectValue()));

			table1Widget->setItem(tableRow,0, tagKeyItem);
			table1Widget->setItem(tableRow,1, tagValueItem);
			tableRow++;
		}

		++itr;
	}

}

void ConclusionPage::setTable2()
{
	int pos = field("image.list2").toInt();
	QString path  = ((RegistrationWizard*)(wizard()))->fileNameList->at(pos)->at(0);

	//Dont process Nifti header file
	if (path.contains(".nii"))
		return;

	//Read Header file
	ReaderType::Pointer reader = ReaderType::New();
	ImageIOType::Pointer dicomIO = ImageIOType::New();
	reader->SetImageIO( dicomIO );
	reader->SetFileName( path.toStdString() );  //Use first dicom file in the series
	reader->Update();

	typedef itk::MetaDataDictionary   DictionaryType;
	const  DictionaryType & dictionary = dicomIO->GetMetaDataDictionary();
	typedef itk::MetaDataObject< std::string > MetaDataStringType;
	DictionaryType::ConstIterator itr = dictionary.Begin();
	DictionaryType::ConstIterator end = dictionary.End();

	//Display in table
	int tableRow=0;
	while( itr != end )
	{
		itk::MetaDataObjectBase::Pointer  entry = itr->second;

		MetaDataStringType::Pointer entryvalue =
			dynamic_cast<MetaDataStringType *>( entry.GetPointer() );

		if( entryvalue )
		{			
			table2Widget->setRowCount(tableRow+1);

			std::string tagkey   = itr->first;
			std::string labelId;
			bool found =  itk::GDCMImageIO::GetLabelFromTag( tagkey, labelId );

			QTableWidgetItem *tagKeyItem = new QTableWidgetItem();

			if(found)
				tagKeyItem->setText(QString::fromStdString(labelId));
			else
				tagKeyItem->setText(QString::fromStdString(tagkey));

			QTableWidgetItem *tagValueItem = new QTableWidgetItem(QString::fromStdString(entryvalue->GetMetaDataObjectValue()));

			table2Widget->setItem(tableRow,0, tagKeyItem);
			table2Widget->setItem(tableRow,1, tagValueItem);
			tableRow++;
		}

		++itr;
	}
}

void ConclusionPage::setTable3()
{
	int pos = field("image.list3").toInt();
	QString path  = ((RegistrationWizard*)(wizard()))->fileNameList->at(pos)->at(0);
	//Dont process Nifti header file
	if (path.contains(".nii"))
		return;

	//Read Header file
	ReaderType::Pointer reader = ReaderType::New();
	ImageIOType::Pointer dicomIO = ImageIOType::New();
	reader->SetImageIO( dicomIO );
	reader->SetFileName( path.toStdString() );  //Use first dicom file in the series
	reader->Update();

	typedef itk::MetaDataDictionary   DictionaryType;
	const  DictionaryType & dictionary = dicomIO->GetMetaDataDictionary();
	typedef itk::MetaDataObject< std::string > MetaDataStringType;
	DictionaryType::ConstIterator itr = dictionary.Begin();
	DictionaryType::ConstIterator end = dictionary.End();

	//Display in table
	int tableRow=0;
	while( itr != end )
	{
		itk::MetaDataObjectBase::Pointer  entry = itr->second;

		MetaDataStringType::Pointer entryvalue =
			dynamic_cast<MetaDataStringType *>( entry.GetPointer() );

		if( entryvalue )
		{			
			table3Widget->setRowCount(tableRow+1);

			std::string tagkey   = itr->first;
			std::string labelId;
			bool found =  itk::GDCMImageIO::GetLabelFromTag( tagkey, labelId );

			QTableWidgetItem *tagKeyItem = new QTableWidgetItem();

			if(found)
				tagKeyItem->setText(QString::fromStdString(labelId));
			else
				tagKeyItem->setText(QString::fromStdString(tagkey));

			QTableWidgetItem *tagValueItem = new QTableWidgetItem(QString::fromStdString(entryvalue->GetMetaDataObjectValue()));

			table3Widget->setItem(tableRow,0, tagKeyItem);
			table3Widget->setItem(tableRow,1, tagValueItem);
			tableRow++;
		}

		++itr;
	}
}
void ConclusionPage::setTable4()
{
    int pos = field("image.list4").toInt();
    QString path  = ((RegistrationWizard*)(wizard()))->fileNameList->at(pos)->at(0);
    
    //Dont process Nifti header file
    if (path.contains(".nii"))
        return;
    
    //Read Header file
    ReaderType::Pointer reader = ReaderType::New();
    ImageIOType::Pointer dicomIO = ImageIOType::New();
    reader->SetImageIO( dicomIO );
    reader->SetFileName( path.toStdString() );  //Use first dicom file in the series
    reader->Update();
    
    typedef itk::MetaDataDictionary   DictionaryType;
    const  DictionaryType & dictionary = dicomIO->GetMetaDataDictionary();
    typedef itk::MetaDataObject< std::string > MetaDataStringType;
    DictionaryType::ConstIterator itr = dictionary.Begin();
    DictionaryType::ConstIterator end = dictionary.End();
    
    //Display in table
    int tableRow=0;
    while( itr != end )
    {
        itk::MetaDataObjectBase::Pointer  entry = itr->second;
        
        MetaDataStringType::Pointer entryvalue =
        dynamic_cast<MetaDataStringType *>( entry.GetPointer() );
        
        if( entryvalue )
        {
            table3Widget->setRowCount(tableRow+1);
            
            std::string tagkey   = itr->first;
            std::string labelId;
            bool found =  itk::GDCMImageIO::GetLabelFromTag( tagkey, labelId );
            
            QTableWidgetItem *tagKeyItem = new QTableWidgetItem();
            
            if(found)
                tagKeyItem->setText(QString::fromStdString(labelId));
            else
                tagKeyItem->setText(QString::fromStdString(tagkey));
            
            QTableWidgetItem *tagValueItem = new QTableWidgetItem(QString::fromStdString(entryvalue->GetMetaDataObjectValue()));
            
            table4Widget->setItem(tableRow,0, tagKeyItem);
            table4Widget->setItem(tableRow,1, tagValueItem);
            tableRow++;
        }
        
        ++itr;
    }
}

void ConclusionPage::setTable5()
{
    int pos = field("image.list5").toInt();
    QString path  = ((RegistrationWizard*)(wizard()))->fileNameList->at(pos)->at(0);
    
    //Dont process Nifti header file
    if (path.contains(".nii"))
        return;
    
    //Read Header file
    ReaderType::Pointer reader = ReaderType::New();
    ImageIOType::Pointer dicomIO = ImageIOType::New();
    reader->SetImageIO( dicomIO );
    reader->SetFileName( path.toStdString() );  //Use first dicom file in the series
    reader->Update();
    
    typedef itk::MetaDataDictionary   DictionaryType;
    const  DictionaryType & dictionary = dicomIO->GetMetaDataDictionary();
    typedef itk::MetaDataObject< std::string > MetaDataStringType;
    DictionaryType::ConstIterator itr = dictionary.Begin();
    DictionaryType::ConstIterator end = dictionary.End();
    
    //Display in table
    int tableRow=0;
    while( itr != end )
    {
        itk::MetaDataObjectBase::Pointer  entry = itr->second;
        
        MetaDataStringType::Pointer entryvalue =
        dynamic_cast<MetaDataStringType *>( entry.GetPointer() );
        
        if( entryvalue )
        {
            table3Widget->setRowCount(tableRow+1);
            
            std::string tagkey   = itr->first;
            std::string labelId;
            bool found =  itk::GDCMImageIO::GetLabelFromTag( tagkey, labelId );
            
            QTableWidgetItem *tagKeyItem = new QTableWidgetItem();
            
            if(found)
                tagKeyItem->setText(QString::fromStdString(labelId));
            else
                tagKeyItem->setText(QString::fromStdString(tagkey));
            
            QTableWidgetItem *tagValueItem = new QTableWidgetItem(QString::fromStdString(entryvalue->GetMetaDataObjectValue()));
            
            table5Widget->setItem(tableRow,0, tagKeyItem);
            table5Widget->setItem(tableRow,1, tagValueItem);
            tableRow++;
        }
        
        ++itr;
    }
}

FindImageThread::FindImageThread(QObject *parent) :
QThread(parent)
{
	m_stop = false;
}

void FindImageThread::setDirectory(QString dir)
{
	m_dir = dir;
}

void FindImageThread::run()
{
	QMutex mutex;
	mutex.lock();

	m_stop = false;

	if(this->m_stop)
		return;

	QStringList strlstAllDir;
	strlstAllDir.append(m_dir);
	QDirIterator it(m_dir, QDir::Dirs|QDir::NoDotAndDotDot,QDirIterator::Subdirectories);

	while (it.hasNext())
	{
		strlstAllDir.append(it.next());
	}

	for (int i=0;i<strlstAllDir.size();i++)
	{
		if(this->m_stop)
			return;

		//Load DICOM data
		NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
		nameGenerator->SetUseSeriesDetails( true );
		//nameGenerator->AddSeriesRestriction("0008|0021" );
		nameGenerator->GlobalWarningDisplayOff();
		nameGenerator->SetDirectory(strlstAllDir.at(i).toStdString().c_str() );

		//Series number
		typedef std::vector<std::string> SeriesIdContainer;
		const SeriesIdContainer & seriesUID = nameGenerator->GetSeriesUIDs();

		for(int j=0;j<seriesUID.size();j++)
		{
			if(this->m_stop)
				return;

			QTreeWidgetItem *dicomItem = new QTreeWidgetItem;

			ReaderType::Pointer reader = ReaderType::New();
			ImageIOType::Pointer dicomIO = ImageIOType::New();
			reader->SetImageIO( dicomIO );

			typedef std::vector<std::string>    FileNamesContainer;
			FileNamesContainer fileNames = nameGenerator->GetFileNames(seriesUID[j]);

			reader->SetFileNames( fileNames );
			try
			{	
				reader->Update();
			}
			catch (itk::ExceptionObject& e)
			{
				std::cerr<<e;
			}

			typedef itk::MetaDataDictionary   DictionaryType;

			const  DictionaryType & dictionary = dicomIO->GetMetaDataDictionary();

			DictionaryType::ConstIterator itr = dictionary.Begin();
			DictionaryType::ConstIterator end = dictionary.End();

			typedef itk::MetaDataObject< std::string > MetaDataStringType;

			//Patient Name
			std::string patientNameTag = "0010|0010";
			std::string patientName = "/";

			DictionaryType::ConstIterator patientNameTagItr = dictionary.Find(patientNameTag);

			if(patientNameTagItr!=end)
			{
				MetaDataStringType::ConstPointer patientNameEntryvalue =
					dynamic_cast<const MetaDataStringType *>( patientNameTagItr->second.GetPointer() );

				patientName = patientNameEntryvalue->GetMetaDataObjectValue();	
			}

			//Date
			std::string dateTag = "0008|0020";
			std::string date = "/";

			DictionaryType::ConstIterator dateTagItr = dictionary.Find(dateTag);
			if(dateTagItr!=end)
			{
				MetaDataStringType::ConstPointer dateEntryvalue =
					dynamic_cast<const MetaDataStringType *>( dateTagItr->second.GetPointer() );

				date = dateEntryvalue->GetMetaDataObjectValue();
			}

			date = date.insert(4,"/");
			date = date.insert(7,"/");

			//Modality
			std::string modlaityTag = "0008|0060";
			std::string modality = "/";

			DictionaryType::ConstIterator modalityTagItr = dictionary.Find(modlaityTag);
			if(modalityTagItr!=end)
			{
				MetaDataStringType::ConstPointer modalityEntryvalue =
					dynamic_cast<const MetaDataStringType *>( modalityTagItr->second.GetPointer() );

				modality = modalityEntryvalue->GetMetaDataObjectValue();
			}

			//Sequence Name
			std::string seriesDescriptionTag = "0008|103e";
			std::string seriesDescription = "/";

			DictionaryType::ConstIterator seriesDescriptionTagItr = dictionary.Find(seriesDescriptionTag);

			if(seriesDescriptionTagItr!=end)
			{
				MetaDataStringType::ConstPointer seriesDescriptionEntryvalue =
					dynamic_cast<const MetaDataStringType *>( seriesDescriptionTagItr->second.GetPointer() );

				seriesDescription = seriesDescriptionEntryvalue->GetMetaDataObjectValue();
			}

			//Sequence Name
			std::string numberOfTemporalPositionsTag = "0020|0105";
			std::string numberOfTemporalPositions = "/";

			DictionaryType::ConstIterator numberOfTemporalPositionsTagItr = dictionary.Find(numberOfTemporalPositionsTag);

			if(numberOfTemporalPositionsTagItr!=end)
			{
				MetaDataStringType::ConstPointer numberOfTemporalPositionsEntryvalue =
					dynamic_cast<const MetaDataStringType *>( numberOfTemporalPositionsTagItr->second.GetPointer() );

				numberOfTemporalPositions = numberOfTemporalPositionsEntryvalue->GetMetaDataObjectValue();
			}

			//set Item information		
			dicomItem->setText(0,QString::fromStdString(patientName));
			dicomItem->setText(1,QString::fromStdString(modality));
			dicomItem->setText(2,QString::fromStdString(seriesDescription));			
			dicomItem->setText(3,QString::number(fileNames.size()));
			dicomItem->setText(4,QString::fromStdString(date));
			dicomItem->setText(5,seriesUID[j].c_str());
			dicomItem->setText(6,"DICOM");
			dicomItem->setText(7,strlstAllDir.at(i));

			QStringList* nameList = new QStringList;

			for (int k=0;k<fileNames.size();k++)
			{
				nameList->append(QString::fromStdString(fileNames.at(k)));
			}

			updateTree(dicomItem,nameList);
		}
		
		//Find Nifti file
		QStringList filterList;
		filterList<<"*.nii"<<"*.nii.gz";
		QDir dir(strlstAllDir.at(i));
		QFileInfoList niiFileList = dir.entryInfoList(filterList);
		
		for (int m=0; m<niiFileList.size();m++)
		{
			QTreeWidgetItem *niiItem = new QTreeWidgetItem;
			QFileInfo niiFile(niiFileList.at(m));
			niiItem->setText(0,niiFile.baseName());
			niiItem->setText(1,"/");
			niiItem->setText(2,"/");
			niiItem->setText(3,"/");
			niiItem->setText(4,"/");
			niiItem->setText(5,"/");
			niiItem->setText(6,"Nifti");
			niiItem->setText(7,strlstAllDir.at(i));
	
			QStringList* nameList = new QStringList;
			nameList->append(niiFileList.at(m).absoluteFilePath());

			updateTree(niiItem,nameList);
		}


		updateProgressBar(double(i+1)/double(strlstAllDir.size())*100);
	}

	mutex.unlock();
}

void FindImageThread::forceStop()
{
	m_stop = true;
}


