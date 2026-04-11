/* i18n.h — Locale-aware CLI strings for uchardet.
 *
 * Detects the user's language from LC_ALL, LC_MESSAGES, or LANG
 * environment variables and returns translated UI strings.
 * Error messages include the English original for searchability:
 *   "Datos error. (search for: Handle data error)"
 *
 * Follows the same 30-language set as the project README translations.
 */
#ifndef I18N_H___
#define I18N_H___

struct UchardetStrings {
    const char* tool_name;        /* "uchardet Command Line Tool" */
    const char* version_fmt;      /* "Version %s"                 */
    const char* authors_fmt;      /* "Authors: %s"                */
    const char* bug_report_fmt;   /* "Bug Report: %s"             */
    const char* usage;            /* "Usage:"                     */
    const char* usage_cmd;        /* " uchardet [Options] [File]..." */
    const char* options;          /* "Options:"                   */
    const char* opt_version;      /* " -v, --version  ..."        */
    const char* opt_help;         /* " -h, --help     ..."        */
    const char* unknown;          /* "unknown"                    */
    const char* handle_data_err;  /* "Handle data error."         */
    const char* use_help_fmt;     /* "Please use %s --help."      */
};

/* Returns the string table for the current locale.
 * Falls back to English if the locale is unrecognized. */
const UchardetStrings* i18n_get_strings(void);

#endif /* I18N_H___ */
