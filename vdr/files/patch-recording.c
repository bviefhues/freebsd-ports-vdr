--- recording.c.orig	2015-02-16 07:49:14 UTC
+++ recording.c
@@ -32,6 +32,10 @@
 
 #define SUMMARYFALLBACK
 
+#if defined(__FreeBSD__) && !defined(O_LARGEFILE)
+#define O_LARGEFILE 0
+#endif
+
 #define RECEXT       ".rec"
 #define DELEXT       ".del"
 /* This was the original code, which works fine in a Linux only environment.
@@ -472,10 +476,12 @@ bool cRecordingInfo::Read(FILE *f)
              case 'E': {
                          unsigned int EventID;
                          time_t StartTime;
+                         long stime;
                          int Duration;
                          unsigned int TableID = 0;
                          unsigned int Version = 0xFF;
-                         int n = sscanf(t, "%u %ld %d %X %X", &EventID, &StartTime, &Duration, &TableID, &Version);
+                         int n = sscanf(t, "%u %ld %d %X %X", &EventID, &stime, &Duration, &TableID, &Version);
+                         StartTime = (time_t)stime;
                          if (n >= 3 && n <= 5) {
                             ownEvent->SetEventID(EventID);
                             ownEvent->SetStartTime(StartTime);
