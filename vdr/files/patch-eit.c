--- eit.c.orig	2015-02-01 14:55:27 UTC
+++ eit.c
@@ -343,10 +343,15 @@ cTDT::cTDT(const u_char *Data)
   if (abs(diff) > MAX_TIME_DIFF) {
      mutex.Lock();
      if (abs(diff) > MAX_ADJ_DIFF) {
+#ifdef __FreeBSD__
+        isyslog("system time would have changed from %s (%ld) to %s (%ld)", *TimeToString(loctim), (long)loctim, *TimeToString(dvbtim), (long)dvbtim);
+        esyslog("stime() not available on FreeBSD, maybe use adjtime()?");
+#else
         if (stime(&dvbtim) == 0)
            isyslog("system time changed from %s (%ld) to %s (%ld)", *TimeToString(loctim), loctim, *TimeToString(dvbtim), dvbtim);
         else
            esyslog("ERROR while setting system time: %m");
+#endif
         }
      else if (time(NULL) - lastAdj > ADJ_DELTA) {
         lastAdj = time(NULL);
@@ -354,7 +359,7 @@ cTDT::cTDT(const u_char *Data)
         delta.tv_sec = diff;
         delta.tv_usec = 0;
         if (adjtime(&delta, NULL) == 0)
-           isyslog("system time adjustment initiated from %s (%ld) to %s (%ld)", *TimeToString(loctim), loctim, *TimeToString(dvbtim), dvbtim);
+           isyslog("system time adjustment initiated from %s (%ld) to %s (%ld)", *TimeToString(loctim), (long)loctim, *TimeToString(dvbtim), (long)dvbtim);
         else
            esyslog("ERROR while adjusting system time: %m");
         }
