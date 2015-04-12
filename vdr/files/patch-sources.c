--- sources.c.orig	2014-03-09 12:05:42 UTC
+++ sources.c
@@ -31,7 +31,14 @@ cSource::~cSource()
 bool cSource::Parse(const char *s)
 {
   char *codeBuf = NULL;
+#ifdef __FreeBSD__
+  codeBuf = MALLOC(char, 10);
+  if (description == NULL)
+	description = MALLOC(char, 256);
+  if (2 == sscanf(s, "%9[^ ] %255[^\n]", codeBuf, description))
+#else
   if (2 == sscanf(s, "%m[^ ] %m[^\n]", &codeBuf, &description))
+#endif
      code = FromString(codeBuf);
   free(codeBuf);
   return code != stNone && description && *description;
