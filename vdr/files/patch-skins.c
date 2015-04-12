--- skins.c.orig	2013-08-18 12:07:22 UTC
+++ skins.c
@@ -11,6 +11,19 @@
 #include "interface.h"
 #include "status.h"
 
+#if defined(__FreeBSD__) && __FreeBSD_version < 1000029
+/* XXX Implement strchrnul for FreeBSD. */
+static char *
+strchrnul (const char *s, int c_in)
+{
+	char c = c_in;
+	while (*s && (*s != c))
+		s++;
+
+	return (char *) s;
+}
+#endif
+
 // --- cSkinQueuedMessage ----------------------------------------------------
 
 class cSkinQueuedMessage : public cListObject {
