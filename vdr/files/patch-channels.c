--- channels.c.orig	2015-02-01 13:47:05 UTC
+++ channels.c
@@ -29,7 +29,12 @@ tChannelID tChannelID::FromString(const 
   int tid;
   int sid;
   int rid = 0;
+#ifdef __FreeBSD__
+  sourcebuf = MALLOC(char, 10);
+  int fields = sscanf(s, "%9[^-]-%d-%d-%d-%d", sourcebuf, &nid, &tid, &sid, &rid);
+#else
   int fields = sscanf(s, "%m[^-]-%d-%d-%d-%d", &sourcebuf, &nid, &tid, &sid, &rid);
+#endif
   if (fields == 4 || fields == 5) {
      int source = cSource::FromString(sourcebuf);
      free(sourcebuf);
@@ -604,7 +609,18 @@ bool cChannel::Parse(const char *s)
      char *apidbuf = NULL;
      char *tpidbuf = NULL;
      char *caidbuf = NULL;
+#ifdef __FreeBSD__
+     namebuf = MALLOC(char, 256);
+     sourcebuf = MALLOC(char, 10);
+     parambuf = MALLOC(char, 256);
+     vpidbuf = MALLOC(char, 256);
+     apidbuf = MALLOC(char, 256);
+     tpidbuf = MALLOC(char, 256);
+     caidbuf = MALLOC(char, 256);
+     int fields = sscanf(s, "%255[^:]:%d :%255[^:]:%255[^:] :%d :%255[^:]:%255[^:]:%255[^:]:%255[^:]:%d :%d :%d :%d ", namebuf, &frequency, parambuf, sourcebuf, &srate, vpidbuf, apidbuf, tpidbuf, caidbuf, &sid, &nid, &tid, &rid);
+#else
      int fields = sscanf(s, "%m[^:]:%d :%m[^:]:%m[^:] :%d :%m[^:]:%m[^:]:%m[^:]:%m[^:]:%d :%d :%d :%d ", &namebuf, &frequency, &parambuf, &sourcebuf, &srate, &vpidbuf, &apidbuf, &tpidbuf, &caidbuf, &sid, &nid, &tid, &rid);
+#endif
      if (fields >= 9) {
         if (fields == 9) {
            // allow reading of old format
