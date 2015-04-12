--- i18n.h.orig	2012-03-11 14:07:45 UTC
+++ i18n.h
@@ -16,7 +16,7 @@
 #define I18N_DEFAULT_LOCALE "en_US"
 #define I18N_MAX_LOCALE_LEN 16       // for buffers that hold en_US etc.
 #define I18N_MAX_LANGUAGES  256      // for buffers that hold all available languages
-
+ 
 void I18nInitialize(const char *LocaleDir = NULL);
    ///< Detects all available locales and loads the language names and codes.
    ///< If LocaleDir is given, it must point to a static string that lives
@@ -46,7 +46,11 @@ const cStringList *I18nLanguages(void);
    ///< have an actual locale installed. The rest are just dummy entries
    ///< to allow having three letter language codes for other languages
    ///< that have no actual locale on this system.
+#ifdef __FreeBSD__
+const char *I18nTranslate(const char *s, const char *Plugin = NULL) __format_arg(1);
+#else
 const char *I18nTranslate(const char *s, const char *Plugin = NULL) __attribute_format_arg__(1);
+#endif
    ///< Translates the given string (with optional Plugin context) into
    ///< the current language. If no translation is available, the original
    ///< string will be returned.
