
%option reentrant noyywrap nounistd stack

%{
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

#include <assert.h>
#include "parser.h"

#pragma warning( disable: 4267 4018 4244 4996 )

using namespace CreoleAtoms;

#if defined(WIN32) || defined(_WINDOWS)
#  include <io.h>
#  define isatty _isatty
#endif

%}

%x VERBATIMBLOCK
%x VERBATIMINLINE
%x PLACEHOLDER
%x PLUGIN
%x IMAGEURL
%x IMAGESIZE
%x IMAGEALT
%x LINKURL
%s HEADING
%s TABLEROW1
%s TABLEROW

WS         [[:blank:]]*
EOL        \n
ANY        .|\n
PROTOCOL   "www."|"WWW."|"http://"|"HTTP://"|"https://"|"HTTPS://"|"ftp://"|"FTP://"|"mailto:"|"MAILTO:"|"gopher://"|"GOPHER://"|"file://"|"FILE://"
SIZE       [0-9]+"%"?

%%

%{
    #define PUSH(STATE) yy_push_state(STATE, yyg)
    #define POP() yy_pop_state(yyg)

    Parser& p = *(Parser*)yyget_extra(yyg);
%}

<<EOF>>     {
    return 0;
    }
    
^{WS}{EOL}      {
    p.flushBlock();
    }

^{WS}"="{1,6}   {
    PUSH(HEADING);
    p.flushBlock();
    char* cp = strchr(yytext, '=') + 1;
    int level = 1;
    for (; level <= 6 && *cp == '='; ++level, ++cp) /* nix */;
    p.startElement(HeadingTag[level - 1]);
    }

<HEADING>"="+{WS}{EOL}  {
    POP();
    // ignore trailing '='
    p.flushBlock();
    }

<HEADING>{EOL}  {
    POP();
    p.flushBlock();
    }
    
^"{{{"{WS}{EOL}     {
    PUSH(VERBATIMBLOCK);
    p.flushBlock();
    p.startElement(PreformattedBlockTag);
    }

<VERBATIMBLOCK>^"}}}"{WS}{EOL}  {
    POP();
    p.flushBlock();
    }

<VERBATIMBLOCK>^" "+"}}}"  {
    p.appendChars(yytext + 1); // skip 1st blank
    }

<VERBATIMBLOCK>{ANY}    {
    p.appendChars(yytext);
    }

^{WS}"*"+    {
    char* cp = strchr(yytext, '*');
    size_t order = strlen(cp);
    if (order == 2 && !p.inList()) {
        // special case for '**' ambiguity
        p.ensureBlock();
        p.toggleStyle(BoldTag);
    } else {
        p.startListItem(order, false);
    }
    }

^{WS}"#"+    {
    char* cp = strchr(yytext, '#');
    size_t order = strlen(cp);
    if (order == 2 && !p.inList() && p.hasFeatures(CREOLE_ADDITIONS)) {
        // special case for '##' ambiguity
        p.ensureBlock();
        p.toggleStyle(MonospaceTag);
    } else {
        p.startListItem(order, true);
    }
    }

^{WS}":"+    {
    if (p.hasFeatures(CREOLE_ADDITIONS)) {
        char* cp = strchr(yytext, ':');
        size_t order = strlen(cp);
        if (order == 1 && p.inDefinitionTerm()) {
            p.startDefinition();
        } else {
            p.startIndent(order, false);
        }
    } else {
        p.ensureBlock();
        p.appendChars(yytext);
    }
    }
    
^{WS}";"    {
    if (p.hasFeatures(CREOLE_ADDITIONS)) {
        p.startDefinitionTerm();
    } else {
        p.ensureBlock();
        p.appendChars(yytext);
    }
    }
    
^{WS}">"+    {
    if (p.hasFeatures(CREOLE_ADDITIONS)) {
        char* cp = strchr(yytext, '>');
        size_t order = strlen(cp);
        p.startIndent(order, true);
    } else {
        p.ensureBlock();
        p.appendChars(yytext);
    }
    }

^{WS}"-"{4,}{WS}{EOL}    {
    p.flushBlock();
    size_t level = p.startElement(HorizontalRuleTag);
    p.endElement(level);
    }

^{WS}"|""="?    {
    bool isHeader = (yytext[yyleng - 1] == '=');
    if (!p.inTable()) {
        PUSH(TABLEROW1);
        p.startFirstTableRow(isHeader);
    } else {
        PUSH(TABLEROW);
        p.startTableRow(isHeader);
    }
    }

<TABLEROW1>("|"{WS})?{EOL}  {
    POP();
    p.endFirstTableRow();
    }

<TABLEROW1>"|""="?  {
    bool isHeader = (yytext[yyleng - 1] == '=');
    p.startCellInFirstRow(isHeader);
    }

<TABLEROW>"|"{WS}{EOL}   {
    POP();
    p.endTableRow();
    }
    
<TABLEROW>"|""="?  {
    bool isHeader = (yytext[yyleng - 1] == '=');
    if (!p.startCell(isHeader)) {
        POP();
        p.appendChars(yytext);
    }
    }

{PROTOCOL}[^[:blank:][:cntrl:]]+     {
    p.ensureBlock();
    p.putFreeUrl(yytext);
    }

"~"{PROTOCOL}[^[:blank:][:cntrl:]]+     {
    p.ensureBlock();
    p.appendChars(yytext + 1);
    }

"**"    {
    p.ensureBlock();
    p.toggleStyle(BoldTag);
    }

"//"    {
    p.ensureBlock();
    p.toggleStyle(ItalicTag);
    }

"##"    {
    p.ensureBlock();
    if (p.hasFeatures(CREOLE_ADDITIONS)) {
        p.toggleStyle(MonospaceTag);
    } else {
        p.appendChars("##");
    }
    }

"^^"    {
    p.ensureBlock();
    if (p.hasFeatures(CREOLE_ADDITIONS)) {
        p.toggleStyle(SuperscriptTag);
    } else {
        p.appendChars("^^");
    }
    }
    
",,"    {
    p.ensureBlock();
    if (p.hasFeatures(CREOLE_ADDITIONS)) {
        p.toggleStyle(SubscriptTag);
    } else {
        p.appendChars(",,");
    }
    }

"__"    {
    p.ensureBlock();
    if (p.hasFeatures(CREOLE_ADDITIONS)) {
        p.toggleStyle(UnderlineTag);
    } else {
        p.appendChars("__");
    }
    }

":"    {
    if (p.hasFeatures(CREOLE_ADDITIONS) && p.inDefinitionTerm()) {
        p.startDefinition();
    } else {
        p.ensureBlock();
        p.appendChars(yytext);
    }
    }

"\\\\"  {
    p.ensureBlock();
    size_t level = p.startElement(BreakLineTag);
    p.endElement(level);
    }

"[["        {
    p.ensureBlock();
    if (p.startLink()) {
        PUSH(LINKURL);
    }
    }
    
<LINKURL>"|"    {
    POP();
    p.endLinkUrl();
    }
    
<LINKURL>"]]"   {
    POP();
    p.endLink(true);
    }

<LINKURL>{ANY}  {
    p.appendChars(yytext);
    }
    
"]]"    {
    p.endLink();
    }
    
"{{{"       {
    PUSH(VERBATIMINLINE);
    p.ensureBlock();
    p.setVerbatimLevel(p.startElement(PreformattedInlineTag));
    }

<VERBATIMINLINE>"}}}""}"*   {
    POP();
    if (yyleng > 3) {
        p.appendChars(yytext + 3);
    }
    p.endElement(p.verbatimLevel());
    }

<VERBATIMINLINE>{ANY}    {
    p.appendChars(yytext);
    }

"{{"    {
    PUSH(IMAGEURL);
    p.ensureBlock();
    p.startImage();
    }

<IMAGEURL>"?"   {
    if (p.hasFeatures(CREOLE_IMAGESIZE)) {
        POP();
        PUSH(IMAGESIZE);
    } else {
        p.appendChar('?');
    }
    }

<IMAGESIZE>{SIZE}   {
    p.imageSize(yytext);
    }

<IMAGEURL,IMAGESIZE,IMAGEALT>"}}"     {
    POP();
    p.endImage();
    }

<IMAGEURL,IMAGESIZE>"|"      {
    POP();
    PUSH(IMAGEALT);
    p.startImageAlt();
    }
    
<IMAGEURL,IMAGEALT>{ANY}    {
    p.appendChars(yytext);
    }

<IMAGESIZE>{ANY}    {
    // ignore anything but size
    }

^"<<<"   {
    PUSH(PLACEHOLDER);
    p.flushPendingChars();
    p.setAtBol(1);
    }

"<<<"   {
    PUSH(PLACEHOLDER);
    p.flushPendingChars();
    p.setAtBol(0);
    }

<PLACEHOLDER>">>>"">"*   {
    POP();
    if (yyleng > 3) {
        // output access '>'s
        p.appendChars(yytext + 3);
    }
    p.endPlaceholder();
    p.restoreAtBol();
    }

<PLACEHOLDER>{ANY}    {
    p.appendChars(yytext);
    }

^"<<"   {
    if (p.hasFeatures(CREOLE_ADDITIONS)) {
        PUSH(PLUGIN);
        p.flushPendingChars();
        p.setAtBol(1);
    } else {
        p.ensureBlock();
        p.appendChars("<<");
    }
    }

"<<"   {
    if (p.hasFeatures(CREOLE_ADDITIONS)) {
        PUSH(PLUGIN);
        p.flushPendingChars();
        p.setAtBol(0);
    } else {
        p.ensureBlock();
        p.appendChars("<<");
    }
    }

<PLUGIN>">>"">"*   {
    POP();
    if (yyleng > 2) {
        // output access '>'s
        p.appendChars(yytext + 2);
    }
    p.endPlugin();
    p.restoreAtBol();
    }

<PLUGIN>{ANY}    {
    p.appendChars(yytext);
    }

"~"/[[:blank:]\n]   {
    p.ensureBlock();
    p.appendChars(yytext);
    }

"~".    {
    p.ensureBlock();
    p.appendChars(yytext + 1);
    }
    
{ANY}       {
    p.ensureBlock();
    p.appendChars(yytext);
    }

%%

void BaseParser::createScanner()
{
    scanner_ = 0;
    yyscan_t scanner;
    int err = yylex_init_extra(this, &scanner);
    if (!err) {
        scanner_ = scanner;
    }
}

void BaseParser::freeScanner()
{
    if (scanner_) {
        yylex_destroy(scanner_);
        scanner_ = 0;
    }
}

CreoleError BaseParser::readBuffer(const char* text, int len)
{
    assert(text && len > 0);
    
    if (!scanner_)
        return CREOLE_ALLOC_ERR;

    prepareBuffer(input_, text, len);
    YY_BUFFER_STATE buff = yy_scan_buffer(&(input_[0]), input_.size(), scanner_);
    if (!buff)
        return CREOLE_ALLOC_ERR;
        
    CreoleError err = CREOLE_OK;
    
    // scan buffer
    if (yylex(scanner_))
        err = CREOLE_INPUT_ERR;
    
    yy_delete_buffer(buff, scanner_);
    return err;
}

CreoleError BaseParser::readPluginText(const char* text, int len)
{
    assert(text && len > 0);
    
    if (!scanner_)
        return CREOLE_ALLOC_ERR;

    // alloc input buffer
    std::vector<char> input;
    prepareBuffer(input, text, len);
    
    yyscan_t yyscanner = scanner_; // needed for flex macros
    struct yyguts_t * yyg = (struct yyguts_t*)yyscanner; // dito

    // scan text from buffer, be shure to keep at_bol consistent
    YY_BUFFER_STATE curBuff = YY_CURRENT_BUFFER;
    YY_BUFFER_STATE buff = yy_scan_buffer(&(input[0]), input.size(), scanner_);
    if (!buff)
        return CREOLE_ALLOC_ERR;
    yy_set_bol(atBol());
    CreoleError err = CREOLE_OK;
    if (yylex(yyg))
        err = CREOLE_INPUT_ERR;
    yy_delete_buffer(buff, yyg);
    yy_switch_to_buffer(curBuff, yyg);
    return err;
}

void BaseParser::setScannerAtBol(int at_bol)
{
    assert(scanner_);

    yyscan_t yyscanner = scanner_; // needed for flex macros
    struct yyguts_t * yyg = (struct yyguts_t*)yyscanner; // dito

    yy_set_bol(at_bol);
}
