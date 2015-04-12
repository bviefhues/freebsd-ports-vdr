--- ci.c.orig	2015-02-02 13:57:39 UTC
+++ ci.c
@@ -10,7 +10,7 @@
 #include "ci.h"
 #include <ctype.h>
 #include <linux/dvb/ca.h>
-#include <malloc.h>
+#include <stdlib.h>
 #include <netinet/in.h>
 #include <poll.h>
 #include <string.h>
