#include "pThread.h"

#ifdef _WIN32
#if defined(USE_WINTHREADS)
    #include <windows.h>
    #pragma comment(lib, "pthreadVC2")
#endif
#else
    #include <unistd.h>
    void Sleep(unsigned long milliseconds)
    {
        if (milliseconds<1000)
            usleep(milliseconds*1000);
        else
            sleep(milliseconds/1000);
    }
#endif

#if defined(USE_WINTHREADS)
HANDLE appMutex;
CRITICAL_SECTION CriticalSection;
#else
pthread_mutex_t appMutex;
#endif
pThreads pthreads;

#if defined(USE_WINTHREADS)
 DWORD WINAPI ThreadLoop( LPVOID lpParam ) {
  DWORD dwCount=0, dwWaitResult;
  while ( dwCount < 20 ) {
   dwWaitResult=WaitForSingleObject(appMutex,INFINITE);
   switch ( dwWaitResult ) {
    case WAIT_OBJECT_0:
      __try {
       pThread *pthread=(pThread *) lpParam;
       pthread->Main();
       ExitThread(0);
      }
      __finally { if ( !ReleaseMutex(appMutex) ) { } }
     break;
    case WAIT_ABANDONED: return FALSE;
   }
  }
  return TRUE;
 }
#else
static void *ThreadLoop( void *p ) {
 pThread *t=(pThread *) p;
 t->Main();
// OUTPUT("%d looping",(int) t->iret);
 return NULL;
}
#endif

pThread::pThread(void)
{
#if defined(USE_WINTHREADS)
#else
 detached=true;
#endif
}

pThread::pThread(bool lock)
{
#if defined(USE_WINTHREADS)
#else
 locking=lock;
 detached=true;
#endif
}

pThread::~pThread(void)
{
#if defined(USE_WINTHREADS)
 CloseHandle( thread );
#else
 if ( created && detached ) {
  pthread_attr_destroy(&attr);
 }
 if ( created && locking ) {
  pthread_mutex_destroy(&mutex);
 }
#endif
}

void pThread::Pause() {
 if ( started && !paused ) {
  SuspendThread(thread);
  paused=true;
 }
}

void pThread::Unpause() {
 if ( started && paused ) {
  ResumeThread(thread);
  paused=false;
 }
}

void pThread::Create(void) {
 if ( created || deleteMe ) return;
#if defined(USE_WINTHREADS)
 thread=CreateThread( NULL, 16384, (LPTHREAD_START_ROUTINE) ThreadLoop, (LPVOID) this, CREATE_SUSPENDED, &ID ); // receive thread identifier
 if ( !thread ) {
  DWORD dw=GetLastError();
  if ( dw != 0 ) OUTPUT("pThread::Create() error #%d (%s)\n", (int) dw, TranslateErrorCode(dw) );
 }
#else
 pthread_attr_init(&attr);
 if ( detached ) {
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
 }
 iret = pthread_create( &thread, &attr, ThreadLoop, this );
 if ( locking ) {
  if (pthread_mutex_init(&mutex, 0))
   throw std::runtime_error("pthread_mutex_init failed");
 }
#endif
 created=true;
}

void pThread::Join(void) {
#if defined(USE_WINTHREADS)
#else
 pthread_join( thread, NULL);
#endif
}

void pThread::Lock(void) {
 if ( created && !deleteMe ) {
#if defined(USE_WINTHREADS)
 EnterCriticalSection(&CriticalSection);
#else
 if ( locking ) pthread_mutex_lock(&mutex);
#endif
 }
}

void pThread::Unlock(void) {
 if ( created && !deleteMe ) {
#if defined(USE_WINTHREADS)
 LeaveCriticalSection(&CriticalSection);
#else
 if ( locking ) pthread_mutex_unlock(&mutex);
#endif
 }
}

//

pThreads::pThreads(void)
{
 locked=0;
#if defined(USE_WINTHREADS)
 appMutex=CreateMutex(NULL,FALSE,NULL);
 if (appMutex == NULL) {
  DWORD dw=GetLastError();
  if ( dw != 0 ) OUTPUT("pThreads::pThreads() error #%d (%s)\n", (int) dw, TranslateErrorCode(dw) );
 }
 if (!InitializeCriticalSectionAndSpinCount(&CriticalSection, 0x00000400) ) {
  DWORD dw=GetLastError();
  if ( dw != 0 ) OUTPUT("pThreads::pThreads() CRITICAL_SECTION error #%d (%s)\n", (int) dw, TranslateErrorCode(dw) );
 }
#else
 if (pthread_mutex_init(&appMutex, 0))
  throw std::runtime_error("pthread_mutex_init failed");
#endif
}

pThreads::~pThreads(void)
{
 pThread *p,*n;
 for ( p=(pThread *) first; p; p=n ) {
  n=(pThread *) (p->next);
  Remove(p);
  delete p;
 }
#if defined(USE_WINTHREADS)
 DeleteCriticalSection(&CriticalSection);
 CloseHandle(appMutex);
#else
 pthread_mutex_destroy(&appMutex);
#endif
}

void pThreads::Join(void) {
 pThread *p,*n;
 for ( p=(pThread *) first; p; p=n ) {
  n=(pThread *) (p->next);
  p->Join();
 }
}

void pThreads::Clean(void) {
 this->Lock();
 pThread *p,*n;
 for ( p=(pThread *) first; p; p=n ) {
  n=(pThread *) (p->next);
  if ( p->deleteMe ) {
   Remove(p);
   p->OnDestroy();
   delete p;
  }
 }
 this->Unlock();
}

void pThreads::Lock(void) {
#if defined(USE_WINTHREADS)
#else
 if ( appMutex == null ) return;
 if ( locked==0 ) pthread_mutex_lock(&appMutex);
#endif
 locked++;
}

void pThreads::Unlock(void) {
#if defined(USE_WINTHREADS)
#else
 if ( appMutex == null ) return;
 if ( locked==1 ) pthread_mutex_unlock(&appMutex);
#endif
 locked--;
}

void pThreads::New( pThread *p ) {
 Prepend(p);
 p->Create();
}

void pThreads::Kill( pThread *p ) {
 this->Lock();
 Remove(p);
#if defined(USE_WINTHREADS)
 p->OnKill();
#endif
 delete p;
 this->Unlock();
}

void pThreads::Loop() {
#if defined(USE_WINTHREADS)
 EACHN(this->first,pThread,p,{
  if ( !p->started ) {
   ResumeThread(p->thread);
   p->started=true;
  } else if ( p->deleteMe ) {
   Remove(p);
   delete p;
  }
 });
#else
#endif
}

const char *TranslateErrorCode( DWORD ERR ) {
 switch (ERR) { 
  case 0: return "The operation completed successfully.";
  case 1: return "Incorrect function.";
  case 10: return "The environment is incorrect.";
  case 100: return "Cannot create another system semaphore.";
  case 101: return "The exclusive semaphore is owned by another process.";
  case 102: return "The semaphore is set and cannot be closed.";
  case 103: return "The semaphore cannot be set again.";
  case 104: return "Cannot request exclusive semaphores at interrupt time.";
  case 105: return "The previous ownership of this semaphore has ended.";
  case 106: return "Insert the diskette for drive %1.";
  case 107: return "The program stopped because an alternate diskette was not inserted.";
  case 108: return "The disk is in use or locked by another process.";
  case 109: return "The pipe has been ended.";
  case 11: return "An attempt was made to load a program with an incorrect format.";
  case 110: return "The system cannot open the device or file specified.";
  case 111: return "The file name is too long.";
  case 112: return "There is not enough space on the disk.";
  case 113: return "No more internal file identifiers available.";
  case 114: return "The target internal file identifier is incorrect.";
  case 117: return "The IOCTL call made by the application program is not correct.";
  case 118: return "The verify-on-write switch parameter value is not correct.";
  case 119: return "The system does not support the command requested.";
  case 12: return "The access code is invalid.";
  case 120: return "This function is not supported on this system.";
  case 121: return "The semaphore timeout period has expired.";
  case 122: return "The data area passed to a system call is too small.";
  case 123: return "The filename, directory name, or volume label syntax is incorrect.";
  case 124: return "The system call level is not correct.";
  case 125: return "The disk has no volume label.";
  case 126: return "The specified module could not be found.";
  case 127: return "The specified procedure could not be found.";
  case 128: return "There are no child processes to wait for.";
  case 129: return "The %1 application cannot be run in Win32 mode.";
  case 13: return "The data is invalid.";
  case 130: return "Attempt to use a file handle to an open disk partition for an operation other than raw disk I/O.";
  case 131: return "An attempt was made to move the file pointer before the beginning of the file.";
  case 132: return "The file pointer cannot be set on the specified device or file.";
  case 133: return "A JOIN or SUBST command cannot be used for a drive that contains previously joined drives.";
  case 134: return "An attempt was made to use a JOIN or SUBST command on a drive that has already been joined.";
  case 135: return "An attempt was made to use a JOIN or SUBST command on a drive that has already been substituted.";
  case 136: return "The system tried to delete the JOIN of a drive that is not joined.";
  case 137: return "The system tried to delete the substitution of a drive that is not substituted.";
  case 138: return "The system tried to join a drive to a directory on a joined drive.";
  case 139: return "The system tried to substitute a drive to a directory on a substituted drive.";
  case 14: return "Not enough storage is available to complete this operation.";
  case 140: return "The system tried to join a drive to a directory on a substituted drive.";
  case 141: return "The system tried to SUBST a drive to a directory on a joined drive.";
  case 142: return "The system cannot perform a JOIN or SUBST at this time.";
  case 143: return "The system cannot join or substitute a drive to or for a directory on the same drive.";
  case 144: return "The directory is not a subdirectory of the root directory.";
  case 145: return "The directory is not empty.";
  case 146: return "The path specified is being used in a substitute.";
  case 147: return "Not enough resources are available to process this command.";
  case 148: return "The path specified cannot be used at this time.";
  case 149: return "An attempt was made to join or substitute a drive for which a directory on the drive is the target of a previous substitute.";
  case 15: return "The system cannot find the drive specified.";
  case 150: return "System trace information was not specified in your CONFIG.SYS file, or tracing is disallowed.";
  case 151: return "The number of specified semaphore events for DosMuxSemWait is not correct.";
  case 152: return "DosMuxSemWait did not execute; too many semaphores are already set.";
  case 153: return "The DosMuxSemWait list is not correct.";
  case 154: return "The volume label you entered exceeds the label character limit of the target file system.";
  case 155: return "Cannot create another thread.";
  case 156: return "The recipient process has refused the signal.";
  case 157: return "The segment is already discarded and cannot be locked.";
  case 158: return "The segment is already unlocked.";
  case 159: return "The address for the thread ID is not correct.";
  case 16: return "The directory cannot be removed.";
  case 160: return "One or more arguments are not correct.";
  case 161: return "The specified path is invalid.";
  case 162: return "A signal is already pending.";
  case 164: return "No more threads can be created in the system.";
  case 167: return "Unable to lock a region of a file.";
  case 17: return "The system cannot move the file to a different disk drive.";
  case 170: return "The requested resource is in use.";
  case 173: return "A lock request was not outstanding for the supplied cancel region.";
  case 174: return "The file system does not support atomic changes to the lock type.";
  case 18: return "There are no more files.";
  case 180: return "The system detected a segment number that was not correct.";
  case 182: return "The operating system cannot run %1.";
  case 183: return "Cannot create a file when that file already exists.";
  case 186: return "The flag passed is not correct.";
  case 187: return "The specified system semaphore name was not found.";
  case 188: return "The operating system cannot run %1.";
  case 189: return "The operating system cannot run %1.";
  case 19: return "The media is write protected.";
  case 190: return "The operating system cannot run %1.";
  case 191: return "Cannot run %1 in Win32 mode.";
  case 192: return "The operating system cannot run %1.";
  case 193: return "is not a valid Win32 application.";
  case 194: return "The operating system cannot run %1.";
  case 195: return "The operating system cannot run %1.";
  case 196: return "The operating system cannot run this application program.";
  case 197: return "The operating system is not presently configured to run this application.";
  case 198: return "The operating system cannot run %1.";
  case 199: return "The operating system cannot run this application program.";
  case 2: return "The system cannot find the file specified.";
  case 20: return "The system cannot find the device specified.";
  case 200: return "The code segment cannot be greater than or equal to 64K.";
  case 201: return "The operating system cannot run %1.";
  case 202: return "The operating system cannot run %1.";
  case 203: return "The system could not find the environment option that was entered.";
  case 205: return "No process in the command subtree has a signal handler.";
  case 206: return "The filename or extension is too long.";
  case 207: return "The ring 2 stack is in use.";
  case 208: return "The global filename characters, * or ?, are entered incorrectly or too many global filename characters are specified.";
  case 209: return "The signal being posted is not correct.";
  case 21: return "The device is not ready.";
  case 210: return "The signal handler cannot be set.";
  case 212: return "The segment is locked and cannot be reallocated.";
  case 214: return "Too many dynamic-link modules are attached to this program or dynamic-link module.";
  case 215: return "Cannot nest calls to LoadModule.";
  case 216: return "The version of %1 is not compatible with the version you're running. Check your computer's system information to see whether you need a x86 ; or x64 ; version of the program, and then contact the software publisher.";
  case 217: return "The image file %1 is signed, unable to modify.";
  case 218: return "The image file %1 is strong signed, unable to modify.";
  case 22: return "The device does not recognize the command.";
  case 220: return "This file is checked out or locked for editing by another user.";
  case 221: return "The file must be checked out before saving changes.";
  case 222: return "The file type being saved or retrieved has been blocked.";
  case 223: return "The file size exceeds the limit allowed and cannot be saved.";
  case 224: return "Access Denied. Before opening files in this location, you must first add the web site to your trusted sites list, browse to the web site, and select the option to login automatically.";
  case 225: return "Operation did not complete successfully because the file contains a virus.";
  case 226: return "This file contains a virus and cannot be opened. Due to the nature of this virus, the file has been removed from this location.";
  case 229: return "The pipe is local.";
  case 23: return "Data error ;.";
  case 230: return "The pipe state is invalid.";
  case 231: return "All pipe instances are busy.";
  case 232: return "The pipe is being closed.";
  case 233: return "No process is on the other end of the pipe.";
  case 234: return "More data is available.";
  case 24: return "The program issued a command but the command length is incorrect.";
  case 240: return "The session was canceled.";
  case 25: return "The drive cannot locate a specific area or track on the disk.";
  case 254: return "The specified extended attribute name was invalid.";
  case 255: return "The extended attributes are inconsistent.";
  case 258: return "The wait operation timed out.";
  case 259: return "No more data is available.";
  case 26: return "The specified disk or diskette cannot be accessed.";
  case 266: return "The copy functions cannot be used.";
  case 267: return "The directory name is invalid.";
  case 27: return "The drive cannot find the sector requested.";
  case 275: return "The extended attributes did not fit in the buffer.";
  case 276: return "The extended attribute file on the mounted file system is corrupt.";
  case 277: return "The extended attribute table file is full.";
  case 278: return "The specified extended attribute handle is invalid.";
  case 28: return "The printer is out of paper.";
  case 282: return "The mounted file system does not support extended attributes.";
  case 288: return "Attempt to release mutex not owned by caller.";
  case 29: return "The system cannot write to the specified device.";
  case 298: return "Too many posts were made to a semaphore.";
  case 299: return "Only part of a ReadProcessMemory or WriteProcessMemory request was completed.";
  case 3: return "The system cannot find the path specified.";
  case 30: return "The system cannot read from the specified device.";
  case 300: return "The oplock request is denied.";
  case 301: return "An invalid oplock acknowledgment was received by the system.";
  case 302: return "The volume is too fragmented to complete this operation.";
  case 303: return "The file cannot be opened because it is in the process of being deleted.";
  case 304: return "Short name settings may not be changed on this volume due to the global registry setting.";
  case 305: return "Short names are not enabled on this volume.";
  case 306: return "The security stream for the given volume is in an inconsistent state. Please run CHKDSK on the volume.";
  case 307: return "A requested file lock operation cannot be processed due to an invalid byte range.";
  case 308: return "The subsystem needed to support the image type is not present.";
  case 309: return "The specified file already has a notification GUID associated with it.";
  case 31: return "A device attached to the system is not functioning.";
  case 317: return "The system cannot find message text for message number 0x%1 in the message file for %2.";
  case 318: return "The scope specified was not found.";
  case 32: return "The process cannot access the file because it is being used by another process.";
  case 33: return "The process cannot access the file because another process has locked a portion of the file.";
  case 34: return "The wrong diskette is in the drive. Insert %2 ; into drive %1.";
  case 350: return "No action was taken as a system reboot is required.";
  case 351: return "The shutdown operation failed.";
  case 352: return "The restart operation failed.";
  case 353: return "The maximum number of sessions has been reached.";
  case 36: return "Too many files opened for sharing.";
  case 38: return "Reached the end of the file.";
  case 39: return "The disk is full.";
  case 4: return "The system cannot open the file.";
  case 400: return "The thread is already in background processing mode.";
  case 401: return "The thread is not in background processing mode.";
  case 402: return "The process is already in background processing mode.";
  case 403: return "The process is not in background processing mode.";
  case 487: return "Attempt to access invalid address.";
  case 5: return "Access is denied.";
  case 50: return "The request is not supported.";
  case 51: return "Windows cannot find the network path. Verify that the network path is correct and the destination computer is not busy or turned off. If Windows still cannot find the network path, contact your network administrator.";
  case 52: return "You were not connected because a duplicate name exists on the network. If joining a domain, go to System in Control Panel to change the computer name and try again. If joining a workgroup, choose another workgroup name.";
  case 53: return "The network path was not found.";
  case 54: return "The network is busy.";
  case 55: return "The specified network resource or device is no longer available.";
  case 56: return "The network BIOS command limit has been reached.";
  case 57: return "A network adapter hardware error occurred.";
  case 58: return "The specified server cannot perform the requested operation.";
  case 59: return "An unexpected network error occurred.";
  case 6: return "The handle is invalid.";
  case 60: return "The remote adapter is not compatible.";
  case 61: return "The printer queue is full.";
  case 62: return "Space to store the file waiting to be printed is not available on the server.";
  case 63: return "Your file waiting to be printed was deleted.";
  case 64: return "The specified network name is no longer available.";
  case 65: return "Network access is denied.";
  case 66: return "The network resource type is not correct.";
  case 67: return "The network name cannot be found.";
  case 68: return "The name limit for the local computer network adapter card was exceeded.";
  case 69: return "The network BIOS session limit was exceeded.";
  case 7: return "The storage control blocks were destroyed.";
  case 70: return "The remote server has been paused or is in the process of being started.";
  case 71: return "No more connections can be made to this remote computer at this time because there are already as many connections as the computer can accept.";
  case 72: return "The specified printer or disk device has been paused.";
  case 8: return "Not enough storage is available to process this command.";
  case 80: return "The file exists.";
  case 82: return "The directory or file cannot be created.";
  case 83: return "Fail on INT 24.";
  case 84: return "Storage to process this request is not available.";
  case 85: return "The local device name is already in use.";
  case 86: return "The specified network password is not correct.";
  case 87: return "The parameter is incorrect.";
  case 88: return "A write fault occurred on the network.";
  case 89: return "The system cannot start another process at this time.";
  case 9: return "The storage control block address is invalid.";
  default: return "unknown.";
 }
}
    