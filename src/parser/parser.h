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

#ifndef PARSER_H__
#define PARSER_H__

#include <string>
#include <string.h>
#include <vector>
#include <map>
#include <limits.h>
#include <unistd.h>
#include "creole.h"
#include "atom.h"

/// The type of the generated flex scanner.
typedef void* yyscan_t;

namespace CreoleAtoms
{
    /** @name Element names.
     */
    //@{
    extern Atom BodyTag;
    extern Atom PreformattedBlockTag;
    extern Atom HeadingTag[6];
    extern Atom ParagraphTag;
    extern Atom IndentedParagraphTag;
    extern Atom HorizontalRuleTag;
    extern Atom PlaceholderTag;
    extern Atom LinkTag;
    extern Atom ImageTag;
    extern Atom BoldTag;
    extern Atom ItalicTag;
    extern Atom MonospaceTag;
    extern Atom SuperscriptTag;
    extern Atom SubscriptTag;
    extern Atom UnderlineTag;
    extern Atom PreformattedInlineTag;
    extern Atom BreakLineTag;
    extern Atom TableTag;
    extern Atom TableRowTag;
    extern Atom TableHeadingTag;
    extern Atom TableCellTag;
    extern Atom ListTag;
    extern Atom OrderedListTag;
    extern Atom ListItemTag;
    extern Atom DefinitionListTag;
    extern Atom DefinitionTermTag;
    extern Atom DefinitionTag;
    //@}

    /** @name Attribute names.
     */
    //@{
    extern Atom HrefAttr;
    extern Atom SrcAttr;
    extern Atom AltAttr;
    extern Atom WidthAttr;
    extern Atom HeightAttr;
    extern Atom ClassAttr;
    //@}

    /// Value 'citation' of class attribute.
    extern Atom CitationVal;
}

/// Coloring class for C API ASCII parser handle.
struct CreoleParserImpl
{
};

/// Coloring class for C API UTF-16 parser handle.
struct CreoleParserImplW
{
};

/// Exception to signal Unicode encoding error.
struct EncodingError : std::exception
{
};

/** Creole parser base class.
 */
class BaseParser
{
public:
    /// Constructor.
    BaseParser();

    /// Destructor.
    virtual ~BaseParser();

    /// Set user data.
    void setUserData(void* userData);

    /// Set features flags.
    void setFeatures(int features);

    /** @name Members used by flex scanner rules.
     */
    //@{

    /// Test if features given by flags are supported.
    bool hasFeatures(int featureFlags) const;

    /// Get current level of verbatim element.
    size_t verbatimLevel() const;

    /// Set level of current verbatim element.
    void setVerbatimLevel(size_t);

    /// Get currently stored 'at_bol' state.
    int atBol() const;

    /**Set current scanner 'at_bol' state and save it.
     * Used to remember 'at_bol' state of scanner between different blocks.
     */
    void setAtBol(int);

    /// Restore scanners 'at_bol' state to currently saved state.
    void restoreAtBol();

    /// Output any pending character data.
    void flushPendingChars();

    /// Output any pending elements of current block.
    void flushBlock();

    /** Ensure that we are in a block.
     * If outside any block a new paragraph is started.
     */
    void ensureBlock();

    /// Test if we are in a paragraph.
    bool inParagraph();

    /// Test if we are in an ul or ol list.
    bool inList();

    /// Test if we are in a definition list.
    bool inDefinitionList();

    /// Test if we are in a definition term.
    bool inDefinitionTerm();

    /// Test if we are in a table.
    bool inTable();

    /** Start element with given tag.
     * @return Elements level in tag stack.
     */
    size_t startElement(Atom tag);

    /** Start element with given tag and attributes.
     * @return Elements level in tag stack.
     */
    size_t startElement(Atom, const char** atts, int numAtts);

    /// End all currently open elements up to given level in tag stack.
    void endElement(size_t level);
    
    /// Append character to character data.
    void appendChar(char c);

    /// Append string to character data.
    void appendChars(const char* text);

    /// Output free link.
    void putFreeUrl(char* url);

    /// Start element for list item with given nesting depth.
    void startListItem(size_t level, bool isOrdered);

    /// Start indented block with given nesting depth.
    void startIndent(int order, bool isCitation);

    /// Start definition term.
    void startDefinitionTerm();

    /// Start definition definition.
    void startDefinition();

    /// Toggle inline style with given tag.
    void toggleStyle(Atom tag);

    /// Start image.
    void startImage();

    /// Set size of current image.
    void imageSize(const char* size);

    /// Start alternative text for image.
    void startImageAlt();

    /// End image.
    void endImage();

    /// Start link.
    bool startLink();

    /// End URL of link.
    void endLinkUrl();

    /// End link.
    void endLink(bool urlOnly = false);

    /// Start first row of a table.
    void startFirstTableRow(bool isHeader);

    /// Start cell in first row of a table.
    void startCellInFirstRow(bool isHeader);

    /// End first row of a table.
    void endFirstTableRow();

    /// Start non-first row of a table.
    void startTableRow(bool isHeader);

    /// Start table cell not in first row.
    bool startCell(bool isHeader);

    /// End non-first table row.
    void endTableRow();

    /// End placeholder text.
    virtual void endPlaceholder() = 0;

    /// End plugin text.
    virtual void endPlugin() = 0;

    //@}

protected:
    /// A stack of nested element tags.
    typedef std::vector<Atom> TagStack;

    /// A pair (list element level, ordered list flag).
    typedef std::pair<size_t, bool> ListLevel;

    /// A stack of nested lists.
    typedef std::vector<ListLevel> ListStack;

    /// A mapping of wiki names to URLs.
    typedef std::map<std::string , std::string> WikiUrlMap;

    /// Copy input text to scanner buffer, converting EOLs.
    static void prepareBuffer(std::vector<char>& input, const char* text, int len);

    /// Create flex scanner.
    void createScanner();

    /// Free flex scanner.
    void freeScanner();

    /// Reset parsers state.
    void reset();

    /// Parse buffers text.
    CreoleError readBuffer(const char* text, int len);

    /// Parse plugins text.
    CreoleError readPluginText(const char* text, int len);

    /// Raise event for start of element w/o attributes.
    void putStartElement(Atom tag);

    /// Raise event for start of element.
    virtual void putStartElement(Atom tag, const char** atts, int numAtts) = 0;

    /// Raise event for end of element.
    virtual void putEndElement(Atom tag) = 0;

    /// Raise event for character data.
    virtual void putCharacters(const char* text) = 0;

    /// Map link to URL.
    std::string mapUrl(const std::string& url);

    /// Map free link to URL.
    std::string mapFreeUrl(const std::string& url);

    /// Get current 'at_bol' state of scanner.
    int scannerAtBol();

    /// Set current 'at_bol' state of scanner.
    void setScannerAtBol(int);

    yyscan_t scanner_; ///< The flex scanner.
    std::vector<char> input_; ///< Input buffer.
    int atBol_; ///< Stores flags for begin of line when parsing plugin text.
    void* userData_; ///< User data for callbacks.
    int features_; ///< Parser features.

    WikiUrlMap wikiUrls_; ///< A map from wiki URLs to real URLs.

    TagStack tagStack_; ///< Stack of currently open elements.
    std::string pendingChars_; ///< Currently pending character data.
    size_t verbatimLevel_; ///< Tag level of current verbatim element.

    ListStack listStack_; ///< Stack of currently open lists.
    size_t listItemLevel_; ///< Tag level where current list item started.

    std::string imageUrl_; ///< URL during image reading.
    std::string imageWidth_; ///< Image with during image reading.
    std::string imageHeight_; ///< Image height during image reading.

    std::string linkUrl_; ///< URL during link reading.
    size_t linkLevel_; ///< Tag level of current link.

    size_t tableColumns_; ///< Number of columns in table.
    size_t currentColumn_; ///< Current column in table.
};

/** The ASCII creole/c parser.
 */
class Parser : public BaseParser, public CreoleParserImpl
{
public:
    /// Constructor.
    Parser();

    /// Define mapping between wiki name and URL.
    void setWikiUrl(const char* wiki, const char* url);

    /// Set start element handler.
    void setStartElementHandler(CreoleStartElementHandler handler);

    /// Set end element handler.
    void setEndElementHandler(CreoleEndElementHandler handler);

    /// Set character data handler.
    void setCharactersHandler(CreoleCharactersHandler handler);

    /// Set placeholder handler.
    void setPlaceholderHandler(CreolePluginHandler handler);

    /// Set plugin handler.
    void setPluginHandler(CreolePluginHandler handler);

    /// Parse WiKi documents text.
    CreoleError parseDocument(const char* text, int length);

    /// Parse text generated by plugin.
    CreoleError parsePluginText(const char* text, int length);

    /// End placeholder text.
    void endPlaceholder();

    /// End plugin text.
    void endPlugin();

protected:
    /// Call start element handler.
    void putStartElement(Atom tag, const char** atts, int numAtts);

    /// Call end element handler.
    void putEndElement(Atom tag);

    /// Call character data handler.
    void putCharacters(const char* text);

private:
    /** @name Callback handlers.
     */
    //@{
    CreoleStartElementHandler startElementHandler_;
    CreoleEndElementHandler endElementHandler_;
    CreoleCharactersHandler charactersHandler_;
    CreolePluginHandler placeholderHandler_;
    CreolePluginHandler pluginHandler_;
    //@}
};

/** The UTF-16 creole/c parser.
 */
class ParserW : public BaseParser, public CreoleParserImplW
{
public:
    /// Constructor.
    ParserW();

    /// Define mapping between wiki name and URL.
    void setWikiUrl(const CreoleWChar* wiki, const CreoleWChar* url);

    /// Set start element handler.
    void setStartElementHandler(CreoleStartElementHandlerW handler);

    /// Set end element handler.
    void setEndElementHandler(CreoleEndElementHandlerW handler);

    /// Set character data handler.
    void setCharactersHandler(CreoleCharactersHandlerW handler);

    /// Set placeholder handler.
    void setPlaceholderHandler(CreolePluginHandlerW handler);

    /// Set plugin handler.
    void setPluginHandler(CreolePluginHandlerW handler);

    /// Parse WiKi documents text.
    CreoleError parseDocument(const CreoleWChar* text, int length);

    /// Parse text generated by plugin.
    CreoleError parsePluginText(const CreoleWChar* text, int length);

    /// End placeholder text.
    void endPlaceholder();

    /// End plugin text.
    void endPlugin();

protected:
    /// Call start element handler.
    void putStartElement(Atom tag, const char** atts, int numAtts);

    /// Call end element handler.
    void putEndElement(Atom tag);
    
    /// Call character data handler.
    void putCharacters(const char* text);

private:
    /// UTF8 text buffer.
    typedef std::vector<char> TextBuffer;

    /// UTF16 text buffer.
    typedef std::vector<CreoleWChar> TextBufferW;

    /** @name Unicode conversion functions.
     */
    //@{
    static void toUtf8(const CreoleWChar* text, int length, TextBuffer& result, int* resultLength=0);
    static void toUtf8(const CreoleWChar* text, TextBuffer& result, int* resultLength=0);
    static void toUtf16(const char* text, TextBufferW& result);
    //@}

    /** @name Callback handlers.
     */
    //@{
    CreoleStartElementHandlerW startElementHandler_;
    CreoleEndElementHandlerW endElementHandler_;
    CreoleCharactersHandlerW charactersHandler_;
    CreolePluginHandlerW placeholderHandler_;
    CreolePluginHandlerW pluginHandler_;
    //@}

    TextBuffer buff_; ///< UTF8 result buffer for converting strings.
    TextBufferW buffW_; ///< UTF16 result buffer for converting strings.
    std::vector<const CreoleWChar* > attsW_; ///< Result buffer for converting attributes.
    std::vector<TextBufferW> attsDataW_; ///< Result buffers for converting attributes.
};

#endif /* PARSER_H__ */
