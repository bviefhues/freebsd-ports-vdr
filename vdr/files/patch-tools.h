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
