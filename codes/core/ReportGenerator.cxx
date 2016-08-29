#include <stdio.h>
#include <process.h>
#include <QDebug>
#include <QHeaderView>
#include <QPainter>
#include <QTextCursor>
#include "ReportGenerator.h"


char* SpanText(char* text, char* style) {
	char* outChars = (char*)malloc(sizeof(char)*500);
	sprintf(outChars, "<span style='%s'> %s </span>", style, text);
	qDebug() << outChars;
	return outChars;
}

ReportGenerator::ReportGenerator(QObject *parent) :
    AbstractFilter(parent)
{
	m_isNeedSignature	= true;
	m_printer			= new QPrinter(QPrinter::HighResolution);
	m_printingTextEdit	= new QTextEdit;

	//m_titleFont.setBold(true);
	//m_titleFont.setPointSize(18);
	
	m_headerFont.setBold(true);
	m_headerFont.setPointSize(15);
	m_headerFont.setFamily("Calibri");
	
	m_subHeaderFont.setBold(true);
	//m_subHeaderFont.setUnderline(true);
	m_subHeaderFont.setPointSize(9);


	m_normalFont.setPointSize(9);
	m_normalFont.setFamily("Calibri");

	//m_importantFont.setBold(true);
	//m_importantFont.setPointSize(12);
}

ReportGenerator::~ReportGenerator()
{
	delete m_printingTextEdit;
	delete m_printer;

	for (int i = 0; i < m_tableList.size(); i++)
		delete m_tableList[i];
}

int ReportGenerator::Update()
{
	emit Process(0);

	if (!this->HasFilledInAllRequiredInfo())
	{
		emit Process(100);
		emit Finished("Haven't finished required information.");
		return 1;
	}

	emit Process(40);
	this->SetupPrinter();

	emit Process(50);
	this->AddInfo2PDF();

	emit Process(60);
	this->AddTables2PDF();

	emit Process(70);
	//this->AddFigures2PDF();

	m_printingTextEdit->print(m_printer);

	emit Process(100);
	emit Finished();
	return 0;
}

bool ReportGenerator::HasFilledInAllRequiredInfo()
{
	if (m_pdfPath			== "" ||
		//m_doctorName		== "" ||
		//m_patientName		== "" ||
		//m_softwareName		== "" ||
		//m_softwareversion	== "" ||
		m_reportName		== "" 
		//m_departmentName	== "" ||
		//m_universityName	== ""
		)
		return false;

	return true;
}

void ReportGenerator::SetupPrinter()
{
	m_printer->setOutputFormat(QPrinter::PdfFormat);
	m_printer->setOutputFileName(m_pdfPath);
	m_printer->setPaperSize(QPrinter::A4);
	m_printer->setOrientation(QPrinter::Portrait);
	m_printer->setFullPage(false);
	//QPagedPaintDevice::PageSize pagesize;
	//m_printer->setPageSize(pagesize); // the document needs a valid PageSize
	m_printer->setPageMargins(0, 0, 0, 0, QPrinter::Millimeter);
}

void ReportGenerator::AddInfo2PDF()
{
	//PDF Text
	//m_printingTextEdit->autoFormatting();

	//QTextCursor cursor = m_printingTextEdit->textCursor();
	//cursor.movePosition(QTextCursor::Start);

	//QTextBlockFormat textBlockFormat = cursor.blockFormat();
	//cursor.mergeBlockFormat(textBlockFormat);

	qDebug() << SpanText("test", "color:red");
	//Software name
	//QTextEdit softwareNameTextEdit;
	//softwareNameTextEdit.setFont(m_titleFont);
	//softwareNameTextEdit.setText("<center>"+m_softwareName + "<br></center>\n");
	//softwareNameTextEdit.setText("<p align=\"center\"><u>" + QString::fromStdString(SpanText("test<br>", "color:red;")) + "</u></p>\n");
	//softwareNameTextEdit.setStyleSheet(" margin-right: 10px; ");

	//qDebug() << softwareNameTextEdit.toHtml();
	//m_printingTextEdit->insertHtml(softwareNameTextEdit.toHtml());


	//Report name
	QTextEdit* reportNameTextEdit = new QTextEdit;

	reportNameTextEdit->setFont(m_headerFont);
	reportNameTextEdit->setText("<center>" + m_reportName +"<br></center>\n");
	//reportNameTextEdit->setStyleSheet(" margin-right: 10px; ");
	//reportNameTextEdit->setText("<p align=\"left\"><u>" + QString::fromStdString(SpanText("test<br>", "color:red;text-align:center;")) + "</u></p>");
	qDebug() << reportNameTextEdit->toHtml();

	m_printingTextEdit->insertHtml(reportNameTextEdit->toHtml());

	//Add a empty line
	QTextEdit emptyLine;
	emptyLine.setFont(m_normalFont);
	emptyLine.setText("\n");
	m_printingTextEdit->insertHtml(emptyLine.toHtml());

	/*
	//Add Info
	//Patient Name
	QTextEdit* patientInfoTextEdit = new QTextEdit;
	patientInfoTextEdit->setFont(m_subHeaderFont);
	patientInfoTextEdit->setText("<u>Patient Information<br></u>\n");
	patientInfoTextEdit->setFontUnderline(true);
	qDebug() << patientInfoTextEdit->toHtml();

	m_printingTextEdit->insertHtml(patientInfoTextEdit->toHtml() + "\n");

	//Patient Name
	QTextEdit patientNameTextEdit;
	patientNameTextEdit.setFont(m_normalFont);
	patientNameTextEdit.setText("Patient Name: " + m_patientName + "\n");
	patientNameTextEdit.setStyleSheet(" color:red;");
	patientNameTextEdit.update();
	m_printingTextEdit->textCursor().insertHtml(patientNameTextEdit.toHtml());
	qDebug() << patientNameTextEdit.toHtml();
	qDebug() << m_printingTextEdit->toHtml();

	//Patient ID
	QTextEdit patientIDTextEdit;
	patientIDTextEdit.setFont(m_normalFont);
	patientIDTextEdit.setText("Patient ID: " + m_patientID + "\n");

	m_printingTextEdit->insertHtml(patientIDTextEdit.toHtml());

	//Patient Age
	QTextEdit patientAgeTextEdit;
	patientAgeTextEdit.setFont(m_normalFont);
	patientAgeTextEdit.setText("Patient Age: " + QString::number(m_patientAge) + "\n");

	m_printingTextEdit->insertHtml(patientAgeTextEdit.toHtml());

	//Patient Sex
	QTextEdit patientSexTextEdit;
	patientSexTextedit.setFont(m_normalFont);
	patientSexTextEdit.setText("Patient Sex: " + m_patientSex + "\n");

	m_printingTextEdit->insertHtml(patientSexTextEdit.toHtml());

	//Doctor In Charge
	QTextEdit doctorInChargedTextEdit;
	doctorInChargedTextEdit.setFont(m_normalFont);
	doctorInChargedTextEdit.setText("Referred by: dr. " + m_doctorName + "\n");

	m_printingTextEdit->insertHtml(doctorInChargedTextEdit.toHtml());

	//Date 
	QTextEdit dateTextEdit;
	dateTextEdit.setFont(m_normalFont);
	dateTextEdit.setText("Date: " + m_dateTime.toString("dd") + "/" + m_dateTime.toString("MM") + "/" + m_dateTime.toString("yyyy") + "\n");

	m_printingTextEdit->insertHtml(dateTextEdit.toHtml());

	//Add a empty line
	QTextEdit emptyLine;
	emptyLine.setFont(m_normalFont);
	emptyLine.setText("\n\n");
	m_printingTextEdit->insertHtml(emptyLine.toHtml());

	//Add Summary

	if (m_summaryMap.count() > 0)
	{
		//add Sub header
		QTextEdit summaryNameSubHeaderTextEdit;
		summaryNameSubHeaderTextEdit.setFont(m_subHeaderFont);
		summaryNameSubHeaderTextEdit.setText("<u>"+m_summaryName+"<br></u>\n");

		m_printingTextEdit->insertHtml(summaryNameSubHeaderTextEdit.toHtml());

		QMapIterator<QString, QString> i(m_summaryMap);
		while (i.hasNext()) 
		{
			i.next();

			QTextEdit summaryTagTextEdit;
			summaryTagTextEdit.setFont(m_normalFont);
			summaryTagTextEdit.setText(i.key() + ": ");

			m_printingTextEdit->insertHtml(summaryTagTextEdit.toHtml());

			QTextEdit summaryValueTextEdit;
			summaryValueTextEdit.setFont(m_normalFont);
			summaryValueTextEdit.setText(i.value() + "\n");
			summaryValueTextEdit.setAlignment(Qt::AlignRight);

			m_printingTextEdit->insertHtml(summaryValueTextEdit.toHtml());
		}

		//Add a empty line
		QTextEdit emptyLine;
		emptyLine.setFont(m_normalFont);
		emptyLine.setText("\n\n");
		m_printingTextEdit->insertHtml(emptyLine.toHtml());
	}

	//Add doctors'comment
	if (m_doctorsCommentMap.count() > 0)
	{
		QMapIterator<QString, QString> i(m_doctorsCommentMap);
		while (i.hasNext())
		{
			i.next();

			//add Sub header
			QTextEdit commentSubHeaderTextEdit;
			commentSubHeaderTextEdit.setFont(m_subHeaderFont);
			commentSubHeaderTextEdit.setText("<u>Comment [dr." + i.key() + "]<br></u>\n");

			m_printingTextEdit->insertHtml(commentSubHeaderTextEdit.toHtml());

			//add comment
			QTextEdit commentTextEdit;
			commentTextEdit.setFont(m_normalFont);
			commentTextEdit.setText(i.value() + "\n");

			m_printingTextEdit->insertHtml(commentTextEdit.toHtml());

			//Add a empty line
			QTextEdit emptyLine;
			emptyLine.setFont(m_normalFont);
			emptyLine.setText("\n\n");
			m_printingTextEdit->insertHtml(emptyLine.toHtml());
		}


	}
	
	//Add Appendix
	int pageNumber = 2;
	if (m_tableList.size() > 0 || m_figureTitleList.size() > 0)
	{
		QTextEdit appendixTextEdit;
		appendixTextEdit.setFont(m_subHeaderFont);
		appendixTextEdit.setText("<u>Appendix<br></u>\n");

		m_printingTextEdit->insertHtml(appendixTextEdit.toHtml());

		if (m_tableList.size() > 0)
		{
			QTextEdit tableTextEdit;
			tableTextEdit.setFont(m_importantFont);
			tableTextEdit.setText("Table\n");
			
			m_printingTextEdit->insertHtml(tableTextEdit.toHtml());
			
			for (int i = 0; i < m_tableList.size(); i++)
			{
				//Table Name
				QTextEdit tableNameTextEdit;
				tableNameTextEdit.setFont(m_normalFont);
				tableNameTextEdit.setText("    " + m_tableList.at(i)->windowTitle());
				
				m_printingTextEdit->insertHtml(tableNameTextEdit.toHtml());

				//Table Page
				QTextEdit tablePageTextEdit;
				tablePageTextEdit.setFont(m_normalFont);
				tablePageTextEdit.setText(" " + QString::number(pageNumber)+"\n");
				pageNumber++;

				m_printingTextEdit->insertHtml(tablePageTextEdit.toHtml());
			}
		}

		if (m_figureTitleList.size() > 0)
		{
			QTextEdit figureTextEdit;
			figureTextEdit.setFont(m_importantFont);
			figureTextEdit.setText("Figure\n");

			m_printingTextEdit->insertHtml(figureTextEdit.toHtml());

			for (int i = 0; i < m_figureTitleList.size(); i++)
			{
				//Table Name
				QTextEdit figureNameTextEdit;
				figureNameTextEdit.setFont(m_normalFont);
				figureNameTextEdit.setText("    " + m_figureTitleList.at(i));

				m_printingTextEdit->insertHtml(figureNameTextEdit.toHtml());

				//Table Page
				QTextEdit figurePageTextEdit;
				figurePageTextEdit.setFont(m_normalFont);
				figurePageTextEdit.setText(" " + QString::number(pageNumber) + "\n");
				pageNumber++;

				m_printingTextEdit->insertHtml(figurePageTextEdit.toHtml());
			}
		}

		//Add a empty line
		QTextEdit emptyLine;
		emptyLine.setFont(m_normalFont);
		emptyLine.setText("\n\n\n\n\n\n\n\n\n\n\n\n");
		m_printingTextEdit->insertHtml(emptyLine.toHtml());
	}

	//Add Date & Signature
	if (m_isNeedSignature)
	{
		//acknowledgment line
		QTextEdit confirmSignatureextEdit;
		confirmSignatureextEdit.setFont(m_importantFont);
		confirmSignatureextEdit.setText("*By signing below you are acknowledging that you have read and agree to and accept all of the information contained in this report.\n\n\n");

		m_printingTextEdit->insertHtml(confirmSignatureextEdit.toHtml());
	
		//Signature line
		QTextEdit signatureTextEdit;
		signatureTextEdit.setFont(m_normalFont);
		signatureTextEdit.setText("Date: _________________                                            Signature: _________________\n");

		m_printingTextEdit->insertHtml(signatureTextEdit.toHtml());

		//doctor's name line
		QTextEdit nameTextEdit;
		nameTextEdit.setFont(m_normalFont);
		nameTextEdit.setText("                                                                                                      dr. " + m_doctorName);
		//nameTextEdit.setText("<blockquote><center><u> dr. " + m_doctorName +"<br></u></center></blockquote>\n");
		//nameTextEdit.setAlignment(Qt::AlignRight);
		//Add a empty line
		QTextEdit emptyLine;

		m_printingTextEdit->insertHtml(nameTextEdit.toHtml());
	}

	m_printingTextEdit->insertHtml(emptyLine.toHtml());
	*/
}

void ReportGenerator::AddFigures2PDF()
{
	for (int i = 0; i < m_figureTitleList.size(); i++)
	{
		//m_printer->newPage();

		//Add a empty line
		//QTextEdit emptyLine;
		//emptyLine.setFont(m_normalFont);
		//emptyLine.setText(m_figureTitleList.at(i)+ "\n");
		//m_printingTextEdit->insertHtml(emptyLine.toHtml());


		QImage image(m_figureImageList.at(i));
		QImage scaledImage;

		double width2A4Ratio	= double(image.width())  / double(420.0);
		double height2A4Ratio	= double(image.height()) / double(594.0);

		if (width2A4Ratio > 1 || height2A4Ratio > 1)
		{
			if (width2A4Ratio > height2A4Ratio)
				scaledImage = image.scaledToWidth(350);
			else
				scaledImage = image.scaledToHeight(350);
		}
		else
			scaledImage = image;

		QTextImageFormat headerImageFormat;
		headerImageFormat.setWidth(scaledImage.width());
		headerImageFormat.setHeight(scaledImage.height());
		headerImageFormat.setName(m_figureImageList.at(i));
		
		QTextCursor cursor = m_printingTextEdit->textCursor();
		qDebug() << cursor.position() << endl;
		cursor.insertImage(headerImageFormat);

		
	}
}

void ReportGenerator::AddTables2PDF()
{
	for (int i = 0; i < m_tableList.size(); i++)
	{
		m_printer->newPage();

		QTextCursor cursor = m_printingTextEdit->textCursor();

		//m_tableList.at(i)->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		//m_tableList.at(i)->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
		//m_tableList.at(i)->adjustSize();
		//m_tableList.at(i)->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
		//m_tableList.at(i)->resizeColumnsToContents();
		//m_tableList.at(i)->resizeRowsToContents();

		//int w = m_tableList.at(i)->verticalHeader()->width() + 4; // +4 seems to be needed
		//for (int j = 0; j < m_tableList.at(i)->columnCount(); j++)
		//	w += m_tableList.at(i)->columnWidth(j); // seems to include grid line (on my machine)
		//int h = m_tableList.at(i)->horizontalHeader()->height();
		//for (int j = 0; j < m_tableList.at(i)->rowCount(); j++)
		//	h += m_tableList.at(i)->rowHeight(j);

		//w += m_tableList.at(i)->contentsMargins().left() + m_tableList.at(i)->contentsMargins().right();
		//h += m_tableList.at(i)->contentsMargins().top()  + m_tableList.at(i)->contentsMargins().bottom();

		//m_tableList.at(i)->setMaximumSize(QSize(w, h));
		//m_tableList.at(i)->setMinimumSize(QSize(w, h));

		//Convert Table to Images
		QImage img(m_tableList.at(i)->width(), m_tableList.at(i)->height(), QImage::Format_RGB32);
		//img.setDotsPerMeterX(1); // 2.54 cm = 1 inch
		//img.setDotsPerMeterY(1);
		QPainter painter(&img);
		painter.initFrom(m_tableList.at(i));
		painter.setRenderHint(QPainter::Antialiasing, true);
		painter.eraseRect(m_tableList.at(i)->rect());

		img.fill(Qt::transparent);
		m_tableList.at(i)->render(&painter, QPoint(), QRegion(), QWidget::DrawChildren);
		painter.end();
		//img = img.scaledToWidth(printer->width());
		cursor.insertImage(img);

		//Add a empty line
		QTextEdit emptyLine;
		emptyLine.setFont(m_normalFont);
		emptyLine.setText("\n\n");
		//emptyLine.setText("Table " + QString::number(i + 1) + ": " + m_tableList.at(i)->windowTitle() + "\n");
		m_printingTextEdit->insertHtml(emptyLine.toHtml());
	}
}

void ReportGenerator::SetDirectory(QString pdfPath)
{
	m_pdfPath = pdfPath;
}

QString ReportGenerator::GetDirectory()
{
	return m_pdfPath;
}

void ReportGenerator::SetSoftwareName(QString softwareName)
{
	m_softwareName = softwareName;
}

void ReportGenerator::SetSoftwareVersion(QString softwareVersion)
{
	m_softwareversion = softwareVersion;
}

void ReportGenerator::SetReportName(QString reportName)
{
	m_reportName = reportName;
}

void ReportGenerator::SetDepartmentName(QString departmentName)
{
	m_departmentName = departmentName;
}

void ReportGenerator::SetUniversityName(QString universityName)
{
	m_universityName = universityName;
}

void ReportGenerator::SetDateAndTime(int y, int m, int d, int h, int min, int s)
{
	m_dateTime.setDate(QDate(y,m,d));
	m_dateTime.setTime(QTime(min, s));
}

void ReportGenerator::SetDateAndTime(bool isCurrentTime)
{
	m_dateTime = QDateTime::currentDateTime();
}

void ReportGenerator::SetDateAndTime(QDateTime dateTime)
{
	m_dateTime = dateTime;
}

void ReportGenerator::SetPatientName(QString patientName)
{
	m_patientName = patientName;
}

void ReportGenerator::SetPatientID(QString patientID)
{
	m_patientID = patientID;
}

void ReportGenerator::SetPatientAge(double patientAge)
{
	m_patientAge = patientAge;
}

void ReportGenerator::SetPatientSex(QString patientSex)
{
	m_patientSex = patientSex;
}

void ReportGenerator::SetDoctorInCharged(QString doctorsName)
{
	m_doctorName = doctorsName;
}

void ReportGenerator::AddComment(QString doctorsname, QString comment)
{
	m_doctorsCommentMap.insert(doctorsname, comment);
}

void ReportGenerator::SetSummaryName(QString summaryName)
{
	m_summaryName = summaryName;
}

void ReportGenerator::AddSummary(QString tag, QString value)
{
	m_summaryMap.insert(tag, value);
}

void ReportGenerator::SetTableSize(int n, int w, int h)
{
	m_tableList[n]->setMaximumSize(w, h);
	m_tableList[n]->setMinimumSize(w, h);
}

void ReportGenerator::AddTable(int n, QString tableTitle, QStringList horizontalHeaderList, QStringList verticalHeaderList, int row, int column)
{
	QTableWidget* widget = new QTableWidget;
	widget->setWindowTitle(tableTitle);
	widget->setRowCount(row);
	widget->setColumnCount(column);
	widget->setFont(m_normalFont);

	if (horizontalHeaderList.size()>0)
		widget->setHorizontalHeaderLabels(horizontalHeaderList);
	else
		widget->horizontalHeader()->setVisible(false);
	
	if (verticalHeaderList.size()>0)
		widget->setVerticalHeaderLabels(verticalHeaderList);
	else
		widget->verticalHeader()->setVisible(false);

	widget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	widget->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	widget->horizontalHeader()->setDefaultSectionSize(175);
	widget->adjustSize();
	//widget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
	//widget->resizeColumnsToContents();
	//widget->resizeRowsToContents();

	//for (int i = 0; i < column; i++)
	//	widget->setColumnWidth(i, 680.0 / double(column));

	//int w = widget->verticalHeader()->width()+4; // +4 seems to be needed
	//for (int i = 0; i < widget->columnCount(); i++)
	//	w += widget->columnWidth(i); // seems to include grid line (on my machine)
	//int h = widget->horizontalHeader()->height();
	//for (int i = 0; i < widget->rowCount(); i++)
	//	h += widget->rowHeight(i);

	//w += widget->contentsMargins().left() + widget->contentsMargins().right();
	//h += widget->contentsMargins().top()  + widget->contentsMargins().bottom();

	//widget->setMaximumSize(QSize(600, 200));
	//widget->setMinimumSize(QSize(600, 200));

	m_tableList.insert(n, widget);
}

void ReportGenerator::SetTableItem(int n, int row, int column, QString value)
{
	m_tableList[n]->setItem(row, column, new QTableWidgetItem(value));
	//qDebug() << m_tableList[n]->item(row, column)->text() << endl;
	
}
void ReportGenerator::SetTableImageItem(int n, int row, int column, int image_no)
{
	QImage image(m_figureImageList.at(image_no));
	QImage scaledImage;

	double width2A4Ratio = double(image.width()) / double(420.0);
	double height2A4Ratio = double(image.height()) / double(594.0);

	if (width2A4Ratio > 1 || height2A4Ratio > 1)
	{
		if (width2A4Ratio > height2A4Ratio)
			scaledImage = image.scaledToWidth(300);
		else
			scaledImage = image.scaledToHeight(300);
	}
	else
		scaledImage = image;
	QTableWidgetItem *item = new QTableWidgetItem;
	item->setData(Qt::DecorationRole, scaledImage);
	m_tableList[n]->setItem(row, column, item);
}
void ReportGenerator::SetTableItemBackgroundColor(int n, int row, int column, int color[4])
{
	m_tableList[n]->item(row, column)->setBackgroundColor(QColor(color[0], color[1], color[2], color[3]));
}

void ReportGenerator::SetTableItemColumnSpan(int n, int row, int column, int columnspan)
{
	int rowSpan = m_tableList[n]->rowSpan(row, column);
	m_tableList[n]->setSpan(row, column, rowSpan, columnspan);
}
void ReportGenerator::SetTableItemRowSpan(int n, int row, int column, int rowspan)
{
	int columnSpan = m_tableList[n]->rowSpan(row, column);
	m_tableList[n]->setSpan(row, column, rowspan, columnSpan);
}

void ReportGenerator::SetTableItemBold(int n, int row, int column, bool b)
{
	if (b)
		m_tableList[n]->item(row, column)->setFont(m_subHeaderFont);
	else
		m_tableList[n]->item(row, column)->setFont(m_normalFont);

}
void ReportGenerator::AddFigure(int n, QString figureTitle, QString imagePath)
{
	//QImage image(imagePath);

	m_figureTitleList.insert(n, figureTitle);
	m_figureImageList.insert(n, imagePath);
}

void ReportGenerator::SetNeedSignature(bool b)
{
	m_isNeedSignature = b;
}

