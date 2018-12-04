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
#include "ConvertUTF.h"

#define YY_END_OF_BUFFER_CHAR 0

namespace CreoleAtoms
{

Atom BodyTag("body");
Atom PreformattedBlockTag("pre");
Atom HeadingTag[6] = { "h1", "h2", "h3", "h4", "h5", "h6" };
Atom ParagraphTag("p");
Atom IndentedParagraphTag("blockquote");
Atom HorizontalRuleTag("hr");
Atom LinkTag("a");
Atom ImageTag("img");
Atom BoldTag("b");
Atom ItalicTag("i");
Atom MonospaceTag("tt");
Atom SuperscriptTag("sup");
Atom SubscriptTag("sub");
Atom UnderlineTag("u");
Atom PreformattedInlineTag("verb");
Atom BreakLineTag("br");
Atom TableTag("table");
Atom TableRowTag("tr");
Atom TableHeadingTag("th");
Atom TableCellTag("td");
Atom ListTag("ul");
Atom OrderedListTag("ol");
Atom ListItemTag("li");
Atom DefinitionListTag("dl");
Atom DefinitionTermTag("dt");
Atom DefinitionTag("dd");

Atom HrefAttr("href");
Atom SrcAttr("src");
Atom AltAttr("alt");
Atom WidthAttr("width");
Atom HeightAttr("height");
Atom ClassAttr("class");

Atom CitationVal("citation");

}

namespace {

const size_t TableRowLevel = 1;
const size_t TableCellLevel = 2;
const size_t DefinitionListLevel = 0;
const size_t DefinitionTermLevel = 1;
const size_t DefinitionLevel = 1;

inline bool isHighSurrogate(CreoleWChar c)
{
    return (c >= 0xD800) && (c <= 0xDBFF);
}

inline bool isLowSurrogate(CreoleWChar c)
{
    return (c >= 0xDC00) && (c <= 0xDFFF);
}

inline bool isWhiteOrCtrl(char c)
{
    return (c >= 0) && (c <= ' ');
}

std::string trim(const std::string& text)
{
    const size_t len = text.length();
    size_t begin = 0;
    for (; begin < len && isWhiteOrCtrl(text[begin]); ++begin) /* nix */;
    if (begin == len)
        return std::string();

    size_t end = len - 1;
    for (; end > begin && isWhiteOrCtrl(text[end]); --end) /* nix */;

    return text.substr(begin, end + 1 - begin);
}

} // utils

using namespace CreoleAtoms;

// --------------------------------------------------------------------------
// BaseParser

BaseParser::BaseParser()
    : scanner_(0)
    , userData_(0)
    , features_(CREOLE_ADDITIONS | CREOLE_IMAGESIZE)
{
    reset();
}

BaseParser::~BaseParser()
{
    freeScanner();
}

void BaseParser::setUserData(void* userData)
{
    userData_ = userData;
}

int BaseParser::atBol() const
{
    return atBol_;
}

void BaseParser::setAtBol(int bol)
{
    setScannerAtBol(bol);
    atBol_ = bol;
}

void BaseParser::restoreAtBol()
{
    setScannerAtBol(atBol_);
}

size_t BaseParser::verbatimLevel() const
{
    return verbatimLevel_;
}

void BaseParser::setVerbatimLevel(size_t level)
{
    verbatimLevel_ = level;
}

void BaseParser::setFeatures(int features)
{
    features_ = features;
}

bool BaseParser::hasFeatures(int featureFlags) const
{
    return 0 != (featureFlags & features_);
}

void BaseParser::flushPendingChars()
{
    if (!pendingChars_.empty()) {
        putCharacters(pendingChars_.c_str());
        pendingChars_.clear();
    }
}

void BaseParser::flushBlock()
{
    // lists, images, links, tables end
    listItemLevel_ = 0;
    listStack_.clear();

    tableColumns_ = 0;
    currentColumn_ = 0;

    imageUrl_.clear();
    linkUrl_.clear();
    linkLevel_ = 0;

    flushPendingChars();
    if (!tagStack_.empty()) {
        for (TagStack::reverse_iterator i = tagStack_.rbegin(); i != tagStack_.rend(); ++i) {
            putEndElement(*i);
        }
        tagStack_.clear();
        appendChar('\n');
    }
}

void BaseParser::ensureBlock()
{
    if (tagStack_.empty()) {
        startElement(ParagraphTag);
    }
}

void BaseParser::putStartElement(Atom tag)
{
    const char* atts[1] = {0};
    putStartElement(tag, atts, 0);
}

std::string BaseParser::mapUrl(const std::string& url)
{
    std::string result = trim(url);
    // add http:// prefix to www. prefix
    if (0 == ::strncmp(result.c_str(), "www.", 4) 
        || 0 == ::strncmp(result.c_str(), "WWW.", 4)) {
        result = std::string("http://") + result;
    }
    size_t pos = result.find(':');
    if (pos == std::string::npos) {
        // interwiki link
        WikiUrlMap::iterator iu = wikiUrls_.find(std::string());
        if (iu != wikiUrls_.end()) {
            result = iu->second + result;
        }
    } else {
        std::string protocol = result.substr(0, pos);
        WikiUrlMap::iterator iu = wikiUrls_.find(protocol);
        if (iu != wikiUrls_.end()) {
            result = iu->second + result.substr(pos + 1, result.size());
        }
    }
    return result;
}

std::string BaseParser::mapFreeUrl(const std::string& url)
{
    std::string result = trim(url);
    // add http:// prefix to www. prefix
    if (0 == ::strncmp(result.c_str(), "www.", 4) 
        || 0 == ::strncmp(result.c_str(), "WWW.", 4)) {
        result = std::string("http://") + result;
    }
    return result;
}

void BaseParser::prepareBuffer(std::vector<char>& input, const char* text, int len)
{
    // alloc input buffer
    input.reserve(len + 2);
    input.clear();

    // copy text to buffer, converting EOLs
    const char* end = text + len;
    for (; text < end; ++text) {
        // convert \r and \r\n to \n
        if (*text == '\r') {
            input.push_back('\n');
            ++text;
            if (text >= end || *text != '\n') {
                --text;
            }
        } else {
            input.push_back(*text);
        }
    }
    input.push_back(YY_END_OF_BUFFER_CHAR);
    input.push_back(YY_END_OF_BUFFER_CHAR);
}

void BaseParser::reset()
{
    input_.clear();
    atBol_ = 1;

    pendingChars_.clear();
    tagStack_.clear();
    verbatimLevel_ = 0;

    listStack_.clear();
    listItemLevel_ = 0;

    imageUrl_.clear();
    imageWidth_.clear();
    imageHeight_.clear();
    linkUrl_.clear();
    linkLevel_ = 0;

    tableColumns_ = 0;
    currentColumn_ = 0;

    freeScanner();
    createScanner();
}

bool BaseParser::inParagraph()
{
    return !tagStack_.empty() && tagStack_.front() == ParagraphTag;
}

bool BaseParser::inList()
{
    return !tagStack_.empty() 
             && (tagStack_.front() == ListTag 
                   || tagStack_.front() == OrderedListTag);
}

bool BaseParser::inTable()
{
    return !tagStack_.empty() && tagStack_.front() == TableTag;
}

bool BaseParser::inDefinitionList()
{
    return !tagStack_.empty() && tagStack_.front() == DefinitionListTag;
}

bool BaseParser::inDefinitionTerm()
{
    return tagStack_.size() >= 2 && tagStack_[1] == DefinitionTermTag;
}

size_t BaseParser::startElement(Atom tag)
{
    flushPendingChars();
    putStartElement(tag);
    size_t level = tagStack_.size();
    tagStack_.push_back(tag);
    return level;
}

size_t BaseParser::startElement(Atom tag, const char** atts, int numAtts)
{
    flushPendingChars();
    putStartElement(tag, atts, numAtts);
    size_t level = tagStack_.size();
    tagStack_.push_back(tag);
    return level;
}

void BaseParser::endElement(size_t level)
{
    assert(level < tagStack_.size());
    flushPendingChars();
    while (tagStack_.size() > level) {
        putEndElement(tagStack_.back());
        tagStack_.pop_back();
    }
}

void BaseParser::putFreeUrl(char* url)
{
    static const char* punctuation = ",.?!:;\"'";

    assert(url && *url);

    // trim trailing punctuation
    char *last = url + strlen(url) - 1;
    char lastChar = *last;
    if (::strchr(punctuation, lastChar)) {
        *last = '\0';
    }

    std::string mappedUrl = mapFreeUrl(std::string(url));
    const char* atts[3] = {0, 0, 0};
    atts[0] = HrefAttr.text();
    atts[1] = mappedUrl.c_str();

    flushPendingChars();
    size_t level = startElement(LinkTag, atts, 2);
    appendChars(url);
    endElement(level);

    if (! *last) {
        appendChar(lastChar);
    }
}

void BaseParser::startListItem(size_t order, bool isOrdered)
{
    if (!inList()) {
        flushBlock();
    } else {
        assert(listItemLevel_ > 0);
        endElement(listItemLevel_);
        listItemLevel_ = 0;
    }
    if (listStack_.size() > order) {
        endElement(listStack_[order].first);
        listStack_.resize(order);
    }
    if (listStack_.size() == order && listStack_.back().second != isOrdered) {
        endElement(listStack_[order - 1].first);
        listStack_.pop_back();
    }
    while (listStack_.size() < order) {
        size_t level = startElement(isOrdered ? OrderedListTag : ListTag);
        listStack_.push_back(ListLevel(level, isOrdered));
    }
    listItemLevel_ = startElement(ListItemTag);
}

void BaseParser::startIndent(int order, bool isCitation)
{
    assert(order > 0);
    flushBlock();
    const char* atts[3] = { 0,0,0 };
    if (isCitation) {
        atts[0] = ClassAttr.text();
        atts[1] = CitationVal.text();
    }
    for (int i = 0; i < order; ++i) {
        startElement(IndentedParagraphTag, atts, (isCitation ? 2 : 0));
    }
}

void BaseParser::startDefinitionTerm()
{
    if (inDefinitionList()) {
        endElement(DefinitionTermLevel);
    } else {
        flushBlock();
        startElement(DefinitionListTag);
    }
    size_t level = startElement(DefinitionTermTag);
    assert(level = DefinitionTermLevel);
}

void BaseParser::startDefinition()
{
    endElement(DefinitionTermLevel);
    size_t level = startElement(DefinitionTag);
    assert(level = DefinitionLevel);
}

void BaseParser::toggleStyle(Atom tag)
{
    if (tagStack_.back() == tag) {
        endElement(tagStack_.size() - 1);
    } else {
        startElement(tag);
    }
}

void BaseParser::startImage()
{
    flushPendingChars();
    imageUrl_.clear();
    imageWidth_.clear();
    imageHeight_.clear();
}

void BaseParser::imageSize(const char* size)
{
    if (imageWidth_.empty()) {
        imageWidth_ = size;
    } else if (imageHeight_.empty()) {
        imageHeight_ = size;
    }
}

void BaseParser::startImageAlt()
{
    imageUrl_ = pendingChars_;
    pendingChars_.clear();
}

void BaseParser::endImage()
{
    if (imageUrl_.empty()) {
        imageUrl_ = pendingChars_;
        pendingChars_.clear();
    }
    std::string alt = pendingChars_;
    pendingChars_.clear();

    std::string mappedUrl = mapUrl(imageUrl_);
    const char* atts[9] = { 0,0,0,0,0,0,0,0,0 };
    atts[0] = SrcAttr.text();
    atts[1] = mappedUrl.c_str();
    int idx = 2;
    if (!imageWidth_.empty()) {
        atts[idx++] = WidthAttr.text();
        atts[idx++] = imageWidth_.c_str();
    }
    if (!imageHeight_.empty()) {
        atts[idx++] = HeightAttr.text();
        atts[idx++] = imageHeight_.c_str();
    }
    if (!alt.empty()) {
        atts[idx++] = AltAttr.text();
        atts[idx++] = alt.c_str();
    }

    size_t level = startElement(ImageTag, atts, idx);
    endElement(level);

    imageUrl_.clear();
}

bool BaseParser::startLink()
{
    if (linkLevel_ > 0) {
        // link in link text
        appendChars("[[");
        return false;
    } else {
        flushPendingChars();
        linkUrl_.clear();
        return true;
    }
}

void BaseParser::endLinkUrl()
{
    if (linkLevel_ == 0) {
        linkUrl_ = pendingChars_;
        pendingChars_.clear();

        std::string mappedUrl = mapUrl(linkUrl_);
        const char* atts[3] = { 0,0,0 };
        atts[0] = HrefAttr.text();
        atts[1] = mappedUrl.c_str();

        linkLevel_ = startElement(LinkTag, atts, 2);
    }
}

void BaseParser::endLink(bool urlOnly)
{
    if (urlOnly) {
        endLinkUrl();
        assert(linkLevel_ > 0);
        pendingChars_ = linkUrl_;
    }    
    if (linkLevel_ > 0) {
        endElement(linkLevel_);
        linkUrl_.clear();
        linkLevel_ = 0;
    } else {
        ensureBlock();
        appendChars("]]");
    }
}

void BaseParser::startFirstTableRow(bool isHeader)
{
    flushBlock();
    tableColumns_ = 1;
    currentColumn_ = 0;
    startElement(TableTag);
    startElement(TableRowTag);
    size_t level = startElement(isHeader ? TableHeadingTag : TableCellTag);
    assert(level == TableCellLevel);
}

void BaseParser::startCellInFirstRow(bool isHeader)
{
    ++tableColumns_;
    ++currentColumn_;
    endElement(TableCellLevel);
    size_t level = startElement(isHeader ? TableHeadingTag : TableCellTag);
    assert(level == TableCellLevel);
}

void BaseParser::endFirstTableRow()
{
    currentColumn_ = 0;
    endElement(TableRowLevel);
    appendChar('\n');
}

void BaseParser::startTableRow(bool isHeader)
{
    currentColumn_ = 0;
    if (tagStack_.size() > TableRowLevel) {
        endElement(TableRowLevel);
    }
    startElement(TableRowTag);
    size_t level = startElement(isHeader ? TableHeadingTag : TableCellTag);
    assert(level == TableCellLevel);
}

bool BaseParser::startCell(bool isHeader)
{
    ++currentColumn_;
    if (currentColumn_ < tableColumns_) {
        endElement(TableCellLevel);
        size_t level = startElement(isHeader ? TableHeadingTag : TableCellTag);
        assert(level == TableCellLevel);
        return true;
    }
    return false;
}

void BaseParser::endTableRow()
{
    currentColumn_ = 0;
    endElement(TableRowLevel);
    appendChar('\n');
}

void BaseParser::appendChar(char c)
{
    pendingChars_ += c;
}

void BaseParser::appendChars(const char* text)
{
    pendingChars_ += text;
}

// --------------------------------------------------------------------------
// Parser

Parser::Parser() 
    : startElementHandler_(0)
    , endElementHandler_(0)
    , charactersHandler_(0)
    , pluginHandler_(0)
{}

void Parser::setWikiUrl(const char* wiki, const char* url)
{
    if (!wiki) wiki = "";
    if (!url) url = "";
    wikiUrls_[std::string(wiki)] = std::string(url);
}

void Parser::setStartElementHandler(CreoleStartElementHandler handler)
{
    startElementHandler_ = handler;
}

void Parser::setEndElementHandler(CreoleEndElementHandler handler)
{
    endElementHandler_ = handler;
}

void Parser::setCharactersHandler(CreoleCharactersHandler handler)
{
    charactersHandler_ = handler;
}

void Parser::setPlaceholderHandler(CreolePluginHandler handler)
{
    placeholderHandler_ = handler;
}

void Parser::setPluginHandler(CreolePluginHandler handler)
{
    pluginHandler_ = handler;
}

CreoleError Parser::parseDocument(const char* text, int length)
{
    if (length < 0)
        return CREOLE_ARGS_ERR;
    if (length > 0 && !text)
        return CREOLE_ARGS_ERR;

    reset();
    BaseParser::putStartElement(BodyTag);
    if (length > 0) {
        CreoleError err = readBuffer(text, length);
        if (err)
            return err;
    }
    flushBlock();
    putEndElement(BodyTag);
    return CREOLE_OK;
}

CreoleError Parser::parsePluginText(const char* text, int length)
{
    if (length < 0)
        return CREOLE_ARGS_ERR;
    if (length > 0 && !text)
        return CREOLE_ARGS_ERR;
    if (length == 0)
        return CREOLE_OK;

    return readPluginText(text, length);
}

void Parser::putCharacters(const char* text)
{
    if (charactersHandler_) {
        (*charactersHandler_)(userData_, text);
    }
}

void Parser::putStartElement(Atom tag, const char** atts, int numAtts)
{
    if (startElementHandler_) {
        (*startElementHandler_)(userData_, tag.text(), atts, numAtts);
    }
}

void Parser::putEndElement(Atom tag)
{
    if (endElementHandler_) {
        (*endElementHandler_)(userData_, tag.text());
    }
}

void Parser::endPlaceholder()
{
    // we collect placeholder content in pendingChars_
    if (!placeholderHandler_) {
        pendingChars_.clear();
    } else {
        std::string text(pendingChars_);
        pendingChars_.clear();
        (*placeholderHandler_)(userData_, text.c_str());
    }
}

void Parser::endPlugin()
{
    // we collect plugin content in pendingChars_
    if (!pluginHandler_) {
        pendingChars_.clear();
    } else {
        std::string text(pendingChars_);
        pendingChars_.clear();
        (*pluginHandler_)(userData_, text.c_str());
    }
}

// --------------------------------------------------------------------------
// ParserW

void ParserW::toUtf8(const CreoleWChar* text, int length, TextBuffer& result, int* resultLength)
{
    if (result.size() < 128) {
        result.resize(128);
    }
    if (text && length > 0) {
        for (;;) {
            const UTF16* src = (const UTF16*)text;
            UTF8* dst = (UTF8*)&(result[0]);
            ConversionResult res = ConvertUTF16toUTF8(&src, src + length, &dst, dst + result.size() - 1, lenientConversion);
            if (res == conversionOK) {
                *dst = 0; // 0-terminate result
                if (resultLength) {
                    *resultLength = dst - (UTF8*)&(result[0]);
                }
                return;
            }
            if (res != targetExhausted)
                throw EncodingError();
            result.resize(2 * result.size());
        }
    } else {
        result[0] = 0;
        if (resultLength) {
            *resultLength = 0;
        }
    }
}

void ParserW::toUtf8(const CreoleWChar* text, TextBuffer& result, int* resultLength)
{
    size_t length = 0;
    if (text) {
#if defined _WINDOWS || defined WIN32
        length = ::wcslen(text);
#else
        const CreoleWChar* end;
        for (end = text; *end; ++end) /* nix */;
        length = end - text;
#endif
    }
    if (length > INT_MAX)
        throw std::bad_alloc();
    toUtf8(text, int(length), result, resultLength);
}

void ParserW::toUtf16(const char* text, TextBufferW& result)
{
    if (result.size() < 128) {
        result.resize(128);
    }
    if (text && text[0]) {
        size_t length = ::strlen(text);
        if (length > INT_MAX)
            throw std::bad_alloc();
        for (;;) {
            const UTF8* src = (const UTF8*)text;
            UTF16* dst = (UTF16*)&(result[0]);
            ConversionResult res = ConvertUTF8toUTF16(&src, src + length, &dst, dst + result.size() - 1, lenientConversion);
            if (res == conversionOK) {
                *dst = 0; // 0-terminate result
                return;
            }
            if (res != targetExhausted)
                throw EncodingError();
            result.resize(2 * result.size());
        }
    } else {
        result[0] = 0;
    }
}

ParserW::ParserW() 
    : startElementHandler_(0)
    , endElementHandler_(0)
    , charactersHandler_(0)
    , pluginHandler_(0)
{}

void ParserW::setWikiUrl(const CreoleWChar* wiki, const CreoleWChar* url)
{
    TextBuffer wikiA;
    toUtf8(wiki, wikiA);
    toUtf8(url, buff_);
    wikiUrls_[std::string(&(wikiA[0]))] = std::string(&(buff_[0]));
}

void ParserW::setStartElementHandler(CreoleStartElementHandlerW handler)
{
    startElementHandler_ = handler;
}

void ParserW::setEndElementHandler(CreoleEndElementHandlerW handler)
{
    endElementHandler_ = handler;
}

void ParserW::setCharactersHandler(CreoleCharactersHandlerW handler)
{
    charactersHandler_ = handler;
}

void ParserW::setPlaceholderHandler(CreolePluginHandlerW handler)
{
    placeholderHandler_ = handler;
}

void ParserW::setPluginHandler(CreolePluginHandlerW handler)
{
    pluginHandler_ = handler;
}

CreoleError ParserW::parseDocument(const CreoleWChar* text, int length)
{
    if (length < 0)
        return CREOLE_ARGS_ERR;
    if (length > 0 && !text)
        return CREOLE_ARGS_ERR;

    reset();
    BaseParser::putStartElement(BodyTag);
    if (length > 0) {
        int buffLen = 0;
        toUtf8(text, length, buff_, &buffLen);
        if (buffLen > INT_MAX)
            return CREOLE_ALLOC_ERR;
        CreoleError err = readBuffer(&(buff_[0]), buffLen);
        if (err)
            return err;
    }
    flushBlock();
    putEndElement(BodyTag);
    return CREOLE_OK;
}

CreoleError ParserW::parsePluginText(const CreoleWChar* text, int length)
{
    if (length < 0)
        return CREOLE_ARGS_ERR;
    if (length > 0 && !text)
        return CREOLE_ARGS_ERR;
    if (length == 0)
        return CREOLE_OK;

    int buffLen = 0;
    TextBuffer buff;
    toUtf8(text, length, buff, &buffLen);
    if (buffLen > INT_MAX)
        return CREOLE_ALLOC_ERR;
    return readPluginText(&(buff[0]), buffLen);
}

void ParserW::putCharacters(const char* text)
{
    if (charactersHandler_) {
        toUtf16(text, buffW_);
        (*charactersHandler_)(userData_, &(buffW_[0]));
    }
}

void ParserW::putStartElement(Atom tag, const char** atts, int numAtts)
{
    if (startElementHandler_) {
        toUtf16(tag.text(), buffW_);
        if (numAtts == 0) {
            const CreoleWChar* attsW[1] = { 0 };
            (*startElementHandler_)(userData_, &(buffW_[0]), attsW, 0);
        } else {
            attsW_.resize(numAtts + 1);
            attsDataW_.resize(numAtts);
            for (int i = 0; i < numAtts; ++i) {
                toUtf16(atts[i], attsDataW_[i]);
                attsW_[i] = &(attsDataW_[i][0]);
            }
            attsW_[numAtts] = 0;
            (*startElementHandler_)(userData_, &(buffW_[0]), &(attsW_[0]), numAtts);
        }
    }
}

void ParserW::putEndElement(Atom tag)
{
    if (endElementHandler_) {
        toUtf16(tag.text(), buffW_);
        (*endElementHandler_)(userData_, &(buffW_[0]));
    }
}

void ParserW::endPlaceholder()
{
    // we collect plugin content in pendingChars_
    if (!placeholderHandler_) {
        pendingChars_.clear();
    } else {
        toUtf16(pendingChars_.c_str(), buffW_);
        pendingChars_.clear();
        (*placeholderHandler_)(userData_, &(buffW_[0]));
    }
}

void ParserW::endPlugin()
{
    // we collect plugin content in pendingChars_
    if (!pluginHandler_) {
        pendingChars_.clear();
    } else {
        toUtf16(pendingChars_.c_str(), buffW_);
        pendingChars_.clear();
        (*pluginHandler_)(userData_, &(buffW_[0]));
    }
}
