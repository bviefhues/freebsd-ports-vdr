--- thread.c.orig	2013-12-29 15:26:33 UTC
+++ thread.c
@@ -8,16 +8,21 @@
  */
 
 #include "thread.h"
-#include <errno.h>
-#include <linux/unistd.h>
-#include <malloc.h>
-#include <stdarg.h>
+#ifdef __FreeBSD__
+#include <pthread.h>
+#include <signal.h>
+#include <pthread_np.h>
+#endif
+#include <unistd.h>
 #include <stdlib.h>
+#include <stdarg.h>
 #include <sys/resource.h>
 #include <sys/syscall.h>
 #include <sys/time.h>
 #include <sys/wait.h>
+#ifndef __FreeBSD__
 #include <sys/prctl.h>
+#endif
 #include <unistd.h>
 #include "tools.h"
 
@@ -143,7 +148,9 @@ cRwLock::cRwLock(bool PreferWriter)
 {
   pthread_rwlockattr_t attr;
   pthread_rwlockattr_init(&attr);
+#ifndef __FreeBSD__
   pthread_rwlockattr_setkind_np(&attr, PreferWriter ? PTHREAD_RWLOCK_PREFER_WRITER_NP : PTHREAD_RWLOCK_PREFER_READER_NP);
+#endif
   pthread_rwlock_init(&rwlock, &attr);
 }
 
@@ -179,7 +186,11 @@ cMutex::cMutex(void)
   locked = 0;
   pthread_mutexattr_t attr;
   pthread_mutexattr_init(&attr);
-  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK_NP);
+#ifdef __FreeBSD__
+  pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
+#else
+   pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK_NP);
+#endif
   pthread_mutex_init(&mutex, &attr);
 }
 
@@ -229,7 +240,11 @@ void cThread::SetPriority(int Priority)
 
 void cThread::SetIOPriority(int Priority)
 {
+#ifdef __FreeBSD__
+  esyslog("ERROR: syscall(SYS_ioprio_set ...) unsupported on FreeBSD");
+#else
   if (syscall(SYS_ioprio_set, 1, 0, (Priority & 0xff) | (3 << 13)) < 0) // idle class
+#endif
      LOG_ERROR;
 }
 
@@ -340,7 +355,11 @@ void cThread::Cancel(int WaitSeconds)
 
 tThreadId cThread::ThreadId(void)
 {
-  return syscall(__NR_gettid);
+#ifdef __FreeBSD__
+   return reinterpret_cast<long>(::pthread_self());
+#else
+   return syscall(__NR_gettid);
+#endif
 }
 
 void cThread::SetMainThreadId(void)
