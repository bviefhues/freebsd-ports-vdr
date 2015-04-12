--- config.c.orig	2015-02-10 12:24:13 UTC
+++ config.c
@@ -16,6 +16,7 @@
 #include "menu.h"
 #include "plugin.h"
 #include "recording.h"
+#include <netinet/in.h>
 
 // IMPORTANT NOTE: in the 'sscanf()' calls there is a blank after the '%d'
 // format characters in order to allow any number of blanks after a numeric
