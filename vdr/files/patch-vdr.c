--- vdr.c.orig	2015-02-10 14:13:12 UTC
+++ vdr.c
@@ -32,8 +32,13 @@
 #include <pwd.h>
 #include <signal.h>
 #include <stdlib.h>
+#ifndef __FreeBSD__
 #include <sys/capability.h>
 #include <sys/prctl.h>
+#else
+#include <sys/types.h>
+#include <login_cap.h>
+#endif
 #ifdef SDNOTIFY
 #include <systemd/sd-daemon.h>
 #endif
@@ -112,8 +117,10 @@ static bool SetUser(const char *UserName
         fprintf(stderr, "vdr: cannot set user id %u: %s\n", (unsigned int)user->pw_uid, strerror(errno));
         return false;
         }
+#ifndef __FreeBSD__
      if (UserDump && prctl(PR_SET_DUMPABLE, 1, 0, 0, 0) < 0)
         fprintf(stderr, "vdr: warning - cannot set dumpable: %s\n", strerror(errno));
+#endif
      setenv("HOME", user->pw_dir, 1);
      setenv("USER", user->pw_name, 1);
      setenv("LOGNAME", user->pw_name, 1);
@@ -122,6 +129,7 @@ static bool SetUser(const char *UserName
   return true;
 }
 
+#ifndef __FreeBSD__
 static bool DropCaps(void)
 {
   // drop all capabilities except selected ones
@@ -148,6 +156,7 @@ static bool SetKeepCaps(bool On)
      }
   return true;
 }
+#endif
 
 static void SignalHandler(int signum)
 {
@@ -506,14 +515,18 @@ int main(int argc, char *argv[])
   if (VdrUser && geteuid() == 0) {
      StartedAsRoot = true;
      if (strcmp(VdrUser, "root")) {
+#ifndef __FreeBSD__
         if (!SetKeepCaps(true))
            return 2;
+#endif
         if (!SetUser(VdrUser, UserDump))
            return 2;
+#ifndef __FreeBSD__
         if (!SetKeepCaps(false))
            return 2;
         if (!DropCaps())
            return 2;
+#endif
         }
      }
 
@@ -684,6 +697,25 @@ int main(int argc, char *argv[])
            CodeSet++; // skip the dot
         }
      }
+#ifdef __FreeBSD__
+  // If we are started without a locale query login.conf(5) for
+  // a lang setting
+
+  if (!CodeSet || !strcmp(CodeSet, "US-ASCII")) {
+     struct passwd *User = getpwnam(VdrUser);
+     login_cap_t *Lc = login_getuserclass(User);
+     if (!Lc)
+        Lc = login_getpwclass(User);
+     if (Lc) {
+        const char *Lang = login_getcapstr(Lc, "lang", NULL, NULL);
+        if (Lang && setlocale(LC_CTYPE, Lang)) {
+           CodeSet = nl_langinfo(CODESET);
+           setenv("LANG", Lang, 1);
+           }
+        login_close(Lc);
+        }
+     }
+#endif
   if (CodeSet) {
      bool known = SI::SetSystemCharacterTable(CodeSet);
      isyslog("codeset is '%s' - %s", CodeSet, known ? "known" : "unknown");
