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

#include "creole.h"
#include "parser.h"

/* ASCII API
 */

CreoleParser CreoleCreateParser()
{
    Parser* pParser = new (std::nothrow) Parser();
    return static_cast<CreoleParser>(pParser);
}

void CreoleFreeParser(CreoleParser parser)
{
    delete static_cast<Parser*>(parser);
}

void CreoleSetUserData(CreoleParser parser, void* userData)
{
    Parser* pParser = static_cast<Parser*>(parser);
    if (pParser)
        pParser->setUserData(userData);
}

void CreoleSetFeatures(CreoleParser parser, int features)
{
    Parser* pParser = static_cast<Parser*>(parser);
    if (pParser)
        pParser->setFeatures(features);
}

void CreoleSetWikiUrl(CreoleParser parser, const char* wiki, const char* url)
{
    Parser* pParser = static_cast<Parser*>(parser);
    if (pParser)
        pParser->setWikiUrl(wiki, url);
}

void CreoleSetStartElementHandler(CreoleParser parser, CreoleStartElementHandler handler)
{
    Parser* pParser = static_cast<Parser*>(parser);
    if (pParser)
        pParser->setStartElementHandler(handler);
}

void CreoleSetEndElementHandler(CreoleParser parser, CreoleEndElementHandler handler)
{
    Parser* pParser = static_cast<Parser*>(parser);
    if (pParser)
        pParser->setEndElementHandler(handler);
}

void CreoleSetCharactersHandler(CreoleParser parser, CreoleCharactersHandler handler)
{
    Parser* pParser = static_cast<Parser*>(parser);
    if (pParser)
        pParser->setCharactersHandler(handler);
}

void CreoleSetPlaceholderHandler(CreoleParser parser, CreolePlaceholderHandler handler)
{
    Parser* pParser = static_cast<Parser*>(parser);
    if (pParser)
        pParser->setPlaceholderHandler(handler);
}

void CreoleSetPluginHandler(CreoleParser parser, CreolePluginHandler handler)
{
    Parser* pParser = static_cast<Parser*>(parser);
    if (pParser)
        pParser->setPluginHandler(handler);
}

CreoleError CreoleParseDocument(CreoleParser parser, const char* text, int length)
{
    Parser* pParser = static_cast<Parser*>(parser);
    if (!pParser)
        return CREOLE_ARGS_ERR;
    CreoleError err = CREOLE_OK;
    try {
        err = pParser->parseDocument(text, length);
    } catch (std::bad_alloc&) {
        err = CREOLE_ALLOC_ERR;
    } catch (EncodingError&) {
        err = CREOLE_ENCODING_ERR;
    }
    return err;
}

CreoleError CreoleParsePluginText(CreoleParser parser, const char* text, int length)
{
    Parser* pParser = static_cast<Parser*>(parser);
    if (!pParser)
        return CREOLE_ARGS_ERR;
    CreoleError err = CREOLE_OK;
    try {
        err = pParser->parsePluginText(text, length);
    } catch (std::bad_alloc&) {
        err = CREOLE_ALLOC_ERR;
    } catch (EncodingError&) {
        err = CREOLE_ENCODING_ERR;
    }
    return err;
}

/* UTF-16 API
 */

CreoleParserW CreoleCreateParserW()
{
    ParserW* pParser = new (std::nothrow) ParserW();
    return static_cast<CreoleParserW>(pParser);
}

void CreoleFreeParserW(CreoleParserW parser)
{
    delete static_cast<ParserW*>(parser);
}

void CreoleSetUserDataW(CreoleParserW parser, void* userData)
{
    ParserW* pParser = static_cast<ParserW*>(parser);
    if (pParser)
        pParser->setUserData(userData);
}

void CreoleSetFeaturesW(CreoleParserW parser, int features)
{
    ParserW* pParser = static_cast<ParserW*>(parser);
    if (pParser)
        pParser->setFeatures(features);
}

void CreoleSetWikiUrlW(CreoleParserW parser, const CreoleWChar* wiki, const CreoleWChar* url)
{
    ParserW* pParser = static_cast<ParserW*>(parser);
    if (pParser)
        pParser->setWikiUrl(wiki, url);
}

void CreoleSetStartElementHandlerW(CreoleParserW parser, CreoleStartElementHandlerW handler)
{
    ParserW* pParser = static_cast<ParserW*>(parser);
    if (pParser)
        pParser->setStartElementHandler(handler);
}

void CreoleSetEndElementHandlerW(CreoleParserW parser, CreoleEndElementHandlerW handler)
{
    ParserW* pParser = static_cast<ParserW*>(parser);
    if (pParser)
        pParser->setEndElementHandler(handler);
}

void CreoleSetCharactersHandlerW(CreoleParserW parser, CreoleCharactersHandlerW handler)
{
    ParserW* pParser = static_cast<ParserW*>(parser);
    if (pParser)
        pParser->setCharactersHandler(handler);
}

void CreoleSetPlaceholderHandlerW(CreoleParserW parser, CreolePlaceholderHandlerW handler)
{
    ParserW* pParser = static_cast<ParserW*>(parser);
    if (pParser)
        pParser->setPlaceholderHandler(handler);
}

void CreoleSetPluginHandlerW(CreoleParserW parser, CreolePluginHandlerW handler)
{
    ParserW* pParser = static_cast<ParserW*>(parser);
    if (pParser)
        pParser->setPluginHandler(handler);
}

CreoleError CreoleParseDocumentW(CreoleParserW parser, const CreoleWChar* text, int length)
{
    ParserW* pParser = static_cast<ParserW*>(parser);
    if (!pParser)
        return CREOLE_ARGS_ERR;
    CreoleError err = CREOLE_OK;
    try {
        err = pParser->parseDocument(text, length);
    } catch (std::bad_alloc&) {
        err = CREOLE_ALLOC_ERR;
    } catch (EncodingError&) {
        err = CREOLE_ENCODING_ERR;
    }
    return err;
}

CreoleError CreoleParsePluginTextW(CreoleParserW parser, const CreoleWChar* text, int length)
{
    ParserW* pParser = static_cast<ParserW*>(parser);
    if (!pParser)
        return CREOLE_ARGS_ERR;
    CreoleError err = CREOLE_OK;
    try {
        err = pParser->parsePluginText(text, length);
    } catch (std::bad_alloc&) {
        err = CREOLE_ALLOC_ERR;
    } catch (EncodingError&) {
        err = CREOLE_ENCODING_ERR;
    }
    return err;
}
