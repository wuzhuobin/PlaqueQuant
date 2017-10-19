#include <qwidget.h>
#include <qdatetime.h>
#include <qstring.h>
#include <qmessagebox.h>
#include <qmap.h>
/**
 * API class for using the Vikey USB authentication
 * API class for using the Vikey USB authentication system. The class is derived from 
 * QWidget. And this API class can only be used under Windows OS.
 * 
 * Updated Log:
 * version 1.0.0
 * Original one provided authentication method for having key, soft id, hard id
 * product name, expired date time
 *
 * version 1.1.0
 * user password can be used in authentication as well
 * 
 * @class	EncryptionAuthentication
 * @author	WUZHUOBIN
 * @version	1.1.0
 * @since	2016.11.17
 *
 */


class EncryptionAuthentication: public QWidget
{
public:
	/**
	 * Return value of the following methods:
	 * @see	#authenticationBySoftID()
	 * @see	#authenticationByHardID()
	 * @see	#authenticationByProductName()
	 * @see	#authenticationByHavingKey()
	 * @see	#authenticationByExpiredDateTime()
	 * @see	#authenticationByUserPassword()
	 */
	enum AUTHENTICATION_FLAG
	{
		
		FAIL = 0, ///< Faied the authenticaiton
		SUCCESSFUL = 1 ///< Passed the authentication
	};
	/**
	 * Input parameters errorCode and return value of the following methods:
	 * @see	#authenticationExec()
	 * @see	#authenticationExecAndKeyType()
	 */
	enum AUTHENTICATION_ERROR_CODE {
		NORMAL = 0X00, ///< Passed
		HAVING_KEY = 0X01, ///< No key or multiple keys
		SOFT_ID = 0X02, ///< Wrong soft id
		HARD_ID = 0X04, ///< Wrong hard id
		PRODUCT_NAME = 0X08, ///< Wrong product name
		EXPIRED_DATE_TIME = 0X10, ///< software is expired
		USER_PASSWORD = 0X20 ///< user password
	};
	/**
	 * Constructor
	 * @see #reloadViKey()
	 * @see #authenticationByHardID()
	 * @see #authenticationBySoftID()
	 * @see #authenticationByProductName()
	 * @see #authenticationByProductNameAndKeyType()
	 * @see #authenticationByExpiredDateTime()
	 * @see #authenticationByExpiredDateTimeAndKeytype()
	 * @see #authenticationByUserPassword()
	 *
	 * Constructor function of the EncryptionAuthentication. It will call 
	 * #reloadViKey()  when the class is constructed for initialization.
	 * If the parameters are using the default initialzation value, it will directly pass
	 * the authentication.
	 * @param	hardID setting the #m_hardID. Checked by 
	 *			#authenticationByHardID(). 
	 * @param	softID setting the #m_softID. Checked by 
	 *			#authenticationBySoftID().
	 * @param	productName setting the #m_productName. Checked by 
	 *			#authenticationByProductName().
	 *			#authenticationByProductNameAndKeyType().
	 *			
	 * @param	expiredDateTime settting the #m_expiredDateTime. Checked by 
	 *			#authenticationByExpiredDateTime() &&
	 *			#authenticationByExpiredDateTimeAndKeytype().
	 * @param	userPassword setting the #m_userPassword. checked by 
	 *			#authenticationByUserPassword().
	 * @param	AdminPassword setting the #m_adminPassword. 
	 * @param	parent the parent QWidget.
	 */
	EncryptionAuthentication(
		unsigned long hardID = 0,
		QString softID = QString(),
		QString productName = QString(),
		QDateTime expiredDateTime = QDateTime(), 
		QString userPassword = "11111111",
		QString AdminPassword = "00000000",
		QWidget* parent = nullptr);
	/**
	 * Destructor
	 */
	~EncryptionAuthentication();
	/**
	 * set error message of the #m_messageBox 
	 * @see #m_messageBox
	 * the message is using a QMap for mapping, please only use the errorCode to specify 
	 * your own message.
	 * @param	errorCode The key of the map.
	 * @param	message The QString message you want the user to see.
	 */
	void setErrorMessage(int errorCode, QString message);

	/**
	 * method to check whether the USB key can pass the authentication
	 * @see #reloadViKey()
	 * @see #m_messages
	 * @see #authenticationBySoftID()
	 * @see #authenticationByHardID()
	 * @see #authenticationByProductName()
	 * @see #authenticationByHavingKey()
	 * @see #authenticationByExpiredDateTime()
	 * @see #authenticationByUserPassword()
	 * @see #AUTHENTICATION_ERROR_CODE
	 *
	 * 
	 * before it check USB key, it will always call reloadVikey bofore
	 * if fail the authentication, it will always pop up a QMessageBox::Critical messagebox
	 * with specifed messages in #m_messages, only the failed requested message will 
	 * pop up
	 * by clicking button "Retry", it will check the usb key again
	 * by clicking button "Cancel", it will exit the program
	 * using input parameters errorCode can specified your checking method
	 * using operator|() can implement multiple authentication
	 * it will call the following mothod
	 * @param	errorCode #NORMAL if this is the only input, it will always pass
	 *					  #HAVING_KEY pass only one right key is inserted
	 *					  #SOFT_ID pass only usb key with right soft_id
	 *					  #HARD_ID pass only usb key with right hard_id
	 *				      #PRODUCT_NAME pass only usb key with right product name
	 *					  #EXPIRED_DATE_TIME pass only the usb key's clock before expiredDateTime
	 *					  #USER_PASSWORD pass only the usb key's can be login with #m_userPassword successfully
	 * @return	#NORMAL, authentication passed.
	 *			others, a or by bit value of the AUTHENTICATION_ERROR_CODE, 
	 *			please refer to #AUTHENTICATION_ERROR_CODE.
	 */
	int authenticationExec(int errorCode);
	/**
	 * almost the same as authenticationExec 
	 * @see #authenticationExec
 	 * @see #authenticationByExpiredDateTime();
	 * @see #authenticationByProductName();	
	 * @see	#authenticationByExpiredDateTimeAndKeytype();
	 * @see #authenticationByProductNameAndKeyType();
	 * 
	 * However, because some usb key do not have clock and product name, this method is 
	 * for general used, but actually it is some kind of not checking.
	 * except for it call the following method: 
	 * #authenticationByProductNameAndKeyType()
	 * #authenticationByExpiredDateTimeAndKeytype()
	 * #authenticationByProductName()
	 * #authenticationByExpiredDateTime()
	 * instead of :
	 * #authenticationByExpiredDateTime()
	 * #authenticationByProductName().
	 * 
	 */
	int authenticationExecAndKeyType(int errorCode);
	/**
	 * method to set the #m_userPassword in case it has been changed.
	 * @param	userPassword the new password.
	 */
	void setUserPassword(QString userPassword);
	/**
	 * @deprecated
	 * since actually admin password is not used in the API, for future usage.
	 * method to set the #m_adminPassword in case it has been changed.
	 * @param	adminPassword the new password.
	 */
	void setAdminPassword(QString adminPassword);
	/**
	 * enable the hint of the expriedDateTimeHint
	 * @param	flag set true if pass the authentication, it will always pop up a hint.
	 *			set false no hint will pop up.
	 */
	void enableExpiredDateTimeHint(bool flag);


private:
	/**
	 * loading the usbkey, it must be invoked before doing any authentication.
	 */
	void reloadViKey();
	/**
	 * authenticationBySoftID, soft id is specified in the usb key.
	 * #m_softID is the same as the softid in usb key.
	 * if #m_softID is initialized by default paramter, it will always return #SUCCESSFUL.
	 * @return	#SUCCESSFUL, #m_softID is the same as the softid in usb key.
	 *			#FAIL, cannot read softid from usb key or incorrect softid.
	 */
	int authenticationBySoftID();
	/**
	 * authenticationByHardID, hard id is hardware id, which is an unique key of every 
	 * hardware, if using this authentication, only one usb key can pass this 
	 * authentication.
	 * if #m_hardID is initialized by default parameter, it will always return #SUCCESSFUL.
	 * @return	#SUCCESSFUL, if #m_hardID is the same as the hard id in usb key.
	 *			#FAIL, cannot read HardId or incorrect hardid.
	 */
	int authenticationByHardID();
	/**
	 * authenticationByProductName, Product name is specified in the usb key as well
	 * but not all usb key can specify it.
	 * if the #m_productName is initialized by the defult parameter, it will always return
	 * #SUCCESSFUL.
	 * @return	#SUCCESSFUL, if the #m_productName is the same as the product name in the 
	 *			usb key.
	 *			#FAIL, cannot read product name or incorrect product name.
	 */
	int authenticationByProductName();
	/**
	 * authenticationByHavingKey
	 * @return	#SUCCESSFUL, pass the authentication.
	 *			#FAIL, incorrect usb key inserted or multiple usb key inserted. 
	 */
	int authenticationByHavingKey();
	/**
	 * authenticationByExpiredDateTime, there is a clock in the usb key, 
	 * if the #m_expiredDateTime is initialzied by the default parameter, it will always 
	 * return #SUCCESSFUL
	 * @return	#SUCCESSFUL, if the #m_expiredDateTime later than the clock in the usb key.
	 *			#FAIL, cannot read the clock or #m_expiredDateTime earlier than 
	 *			the clock in the usb key.
	 *
	 */
	int authenticationByExpiredDateTime();
	/**
 	 * authenticationByExpiredDateTime, there is a clock in the usb key,
	 * if the #m_expiredDateTime is initialzied by the default parameter, it will always
	 * return #SUCCESSFUL.
	 * @return	#SUCCESSFUL, if the #m_expiredDateTime later than the clock in the usb key
 	 *			or cannot read the clock.
	 *			#FAIL,  #m_expiredDateTime earlier than
	 *			the clock in the usb key.
	 *
	 */
	int authenticationByExpiredDateTimeAndKeytype();
	/**
	 * authenticationByProductName, Product name is specified in the usb key as well
	 * but not all usb key can specify it.
	 * if the #m_productName is initialized by the defult parameter, it will always return
	 * #SUCCESSFUL.
	 * @return	#SUCCESSFUL, if the #m_productName is the same as the product name in the
	 *			usb key or cannot read product name.
	 *			#FAIL,  incorrect product name.
	 */
	int authenticationByProductNameAndKeyType();
	/**
	 * authenticationByUserPassword, checking whether the #m_userPassword can be used in 
	 * login the usb key. if #m_userPassword can be used in login, it return #SUCCESSFUL.
	 * @return	#SUCCESSFUL, if the #m_userPassword is corret.
	 *			#FAIL, if not.
	 */
	int authenticationByUserPassword();


	bool  m_expiredDateTimeHintFlag = true;

	unsigned long m_errorType = 0;	///< please refer to the Vikey.h and its manual

	unsigned long m_hardID;
	QString m_softID;
	QString m_productName;
	QDateTime m_expiredDateTime;
	QString m_userPassword;
	QString m_adminPassword;

	QMessageBox m_messageBox;

	/**
     * Pre-filled AUTHENTICATION_ERROR_CODE messages is as follow:
     * m_messages[NORMAL] = "This product will be expired at " +
     * m_expiredDateTime.toString("hh:mm:ss dd/MMM/yyyy") + ".";
     * m_messages[HAVING_KEY] = "Please insert your USB key, and make sure only one key inserted.";
     * m_messages[SOFT_ID] = "Please insert USB key with right Soft ID " + m_softID;
     * m_messages[HARD_ID] = "Please insert USB key with right Hard ID(Unique ID).";
     * m_messages[PRODUCT_NAME] = "Please insert USB key for product's name is " +
     * m_productName;
     * m_messages[EXPIRED_DATE_TIME] = "This product has already been expired at " +
     * m_expiredDateTime.toString("hh:mm:ss dd/MMM/yyyy") +
     * "." + "Please ask the manufacture for a renewed version. ";
     *
     */
	QMap<int, QString> m_messages;

};

