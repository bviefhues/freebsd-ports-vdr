--- svdrp.c.orig	2015-01-12 11:16:27 UTC
+++ svdrp.c
@@ -39,6 +39,13 @@
 #include "tools.h"
 #include "videodir.h"
 
+/* FreeBSD has it's own version of isnumber(),
+   but VDR's version is incompatible */
+#ifdef __FreeBSD__
+#undef isnumber
+#endif
+
+
 // --- cSocket ---------------------------------------------------------------
 
 cSocket::cSocket(int Port, int Queue)
@@ -1423,9 +1430,9 @@ void cSVDRP::CmdNEXT(const char *Option)
      if (!*Option)
         Reply(250, "%d %s", Number, *TimeToString(Start));
      else if (strcasecmp(Option, "ABS") == 0)
-        Reply(250, "%d %ld", Number, Start);
+        Reply(250, "%d %ld", Number, (long)Start);
      else if (strcasecmp(Option, "REL") == 0)
-        Reply(250, "%d %ld", Number, Start - time(NULL));
+        Reply(250, "%d %ld", Number, (long)(Start - time(NULL)));
      else
         Reply(501, "Unknown option: \"%s\"", Option);
      }
