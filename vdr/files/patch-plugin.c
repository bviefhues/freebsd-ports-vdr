--- plugin.c.orig	2012-09-01 13:10:27 UTC
+++ plugin.c
@@ -221,9 +221,13 @@ bool cDll::Load(bool Log)
      return false;
      }
   handle = dlopen(fileName, RTLD_NOW);
-  const char *error = dlerror();
+  const char *error = NULL;
+  if (!handle) {
+     error = dlerror();
+  } 
   if (!error) {
      void *(*creator)(void);
+     dlerror();
      creator = (void *(*)(void))dlsym(handle, "VDRPluginCreator");
      if (!(error = dlerror()))
         plugin = (cPlugin *)creator();
