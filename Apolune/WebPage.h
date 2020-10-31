/*********************************************************************************************
 *  __    __________________   ________________________________   __________  ________       *
 * /\ \  /\  __ \  ___\__  _\ /\  __ \  ___\__  _\  == \  __ \ "-.\ \  __ \ \/\ \__  _\ (tm) *
 * \ \ \_\_\ \/\ \___  \/\ \/ \ \  __ \___  \/\ \/\  __<\ \/\ \ \-.  \  __ \ \_\ \/\ \/      *
 *  \ \_____\_____\_____\ \_\  \ \_\ \_\_____\ \_\ \_\ \_\_____\_\\"\_\_\ \_\_____\ \_\      *
 *   \/_____/_____/_____/\/_/   \/_/\/_/_____/\/_/\/_/\/_/_____/_/ \/_/_/\/_/_____/\/_/      *
 *    --------------------------------------------------------------------------------       *
 *     Lost Astronaut Game Development Framework (c) 2007-2017 H. Elwood Gilliland III       *
 *********************************************************************************************
 * This software is copyrighted software.  Use of this code is given only with permission to *
 * parties who have been granted such permission by its author, Herbert Elwood Gilliland III *
 *********************************************************************************************/
#pragma once

//#define USE_CURL 1

#define WIN32_LEAN_AND_MEAN 
#include <Windows.h>
#include <stdio.h>
#include <time.h>
#if defined(USE_CURL)
#define CURL_DISABLE_TYPECHECK
#define CURL_STATICLIB 1
#include <curl/curl.h>
#include <curl/types.h>
#include <curl/easy.h>
#endif

//#include <stdio.h>
//#include <sys/time.h>
#include <math.h>
#include "moremath.h"

#include "macros.h"
#include "ListItem.h"
#include "LinkedList.h"
#include "Strings.h"

#include "pThread.h"

/* 
 * PHP-style url_encode and decode
 */

/* Converts a hex character to its integer value */
char from_hexc(char ch);

/* Converts an integer value to its hex character*/
char to_hexc(char code);

/* Returns a url-encoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *url_encode(char *str);

/* Returns a url-decoded version of str */
/* IMPORTANT: be sure to free() the returned string after use */
char *url_decode(char *str);

#if defined(USE_CURL)
struct MemoryStruct {
public:
 char *memory;
 size_t size;
 MemoryStruct() { memory=null; size=0; }
 ~MemoryStruct() { if ( memory ) free(memory); } //if ( memory ) delete[] memory; }
};
#endif 

/* 
 * Implements a multi-typed form element which includes file posting.
 */

enum WebFormElementTypes {
 _Input=0,
 _Submit=1,
 _File=2,
 _Select=3
};

class WebFormElement : public ListItem {
public:
 WebFormElementTypes type;
 string name;
 string value;
};

#if defined(USE_CURL)
// Use cURL Easy to transact files and form elements.
static const char WebFormHeaderbuf[] = "Expect:";
class WebForm : public LinkedList {
public:
 Zstring url;
 bool NoExpectHeader;
 ~WebForm() { }
 void SendFile( const char *n, const char *fn ) {
  WebFormElement *fe=new WebFormElement;
  fe->type=_File;
  fe->name=string(n);
  fe->value=string(fn);
  this->Append(fe);
 }
 void Input( const char *n, const char *v ) {
  WebFormElement *fe=new WebFormElement;
  fe->type=_Input;
  fe->name=string(n);
  fe->value=string(v);
  this->Append(fe);
 }
 void Submit() {
  WebFormElement *fe=new WebFormElement;
  fe->type=_Submit;
  fe->name=string("submit");
  fe->value=string("send");
  this->Append(fe);
 }
 CURLcode Post();

#if defined(NEVER)
 // Non-blocking Multi-part Form Post
 bool timedOut;
 void MultiPost( ) {
  CURL *curl;
  CURLM *multi_handle;
  int still_running;
  timedOut=false;
  for ( e:this ) { // Post all the elements.
   if ( e.type == file ) curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, e.name, CURLFORM_FILE, e.val, CURLFORM_END);
   else                  curl_formadd(&formpost, &lastptr, CURLFORM_COPYNAME, e.name, CURLFORM_COPYCONTENTS, e.val, CURLFORM_END);
  }
 
  curl = curl_easy_init();
  multi_handle = curl_multi_init();
  headerlist = curl_slist_append(headerlist, WebFormHeaderbuf);  /* initalize custom header list (stating that Expect: 100-continue is not wanted */ 

  if(curl && multi_handle) {
   curl_easy_setopt(curl, CURLOPT_URL, url);      /* what URL that receives this POST */ 
   curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
   curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headerlist);
   curl_easy_setopt(curl, CURLOPT_HTTPPOST, formpost);
   curl_multi_add_handle(multi_handle, curl); 
   while(CURLM_CALL_MULTI_PERFORM == curl_multi_perform(multi_handle, &still_running));
   while(still_running) {
    struct timeval timeout;
    int rc; /* select() return code */ 
    fd_set fdread;
    fd_set fdwrite;
    fd_set fdexcep;
    int maxfd;
    FD_ZERO(&fdread);
    FD_ZERO(&fdwrite);
    FD_ZERO(&fdexcep);
 
    /* set a suitable timeout to play around with */ 
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    curl_multi_fdset(multi_handle, &fdread, &fdwrite, &fdexcep, &maxfd);    /* get file descriptors from the transfers */ 
 
      /* In a real-world program you OF COURSE check the return code of the
         function calls, *and* you make sure that maxfd is bigger than -1
         so that the call to select() below makes sense! */ 

    rc = select(maxfd+1, &fdread, &fdwrite, &fdexcep, &timeout);
    switch(rc) {
     case -1:        /* select error */         break;
     case 0:     timedOut=true; //   OUTPUT("WebForm:MultiPost() says Timeout!\n");
     default:        /* timeout or readable/writable sockets */ 
//      OUTPUT("WebForm:MultiPost() says Perform!\n");
      while(CURLM_CALL_MULTI_PERFORM == curl_multi_perform(multi_handle, &still_running));
//      OUTPUT("WebForm:MultiPost() says Running: %d!\n", still_running);
     break;
    }
   }
   curl_multi_cleanup(multi_handle);
   curl_easy_cleanup(curl);             /* always cleanup */ 
   curl_formfree(formpost);             /* then cleanup the formpost chain */ 
   curl_slist_free_all (headerlist);    /* free slist */ 
  }
 }
#endif
};

/*
 * Implements a web page payload object.
 */
class WebPage : public pThread {
public:
 Strings lines;
 string url;
 int state;
 MemoryStruct data;
 bool error;
 WebPage() {
  url=string("");
  state=0;
  error=false;
 }
 WebPage( const char *URL ) {
  url=string(URL);
  state=0;
  error=false;
 }
 void Browse();
 virtual bool OnResult() { return true; }
 virtual void Result() {};
 virtual void NotResult() {};
 void Main() {
  Browse();
  pthreads.Lock();
  if ( OnResult() ) Result();
  else NotResult();
  pthreads.Unlock();
 }
 ~WebPage() { }
};

class RemoteFileHTTP {
 Zstring url;
 Zstring result;
 RemoteFileHTTP() { }
 RemoteFileHTTP( const char * s ) { url=string(s); }
 ~RemoteFileHTTP() { }
 void Get();
};

// Requests the result of a form by popping off a thread with curl, waiting for a request, then executing the OnSuccess();
class DelayedWebFormRequest {//: Thread {
 WebForm web;// {};
 CURLcode result;
 float delay;
 bool started;
 bool fired;
 float remaining;

 virtual void Success() {}

 DelayedWebFormRequest() { 
  delay=0.1f; 
  remaining=0.5f;
 }
 ~DelayedWebFormRequest() { }

 void Request() {  
  started=true;
 }
 
 void DelayExpired() {
  if ( started ) if ( !fired ) result=web.Post();
  remaining-=(float) delay;
  if ( remaining <= 0.0f && started ) {
//   ((GuiApplication)__thisModule).Lock(); Success(); ((GuiApplication)__thisModule).Unlock();
//   this.Kill();
  }
 }

};

void DummyPageTest();
#endif

string PostURL( const char *hostname, const char *path, const char *data, bool secure=false, const char *username="", const char *password="" );
string GetURL( const char *url, const char *userpwd="", bool unrestricted=false );