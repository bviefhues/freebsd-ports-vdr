--- libsi/si.c.orig	2015-02-10 13:42:41 UTC
+++ libsi/si.c
@@ -13,7 +13,9 @@
 #include "si.h"
 #include <errno.h>
 #include <iconv.h>
+#ifndef __FreeBSD__
 #include <malloc.h>
+#endif
 #include <stdlib.h> // for broadcaster stupidity workaround
 #include <string.h>
 #include "descriptor.h"
@@ -391,7 +393,11 @@ bool convertCharacterTable(const char *f
   if (SystemCharacterTable) {
      iconv_t cd = iconv_open(SystemCharacterTable, fromCode);
      if (cd != (iconv_t)-1) {
+#ifdef __FreeBSD__
+        const char *fromPtr = (char *)from;
+#else
         char *fromPtr = (char *)from;
+#endif
         while (fromLength > 0 && toLength > 1) {
            if (iconv(cd, &fromPtr, &fromLength, &to, &toLength) == size_t(-1)) {
               if (errno == EILSEQ) {
