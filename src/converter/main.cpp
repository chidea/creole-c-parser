/*
 * This file is part of the creole/c parser and HTML converter.
 *
 * Copyright (c) 2010 Klaus Drescher.
 *
 * This file may be used under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation and
 * appearing in the file LICENSE.LGPL included in the packaging of this file.
 * Please review the following information to ensure the GNU Lesser General
 * Public License version 2.1 requirements will be met:
 * http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 */

/** @file main.cpp
 * Converts Wiki Creole to HTML.
 *
 * The HTML generated in fact is XHTML. The encoding of the output is the
 * same as of the input.
 *
 * Plugin text is ignored.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "converter.h"

/// The version of the converter.
#define VERSION "0.2"

/** Print usage info and exit.
 */
void usage(int err)
{
    fputs(
        "usage: creole [-h] [-b] [-s #] [-t #] [-l #] [-o #] <infile>\n"
        "       Converts Wiki Creole input file to HTML, version " VERSION ".\n"
        "       -o #   output file (default stdout)\n"
        "       -s #   refer to given style sheet\n"
        "       -t #   define page title\n"
        "       -l #   define internal link URL\n"
        "       -b     output <body> contents only without head\n"
        "       -h     display this help\n",
        stderr);
    exit(err);
}

/** The converters main function.
 */
int main(int argc, char* argv[])
{
    // parse args
    const char* inFileName = 0;
    const char* outFileName = 0;
    const char* styleSheetUrl = 0;
    const char* internalLinkUrl = 0;
    const char* title = 0;
    bool bodyOnly = false;
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-o")) {
            ++i;
            if (i >= argc || outFileName)
                usage(1);
            outFileName = argv[i];
        } else if (!strcmp(argv[i], "-s")) {
            ++i;
            if (i >= argc || styleSheetUrl)
                usage(1);
            styleSheetUrl = argv[i];
        } else if (!strcmp(argv[i], "-l")) {
            ++i;
            if (i >= argc || internalLinkUrl)
                usage(1);
            internalLinkUrl = argv[i];
        } else if (!strcmp(argv[i], "-t")) {
            ++i;
            if (i >= argc || title)
                usage(1);
            title = argv[i];
        } else if (!strcmp(argv[i], "-b")) {
            bodyOnly = true;
        } else if (!strcmp(argv[i], "-h")) {
            usage(0);
        } else {
            if (inFileName)
                usage(1);
            inFileName = argv[i];
        }
    }
    if (!inFileName) {
        usage(1);
    }

    Converter converter;
    converter.setInputFileName(inFileName);
    converter.setOutputFileName(outFileName);
    converter.setStyleSheet(styleSheetUrl);
    converter.setPageTitle(title);
    converter.setInternalLinkUrl(internalLinkUrl);
    converter.setOutputBodyOnly(bodyOnly);

    int err = converter.convert();
    if (err) {
        fputs(converter.lastError(), stderr);
    }
	return err;
}
