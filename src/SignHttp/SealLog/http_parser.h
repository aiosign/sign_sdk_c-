/* Copyright Joyent, Inc. and other Node contributors. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
#ifndef http_parser_h
#define http_parser_h
#ifdef __cplusplus
extern "C" {
#endif

/* Also update SONAME in the Makefile whenever you change these. */
#define HTTP_PARSER_VERSION_MAJOR 2
#define HTTP_PARSER_VERSION_MINOR 7
#define HTTP_PARSER_VERSION_PATCH 1

#include <sys/types.h>
#if defined(_WIN32) && !defined(__MINGW32__) && \
  (!defined(_MSC_VER) || _MSC_VER<1600) && !defined(__WINE__)
#include <BaseTsd.h>
#include <stddef.h>
typedef __int8 int8_t;
typedef unsigned __int8 uint8_t;
typedef __int16 int16_t;
typedef unsigned __int16 uint16_t;
typedef __int32 int32_t;
typedef unsigned __int32 uint32_t;
typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#else
#include <stdint.h>
#endif

/* Compile with -DHTTP_PARSER_STRICT=0 to make less checks, but run
 * faster
 */
#ifndef HTTP_PARSER_STRICT
# define HTTP_PARSER_STRICT 1
#endif

/* Maximium header size allowed. If the macro is not defined
 * before including this header then the default is used. To
 * change the maximum header size, define the macro in the build
 * environment (e.g. -DHTTP_MAX_HEADER_SIZE=<value>). To remove
 * the effective limit on the size of the header, define the macro
 * to a very large number (e.g. -DHTTP_MAX_HEADER_SIZE=0x7fffffff)
 */
#ifndef HTTP_MAX_HEADER_SIZE
# define HTTP_MAX_HEADER_SIZE (80*1024)
#endif

typedef struct http_parser http_parser;
typedef struct http_parser_settings http_parser_settings;


/* Callbacks should return non-zero to indicate an error. The parser will
 * then halt execution.
 *
 * The one exception is on_headers_complete. In a HTTP_RESPONSE parser
 * returning '1' from on_headers_complete will tell the parser that it
 * should not expect a body. This is used when receiving a response to a
 * HEAD request which may contain 'Content-Length' or 'Transfer-Encoding:
 * chunked' headers that indicate the presence of a body.
 *
 * Returning `2` from on_headers_complete will tell parser that it should not
 * expect neither a body nor any futher responses on this connection. This is
 * useful for handling responses to a CONNECT request which may not contain
 * `Upgrade` or `Connection: upgrade` headers.
 *
 * http_data_cb does not return data chunks. It will be called arbitrarily
 * many times for each string. E.G. you might get 10 callbacks for "on_url"
 * each providing just a few characters more data.
 */
typedef int (*http_data_cb) (http_parser*, const char *at, size_t length);
typedef int (*http_cb) (http_parser*);

enum state
  { s_dead = 1 /* important that this is > 0 */

  , s_start_req_or_res
  , s_res_or_resp_H
  , s_start_res
  , s_res_H
  , s_res_HT
  , s_res_HTT
  , s_res_HTTP
  , s_res_first_http_major
  , s_res_http_major
  , s_res_first_http_minor
  , s_res_http_minor
  , s_res_first_status_code
  , s_res_status_code
  , s_res_status_start
  , s_res_status
  , s_res_line_almost_done

  , s_start_req

  , s_req_method
  , s_req_spaces_before_url
  , s_req_schema
  , s_req_schema_slash
  , s_req_schema_slash_slash
  , s_req_server_start
  , s_req_server
  , s_req_server_with_at
  , s_req_path
  , s_req_query_string_start
  , s_req_query_string
  , s_req_fragment_start
  , s_req_fragment
  , s_req_http_start
  , s_req_http_H
  , s_req_http_HT
  , s_req_http_HTT
  , s_req_http_HTTP
  , s_req_first_http_major
  , s_req_http_major
  , s_req_first_http_minor
  , s_req_http_minor
  , s_req_line_almost_done

  , s_header_field_start
  , s_header_field
  , s_header_value_discard_ws
  , s_header_value_discard_ws_almost_done
  , s_header_value_discard_lws
  , s_header_value_start
  , s_header_value
  , s_header_value_lws

  , s_header_almost_done

  , s_chunk_size_start
  , s_chunk_size
  , s_chunk_parameters
  , s_chunk_size_almost_done

  , s_headers_almost_done
  , s_headers_done

  /* Important: 's_headers_done' must be the last 'header' state. All
   * states beyond this must be 'body' states. It is used for overflow
   * checking. See the PARSING_HEADER() macro.
   */

  , s_chunk_data
  , s_chunk_data_almost_done
  , s_chunk_data_done

  , s_body_identity
  , s_body_identity_eof

  , s_message_done
  };


/* Request Methods */
#define HTTP_METHOD_MAP(XX)         \
  XX(0,  DELETE,      DELETE)       \
  XX(1,  GET,         GET)          \
  XX(2,  HEAD,        HEAD)         \
  XX(3,  POST,        POST)         \
  XX(4,  PUT,         PUT)          \
  /* pathological */                \
  XX(5,  CONNECT,     CONNECT)      \
  XX(6,  OPTIONS,     OPTIONS)      \
  XX(7,  TRACE,       TRACE)        \
  /* WebDAV */                      \
  XX(8,  COPY,        COPY)         \
  XX(9,  LOCK,        LOCK)         \
  XX(10, MKCOL,       MKCOL)        \
  XX(11, MOVE,        MOVE)         \
  XX(12, PROPFIND,    PROPFIND)     \
  XX(13, PROPPATCH,   PROPPATCH)    \
  XX(14, SEARCH,      SEARCH)       \
  XX(15, UNLOCK,      UNLOCK)       \
  XX(16, BIND,        BIND)         \
  XX(17, REBIND,      REBIND)       \
  XX(18, UNBIND,      UNBIND)       \
  XX(19, ACL,         ACL)          \
  /* subversion */                  \
  XX(20, REPORT,      REPORT)       \
  XX(21, MKACTIVITY,  MKACTIVITY)   \
  XX(22, CHECKOUT,    CHECKOUT)     \
  XX(23, MERGE,       MERGE)        \
  /* upnp */                        \
  XX(24, MSEARCH,     M-SEARCH)     \
  XX(25, NOTIFY,      NOTIFY)       \
  XX(26, SUBSCRIBE,   SUBSCRIBE)    \
  XX(27, UNSUBSCRIBE, UNSUBSCRIBE)  \
  /* RFC-5789 */                    \
  XX(28, PATCH,       PATCH)        \
  XX(29, PURGE,       PURGE)        \
  /* CalDAV */                      \
  XX(30, MKCALENDAR,  MKCALENDAR)   \
  /* RFC-2068, section 19.6.1.2 */  \
  XX(31, LINK,        LINK)         \
  XX(32, UNLINK,      UNLINK)       \

enum http_method
  {
#define XX(num, name, string) HTTP_##name = num,
  HTTP_METHOD_MAP(XX)
#undef XX
  };


enum http_parser_type { HTTP_REQUEST, HTTP_RESPONSE, HTTP_BOTH };


/* Flag values for http_parser.flags field */
enum flags
  { F_CHUNKED               = 1 << 0
  , F_CONNECTION_KEEP_ALIVE = 1 << 1
  , F_CONNECTION_CLOSE      = 1 << 2
  , F_CONNECTION_UPGRADE    = 1 << 3
  , F_TRAILING              = 1 << 4
  , F_UPGRADE               = 1 << 5
  , F_SKIPBODY              = 1 << 6
  , F_CONTENTLENGTH         = 1 << 7
  };


/* Map for errno-related constants
 *
 * The provided argument should be a macro that takes 2 arguments.
 */
#define HTTP_ERRNO_MAP(XX)                                           \
  /* No error */                                                     \
  XX(OK, "success")                                                  \
                                                                     \
  /* Callback-related errors */                                      \
  XX(CB_message_begin, "the on_message_begin callback failed")       \
  XX(CB_url, "the on_url callback failed")                           \
  XX(CB_header_field, "the on_header_field callback failed")         \
  XX(CB_header_value, "the on_header_value callback failed")         \
  XX(CB_headers_complete, "the on_headers_complete callback failed") \
  XX(CB_body, "the on_body callback failed")                         \
  XX(CB_message_complete, "the on_message_complete callback failed") \
  XX(CB_status, "the on_status callback failed")                     \
  XX(CB_chunk_header, "the on_chunk_header callback failed")         \
  XX(CB_chunk_complete, "the on_chunk_complete callback failed")     \
                                                                     \
  /* Parsing-related errors */                                       \
  XX(INVALID_EOF_STATE, "stream ended at an unexpected time")        \
  XX(HEADER_OVERFLOW,                                                \
     "too many header bytes seen; overflow detected")                \
  XX(CLOSED_CONNECTION,                                              \
     "data received after completed connection: close message")      \
  XX(INVALID_VERSION, "invalid HTTP version")                        \
  XX(INVALID_STATUS, "invalid HTTP status code")                     \
  XX(INVALID_METHOD, "invalid HTTP method")                          \
  XX(INVALID_URL, "invalid URL")                                     \
  XX(INVALID_HOST, "invalid host")                                   \
  XX(INVALID_PORT, "invalid port")                                   \
  XX(INVALID_PATH, "invalid path")                                   \
  XX(INVALID_QUERY_STRING, "invalid query string")                   \
  XX(INVALID_FRAGMENT, "invalid fragment")                           \
  XX(LF_EXPECTED, "LF character expected")                           \
  XX(INVALID_HEADER_TOKEN, "invalid character in header")            \
  XX(INVALID_CONTENT_LENGTH,                                         \
     "invalid character in content-length header")                   \
  XX(UNEXPECTED_CONTENT_LENGTH,                                      \
     "unexpected content-length header")                             \
  XX(INVALID_CHUNK_SIZE,                                             \
     "invalid character in chunk size header")                       \
  XX(INVALID_CONSTANT, "invalid constant string")                    \
  XX(INVALID_INTERNAL_STATE, "encountered unexpected internal state")\
  XX(STRICT, "strict mode assertion failed")                         \
  XX(PAUSED, "parser is paused")                                     \
  XX(UNKNOWN, "an unknown error occurred")


/* Define HPE_* values for each errno value above */
#define HTTP_ERRNO_GEN(n, s) HPE_##n,
enum http_errno {
  HTTP_ERRNO_MAP(HTTP_ERRNO_GEN)
};
#undef HTTP_ERRNO_GEN


/* Get an http_errno value from an http_parser */
#define HTTP_PARSER_ERRNO(p)            ((enum http_errno) (p)->http_errno)


struct http_parser {
  /** PRIVATE **/
  unsigned char type;         /* enum http_parser_type */
  unsigned char flags;        /* F_* values from 'flags' enum; semi-public */
  unsigned char state;        /* enum state from http_parser.c */
  unsigned char header_state;	/* enum header_state from http_parser.c */
  unsigned char index;			/* index into current matcher */
  unsigned char lenient_http_headers;

  //const char* _headkey;				/*header param key begin address*/
  //unsigned int _headkeylen;			/*headkey length*/

  unsigned int _headeroffset;		/*http head end offset include '\r\n\r\n' length*/

  unsigned int ireadbytes;/*for one request,read bytes*/

  uint32_t nread;          /* # bytes read in various scenarios */
  uint64_t content_length; /* # bytes in body (0 if no Content-Length header) */

  /** READ-ONLY **/
  //unsigned short http_major;
  //unsigned short http_minor;
  unsigned short status_code; /* responses only */
  unsigned char method;       /* requests only */
  unsigned char http_errno;
  //volatile int mi_inUse;//是否在使用中

  /* 1 = Upgrade header was present and the parser has exited because of that.
   * 0 = No upgrade header present.
   * Should be checked when http_parser_execute() returns in addition to
   * error checking.
   */
  //unsigned char upgrade;

  //void* blobbody;//存放纯净的httpbody的blob以防多次解析http
  //void* blobrecv;//传入的blob buff,可能需要在完成的时候move
  /** PUBLIC **/
  void *data; /* A pointer to get hook to the "connection" or "socket" object */
};


struct http_parser_settings {
  http_cb      on_message_begin;
  http_data_cb on_url;
  http_data_cb on_status;
  http_data_cb on_header_field;
  http_data_cb on_header_value;
  http_cb      on_headers_complete;
  http_data_cb on_body;
  http_cb on_message_complete;
  /* When on_chunk_header is called, the current chunk length is stored
   * in parser->content_length.
   */
  http_cb      on_chunk_header;
  http_cb      on_chunk_complete;
};


enum http_parser_url_fields
  { UF_SCHEMA           = 0
  , UF_HOST             = 1
  , UF_PORT             = 2
  , UF_PATH             = 3
  , UF_QUERY            = 4
  , UF_FRAGMENT         = 5
  , UF_USERINFO         = 6
  , UF_MAX              = 7
  };


/* Result structure for http_parser_parse_url().
 *
 * Callers should index into field_data[] with UF_* values iff field_set
 * has the relevant (1 << UF_*) bit set. As a courtesy to clients (and
 * because we probably have padding left over), we convert any port to
 * a uint16_t.
 */
struct http_parser_url {
  uint16_t field_set;           /* Bitmask of (1 << UF_*) values */
  uint16_t port;                /* Converted UF_PORT string */

  struct {
    uint16_t off;               /* Offset into buffer in which field starts */
    uint16_t len;               /* Length of run in buffer */
  } field_data[UF_MAX];
};


/* Returns the library version. Bits 16-23 contain the major version number,
 * bits 8-15 the minor version number and bits 0-7 the patch level.
 * Usage example:
 *
 *   unsigned long version = http_parser_version();
 *   unsigned major = (version >> 16) & 255;
 *   unsigned minor = (version >> 8) & 255;
 *   unsigned patch = version & 255;
 *   printf("http_parser v%u.%u.%u\n", major, minor, patch);
 */
unsigned long http_parser_version(void);

void http_parser_init(http_parser *parser, enum http_parser_type type);


/* Initialize http_parser_settings members to 0
 */
void http_parser_settings_init(http_parser_settings *settings);


/* Executes the parser. Returns number of parsed bytes. Sets
 * `parser->http_errno` on error. */
size_t http_parser_execute(http_parser *parser,
                           const http_parser_settings *settings,
                           const char *data,
                           size_t len);


/* If http_should_keep_alive() in the on_headers_complete or
 * on_message_complete callback returns 0, then this should be
 * the last message on the connection.
 * If you are the server, respond with the "Connection: close" header.
 * If you are the client, close the connection.
 */
int http_should_keep_alive(const http_parser *parser);

/* Returns a string version of the HTTP method. */
const char *http_method_str(enum http_method m);

/* Return a string name of the given error */
const char *http_errno_name(enum http_errno err);

/* Return a string description of the given error */
const char *http_errno_description(enum http_errno err);

/* Initialize all http_parser_url members to 0 */
void http_parser_url_init(struct http_parser_url *u);

/* Parse a URL; return nonzero on failure */
int http_parser_parse_url(const char *buf, size_t buflen,
                          int is_connect,
                          struct http_parser_url *u);

/* Pause or un-pause the parser; a nonzero value pauses */
void http_parser_pause(http_parser *parser, int paused);

/* Checks if this is the final chunk of the body. */
int http_body_is_final(const http_parser *parser);

#ifdef __cplusplus
}
#endif
#endif

/*
 demo:

char *
create_large_chunked_message (int body_size_in_kb, const char* headers)
{
int i;
size_t wrote = 0;
size_t headers_len = strlen(headers);
size_t bufsize = headers_len + (5+1024+2)*body_size_in_kb + 6;
char * buf = (char*)malloc(bufsize);

memcpy(buf, headers, headers_len);
wrote += headers_len;

for (i = 0; i < body_size_in_kb; i++) {
// write 1kb chunk into the body.
memcpy(buf + wrote, "400\r\n", 5);
wrote += 5;
memset(buf + wrote, 'C', 1024);
wrote += 1024;
strcpy(buf + wrote, "\r\n");
wrote += 2;
}

memcpy(buf + wrote, "0\r\n\r\n", 6);
wrote += 6;
assert(wrote == bufsize);

return buf;
}

int body_cb(http_parser *p, const char *buf, size_t len)
{
//assert(p == parser);
//string str(buf,len);
printf("body_cb,body_is_final:%d, len:%d\n",http_body_is_final(p),len);

//printf("body_cb,body_is_final:%d, body:%s\n",http_body_is_final(p),str.c_str());
HCommonFun::WriteFileAfter("d:/share/abc.txt",buf,len);
return 0;
}

int message_complete_cb(http_parser *p, const char *buf, size_t len)
{
printf("message_complete_cb,body_is_final:%d, \n",http_body_is_final(p));
return 0;
}
int header_field_cb(http_parser *p, const char *buf, size_t len)
{
printf("header_field_cb ...\n");
//assert(p == parser);
//string str(buf,len);
//printf("body_cb,body_is_final:%d, body:%s\n",http_body_is_final(p),str.c_str());
return 0;
}

int header_value_cb(http_parser *p, const char *buf, size_t len)
{
string strkey(p->_headkey,p->_headkeylen),strvalue(buf,len);
printf("header_value_cb key:%s,value:%s...\n",strkey.c_str(),strvalue.c_str());
//assert(p == parser);
//string str(buf,len);
//printf("body_cb,body_is_final:%d, body:%s\n",http_body_is_final(p),str.c_str());
return 0;
}
int TestMultiPartData();//测试分隔符方式的数据
int TestChunkedData();//测试分块传输数据
int TestContentLengthData();//测试content-length数据
int main()
{
int i = sizeof(http_parser);
TestContentLengthData();
return 0;	
}

int TestContentLengthData()
{
http_parser parser;
int i = sizeof(http_parser);
http_parser_settings parserset;
ZeroMemory(&parser,sizeof(http_parser));
http_parser_init(&parser,HTTP_BOTH);
http_parser_settings_init(&parserset);
//parserset.on_body = body_cb;
//parserset.on_header_field = header_field_cb;
parserset.on_header_value = header_value_cb;
parserset.on_message_complete = message_complete_cb;
string str = "POST /Pdf/Create HTTP/1.1\r\nPlatFormID: 211212\r\nContent-Length: 5\r\n\r\n12345";
string strhead = "POST /Pdf/Create HTTP/1.1\r\nPlatFormID: 211212\r\nContent-Length: 5\r\n\r\n";
string strappend = "abcde";
//string str = "POST /Pdf/Create HTTP/1.1\rTransfer-Encoding: chunked\n\r\n7\r\n12345\r\n0\r\n";
//string str = "POST /Pdf/Create HTTP/1.1\r\n";
//parser.data = (void *)str.c_str();
size_t isize = http_parser_execute(&parser,&parserset,strhead.c_str(),strhead.length());
isize = http_parser_execute(&parser,&parserset,strappend.c_str(),strappend.length());
const char* perror = http_errno_description((http_errno)parser.http_errno);
i = http_body_is_final(&parser);
return 0;
}

int TestChunkedData()
{
http_parser parser;
int i = sizeof(http_parser);
http_parser_settings parserset;
ZeroMemory(&parser,sizeof(http_parser));
http_parser_init(&parser,HTTP_BOTH);
http_parser_settings_init(&parserset);
//parserset.on_body = body_cb;
parserset.on_message_complete = message_complete_cb;
//string str = "POST /Pdf/Create HTTP/1.1\r\nContent-Length: 5\r\n\r\n12345";
string str = "POST /Pdf/Create HTTP/1.1\rTransfer-Encoding: chunked\n\r\n7\r\n12345\r\n0\r\n";

//char * msg = create_large_chunked_message(31337,
//	"HTTP/1.0 200 OK\r\n"
//	"Transfer-Encoding: chunked\r\n"
//	"Content-Type: text/plain\r\n"
//	"\r\n");
char * msg = create_large_chunked_message(1,
"HTTP/1.1 200 OK\r\n"
"Transfer-Encoding: chunked\r\n"
"Content-Type: text/plain\r\n"
"\r\n");

HCommonFun::WriteFileAfter("d:/share/abc1.txt",msg,strlen(msg));
//string str = "POST /Pdf/Create HTTP/1.1\r\n";
parser.data = (void *)str.c_str();
//size_t isize = http_parser_execute(&parser,&parserset,str.c_str(),str.length());
size_t isize = http_parser_execute(&parser,&parserset,msg,strlen(msg));
const char* perror = http_errno_description((http_errno)parser.http_errno);
i = http_body_is_final(&parser);
return 0;
}

int TestMultiPartData()
{
http_parser parser;
int i = sizeof(http_parser);
http_parser_settings parserset;
ZeroMemory(&parser,sizeof(http_parser));
http_parser_init(&parser,HTTP_BOTH);
http_parser_settings_init(&parserset);
//parserset.on_body = body_cb;
parserset.on_message_complete = message_complete_cb;
//string str = "POST /Pdf/Create HTTP/1.1\r\nContent-Length: 5\r\n\r\n12345";
string str = "GET / HTTP/1.1\r\n"
"X-SSL-Bullshit:   -----BEGIN CERTIFICATE-----\r\n"
"\tMIIFbTCCBFWgAwIBAgICH4cwDQYJKoZIhvcNAQEFBQAwcDELMAkGA1UEBhMCVUsx\r\n"
"\tETAPBgNVBAoTCGVTY2llbmNlMRIwEAYDVQQLEwlBdXRob3JpdHkxCzAJBgNVBAMT\r\n"
"\tAkNBMS0wKwYJKoZIhvcNAQkBFh5jYS1vcGVyYXRvckBncmlkLXN1cHBvcnQuYWMu\r\n"
"\tdWswHhcNMDYwNzI3MTQxMzI4WhcNMDcwNzI3MTQxMzI4WjBbMQswCQYDVQQGEwJV\r\n"
"\tSzERMA8GA1UEChMIZVNjaWVuY2UxEzARBgNVBAsTCk1hbmNoZXN0ZXIxCzAJBgNV\r\n"
"\tBAcTmrsogriqMWLAk1DMRcwFQYDVQQDEw5taWNoYWVsIHBhcmQYJKoZIhvcNAQEB\r\n"
"\tBQADggEPADCCAQoCggEBANPEQBgl1IaKdSS1TbhF3hEXSl72G9J+WC/1R64fAcEF\r\n"
"\tW51rEyFYiIeZGx/BVzwXbeBoNUK41OK65sxGuflMo5gLflbwJtHBRIEKAfVVp3YR\r\n"
"\tgW7cMA/s/XKgL1GEC7rQw8lIZT8RApukCGqOVHSi/F1SiFlPDxuDfmdiNzL31+sL\r\n"
"\t0iwHDdNkGjy5pyBSB8Y79dsSJtCW/iaLB0/n8Sj7HgvvZJ7x0fr+RQjYOUUfrePP\r\n"
"\tu2MSpFyf+9BbC/aXgaZuiCvSR+8Snv3xApQY+fULK/xY8h8Ua51iXoQ5jrgu2SqR\r\n"
"\twgA7BUi3G8LFzMBl8FRCDYGUDy7M6QaHXx1ZWIPWNKsCAwEAAaOCAiQwggIgMAwG\r\n"
"\tA1UdEwEB/wQCMAAwEQYJYIZIAYb4QgHTTPAQDAgWgMA4GA1UdDwEB/wQEAwID6DAs\r\n"
"\tBglghkgBhvhCAQ0EHxYdVUsgZS1TY2llbmNlIFVzZXIgQ2VydGlmaWNhdGUwHQYD\r\n"
"\tVR0OBBYEFDTt/sf9PeMaZDHkUIldrDYMNTBZMIGaBgNVHSMEgZIwgY+AFAI4qxGj\r\n"
"\tloCLDdMVKwiljjDastqooXSkcjBwMQswCQYDVQQGEwJVSzERMA8GA1UEChMIZVNj\r\n"
"\taWVuY2UxEjAQBgNVBAsTCUF1dGhvcml0eTELMAkGA1UEAxMCQ0ExLTArBgkqhkiG\r\n"
"\t9w0BCQEWHmNhLW9wZXJhdG9yQGdyaWQtc3VwcG9ydC5hYy51a4IBADApBgNVHRIE\r\n"
"\tIjAggR5jYS1vcGVyYXRvckBncmlkLXN1cHBvcnQuYWMudWswGQYDVR0gBBIwEDAO\r\n"
"\tBgwrBgEEAdkvAQEBAQYwPQYJYIZIAYb4QgEEBDAWLmh0dHA6Ly9jYS5ncmlkLXN1\r\n"
"\tcHBvcnQuYWMudmT4sopwqlBWsvcHViL2NybC9jYWNybC5jcmwwPQYJYIZIAYb4QgEDBDAWLmh0\r\n"
"\tdHA6Ly9jYS5ncmlkLXN1cHBvcnQuYWMudWsvcHViL2NybC9jYWNybC5jcmwwPwYD\r\n"
"\tVR0fBDgwNjA0oDKgMIYuaHR0cDovL2NhLmdyaWQt5hYy51ay9wdWIv\r\n"
"\tY3JsL2NhY3JsLmNybDANBgkqhkiG9w0BAQUFAAOCAQEAS/U4iiooBENGW/Hwmmd3\r\n"
"\tXCy6Zrt08YjKCzGNjorT98g8uGsqYjSxv/hmi0qlnlHs+k/3Iobc3LjS5AMYr5L8\r\n"
"\tUO7OSkgFFlLHQyC9JzPfmLCAugvzEbyv4Olnsr8hbxF1MbKZoQxUZtMVu29wjfXk\r\n"
"\thTeApBv7eaKCWpSp7MCbvgzm74izKhu3vlDk9w6qVrxePfGgpKPqfHiOoGhFnbTK\r\n"
"\twTC6o2xq5y0qZ03JonF7OJspEd3I5zKY3E+ov7/ZhW6DqT8UFvsAdjvQbXyhV8Eu\r\n"
"\tYhixw1aKEPzNjNowuIseVogKOLXxWI5vAi5HgXdS0/ES5gDGsABo4fqovUKlgop3\r\n"
"\tRA==\r\n"
"\t-----END CERTIFICATE-----\r\n"
"\r\n";
str.append(str);

size_t isize,ioffset(0);
//string str = "POST /Pdf/Create HTTP/1.1\r\n";
const char* perror = http_errno_description((http_errno)parser.http_errno);
while(ioffset < str.length())
{
isize = http_parser_execute(&parser,&parserset,str.c_str() + ioffset,str.length() - ioffset);
if(isize == 0)
{
perror = http_errno_description((http_errno)parser.http_errno);
}
ioffset += isize;
http_parser_init(&parser,HTTP_BOTH);
}

perror = http_errno_description((http_errno)parser.http_errno);
i = http_body_is_final(&parser);
return 0;
}


*/