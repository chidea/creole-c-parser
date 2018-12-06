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

#include <string.h>
#include <malloc.h>
#include "converter.h"

namespace
{
    /// Helper class closing file pointer when going out of scope.
    class ScopedFilePtr
    {
    public:
        ScopedFilePtr(FILE* pFile) : pFile_(pFile) {}
        ~ScopedFilePtr() {
            if (pFile_) {
                fclose(pFile_);
                pFile_ = 0;
            }
        }
    private:
        FILE* pFile_;
    };

    /// Helper class freeing creole parser when going out of scope.
    class ScopedParser
    {
    public:
        ScopedParser(CreoleParser parser) : parser_(parser) {}
        ~ScopedParser() {
            if (parser_) {
                CreoleFreeParser(parser_);
            }
        }
    private:
        CreoleParser parser_;
    };

    bool isWhite(char c) {
        switch (c) {
            case ' ':
            case '\t':
            case '\n':
            case '\r':
                return true;
        }
        return false;
    }

} // utils

Converter::Converter()
    : inputFileName_(0)
    , outputFileName_(0)
    , styleSheet_(0)
    , pageTitle_(0)
    , internalUrl_(0)
    , bodyOnly_(false)
    , lastError_(0)
    , outputFile_(0)
{}

const char* Converter::lastError() const
{
    return (lastError_ ? lastError_ : "");
}

void Converter::setError(const char* errorText)
{
    lastError_ = errorText;
}

void Converter::setInputFileName(const char* fileName)
{
    inputFileName_ = fileName;
}

void Converter::setOutputFileName(const char* fileName)
{
    outputFileName_ = fileName;
}

void Converter::setStyleSheet(const char* styleSheet)
{
    styleSheet_ = styleSheet;
}

void Converter::setPageTitle(const char* title)
{
    pageTitle_ = title;
}

void Converter::setInternalLinkUrl(const char* url)
{
    internalUrl_ = url;
}

void Converter::setOutputBodyOnly(bool bodyOnly)
{
    bodyOnly_ = bodyOnly;
}

bool Converter::hasUtf8Bom(FILE* inFile)
{
    fpos_t pos;
    if (0 != fgetpos(inFile, &pos)) {
        return false;
    }
    unsigned char bom[3];
    if (3 == fread(bom, 1, 3, inFile)) {
        if (bom[0] == 0xEF && bom[1] == 0xBB && bom[2] == 0xBF) {
            return true;
        }
    }
    fsetpos(inFile, &pos);
    return false;
}

int Converter::convert()
{
    bool isFromStdin = !strcmp(inputFileName_, "-");
    // open files
    FILE* inFile = 0;
    if (inputFileName_) {
        if(isFromStdin) inFile = stdin;
        else inFile = fopen(inputFileName_, "rb");
        if (!inFile) {
            setError("Could not open input file.");
            return 1;
        }
    } else {
        setError("No input file given.");
        return 2;
    }
    bool isUtf8 = isFromStdin? false : hasUtf8Bom(inFile);

    outputFile_ = stdout;
    if (outputFileName_) {
        outputFile_ = fopen(outputFileName_, "w");
        if (!outputFile_) {
            setError("Could not open output file.");
            return 3;
        }
    }
    ScopedFilePtr scopedFilePtr(outputFileName_ ? outputFile_ : 0);

    // init parser
    CreoleParser parser = CreoleCreateParser();
    if (!parser) {
        setError("Could not create parser.");
        return 4;
    }
    ScopedParser scopedParser(parser);

    CreoleSetUserData(parser, this);
    if (internalUrl_) {
        CreoleSetWikiUrl(parser, 0, internalUrl_);
    }
    CreoleSetStartElementHandler(parser, startElementHandler);
    CreoleSetEndElementHandler(parser, endElementHandler);
    CreoleSetCharactersHandler(parser, charactersHandler);
    CreoleSetPluginHandler(parser, pluginHandler);

    if (!outputBodyOnly()) {
        // write head
        fputs(
            "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN\" \"http://www.w3.org/TR/html4/loose.dtd\">\n"
            "<html>\n"
            "<head>\n",
            outputFile_);
        if (isUtf8) {
            fputs("<meta http-equiv=\"content-type\" content=\"text/html; charset=UTF-8\">\n", outputFile_);
        }
        fputs("<meta name=\"generator\" content=\"creole/c\">\n", outputFile_);
        if (pageTitle_) {
            fprintf(outputFile_, "<title>%s</title>\n", pageTitle_);
        }
        if (styleSheet_) {
            fprintf(outputFile_, "<link rel=\"stylesheet\" type=\"text/css\" href=\"%s\">\n", styleSheet_);
        }
        fputs("</head>\n", outputFile_);
    }
    /*
    // read and parse input
    if (fseek(inFile, 0, SEEK_END)) {
        setError("Could not read input file.");
        return 4;
    }
    size_t nsize = ftell(inFile);
    if (nsize <= 0) {
        setError("Could not read input file.");
        return 4;
    }
    rewind(inFile);
    char* text = (char*)malloc(nsize);
    if (!text) {
        setError("Out of memory.");
        return 5;
    }
    size_t nread = fread(text, 1, nsize, inFile);
    if (nread < nsize) {
        setError("Could not read input file.");
        return 4;
    }
    int err = CreoleParseDocument(parser, text, (int)nread);
    if (err != CREOLE_OK) {
        setError("Parser error.");
        return 6;
    }
    free(text);*/
#define MAX_LEN 127
    char line[MAX_LEN], *rst, *linep=(char *)0, *lineaddp;
    int linelen;
    while (!feof(inFile)){
      if(rst = fgets(line, MAX_LEN, inFile)){
        int len = strlen(rst);
        if(len == MAX_LEN-1){ // line is unfinished or fits in line max length
          if(!linep){ // memory single filled
            linep = (char *)malloc(sizeof(char)*MAX_LEN);
            strncpy(linep, line, sizeof(char)*MAX_LEN);
            linelen=len;
          }else{ // memory double+ filled
            linep = (char *)realloc((void *)linep, sizeof(char)*(linelen+MAX_LEN));
            strncpy(linep+linelen, line, sizeof(char)*MAX_LEN);
            linelen+=len;
          }
        }else{ // line is finished
          if(linep){ // has memory longer than MAX_LEN and line is finished
            linep = (char *)realloc((void *)linep, sizeof(char)*(linelen+len+1));
            strncpy(linep+linelen, line, sizeof(char)*(len+1));
            rst = linep;
            len += linelen;
          }
          int err = CreoleParseDocument(parser, rst, (int)len);
          if (err != CREOLE_OK) {
            setError("Parser error.");
            return 6;
          }
          if(linep){
            free(linep);
            linep = 0;
            linelen = 0;
          }
        }
      }else if(ferror(inFile)){
        setError("Cannot read line from input.");
        return 6;
      }
    }
    if (!outputBodyOnly()) {
        fputs("\n</html>\n", outputFile_);
    }

    return 0;
}

void Converter::startElementHandler(void* userData, const char* name, const char** atts, int numAtts)
{
    static_cast<Converter*>(userData)->startElement(name, atts, numAtts);
}

void Converter::endElementHandler(void* userData, const char* name)
{
    static_cast<Converter*>(userData)->endElement(name);
}

void Converter::charactersHandler(void* userData, const char* text)
{
    static_cast<Converter*>(userData)->characters(text);
}

void Converter::pluginHandler(void* userData, const char* text)
{
    static_cast<Converter*>(userData)->plugin(text);
}

void Converter::startElement(const char* name, const char** atts, int numAtts)
{
    // ignore <verb> tags, but not their content
    if (0 == ::strcmp(name, "verb")) {
        return;
    }

    // <br></br> causes some browsers to insert an empty line
    // thus we replace it by <br/>
    if (0 == ::strcmp(name, "br")) {
        fputs("<br/>", outputFile_);
        return;
    }

    if (outputBodyOnly() && 0 == ::strcmp(name, "body")) {
        // ignore <body> tag, but not its content
        return;
    }

    fprintf(outputFile_, "<%s", name);
    for (const char** a = atts; numAtts > 0; a += 2, numAtts -= 2) {
        fprintf(outputFile_, " %s=\"%s\"", *a, *(a+1));
    }
    fputs(">", outputFile_);
}

void Converter::endElement(const char* name)
{
    if (0 == ::strcmp(name, "br")) {
        return;
    }
    if (0 == ::strcmp(name, "verb")) {
        return;
    }
    if (outputBodyOnly() && 0 == ::strcmp(name, "body")) {
        return;
    }
    fprintf(outputFile_, "</%s>", name);
}

void Converter::characters(const char* text)
{
    for (; *text; ++text) {
        switch (*text) {
            case '\t':
            case '\n':
            case '\r':
                fputc(*text, outputFile_);
                break;
            case '&':
                fputs("&amp;", outputFile_);
                break;
            case '<':
                fputs("&lt;", outputFile_);
                break;
#if 0
            // IE doesn't know about &apos;
            case '>':
                fputs("&gt;", outputFile_);
                break;
            case '"':
                fputs("&quot;", outputFile_);
                break;
            case '\'':
                fputs("&apos;", outputFile_);
                break;
#endif
            default:
                {
                    // simply ignore invalid control chars
                    if (*text >= ' ' || *text < 0) {
                        fputc(*text, outputFile_);
                    }
                }
                break;
        }
    }
}

void Converter::plugin(const char* text)
{
    while (isWhite(*text)) {
        ++text;
    }
    // << ! ... >> yields raw text
    if ('!' == *text) {
        fputs(text + 1, outputFile_);
    }
}
