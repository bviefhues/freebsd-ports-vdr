--- tools.h.orig	2015-02-07 15:12:26 UTC
+++ tools.h
@@ -27,6 +27,13 @@
 #include <sys/stat.h>
 #include <sys/types.h>
 
+#ifdef __FreeBSD__
+#include <sys/param.h>
+typedef int (*__compar_fn_t) (__const void *, __const void *);
+#undef isnumber
+#endif
+
+
 typedef unsigned char uchar;
 
 extern int SysLogLevel;
@@ -253,7 +260,7 @@ bool SpinUpDisk(const char *FileName);
 void TouchFile(const char *FileName);
 time_t LastModifiedTime(const char *FileName);
 off_t FileSize(const char *FileName); ///< returns the size of the given file, or -1 in case of an error (e.g. if the file doesn't exist)
-cString WeekDayName(int WeekDay);
+cString WeekDayNameInt(int WeekDay);
     ///< Converts the given WeekDay (0=Sunday, 1=Monday, ...) to a three letter
     ///< day name.
 cString WeekDayName(time_t t);
