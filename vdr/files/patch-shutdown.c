--- shutdown.c.orig	2013-10-02 09:02:01 UTC
+++ shutdown.c
@@ -116,7 +116,7 @@ void cShutdownHandler::CheckManualStart(
      }
   else {
      // Set inactive from now on
-     dsyslog("scheduled wakeup time in %ld minutes, assuming automatic start of VDR", Delta / 60);
+     dsyslog("scheduled wakeup time in %ld minutes, assuming automatic start of VDR", (long)(Delta / 60));
      SetUserInactiveTimeout(-3, true);
      }
 }
@@ -130,7 +130,7 @@ void cShutdownHandler::SetShutdownComman
 void cShutdownHandler::CallShutdownCommand(time_t WakeupTime, int Channel, const char *File, bool UserShutdown)
 {
   time_t Delta = WakeupTime ? WakeupTime - time(NULL) : 0;
-  cString cmd = cString::sprintf("%s %ld %ld %d \"%s\" %d", shutdownCommand, WakeupTime, Delta, Channel, *strescape(File, "\\\"$"), UserShutdown);
+  cString cmd = cString::sprintf("%s %ld %ld %d \"%s\" %d", shutdownCommand, (long)WakeupTime, (long)Delta, Channel, *strescape(File, "\\\"$"), UserShutdown);
   isyslog("executing '%s'", *cmd);
   int Status = SystemExec(cmd, true);
   if (!WIFEXITED(Status) || WEXITSTATUS(Status))
@@ -185,7 +185,7 @@ bool cShutdownHandler::ConfirmShutdown(b
      // Timer within Min Event Timeout
      if (!Interactive)
         return false;
-     cString buf = cString::sprintf(tr("Recording in %ld minutes, shut down anyway?"), Delta / 60);
+     cString buf = cString::sprintf(tr("Recording in %ld minutes, shut down anyway?"), (long)(Delta / 60));
      if (!Interface->Confirm(buf))
         return false;
      }
@@ -200,7 +200,7 @@ bool cShutdownHandler::ConfirmShutdown(b
      // Plugin wakeup within Min Event Timeout
      if (!Interactive)
         return false;
-     cString buf = cString::sprintf(tr("Plugin %s wakes up in %ld min, continue?"), Plugin->Name(), Delta / 60);
+     cString buf = cString::sprintf(tr("Plugin %s wakes up in %ld min, continue?"), Plugin->Name(), (long)(Delta / 60));
      if (!Interface->Confirm(buf))
         return false;
      }
