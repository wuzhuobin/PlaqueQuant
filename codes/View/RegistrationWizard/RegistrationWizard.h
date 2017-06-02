#ifndef REGISTRATIONWIZARD_H
#define REGISTRATIONWIZARD_H

#include <QWizard>
#include <qrunnable.h>
#include <qlist.h>

class QLabel;
class QLineEdit;
class QPushButton;
class QProgressBar;
class QTreeWidget;
class QTableWidget;
class QTabWidget;
class FindImageRunnable;
class DirectoryPage;
class ImagePage;
class ConclusionPage;

class RegistrationWizard : public QWizard
{
	Q_OBJECT

public:

	enum {
		/// the first page
		PAGE_DIRECTORY = 0,
		/// the second page
		PAGE_IMAGE = 1,
		/// the last
		PAGE_CONCLUSION = 2
	};
	/**
	* Constructor.
	* Constructor of RegistrationWizard.
	* @param	dir
	* @param	numOfImages set #m_numOfImages, the number of lineEdits need to be created, these lineEdits
	*			are for displaying what images are going to load
	* @param	parent parent widget.
	*/
	RegistrationWizard(
		QString dir = QString(),
		int numOfImages = 2,
		QWidget *parent = nullptr);
	RegistrationWizard(
		int numOfImages,
		QWidget *parent = nullptr);
	RegistrationWizard(
		QWidget *parent);
	/**
	* Destructor.
	*/
	~RegistrationWizard();

	void setImageModalityNames(unsigned int i, QString imageModalityName = QString());

	QString getFileNames(unsigned int i);
	void setDirectory(QString directory);
	const QString getDirectory();
	/**
	* Get the number of images needs to be loaded.
	* @return	number of images, #m_numberOfImages.
	*/
	int getNumberOfImages();

	private slots:
	void showHelp();

private:

	int m_numOfImages;

	DirectoryPage	*directoryPage;
	ImagePage		*imagePage;
	ConclusionPage	*conclusionPage;

};

class DirectoryPage : public QWizardPage
{
    Q_OBJECT

public:
	DirectoryPage(QString dir = QString(), QWidget *parent = nullptr);
	DirectoryPage(QWidget *parent);

    int nextId() const;

public slots:
	void browse();

private:
	QLabel		*m_imageDirectoryLabel;
	QLineEdit	*m_imageDirectoryLineEdit;
	QPushButton *browseBtn;

};

class ImagePage : public QWizardPage
{
    Q_OBJECT

public:
	ImagePage(int numOfImages = 2, QWidget* parent = nullptr);
	ImagePage(QWidget *parent);
	~ImagePage();

	void setImageModalityNames(unsigned int i, QString imageModalityName);

    int nextId() const;

	virtual void initializePage();
	virtual bool validatePage();

public slots:
	void updateProgressBar();
	
	void setImages();

	void removeImages();

	void updateTreeWidget(QString item);

signals:
	void finishPrevious();

private:
	QProgressBar *progressBar;
	QTreeWidget	 *treeWidget;

	QList<QLabel*> m_imageLabels;
	QList<QLineEdit*> m_imageLineEdits;
	QList<QLineEdit*> m_imageLineEdits2;
	QList<QPushButton*> m_imageSetBtns;
	QList<QPushButton*> m_imageRemoveBtns;

	QThreadPool* m_threadPool;

	QString m_lastDirectory;

};

class ConclusionPage : public QWizardPage
{
	Q_OBJECT

public:
	ConclusionPage(int numOfImages = 2, QWidget* parent = nullptr);
	ConclusionPage(QWidget* parent);


	void initializePage();
	int nextId() const;

private:
	QLabel		 *bottomLabel;
	QTabWidget	 *tabWidget;

	QList<QTableWidget*> m_tableWidgets;
};

class FindImageRunnable :public QObject, public QRunnable
{
	Q_OBJECT;
public:
	FindImageRunnable(QString folderPath, QObject* parent = nullptr);
	void run();

public slots:
	void forceFinish();

signals:
	void addTreeWidgetItem(QString);
	void finish();
private:
	void GDCImageIORead(const std::vector<std::string>& files);
	QString m_folderPath;
	bool m_forceFinish = false;
};



#endif
