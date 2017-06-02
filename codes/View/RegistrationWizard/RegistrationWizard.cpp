#include "RegistrationWizard.h"
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
#include <QTabWidget>
#include <QTableWidget>
#include <QDebug>
#include <QStringList>
#include <QString>
#include <QTableWidgetItem>
#include <QFileInfo>
#include <qthreadpool.h>
#include <qheaderview.h>

#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkGDCMImageIO.h"



RegistrationWizard::RegistrationWizard(
	QString dir, int numOfImages, QWidget *parent)
	: m_numOfImages(numOfImages), QWizard(parent)
{
	directoryPage	= new DirectoryPage(dir, this);
	imagePage		= new ImagePage(numOfImages, this);
	conclusionPage	= new ConclusionPage(numOfImages, this);

    setPage(PAGE_DIRECTORY,  directoryPage);
    setPage(PAGE_IMAGE,		 imagePage );
    setPage(PAGE_CONCLUSION, conclusionPage);
	
    setStartId(PAGE_DIRECTORY);
    setWindowTitle(tr("  Registration Wizard  "));
	//setMinimumSize(800, 800);
	setOption(HaveHelpButton,	true);
	setOption(NoCancelButton,	true);
	setOption(IndependentPages,	false);


    connect(this, SIGNAL(helpRequested()), this, SLOT(showHelp()));

}

RegistrationWizard::RegistrationWizard(int numOfImages, QWidget * parent)
	:RegistrationWizard(QString(), numOfImages, parent)
{
}

RegistrationWizard::RegistrationWizard(QWidget * parent)
	: RegistrationWizard(QString(), 2, parent)
{
}

RegistrationWizard::~RegistrationWizard()
{
	// clear the m_fileNamesList
	//m_fileNamesList.clear();
}

void RegistrationWizard::setImageModalityNames(unsigned int i, QString imageModalityName)
{
	imagePage->setImageModalityNames(i, imageModalityName);
}

void RegistrationWizard::showHelp()
{
    QString message;

    switch (currentId()) {
    case PAGE_DIRECTORY:
        message = tr("Select the image directory");
        break;
    case PAGE_IMAGE:
        message = tr("Select the image series");
        break;
    case PAGE_CONCLUSION:
        message = tr("Confirm the image information");
        break;
    default:
        message = tr("");
    }

    QMessageBox::information(this, tr("Registration Wizard Help"), message);

}

QString RegistrationWizard::getFileNames(unsigned int i)
{
	if (i < 0 || i >= m_numOfImages) {
		return QString();
	}
	else {
		QString fieldImagePath = "image.path" + QString::number(i);
		return field(fieldImagePath).toString();
	}
}

void RegistrationWizard::setDirectory(QString directory)
{
	setField("directory.path", directory);
}

const QString RegistrationWizard::getDirectory()
{
	return field("directory.path").toString();
}

int RegistrationWizard::getNumberOfImages()
{
	return m_numOfImages;
}
DirectoryPage::DirectoryPage(QString dir, QWidget *parent)
	:QWizardPage(parent)
{
	setTitle(tr("Set Image Directory"));
	setSubTitle(tr(""));

	m_imageDirectoryLineEdit = new QLineEdit(dir);
	registerField("directory.path", m_imageDirectoryLineEdit);
	setField("directory.path", dir);

	m_imageDirectoryLabel = new QLabel(tr("&Image Directory:"), this);
	m_imageDirectoryLabel->setBuddy(m_imageDirectoryLineEdit);

	browseBtn = new QPushButton("Browse", this);


	QHBoxLayout* hBoxLayout = new QHBoxLayout(this);
	hBoxLayout->addWidget(m_imageDirectoryLabel);
	hBoxLayout->addWidget(m_imageDirectoryLineEdit);
	hBoxLayout->addWidget(browseBtn);
	setLayout(hBoxLayout);

	connect(browseBtn, SIGNAL(clicked()), this, SLOT(browse()));
}

DirectoryPage::DirectoryPage(QWidget *parent)
	: DirectoryPage(QString(), parent)
{
}

void DirectoryPage::browse()
{
	QDir dir = QDir(m_imageDirectoryLineEdit->text());
	if (!dir.exists()) {
		dir = QDir();
	}

	//Dialog Open Directory
	QString path = QFileDialog::getExistingDirectory(this,
		tr("Open Image Directory"), dir.absolutePath(), 
		QFileDialog::ShowDirsOnly);
	path.replace("\\","/");
	m_imageDirectoryLineEdit->setText(path);
}

int DirectoryPage::nextId() const
{
    return RegistrationWizard::PAGE_IMAGE;
}

ImagePage::ImagePage(int numOfImages, QWidget *parent)
	: QWizardPage(parent)
{
	setTitle(tr("Select image series"));

	//Tree
	QStringList headerList;

	headerList << "Patient Name" << "Modality" << "Series Description" <<
		"Slice" << "Date" << "Series UID" << "Format" << "Path";

	treeWidget = new QTreeWidget(this);
	treeWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	treeWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	treeWidget->setHeaderLabels(headerList);
	treeWidget->setMinimumSize(1000, 200);

	progressBar = new QProgressBar(this);
	progressBar->setTextVisible(true);
	progressBar->setStyleSheet("QProgressBar {text-align: center;}");


	QVBoxLayout* vBoxLayout = new QVBoxLayout(this);
	vBoxLayout->addWidget(treeWidget);
	vBoxLayout->addSpacerItem(new QSpacerItem(50, 1));
	vBoxLayout->addWidget(progressBar);
	setLayout(vBoxLayout);

	// initialization 
	for (int i = 0; i < numOfImages; ++i) {
		m_imageLineEdits << new QLineEdit(this);
		m_imageLineEdits[i]->setReadOnly(true);

		m_imageLineEdits2 << new QLineEdit(this);
		m_imageLineEdits2[i]->setVisible(false);

		m_imageLabels << new QLabel(this);
		m_imageLabels[i]->setBuddy(m_imageLineEdits[i]);
		

		m_imageSetBtns <<new QPushButton("<<", this);
		m_imageSetBtns[i]->setFixedSize(30, 30);

		m_imageRemoveBtns << new QPushButton("X", this);
		m_imageRemoveBtns[i]->setFixedSize(30, 30);


		QHBoxLayout* hBoxLayout = new QHBoxLayout();
		hBoxLayout->addWidget(m_imageLabels[i]);
		hBoxLayout->addWidget(m_imageLineEdits[i]);
		hBoxLayout->addWidget(m_imageSetBtns[i]);
		hBoxLayout->addWidget(m_imageRemoveBtns[i]);
		vBoxLayout->addLayout(hBoxLayout);

		if (i == 0) {
			//* mean compulsory , otherwise optional 
			registerField("image.set" + QString::number(i) + "*", m_imageLineEdits[i]);
			registerField("image.path" + QString::number(i) + "*", m_imageLineEdits2[i]);
		}
		else {
			registerField("image.set" + QString::number(i), m_imageLineEdits[i]);
			registerField("image.path" + QString::number(i), m_imageLineEdits2[i]);
		}

		connect(m_imageSetBtns[i], SIGNAL(clicked()), this, SLOT(setImages()), Qt::UniqueConnection);
		connect(m_imageRemoveBtns[i], SIGNAL(clicked()), this, SLOT(removeImages()), Qt::UniqueConnection);

	}
	m_threadPool = new QThreadPool(this);
	m_threadPool->setExpiryTimeout(0);

}

ImagePage::ImagePage(QWidget *parent)
	:ImagePage(2, parent)
{
    
}

ImagePage::~ImagePage()
{
	emit finishPrevious();
}

void ImagePage::setImageModalityNames(unsigned int i, QString imageModalityName)
{
	if (i < 0 || i >= m_imageLabels.size())
		return;
	else
		m_imageLabels[i]->setText(imageModalityName);
}

bool ImagePage::validatePage()
{
	for (int i = 0; i < m_imageLineEdits.size(); ++i) {
		if (!m_imageLineEdits[i]->text().isEmpty())
			break;
		else if (i == m_imageLineEdits.size()-1) {
			QMessageBox::critical(this, "No image was selected. ", 
				"Please select at least one image.");
			return false;
		}
	}
	return true;
}

void ImagePage::initializePage()
{
	if (m_lastDirectory == field("directory.path").toString()) {
		return;
	}
	m_lastDirectory = field("directory.path").toString();
	//Find Images	
	QStringList allDir;
	allDir << (field("directory.path").toString());
	QDirIterator it(field("directory.path").toString(),
		QDir::Dirs | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
	while (it.hasNext())	
		allDir << it.next();
	// force finish all FindImageRunnable
	emit finishPrevious();
	// setup progress bar
	progressBar->setRange(0, allDir.size());
	progressBar->setValue(0);
	// clean tree widget
	treeWidget->clear();

	for (int i = 0; i < allDir.size(); i++)
	{
		FindImageRunnable* findImage = new FindImageRunnable(allDir[i]);
		m_threadPool->start(findImage);
		connect(findImage, SIGNAL(finish()),
			this, SLOT(updateProgressBar()));
		connect(findImage, SIGNAL(addTreeWidgetItem(QString)),
			this, SLOT(updateTreeWidget(QString)));
		connect(this, SIGNAL(finishPrevious()),
			findImage, SLOT(forceFinish()));
	}
}

int ImagePage::nextId() const
{
	return RegistrationWizard::PAGE_CONCLUSION;
}

void ImagePage::updateProgressBar()
{	
	progressBar->setValue(progressBar->value() + 1);
}

void ImagePage::setImages()
{
	QPushButton* setButton = qobject_cast<QPushButton*>(sender());
	if (setButton == nullptr) 
		return;

	int index = m_imageSetBtns.indexOf(setButton);
	if (index < 0 || !treeWidget->currentItem())
		return;
	else {
		QTreeWidgetItem* treeWidgetItem = treeWidget->currentItem();
		QString text(
			"NAME: " + treeWidgetItem->text(0) + 
			"	DESCRIPTION: " + treeWidgetItem->text(2));
		m_imageLineEdits[index]->setText(text);
		m_imageLineEdits2[index]->setText(treeWidgetItem->text(7));
		//(*m_selectedImages)[index] = treeWidget->indexOfTopLevelItem(treeWidgetItem);
	}

}

void ImagePage::removeImages()
{
	QPushButton* setButton = qobject_cast<QPushButton*>(sender());
	if (setButton == nullptr)
		return;

	int index = m_imageRemoveBtns.indexOf(setButton);
	if (index < 0)
		return;
	else {
		m_imageLineEdits[index]->clear();
		m_imageLineEdits2[index]->clear();
		//(*m_selectedImages)[index] = -1;
	}


}

void ImagePage::updateTreeWidget(QString item)
{
	QTreeWidgetItem* treeWidgetItem = new QTreeWidgetItem(treeWidget);
	QStringList itemList = item.split('\t');
	treeWidgetItem->setText(0, itemList[0]);
	treeWidgetItem->setText(1, itemList[1]);
	treeWidgetItem->setText(2, itemList[2]);
	treeWidgetItem->setText(3, itemList[3]);
	treeWidgetItem->setText(4, itemList[4]);
	treeWidgetItem->setText(5, itemList[5]);
	treeWidgetItem->setText(6, itemList[6]);
	treeWidgetItem->setText(7, itemList[7]);


	treeWidget->addTopLevelItem(treeWidgetItem);
}

ConclusionPage::ConclusionPage(int numOfImages, QWidget *parent)
	: QWizardPage(parent)
{
	for (int i = 0; i < numOfImages; ++i) {
		m_tableWidgets << nullptr;
	}

	setTitle(tr("Image Properties"));

	bottomLabel = new QLabel;
	bottomLabel->setWordWrap(true);
	bottomLabel->setText(tr("Please check the image header information."));

	tabWidget = new QTabWidget(this);


	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(tabWidget);
	layout->addWidget(bottomLabel);
	setLayout(layout);
}


ConclusionPage::ConclusionPage(QWidget *parent)
    : ConclusionPage(2, parent)
{

}

int ConclusionPage::nextId() const
{
    return -1;
}

void ConclusionPage::initializePage()
{
	using itk::GDCMImageIO;
	using itk::MetaDataDictionary;
	using std::string;
	typedef itk::ImageFileReader<itk::Image<unsigned short, 2>> ImageFileReader;
	using itk::MetaDataObjectBase;
	typedef itk::MetaDataObject<string> MetaDataString;


	tabWidget->clear();
	

	for (int i = 0; i < m_tableWidgets.size(); ++i) {

		QString fieldStringImageSet;
		fieldStringImageSet += "image.set";
		fieldStringImageSet += QString::number(i);

		QString imageSet = field(fieldStringImageSet).toString();

		if (imageSet.isEmpty()) {
			continue;
		}

		if (m_tableWidgets[i] == nullptr) {
			m_tableWidgets[i] = new QTableWidget(this);
		}
		tabWidget->addTab(m_tableWidgets[i], imageSet);
		//m_tableWidgets[i]->clear();
		m_tableWidgets[i]->setEditTriggers(QTableWidget::NoEditTriggers);
		m_tableWidgets[i]->setColumnCount(3);
		m_tableWidgets[i]->setRowCount(0);
		m_tableWidgets[i]->setHorizontalHeaderLabels(QStringList()
			<< "Tag" << "Label" << "Description");
		m_tableWidgets[i]->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
		m_tableWidgets[i]->verticalHeader()->setVisible(false);

		try {
			GDCMImageIO::Pointer dicomIO = GDCMImageIO::New();
			ImageFileReader::Pointer reader = ImageFileReader::New();

			QString fieldStringImagePath;
			fieldStringImagePath += "image.path";
			fieldStringImagePath += QString::number(i);

			QStringList imagePaths = field(fieldStringImagePath).toString().split(';');
			if (imagePaths[0].isEmpty()) {
				continue;
			}

			reader->SetFileName(imagePaths[0].toStdString());
			qDebug() << imagePaths;
			reader->SetImageIO(dicomIO);
			reader->Update();

			const MetaDataDictionary& dictionary = dicomIO->GetMetaDataDictionary();
			for (MetaDataDictionary::ConstIterator cit = dictionary.Begin();
				cit != dictionary.End(); ++cit) {

				MetaDataObjectBase::Pointer entry = cit->second;
				MetaDataString::Pointer entryValue = static_cast<MetaDataString*>(
					entry.GetPointer());

				if (entryValue) {
					m_tableWidgets[i]->insertRow(m_tableWidgets[i]->rowCount());
					string tag = cit->first;
					string label;
					string value = entryValue->GetMetaDataObjectValue();
					m_tableWidgets[i]->setItem(m_tableWidgets[i]->rowCount() - 1, 2,
						new QTableWidgetItem(QString::fromStdString(value)));
					m_tableWidgets[i]->setItem(m_tableWidgets[i]->rowCount() - 1, 0,
						new QTableWidgetItem(QString::fromStdString(tag)));
					if (GDCMImageIO::GetLabelFromTag(tag, label)) {
						m_tableWidgets[i]->setItem(m_tableWidgets[i]->rowCount() - 1, 1,
							new QTableWidgetItem(QString::fromStdString(label)));
					}
					else {
						m_tableWidgets[i]->setItem(m_tableWidgets[i]->rowCount() - 1, 1,
							new QTableWidgetItem(tr("Unknown")));
					}

				}
			}
		}
		catch (itk::ExceptionObject& e) {
			qDebug() << "Inputs are not DICOM images.";
			qDebug() << e.what();
		}

		//if (!(*m_DICOMHeaders)[i].isNull()) {
		//	int tableRow = 0;
		//	m_tableWidgets[i]->setRowCount((*m_DICOMHeaders).size());
		//	m_tableWidgets[i]->setColumnCount(2);
		//	for (QMap<QString, QString>::const_iterator cit = (*m_DICOMHeaders)[i]->constBegin();
		//		cit != (*m_DICOMHeaders)[i]->constEnd(); ++cit) {

		//		qDebug() << cit.key();
		//		qDebug() << cit.value();
		//		m_tableWidgets[i]->setItem(tableRow, 0, new QTableWidgetItem(cit.key()));
		//		m_tableWidgets[i]->setItem(tableRow, 1, new QTableWidgetItem(cit.value()));
		//		++tableRow;
		//	}
		//}

	}

}

FindImageRunnable::FindImageRunnable(QString folderPath, QObject* parent)
	:m_folderPath(folderPath), QObject(parent)
{
	setAutoDelete(true);
}

void FindImageRunnable::run()
{
	using std::vector;
	using std::string;
	using itk::GDCMSeriesFileNames;
	if (m_forceFinish) {
		emit finish();
		return;
	}
	// Find Dicom files
	{
		// Generate a sequence of fileNames from a DICOM series
		GDCMSeriesFileNames::Pointer nameGenerator = GDCMSeriesFileNames::New();
		nameGenerator->SetUseSeriesDetails(true);
		nameGenerator->SetDirectory(m_folderPath.toStdString().c_str());
		//nameGenerator->AddSeriesRestriction("0008|0021");
		nameGenerator->GetInputFileNames();

		// basing on all seriesUIDs
		// get file names form corresponding serires UID
		try {
			const vector<string>& seriesUID = nameGenerator->GetSeriesUIDs();

			vector<string>::const_iterator seriesEnd = seriesUID.end();
			for (vector<string>::const_iterator cit = seriesUID.cbegin();
				cit != seriesUID.cend(); ++cit) {

				GDCImageIORead(nameGenerator->GetFileNames(*cit));

			}
		}
		catch (itk::ExceptionObject& e) {
			qDebug() << e.what();
		}

	}

	//Find Nifti file
	{
		QStringList filterList;
		filterList << "*.nii" << "*.nii.gz";
		QDir dir(m_folderPath);
		QFileInfoList niiFileList = dir.entryInfoList(filterList);

		// basing on wildcard to find out all NIFTI files
		for (QFileInfoList::const_iterator cit = niiFileList.cbegin();
			cit != niiFileList.cend(); ++cit)
		{
			vector<string> fileNames(1, cit->absoluteFilePath().toStdString());
			GDCImageIORead(fileNames);
		}
	}
	emit finish();
}

void FindImageRunnable::forceFinish()
{
	m_forceFinish = true;
	disconnect(0, 0, 0);
}

void FindImageRunnable::GDCImageIORead(const std::vector<std::string>& files)
{
	using std::string;
	using std::vector;
	using itk::GDCMImageIO;
	typedef itk::ImageFileReader<itk::Image<unsigned short, 2>> ImageFileReader;

	string _file = files[0];
	string patientName = "/";
	string modality = "/";
	string seriesDescription = "/";
	string slice = "/";
	string date = "/";
	string seriesUID = "/";
	string format = "/";

	try {
		GDCMImageIO::Pointer dicomIO = GDCMImageIO::New();

		ImageFileReader::Pointer imageFilerReader = ImageFileReader::New();
		imageFilerReader->SetNumberOfThreads(4);
		imageFilerReader->SetImageIO(dicomIO);
		imageFilerReader->SetFileName(_file);
		imageFilerReader->Update();

		if (!dicomIO->GetValueFromTag(("0010|0010"), patientName)) {
			//patientName = "/";
		}
		if (!dicomIO->GetValueFromTag("0008|0060", modality)) {
			//modality = "/";
		}
		if (!dicomIO->GetValueFromTag("0008|103e", seriesDescription)) {
			//seriesDescription = "/";
		}

		if (!dicomIO->GetValueFromTag("0020|1002", slice)) {
			//slice = "/";
			slice = std::to_string(files.size());
		}
		if (!dicomIO->GetValueFromTag("0008|0020", date)) {
			//date = "/";
		}
		if (!dicomIO->GetValueFromTag("0020|0052", seriesUID)) {
			//seriesUID = "/";
		}
	}
	catch (itk::ExceptionObject& e) {
		patientName = QFileInfo(QString::fromStdString(_file)).baseName().toStdString();
		qDebug() << "Input are not DICOM images.";
		qDebug() << "Input are assumed to be NIFTI.";
		qDebug() << e.what();

	}
	
	QString _emitString;
	_emitString += QString::fromStdString(patientName) + '\t';
	_emitString += QString::fromStdString(modality) + '\t';
	_emitString += QString::fromStdString(seriesDescription) + '\t';
	_emitString += QString::fromStdString(slice) + '\t';
	_emitString += QString::fromStdString(date) + '\t';
	_emitString += QString::fromStdString(seriesUID) + '\t';
	_emitString += QString::fromStdString(format) + '\t';
	for (vector<string>::const_iterator cit = files.cbegin();
		cit != files.cend(); ++cit) {
		_emitString += QString::fromStdString(*cit);
		_emitString += ';';
	}
	_emitString += QString::fromStdString(_file);

	emit addTreeWidgetItem(_emitString);
}
