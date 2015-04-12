--- tools.c.orig	2015-02-07 15:09:17 UTC
+++ tools.c
@@ -21,13 +21,25 @@ extern "C" {
 #include <locale.h>
 #include <stdlib.h>
 #include <sys/time.h>
+#ifdef __FreeBSD__
+#include <sys/param.h>
+#include <sys/mount.h>
+#else
 #include <sys/vfs.h>
+#endif
 #include <time.h>
 #include <unistd.h>
 #include <utime.h>
 #include "i18n.h"
 #include "thread.h"
 
+/* FreeBSD has it's own version of isnumber(),  
+   but VDR's version is incompatible */
+#ifdef __FreeBSD__
+#undef isnumber   
+#endif
+   
+   
 int SysLogLevel = 3;
 
 #define MAXSYSLOGBUF 256
@@ -618,6 +630,18 @@ char *ReadLink(const char *FileName)
 {
   if (!FileName)
      return NULL;
+#ifdef __FreeBSD__
+  char *resolved_path = NULL;
+  resolved_path = MALLOC(char, PATH_MAX+1);
+  if (realpath(FileName, resolved_path) == NULL)
+  {
+        if (errno != ENOENT) // some other error occurred
+           LOG_ERROR_STR(FileName);
+        else // file doesn't exist
+           resolved_path = strdup(FileName);
+  }
+  return resolved_path;
+#else
   char *TargetName = canonicalize_file_name(FileName);
   if (!TargetName) {
      if (errno == ENOENT) // file doesn't exist
@@ -626,6 +650,7 @@ char *ReadLink(const char *FileName)
         LOG_ERROR_STR(FileName);
      }
   return TargetName;
+#endif
 }
 
 bool SpinUpDisk(const char *FileName)
@@ -642,7 +667,11 @@ bool SpinUpDisk(const char *FileName)
          int f = open(buf, O_WRONLY | O_CREAT, DEFFILEMODE);
          // O_SYNC doesn't work on all file systems
          if (f >= 0) {
+#ifdef __FreeBSD__
+            if (fsync(f) < 0)
+#else
             if (fdatasync(f) < 0)
+#endif
                LOG_ERROR_STR(*buf);
             close(f);
             remove(buf);
@@ -713,7 +742,7 @@ uint64_t cTimeMs::Now(void)
               esyslog("cTimeMs: clock_gettime(CLOCK_MONOTONIC) failed");
            }
         else
-           dsyslog("cTimeMs: not using monotonic clock - resolution is too bad (%ld s %ld ns)", tp.tv_sec, tp.tv_nsec);
+           dsyslog("cTimeMs: not using monotonic clock - resolution is too bad (%ld s %ld ns)", (long)tp.tv_sec, (long)tp.tv_nsec);
         }
      else
         esyslog("cTimeMs: clock_getres(CLOCK_MONOTONIC) failed");
@@ -955,7 +984,11 @@ void cCharSetConv::SetSystemCharacterTab
 const char *cCharSetConv::Convert(const char *From, char *To, size_t ToLength)
 {
   if (cd != (iconv_t)-1 && From && *From) {
+#ifdef __FreeBSD__
+     const char *FromPtr = (char *)From;
+#else
      char *FromPtr = (char *)From;
+#endif
      size_t FromLength = strlen(From);
      char *ToPtr = To;
      if (!ToPtr) {
@@ -1560,7 +1593,11 @@ bool cFile::Open(int FileDes)
      if (!IsOpen()) {
         f = FileDes;
         if (f >= 0) {
+#ifdef __FreeBSD__
+           if ((uint)f < FD_SETSIZE) {
+#else
            if (f < FD_SETSIZE) {
+#endif
               if (f >= maxFiles)
                  maxFiles = f + 1;
               if (!files[f])
@@ -1601,7 +1638,11 @@ bool cFile::AnyFileReady(int FileDes, in
       if (files[i])
          FD_SET(i, &set);
       }
+#ifdef __FreeBSD__
+  if (0 <= FileDes && (uint)FileDes < FD_SETSIZE && !files[FileDes])
+#else
   if (0 <= FileDes && FileDes < FD_SETSIZE && !files[FileDes])
+#endif
      FD_SET(FileDes, &set); // in case we come in with an arbitrary descriptor
   if (TimeoutMs == 0)
      TimeoutMs = 10; // load gets too heavy with 0
@@ -1696,7 +1737,9 @@ bool cSafeFile::Close(void)
 
 // --- cUnbufferedFile -------------------------------------------------------
 
+#ifndef __FreeBSD__
 #define USE_FADVISE
+#endif
 
 #define WRITE_BUFFER KILOBYTE(800)
 
@@ -1757,11 +1800,13 @@ void cUnbufferedFile::SetReadAhead(size_
   readahead = ra;
 }
 
+#ifdef USE_FADVISE
 int cUnbufferedFile::FadviseDrop(off_t Offset, off_t Len)
 {
   // rounding up the window to make sure that not PAGE_SIZE-aligned data gets freed.
   return posix_fadvise(fd, Offset - (FADVGRAN - 1), Len + (FADVGRAN - 1) * 2, POSIX_FADV_DONTNEED);
 }
+#endif
 
 off_t cUnbufferedFile::Seek(off_t Offset, int Whence)
 {
