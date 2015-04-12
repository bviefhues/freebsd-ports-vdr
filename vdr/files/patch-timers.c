--- timers.c.orig	2013-12-28 11:33:08 UTC
+++ timers.c
@@ -21,6 +21,12 @@
 // format characters in order to allow any number of blanks after a numeric
 // value!
 
+/* FreeBSD has it's own version of isnumber(),  
+   but VDR's version is incompatible */
+#ifdef __FreeBSD__
+#undef isnumber   
+#endif
+   
 // --- cTimer ----------------------------------------------------------------
 
 cTimer::cTimer(bool Instant, bool Pause, cChannel *Channel)
@@ -312,7 +318,15 @@ bool cTimer::Parse(const char *s)
      s = s2;
      }
   bool result = false;
+#ifdef __FreeBSD__
+  channelbuffer = MALLOC(char, 256);
+  daybuffer = MALLOC(char, 256);
+  filebuffer = MALLOC(char, 256);
+  aux = MALLOC(char, 256);
+  if (8 <= sscanf(s, "%u :%255[^:]:%255[^:]:%d :%d :%d :%d :%255[^:\n]:%255[^\n]", &flags, channelbuffer, daybuffer, &start, &stop, &priority, &lifetime, filebuffer, aux)) {
+#else
   if (8 <= sscanf(s, "%u :%m[^:]:%m[^:]:%d :%d :%d :%d :%m[^:\n]:%m[^\n]", &flags, &channelbuffer, &daybuffer, &start, &stop, &priority, &lifetime, &filebuffer, &aux)) {
+#endif
      ClrFlags(tfRecording);
      if (aux && !*skipspace(aux)) {
         free(aux);
