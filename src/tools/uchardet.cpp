/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is Mozilla Universal charset detector code.
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 2001
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *          BYVoid <byvoid.kcp@gmail.com>
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */
#include "../uchardet.h"
#include "i18n.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <getopt.h>
#include <iostream>
#include <stdio.h>

#ifndef VERSION
#define VERSION "Unknown"
#endif
#define BUFFER_SIZE 65536

char buffer[BUFFER_SIZE];

void detect(FILE * fp)
{
    const UchardetStrings* s = i18n_get_strings();
    uchardet_t handle = uchardet_new();

    while (!feof(fp))
    {
        size_t len = fread(buffer, 1, BUFFER_SIZE, fp);
        int retval = uchardet_handle_data(handle, buffer, len);
        if (retval != 0)
        {
            fprintf(stderr, "%s\n", s->handle_data_err);
            exit(1);
        }
    }
    uchardet_data_end(handle);

    const char * charset = uchardet_get_charset(handle);
    if (*charset)
    	printf("%s\n", charset);
	else
		printf("%s\n", s->unknown);

    uchardet_delete(handle);
}

void show_version()
{
    const UchardetStrings* s = i18n_get_strings();
    printf("\n");
    printf("%s\n", s->tool_name);
    printf(s->version_fmt, VERSION);
    printf("\n\n");
    printf(s->authors_fmt, "BYVoid, Jehan");
    printf("\n");
    printf(s->bug_report_fmt, "https://bugs.freedesktop.org/enter_bug.cgi?product=uchardet");
    printf("\n\n");
}

void show_usage()
{
    const UchardetStrings* s = i18n_get_strings();
    show_version();
    printf("%s\n", s->usage);
    printf("%s\n", s->usage_cmd);
    printf("\n");
    printf("%s\n", s->options);
    printf("%s\n", s->opt_version);
    printf("%s\n", s->opt_help);
    printf("\n");
}

int main(int argc, char ** argv)
{
    const UchardetStrings* s = i18n_get_strings();

    static struct option longopts[] =
    {
        { "version", no_argument, NULL, 'v' },
        { "help", no_argument, NULL, 'h' },
        { 0, 0, 0, 0 },
    };

    static int oc;
    while((oc = getopt_long(argc, argv, "vh", longopts, NULL)) != -1)
    {
        switch (oc)
        {
        case 'v':
            show_version();
            return 0;
        case 'h':
            show_usage();
            return 0;
        case '?':
            printf(s->use_help_fmt, argv[0]);
            printf("\n");
            return 1;
        }
    }

    FILE * f = stdin;
    int error_seen = 0;
    if (argc < 2)
    {
        // No file arg, use stdin by default
        detect(f);
    }
    for (int i = 1; i < argc; i++)
    {
        const char *filename = argv[i];
        f = fopen(filename, "r");
        if (f == NULL)
        {
            perror(filename);
            error_seen = 1;
            continue;
        }
        if (argc > 2)
        {
            printf("%s: ", filename);
        }
        detect(f);
    }

    return error_seen;
}
