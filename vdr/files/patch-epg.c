--- epg.c.orig	2013-12-28 11:33:08 UTC
+++ epg.c
@@ -32,7 +32,13 @@ cString tComponent::ToString(void)
 bool tComponent::FromString(const char *s)
 {
   unsigned int Stream, Type;
+#ifdef __FreeBSD__
+  if (description == NULL)
+	description = MALLOC(char, 256);
+  int n = sscanf(s, "%X %02X %7s %255[^\n]", &Stream, &Type, language, description); // 7 = MAXLANGCODE2 - 1
+#else
   int n = sscanf(s, "%X %02X %7s %m[^\n]", &Stream, &Type, language, &description); // 7 = MAXLANGCODE2 - 1
+#endif
   if (n != 4 || isempty(description)) {
      free(description);
      description = NULL;
@@ -432,7 +438,7 @@ cString cEvent::GetVpsString(void) const
 void cEvent::Dump(FILE *f, const char *Prefix, bool InfoOnly) const
 {
   if (InfoOnly || startTime + duration + Setup.EPGLinger * 60 >= time(NULL)) {
-     fprintf(f, "%sE %u %ld %d %X %X\n", Prefix, eventID, startTime, duration, tableID, version);
+     fprintf(f, "%sE %u %ld %d %X %X\n", Prefix, eventID, (long)startTime, duration, tableID, version);
      if (!isempty(title))
         fprintf(f, "%sT %s\n", Prefix, title);
      if (!isempty(shortText))
@@ -457,7 +463,7 @@ void cEvent::Dump(FILE *f, const char *P
             }
         }
      if (vps)
-        fprintf(f, "%sV %ld\n", Prefix, vps);
+        fprintf(f, "%sV %ld\n", Prefix, (long)vps);
      if (!InfoOnly)
         fprintf(f, "%se\n", Prefix);
      }
@@ -519,7 +525,9 @@ bool cEvent::Read(FILE *f, cSchedule *Sc
                           int Duration;
                           unsigned int TableID = 0;
                           unsigned int Version = 0xFF; // actual value is ignored
-                          int n = sscanf(t, "%u %ld %d %X %X", &EventID, &StartTime, &Duration, &TableID, &Version);
+                          long stime;
+                          int n = sscanf(t, "%u %ld %d %X %X", &EventID, &stime, &Duration, &TableID, &Version);
+                          StartTime = (time_t)stime;
                           if (n >= 3 && n <= 5) {
                              Event = (cEvent *)Schedule->GetEvent(EventID, StartTime);
                              cEvent *newEvent = NULL;
