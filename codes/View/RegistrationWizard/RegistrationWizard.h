#ifndef REGISTRATIONWIZARD_H
#define REGISTRATIONWIZARD_H

#include <QWizard>
#include <QThread>
#include <qlist.h>
#include <qsharedpointer.h>
#include <qmutex.h>

class QLabel;
class QLineEdit;
class QPushButton;
class QProgressBar;
class QTreeWidget;
class QTableWidget;
class QThread;
class QTabWidget;
class FindImageThread;

class DirectoryPage : public QWizardPage
{
    Q_OBJECT

public:
	DirectoryPage(QWidget *parent = nullptr);

    int nextId() const;
	void setDirectory(QString dir);

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

	QList<QSharedPointer<QStringList>>* m_imagePaths;
	QList<int>* m_selectedImages;

	void setImageModalityNames(unsigned int i, QString imageModalityName);

    int nextId() const;

	virtual void initializePage();
	virtual bool validatePage();

public slots:
	void onUpdateProgressBar(int);
	
	void setImages();

	void removeImages();

private:
	QProgressBar *progressBar;
	QTreeWidget	 *treeWidget;

	QList<QLabel*> m_imageLabels;
	QList<QLineEdit*> m_imageLineEdits;
	QList<QPushButton*> m_imageSetBtns;
	QList<QPushButton*> m_imageRemoveBtns;

	QMutex m_mutex;

	QString lastDirectory;
	friend FindImageThread;
	FindImageThread* thread;
};

class FindImageThread : public QThread
{
	Q_OBJECT

public:
	FindImageThread(ImagePage *parent = 0);
	void run();
	void updateTreeWidget(QStringList* imagePath);

	ImagePage* m_imagePage;
	bool m_stop;


signals:
	void updateProgressBar(int);
};

class ConclusionPage : public QWizardPage
{
    Q_OBJECT

public:
    ConclusionPage(int numOfImages = 2, QWidget* parent = nullptr);
	ConclusionPage(QWidget* parent);

	QList<QSharedPointer<QStringList>>* m_imagePaths;
	QList<int>* m_selectedImages;



    void initializePage();
    int nextId() const;

private:
    QLabel		 *bottomLabel;
	QTabWidget	 *tabWidget;

	QList<QTableWidget*> m_tableWidgets;
};





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
    
	QSharedPointer<QStringList> getFileNames(unsigned int i);
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

	QList<QSharedPointer<QStringList>> m_imagePaths;
	QList<int> m_selectedImages;

};

#endif
