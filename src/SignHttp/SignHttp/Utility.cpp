/*****************************************************************************
* Copyright (c) 2016 Jiocloud.com, Inc. or its affiliates.  All Rights Reserved
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish, dis-
* tribute, sublicense, and/or sell copies of the Software, and to permit
* persons to whom the Software is furnished to do so, subject to the fol-
* lowing conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
* OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABIL-
* ITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
* SHALL THE AUTHOR BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
******************************************************************************/

#include <map>
#include <string.h>
#include <iostream>
#include <fstream>
#include <openssl/hmac.h>
#include <openssl/bio.h>
#include <openssl/evp.h>
#include <openssl/pem.h>
#include <openssl/md5.h>
#include <algorithm>
#include"Utility.h"


using namespace std;

namespace utils
{
	void hmac_sha256(const std::string &canonical_string,const char *secret_key,std::string &encodedStr)
	{
		unsigned len = 32;
		/*unsigned char hmac_256[len+1];*/
		unsigned char hmac_256[33] = { 0 };
		  
	    HMAC_CTX ctx;
	    HMAC_CTX_init(&ctx);
		HMAC_Init_ex(&ctx, secret_key, strlen(secret_key), EVP_sha256(), NULL);
		HMAC_Update(&ctx,(unsigned char*)(canonical_string.c_str()), strlen(canonical_string.c_str()));
		HMAC_Final(&ctx, hmac_256, &len);
	   	HMAC_CTX_cleanup(&ctx);
	   	//hmac_256[len]='\0';
		encodedStr = std::string((const char*)hmac_256, len);
	}
	//std::string base64encode(const char * instring, size_t len)
	//{	
	//	BIO *bio, *b64;
	//	const size_t mlen = len*8/6 + len%6;	
	//	char b64message[mlen+1];
	//	b64 = BIO_new(BIO_f_base64());
	//	bio = BIO_new(BIO_s_mem());
	//	BIO_push(b64, bio);
	//	BIO_set_flags(b64,BIO_FLAGS_BASE64_NO_NL);
	//	BIO_write(b64, instring, len);
	//	BIO_flush(b64);
	//	int length = BIO_read(bio, b64message, mlen);
	//	BIO_free_all(b64);
	//	b64message[length]='\0';
	//	return b64message;

	//}

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

	void base64decode(const char *decode, char *decoded, size_t len, int& de_len)
	{ 
		BIO *bio, *b64;
		const size_t mlen = (len*6)/8 + len%8;
		
		int inlen;
		b64 = BIO_new(BIO_f_base64());
		bio = BIO_new(BIO_s_mem());
		
		BIO_set_flags(b64,BIO_FLAGS_BASE64_NO_NL);
		BIO_write(bio, decode, len+1);
		BIO_push(b64, bio);
		
		inlen = BIO_read(b64, decoded, mlen);
		decoded[inlen]='\0';
		de_len = inlen;

		BIO_flush(b64);
		BIO_free_all(b64);
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
