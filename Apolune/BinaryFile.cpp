#include "BinaryFile.h"

void print_fopen_err( errno_t err ) {
 switch ( err ) {
  case 1: OUTPUT("EPERM `Operation not permitted`\n"); break;
  case 2: OUTPUT("ENOENT `No such file or directory`\n"); break;
  case 3: OUTPUT("ESRCH `No such process`\n"); break;
  case 4: OUTPUT("EINTR `Interrupted function`\n"); break;
  case 5: OUTPUT("EIO `I/O error`\n"); break;
  case 6: OUTPUT("ENXIO `No such device or address`\n"); break;
  case 7: OUTPUT("E2BIG `Argument list too long`\n"); break;
  case 8: OUTPUT("ENOEXEC `Exec format error`\n"); break;
  case 9: OUTPUT("EBADF `Bad file number`\n"); break;
  case 10: OUTPUT("ECHILD `No spawned processes`\n"); break;
  case 11: OUTPUT("EAGAIN `No more processes or not enough memmory or maximum nesting level reached`\n"); break;
  case 12: OUTPUT("ENOMEM `Not enough memory`\n"); break;
  case 13: OUTPUT("EACCES `Permission denied`\n"); break;
  case 14: OUTPUT("EFAULT `Bad address`\n"); break;
  case 16: OUTPUT("EBUSY `Device or resource busy`\n"); break;
  case 17: OUTPUT("EEXIST `File exists`\n"); break;
  case 18: OUTPUT("EXDEV `Cross-device link`\n"); break;
  case 19: OUTPUT("ENODEV `No such device`\n"); break;
  case 20: OUTPUT("ENOTDIR `Not a directory`\n"); break;
  case 21: OUTPUT("EISDIR `Is a directory`\n"); break;
  case 22: OUTPUT("EINVAL `Invalid argument`\n"); break;
  case 23: OUTPUT("ENFILE `Too many files open in system`\n"); break;
  case 24: OUTPUT("EMFILE `Too many open files`\n"); break;
  case 25: OUTPUT("ENOTTY `Inappropriate I/O control operation`\n"); break;
  case 27: OUTPUT("EFBIG `File too large`\n"); break;
  case 28: OUTPUT("ENOSPC `No space left on device`\n"); break;
  case 29: OUTPUT("ESPIPE `Invalid seek`\n"); break;
  case 30: OUTPUT("EROFS `Read-only file system`\n"); break;
  case 31: OUTPUT("EMLINK `Too many links`\n"); break;
  case 32: OUTPUT("EPIPE `Broken pipe`\n"); break;
  case 33: OUTPUT("EDOM `Math argument`\n"); break;
  case 34: OUTPUT("ERANGE `Result too large`\n"); break;
  case 36: OUTPUT("EDEADLK or EDEADLOCK `Resource deadlock would occur` (EDEADLOCK is same as EDEADLK for compatibility with older Microsoft C versions)\n"); break;
  case 38: OUTPUT("ENAMETOOLONG `Filename too long`\n"); break;
  case 39: OUTPUT("ENOLCK `No locks available`\n"); break;
  case 40: OUTPUT("ENOSYS `Function not supported`\n"); break;
  case 41: OUTPUT("ENOTEMPTY `Directory not empty`\n"); break;
  case 42: OUTPUT("EILSEQ `Illegal byte sequence`\n"); break;
  case 80: OUTPUT("STRUNCATE `String was truncated`\n"); break;
 }
}