#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include <QDateTime>
#include "QPrinter.h"
#include <QTableWidget>
#include <QTextDocument>
#include <QTextEdit>
#include "AbstractFilter.h"

class ReportGenerator: public AbstractFilter
{
    Q_OBJECT
public:
    explicit ReportGenerator(QObject *parent = 0);
	~ReportGenerator();

	//Directory
	void 	SetDirectory(QString pdfPath);
	QString GetDirectory();

	//Header and miscellaneous
	void SetSoftwareName(QString softwareName);
	void SetSoftwareVersion(QString softwareVersion);
	void SetReportName(QString reportName);
	void SetDepartmentName(QString departmentName);
	void SetUniversityName(QString universityName);
	void SetDateAndTime(int y, int m, int d, int h, int min, int s);
	void SetDateAndTime(bool isCurrentTime);
	void SetDateAndTime(QDateTime dateTime);

	//Box
	void SetPatientName(QString patientName);
	void SetPatientID(QString patientID);
	void SetPatientAge(double patientAge);
	void SetPatientSex(QString patientSex);
	void SetDoctorInCharged(QString doctorsName);

	//Comment
	void AddComment(QString doctorsname, QString comment);

	//Summary
	void SetSummaryName(QString summaryName);
	void AddSummary(QString tag, QString value);

	//Table
	void AddTable(int n, QString tableTitle, QStringList horizontalHeaderList, QStringList verticalHeaderList, int row, int column);
	void SetTableItem(int n, int row, int column, QString value);
	void SetTableSize(int n, int w, int h);
	void SetTableItemBackgroundColor(int n, int row, int column, int color[4]);
	void SetTableItemRowSpan(int n, int row, int column, int rowspan);
	void SetTableItemColumnSpan(int n, int row, int column, int columnspan);
	void SetTableItemBold(int n, int row, int column, bool b);
	void SetTableImageItem(int n, int row, int column, int image_no);
	//Figure
	void AddFigure(int n, QString figureTitle, QString imagePath);

	//Signature
	void SetNeedSignature(bool b);

public slots:
	int Update();

private:
	bool HasFilledInAllRequiredInfo();
	void SetupPrinter();
	void AddInfo2PDF();
	void AddFigures2PDF();
	void AddTables2PDF();

	//PDF path
	QString  m_pdfPath;

	//Header and miscellaneous
	QString   m_softwareName;
	QString   m_softwareversion;
	QString   m_reportName;
	QString   m_departmentName;
	QString   m_universityName;
	QDateTime m_dateTime;

	//Box 
	QString m_doctorName;
	QString m_patientName;
	QString m_patientID;
	QString m_patientSex;
	double 	m_patientAge;

	//Comment
	QMap<QString, QString> m_doctorsCommentMap;

	//Summary
	QString m_summaryName;
	QMap<QString, QString> m_summaryMap;

	//Table
	QList<QTableWidget*> m_tableList;

	//Figure
	QList<QString>	m_figureImageList;
	QList<QString>	m_figureTitleList;

	//Boolean
	bool m_isNeedSignature;

	QPrinter*		m_printer;
	QTextEdit*		m_printingTextEdit;

	QFont m_titleFont;
	QFont m_headerFont;
	QFont m_subHeaderFont;
	QFont m_importantFont;
	QFont m_normalFont;
};

#endif // REPORTGENERATOR_H
