--- diseqc.c.orig	2015-01-26 12:02:14 UTC
+++ diseqc.c
@@ -228,7 +228,14 @@ bool cDiseqc::Parse(const char *s)
   devices = CurrentDevices;
   bool result = false;
   char *sourcebuf = NULL;
+#ifdef __FreeBSD__
+  sourcebuf = MALLOC(char, 10);
+  if (commands == NULL)
+      commands = MALLOC(char, 256);
+  int fields = sscanf(s, "%9[^ ] %d %c %d %255[^\n]", sourcebuf, &slof, &polarization, &lof, commands);
+#else
   int fields = sscanf(s, "%m[^ ] %d %c %d %m[^\n]", &sourcebuf, &slof, &polarization, &lof, &commands);
+#endif
   if (fields == 4)
      commands = NULL; //XXX Apparently sscanf() doesn't work correctly if the last %m argument results in an empty string
   if (4 <= fields && fields <= 5) {
