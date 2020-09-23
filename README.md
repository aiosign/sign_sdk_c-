# 大众签章开放平台Api接口C++版SDK


#### 环境说明：
- 1.您需要去大众签章开放平台注册一个开发者账号，并且申请一个应用，当您的应用经过审核之后，您将获得一个appid和appsecret，这两个参数决定了您在开放平台的身份标识，只有获得了这两个参数，您才有资格调用开放平台的api接口。
- 2.您需要凭借appid和appsecret先获取token，有些接口还需要传递签名值(后续会详细介绍),才可调用开放平台的api接口，签名算法已封装在DLL里面。
- 3.SDK使用Wininet.lib对HTTPS网络请求进行封装。
### 使用说明：
- 1.您可以从github上直接下载的sdk，通过提供动态库SignHttp.dll，静态库SignHttp.lib和头文件SignHttp.h供外部使用。其中sdk里面还包含几个依赖库，主要是封装签名算法时候用到。
- 2.以下这个代码示例向您展示了调用 Sign SDK 过程：
```c++
//注册个人信息并申请证书
void DataEncToolDlg::RegisterPersonal()
{
	QString strBaseUrl = "https://open.aiosign.com/api/v1";
	QString strUrl = strBaseUrl + QString("/user/personal/register?access_token=%1").arg(m_strToken);
	
	//填充json数据
	Json::Value root;
	Json::FastWriter write;
	root["user_name"] = "测试003";
	root["area_code"] = "370101";
	root["phone"] = "15118014986";
	root["id_type"] = "111";
	root["id_number"] = "370101198001016111";
	root["mail"] = "1562310354@qq.com";
	root["description"] = "测试003";
	std::string strJson = write.write(root);

	string strData;
	//调用注册个人信息并申请证书接口
	Http_RegisterPersonalInfo(strUrl.toStdString().c_str(), /*"ZBvjjigCFjizUwdDny"*/m_strAppSecret.toStdString().c_str(), strJson.c_str(), strData);
	ui->EnctextEdit->clear();
	ui->EnctextEdit->setText(str2qstr(strData));
}
```

SDK中都有相关接口的测试用例，都已经经过相关测试，您可以修改参数执行相关方法（可以参考QT写的DEMO版本）。

### 额外说明：
- 1.在初始化之前先调用记录日志接口
```c++
/*初始化日志*/
SIGNHTTP int Http_InitLog(const char* szPath,const char* szFilename);
```
- 2.签名算法
开放平台api接口的所有的post请求并且请求头为json的接口添加了签名值的校验，签名算法的机制如下： 比如你的请求json为：
```json
{
"app_id": "710510245885661184",
"app_secret": "UJhgoFkMShBtLXcqlC",
"grant_type": "client_credentials"
}
```
首先对字符串做去空格化处理（包括\r\n），然后对字符串做HmacSHA256算法，秘钥为您的appsecret，编码一定要为UTF-8，否则可能会导致获取的加密值不同，然后转为base64编码的字符串的加密字符串，最后对该字符串做MD5摘要，最后的字符串将作为最终的签名值。 签名的工具类代码如下：
```c++
namespace utils
{
	void hmac_sha256(const std::string &canonical_string,const char *secret_key,std::string &encodedStr)
	{
		unsigned len = 32;
		unsigned char hmac_256[33] = { 0 };
		  
	    HMAC_CTX ctx;
	    HMAC_CTX_init(&ctx);
		HMAC_Init_ex(&ctx, secret_key, strlen(secret_key), EVP_sha256(), NULL);
		HMAC_Update(&ctx,(unsigned char*)(canonical_string.c_str()), strlen(canonical_string.c_str()));
		HMAC_Final(&ctx, hmac_256, &len);
	   	HMAC_CTX_cleanup(&ctx);
		encodedStr = std::string((const char*)hmac_256, len);
	}
	void base64encode(const char * instring, size_t len, char * buffer, int & base64_size)
	{
		BIO *b64 = BIO_new(BIO_f_base64());
		BIO *bio = BIO_new(BIO_s_mem());

		bio = BIO_push(b64, bio);
		BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
		BIO_write(bio,instring, len);
		BIO_ctrl(bio, BIO_CTRL_FLUSH, 0, NULL);

		BUF_MEM *bptr = NULL;
		BIO_get_mem_ptr(bio, &bptr);

		size_t slen = bptr->length;
		memcpy(buffer, bptr->data, slen);
		buffer[slen] = '\0';
		base64_size = slen;

		BIO_free_all(bio);
	}
	void md5(const std::string &srcStr, std::string &encodedHexStr)
	{
		// 调用md5哈希    
		unsigned char mdStr[33] = { 0 };
		MD5((const unsigned char *)srcStr.c_str(), srcStr.length(), mdStr);

		// 哈希后的字符串    
		//encodedStr = std::string((const char *)mdStr);
		// 哈希后的十六进制串 32字节    
		char buf[65] = { 0 };
		char tmp[3] = { 0 };
		for (int i = 0; i < 32; i++)
		{
			sprintf(tmp, "%02x", mdStr[i]);
			strcat(buf, tmp);
		}
		buf[32] = '\0'; // 后面都是0，从32字节截断    
		encodedHexStr = std::string(buf);
		transform(encodedHexStr.begin(), encodedHexStr.end(), encodedHexStr.begin(), ::toupper);
	}
}
//获取签名数据
std::string CHttp::GetSignData(const std::string & strSrc, const std::string& secret_key)
{
	std::string strTemp = strSrc.substr(0, strSrc.find_last_of("}") + 1);
	int r = strTemp.find('\r\n');
	while (r != string::npos)
	{
		if (r != string::npos)
		{
			strTemp.replace(r, 1, "");
			r = strTemp.find('\r\n');
		}
	}
	int index = 0;
	if (!strTemp.empty())
	{
		while ((index = strTemp.find(' ', index)) != string::npos)
		{
			strTemp.erase(index, 1);
		}
	}
	std::string strRetData;
	hmac_sha256(strTemp, secret_key.c_str(), strRetData);
	char *szbase64 = new char[strRetData.length() * 3];
	int nbase64len = 0;
	base64encode(strRetData.c_str(), strRetData.length(), szbase64, nbase64len);
	strRetData.clear();
	md5(szbase64,strRetData);
	return std::move(strRetData);
}
```
