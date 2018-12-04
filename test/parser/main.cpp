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
 * Test frame for creole/c parser.
 *
 * Outputs elements and character data as HTML. The encoding 
 * of the output is the same as of the input.
 *
 * Sets up a plugin handler and defines a internal link URL
 * and an external wiki URL for testing.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "creole.h"

/// User data for callbacks.
struct ParserData
{
    CreoleParser parser;
    FILE* outFile;
};


/** Output start element as XML formatted start tag.
 */
void startElementHandler(void* userData, const char* name, const char** atts, int numAtts)
{
    FILE* outFile = ((ParserData*)userData)->outFile;
    // <br></br> causes some browsers to insert an empty line
    // thus we replace it by <br/>
    if (0 == ::strcmp(name, "br")) {
        fputs("<br/>", outFile);
        return;
    }
    fprintf(outFile, "<%s", name);
    for (const char** a = atts; numAtts > 0; a += 2, numAtts -= 2) {
        fprintf(outFile, " %s=\"%s\"", *a, *(a+1));
    }
    fputs(">", outFile);
}

/** Output end element as XML formatted end tag.
 */
void endElementHandler(void* userData, const char* name)
{
    if (0 == ::strcmp(name, "br")) {
        return;
    }
    FILE* outFile = ((ParserData*)userData)->outFile;
    fprintf(outFile, "</%s>", name);
}

/** Output character data as XML character data.
 */
void charactersHandler(void* userData, const char* text)
{
    FILE* outFile = ((ParserData*)userData)->outFile;
    for (; *text; ++text) {
        switch (*text) {
            case '\t':
            case '\n':
            case '\r':
                fputc(*text, outFile);
                break;
            case '&':
                fputs("&amp;", outFile);
                break;
            case '<':
                fputs("&lt;", outFile);
                break;
            case '>':
                fputs("&gt;", outFile);
                break;
            case '"':
                fputs("&quot;", outFile);
                break;
            case '\'':
                fputs("&apos;", outFile);
                break;
            default:
                {
                    // simply ignore invalid control chars
                    if (*text >= ' ' || *text < 0) {
                        fputc(*text, outFile);
                    }
                }
                break;
        }
    }
    fputs(text, outFile);
}

/** Output raw character data.
 */
void writeRaw(ParserData* data, const char* text)
{
    fputs(text, data->outFile);
}

/** Handle placeholder text.
 * 
 * The input <<< ... >>> returns <!-- placeholder: ... -->.
 */
void placeholderHandler(void* userData, const char* text)
{
    ParserData* data = (ParserData*)userData;
    if (text && text[0]) {
        writeRaw(data, "<!-- placeholder:");
        writeRaw(data, text);
        writeRaw(data, "-->");
    }
}

/** Handle plugin text.
 * 
 * The input <<! ... >> reparses the text content.
 * The input <<@ ... >> simply inserts the text content.
 * Other plugin text is ignored.
 */
void pluginHandler(void* userData, const char* text)
{
    ParserData* data = (ParserData*)userData;
    if (text && text[0]) {
        if (text[0] == '!') {
            // <<! ... >> reparses the text content
            CreoleParsePluginText(data->parser, text + 1, strlen(text + 1));
        } else if (text[0] == '@') {
            // <<@ ... >> simply inserts the text content
            writeRaw(data, text + 1);
        }
    }
}

/** Print usage info and exit.
 */
void usage(int err)
{
    fputs(
        "usage: parsertest [-o #] <infile>\n"
        "       Wiki Creole parser test frame\n"
        "       -o #   output file (default stdout)\n",
        stderr);
    exit(err);
}

/** The test frames main function.
 */
int main(int argc, char* argv[])
{
    ParserData data;

    // parse args
    const char* inFileName = 0;
    const char* outFileName = 0;
    for (int i = 1; i < argc; ++i) {
        if (!strcmp(argv[i], "-o")) {
            ++i;
            if (i >= argc || outFileName)
                usage(1);
            outFileName = argv[i];
        } else {
            if (inFileName)
                usage(1);
            inFileName = argv[i];
        }
    }

    // open files
    FILE* inFile = 0;
    if (inFileName) {
        inFile = fopen(inFileName, "rb");
        if (!inFile) {
            fprintf(stderr, "Could not open %s\n", inFileName);
            exit(2);
        }
    } else {
        usage(1);
    }

    data.outFile = stdout;
    if (outFileName) {
        data.outFile = fopen(outFileName, "w");
        if (!data.outFile) {
            fprintf(stderr, "Could not open %s\n", outFileName);
            exit(2);
        }
    }

    // init parser
    data.parser = CreoleCreateParser();
    if (!data.parser) {
        fprintf(stderr, "Could not create parser\n");
        exit(3);
    }
    CreoleSetUserData(data.parser, &data);

    // setup internal link URL and one external wiki URL for testing
    CreoleSetWikiUrl(data.parser, "", "http://www.test.org/");
    CreoleSetWikiUrl(data.parser, "creole", "http://www.wikicreole.org/");

    CreoleSetStartElementHandler(data.parser, startElementHandler);
    CreoleSetEndElementHandler(data.parser, endElementHandler);
    CreoleSetCharactersHandler(data.parser, charactersHandler);
    CreoleSetPlaceholderHandler(data.parser, placeholderHandler);
    CreoleSetPluginHandler(data.parser, pluginHandler);

    // write head
    fputs("<html>\n", data.outFile);

    // read and parse input
    if (fseek(inFile, 0, SEEK_END)) {
        fprintf(stderr, "Could not read input file.");
        return 4;
    }
    size_t nsize = ftell(inFile);
    if (nsize <= 0) {
        fprintf(stderr, "Could not read input file.");
        return 4;
    }
    rewind(inFile);
    char* text = (char*)malloc(nsize);
    if (!text) {
        fprintf(stderr, "Out of memory.");
        return 5;
    }
    size_t nread = fread(text, 1, nsize, inFile);
    if (nread < nsize) {
        fprintf(stderr, "Could not read input file.");
        return 4;
    }
    int err = CreoleParseDocument(data.parser, text, (int)nread);
    if (err != CREOLE_OK) {
        fprintf(stderr, "Parser error.");
        return 6;
    }
    free(text);

    fputs("\n</html>\n", data.outFile);

    return 0;
}
