#include <qwidget.h>
#include <qdatetime.h>
#include <qstring.h>
#include <qmessagebox.h>
#include <qmap.h>



class EncryptionAuthentication: public QWidget
{
public:
	enum AUTHENTICATION_FLAG
	{
		FAIL = 0,
		SUCCESSFUL = 1
	};

	enum AUTHENTICATION_ERROR_CODE {
		NORMAL = 0X00,
		HAVING_KEY = 0X01,
		SOFT_ID = 0X02,
		HARD_ID = 0X04,
		PRODUCT_NAME = 0X08,
		EXPIRED_DATE_TIME = 0X10
	};

	void setErrorMessage(int errorCode, QString message);

	EncryptionAuthentication(
		unsigned long hardID = 0,
		QString softID = QString(),
		QString productName = QString(),
		QDateTime expiredDateTime = QDateTime(), 
		QWidget* parent = nullptr);

	~EncryptionAuthentication();

	int authenticationExec(int errorCode);

	void setUserPassword(QString userPassword);

	void setAdminPassword(QString adminPassword);


private:

	void reloadViKey();
	int authenticationBySoftID();
	int authenticationByHardID();
	int authenticationByProductName();
	int authenticationByHavingKey();
	int authenticationByExpiredDateTime();


	QString m_userPassWord = "11111111";
	QString m_adminPassWord = "00000000";

	// @see	Vikey.h
	// pleas refer to the Vikey.h and its manual
	unsigned long m_errorType = 0;

	unsigned long m_hardID;
	QString m_softID;
	QString m_productName;
	QDateTime m_expiredDateTime;

	QMessageBox m_messageBox;
	
	QMap<int, QString> m_messages;

	bool m_havingKey = false;

};

