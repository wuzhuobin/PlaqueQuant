#ifndef REGISTRATIONWIZARD_H
#define REGISTRATIONWIZARD_H

#include <QWizard>
#include <QThread>

class QCheckBox;
class QLabel;
class QLineEdit;
class QPushButton;
class QProgressBar;
class QTreeWidget;
class QTableWidget;
class QThread;
class QTreeWidgetItem;
class QTabWidget;
class QSpinBox;
class QGroupBox;
class QRadioButton;

class FindImageThread : public QThread
{
	Q_OBJECT

public:
	explicit FindImageThread(QObject *parent = 0);
	void run();
	void setDirectory(QString dir);

public slots:
	void forceStop();

signals:
	void updateTree(QTreeWidgetItem*, QStringList* );
	void updateProgressBar(int);

private:
	QString m_dir;
	bool m_stop;

};

class DirectoryPage : public QWizardPage
{
    Q_OBJECT

public:
    DirectoryPage(QWidget *parent = 0);

    int nextId() const;
	void setDirectory(QString dir);

public slots:
	void browse();

private:
	QLabel		*imageDirectoryLabel;
	QLineEdit	*imageDirectoryLineEdit;
	QPushButton *browseBtn;

};

class ImagePage : public QWizardPage
{
    Q_OBJECT

public:
    ImagePage(QWidget *parent = 0);

    int nextId() const;

	virtual void initializePage();
	virtual bool validatePage();

public slots:
	void onUpdateTree(QTreeWidgetItem*,QStringList*);
	void onUpdateProgressBar(int);
	void setImage1();
	void setImage2();
	void setImage3();
    void setImage4();
	void setImage5();
	void removeImage1();
	void removeImage2();
	void removeImage3();
    void removeImage4();
	void removeImage5();
private:
	QProgressBar *progressBar;
	QTreeWidget	 *treeWidget;

	QLabel		 *image1Label;
	QLineEdit	 *image1LineEdit;
	QPushButton  *image1SetBtn;
	QPushButton	 *image1RemoveBtn;
	QSpinBox	 *image1SpinBox;

	QLabel		 *image2Label;
	QLineEdit	 *image2LineEdit;
	QPushButton  *image2SetBtn;
	QPushButton	 *image2RemoveBtn;
	QSpinBox	 *image2SpinBox;

	QLabel		 *image3Label;
	QLineEdit	 *image3LineEdit;
	QPushButton  *image3SetBtn;
	QPushButton	 *image3RemoveBtn;
	QSpinBox	 *image3SpinBox;
    
    QLabel		 *image4Label;
    QLineEdit	 *image4LineEdit;
    QPushButton  *image4SetBtn;
    QPushButton	 *image4RemoveBtn;
    QSpinBox	 *image4SpinBox;
	
	QLabel		 *image5Label;
    QLineEdit	 *image5LineEdit;
    QPushButton  *image5SetBtn;
    QPushButton	 *image5RemoveBtn;
    QSpinBox	 *image5SpinBox;

	QString lastDirectory;
	FindImageThread* thread;
};


class ConclusionPage : public QWizardPage
{
    Q_OBJECT

public:
    ConclusionPage(QWidget *parent = 0);

    void initializePage();
    int nextId() const;
    void setVisible(bool visible);

	void setTable1();
	void setTable2();
	void setTable3();
    void setTable4();
	void setTable5();
private:
    QLabel       *groupBoxLabel;
    QLabel		 *bottomLabel;
    QGroupBox    *groupBox;
    QRadioButton *sagittalButton;
    QRadioButton *coronalButton;
    QRadioButton *axialButton;
	QTabWidget	 *tabWidget;
	QTableWidget *table1Widget;
	QTableWidget *table2Widget;
	QTableWidget *table3Widget;
    QTableWidget *table4Widget;
	QTableWidget *table5Widget;
};





class RegistrationWizard : public QWizard
{
	Q_OBJECT

public:
	enum { Page_Directory, Page_Image, Page_Conclusion };

    RegistrationWizard(QString dir = "", QWidget *parent = 0);

	QList<QStringList*> *fileNameList;
    
	QString		 getDirectory();
	QStringList* getFileNames1();
	QStringList* getFileNames2();
	QStringList* getFileNames3();
    QStringList* getFileNames4();
	QStringList* getFileNames5();
	int getTotalFileNo();
    
private slots:
	void showHelp();

private:
	DirectoryPage	*directoryPage;
	ImagePage		*imagePage;
	ConclusionPage	*conclusionPage;

};

#endif
