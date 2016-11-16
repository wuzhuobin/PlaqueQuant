#ifndef VIKEY_API
#define VIKEY_API

enum VikeyType
{
	ViKeyAPP = 0,						//实用型加密狗
	ViKeySTD = 1,						//标准型加密狗
	ViKeyNET = 2,						//网络型加密狗
	ViKeyPRO = 3,						//专业型加密狗
	ViKeyWEB = 4,						//身份认证型加密狗
	ViKeyTIME = 5,						//时钟型加密狗，内部独立时钟
	ViKeyMultiFunctional = 0x0A,		//多功能加密狗  支持软件加密 支持文档加密
	ViKeyMultiFunctionalTime = 0x0B,	//多功能时钟加密狗
	ViKeyInvalid	//无效类型
};


#define ViKeyNoLevel		0	//匿名权限(没有登录过加密狗时，此时权限为匿名权限)
#define ViKeyUserLevel		1	//用户权限(调用VikeyUserLogin登录加密狗成功后，此时权限为用户权限)
#define ViKeyAdminLevel		2	//管理员权限(调用VikeyAdminLogin登录加密狗成功后，此时权限为管理员权限)

#define VIKEY_SUCCESS						0x00000000 //成功
#define VIKEY_ERROR_NO_VIKEY				0x80000001 //没有找到ViKey加密锁
#define VIKEY_ERROR_INVALID_PASSWORD		0x80000002 //密码错误
#define VIKEY_ERROR_NEED_FIND				0x80000003 //请先查找加密锁
#define VIKEY_ERROR_INVALID_INDEX			0x80000004 //无效的句柄
#define VIKEY_ERROR_INVALID_VALUE			0x80000005 //数值错误
#define VIKEY_ERROR_INVALID_KEY				0x80000006 //秘钥无效
#define VIKEY_ERROR_GET_VALUE				0x80000007 //读取信息错误
#define VIKEY_ERROR_SET_VALUE				0x80000008 //设置信息错误
#define VIKEY_ERROR_NO_CHANCE				0x80000009 //没有机会
#define VIKEY_ERROR_NO_TAUTHORITY			0x8000000A //权限不足
#define VIKEY_ERROR_INVALID_ADDR_OR_SIZE	0x8000000B //地址或长度错误
#define VIKEY_ERROR_RANDOM					0x8000000C //获取随机数错误
#define VIKEY_ERROR_SEED					0x8000000D //获取种子错误
#define VIKEY_ERROR_CONNECTION				0x8000000E //通信错误
#define VIKEY_ERROR_CALCULATE				0x8000000F //算法或计算错误
#define VIKEY_ERROR_MODULE					0x80000010 //计数器错误
#define VIKEY_ERROR_GENERATE_NEW_PASSWORD	0x80000011 //产生密码错误
#define VIKEY_ERROR_ENCRYPT_FAILED			0x80000012 //加密数据错误
#define VIKEY_ERROR_DECRYPT_FAILED			0x80000013 //解密数据错误
#define VIKEY_ERROR_ALREADY_LOCKED			0x80000014 //ViKey加密锁已经被锁定
#define VIKEY_ERROR_UNKNOWN_COMMAND			0x80000015 //无效的命令
#define VIKEY_ERROR_NO_SUPPORT				0x80000016 //当前ViKey加密锁不支持此功能
#define VIKEY_ERROR_UNKNOWN_ERROR			0xFFFFFFFF //未知错误

#ifdef __cplusplus
extern "C"{
#endif

//【说明】为了让程序员对以下函数一目了然，这里只列出函数的原型，对于函数功能和参数的解释在帮助手册中列出。
//【说明】请到“1-[产品文档]”目录下查看《ViKey系列加密狗使用说明手册》文档。

/*********For ViKeyAPP、ViKeySTD、ViKeyNet、ViKeyPRO、ViKeyTime APIs**********/

DWORD __stdcall VikeyFind(DWORD* pdwCount);
DWORD __stdcall VikeyGetHID(WORD Index, DWORD *pdwHID);
DWORD __stdcall VikeyGetType(WORD Index, VikeyType *pType);
DWORD __stdcall VikeyGetLevel(WORD Index, BYTE *pLevel);
DWORD __stdcall VikeySetPtroductName(WORD Index, WCHAR szName[16]);
DWORD __stdcall VikeyGetPtroductName(WORD Index, WCHAR szName[16]);
DWORD __stdcall VikeyGetPtroductNameA(WORD Index, CHAR szName[16]);

//登陆、注销加密狗
//加密狗密码长度8个字符 字母或数字
DWORD __stdcall VikeyUserLogin(WORD Index, char * pUserPassWord);
DWORD __stdcall VikeyAdminLogin(WORD Index, char * pAdminPassWord);
DWORD __stdcall VikeyLogoff(WORD Index);

//设置密码尝试次数、修改密码
DWORD __stdcall VikeySetUserPassWordAttempt(WORD Index, BYTE cAttempt);
DWORD __stdcall VikeySetAdminPassWordAttempt(WORD Index, BYTE cAttempt);
DWORD __stdcall VikeyGetUserPassWordAttempt(WORD Index, BYTE *pcCurrentAttempt, BYTE *pcMaxAttempt);
DWORD __stdcall VikeyGetAdminPassWordAttempt(WORD Index, BYTE *pcCurrentAttempt, BYTE *pcMaxAttempt);
DWORD __stdcall VikeyResetPassword(WORD Index, char * pNewUserPassWord, char * pNewAdminPassWord);

//软件ID读写
//软件ID长度8个字符 字母或数字
DWORD __stdcall VikeySetSoftIDString(WORD Index, char * pSoftIDString);
DWORD __stdcall VikeyGetSoftIDString(WORD Index, char * pSoftIDString);

//读写数据
DWORD __stdcall VikeyReadData(WORD Index, WORD Addr, WORD Length, BYTE * buffer);
DWORD __stdcall VikeyWriteData(WORD Index, WORD Addr, WORD Length, BYTE * buffer);
DWORD __stdcall ViKeyRandom(WORD Index, WORD* pwRandom1, WORD* pwRandom2, WORD* pwRandom3, WORD* pwRandom4);

//计数器模块
DWORD __stdcall ViKeyDecraseModule(WORD Index, WORD wModuleIndex);
DWORD __stdcall ViKeyGetModule(WORD Index, WORD wModuleIndex, WORD* pwValue);
DWORD __stdcall ViKeySetModule(WORD Index, WORD wModuleIndex, WORD wValue, WORD wMode);
DWORD __stdcall ViKeyCheckModule(WORD Index, WORD wModuleIndex, WORD *IsZero, WORD* CanDecrase);

//加解密
enum Des3KeyLengthType
{
	Des3KeyLength16,
	Des3KeyLength24
};

DWORD __stdcall Vikey3DesSetKey(WORD Index, BYTE * pKey, Des3KeyLengthType KeyType);
DWORD __stdcall Vikey3DesEncrypt(WORD Index, WORD length, BYTE * pText, BYTE* pResult);
DWORD __stdcall Vikey3DesDecrypt(WORD Index, WORD length, BYTE * pText, BYTE* pResult);

DWORD __stdcall VikeyDesSetKey(WORD Index, BYTE * pKey);
DWORD __stdcall VikeyDesEncrypt(WORD Index, WORD InLength, BYTE * pText, BYTE* pResult, WORD *OutLength);
DWORD __stdcall VikeyDesDecrypt(WORD Index, WORD InLength, BYTE * pText, BYTE* pResult, WORD *OutLength);

//自动打开网页接口
DWORD __stdcall VikeySetAutoRunUrl(WORD Index, BYTE *pUrl);
DWORD __stdcall VikeyGetAutoRunUrl(WORD Index, BYTE *pUrl);

//For ViKeyNET、ViKeyPRO
DWORD __stdcall VikeySetMaxClientCount(WORD Index, WORD dwCount);
DWORD __stdcall VikeyGetMaxClientCount(WORD Index, WORD* pdwCount);

//For ViKeyWEB、ViKeyPRO
DWORD __stdcall VikeyMD5(WORD Index, WORD length, BYTE * pText, BYTE* pResult);
DWORD __stdcall VikeySetMD5Key(WORD Index, BYTE * pMD5key);
DWORD __stdcall VikeyHmacMD5(WORD Index, WORD length, BYTE * pText, BYTE* pResult);

DWORD __stdcall VikeySHA1(WORD Index, WORD length, BYTE * pText, BYTE* pResult);
DWORD __stdcall VikeySetSHA1Key(WORD Index, BYTE * pSHA1key);
DWORD __stdcall VikeyHmacSHA1(WORD Index, WORD length, BYTE * pText, BYTE* pResult);


//For ViKeyTIME
typedef struct _VIKEY_TIME 
{ 
	BYTE cYear; 
	BYTE cMonth; 
	BYTE cDay; 
	BYTE cHour; 
	BYTE cMinute; 
	BYTE cSecond; 

	bool operator < (const _VIKEY_TIME &another) const;
	bool operator > (const _VIKEY_TIME &another) const;
} SVikeyTime, *PVIKEYTIME;

//获取时钟型加密狗中的内部时间
DWORD __stdcall VikeyGetTime(WORD Index, PVIKEYTIME pTime);


#ifdef __cplusplus
} //  extern "C"{
#endif

#endif
