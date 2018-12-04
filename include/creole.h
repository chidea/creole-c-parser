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

#ifndef CREOLE_H__
#define CREOLE_H__

/** @file creole.h
 * The creole/c parser API.
 */

#ifdef __cplusplus
extern "C" {
#endif

/** @def CREOLE_LIB
 * On must define this macro if one wants to link against the static link library 
 * instead of the DLL.
 */

/** @def CREOLE_CALL 
 * Defines the calling convention of the creole/c parser API.
 */

/** @def CREOLE_API(T)
 * This macro is used to declare the type of the API functions, where @a T
 * is the return type of the function.
 */

#ifdef _MSC_VER
#  define CREOLE_CALL __cdecl
#  ifdef CREOLE_LIB
#    define CREOLE_API(T) T CREOLE_CALL
#  else
#    ifdef CREOLE_BUILDING
#      define CREOLE_API(T) __declspec(dllexport) T CREOLE_CALL
#    else
#      define CREOLE_API(T) __declspec(dllimport) T CREOLE_CALL
#    endif
#  endif
#else // !_MSC_VER
// TODO
#  define CREOLE_CALL
#  define CREOLE_API(T) T
#endif // !_MSC_VER

/** @defgroup ascii ASCII parsing functions.
 *
 * The functions of this group assume that any text is in some @e ASCII-compatible
 * encoding.
 *
 * This means that any single byte in the text which is a valid ASCII code point must 
 * represent that ASCII character. This includes encodings like ISO 8859-1 (Latin 1), 
 * ISO 8859-5 and UTF-8, but @e not Big5 or Shift_JIS for example.
 *
 * @b Note: Any newlines in the input text are converted to Unix style \\n newlines.
 * Thus Windows&tm; style \\r\\n newlines are converted to sole \\n's.
 *
 * @{
 */

/** Error codes returned by the API functions.
 */
enum CreoleError {
    CREOLE_OK = 0, ///< No error.
    CREOLE_ALLOC_ERR, ///< Out of memory error.
    CREOLE_ARGS_ERR, ///< Illegal argument.
    CREOLE_INPUT_ERR, ///< Illegal text input.
    CREOLE_ENCODING_ERR ///< Unicode encoding error.
};

/** Creole language features flags.
 */
enum CreoleFeatures {
    CREOLE_10 = 0, ///< Basic Creole 1.0 as of http://www.wikicreole.org/wiki/Creole1.0.
    CREOLE_ADDITIONS = 1, ///< Creole 1.0 additions as of http://www.wikicreole.org/wiki/CreoleAdditions.
    CREOLE_IMAGESIZE = 2 ///< Allow to define the size of an image after '?'.
};

/** A handle to a creole/c parser for ASCII text.
 */
typedef struct CreoleParserImpl* CreoleParser;

/** Function to be called at the start of an element of the document tree.
 * @param userData User data of the parser.
 * @param name Name of the element.
 * @param atts Attribute list as list of 0-terminated attribute names and values.
 * @param numAtts Number of strings in the @a atts parameter.
 */
typedef void (CREOLE_CALL *CreoleStartElementHandler)(void* userData, const char* name, const char** atts, int numAtts);

/** Function to be called at the end of an element of the document tree.
 * @param userData User data of the parser.
 * @param name Name of the element.
 */
typedef void (CREOLE_CALL *CreoleEndElementHandler)(void* userData, const char* name);

/** Function to be called when character data of the document is read.
 * @param userData User data of the parser.
 * @param text Character data, a 0-terminated string.
 */
typedef void (CREOLE_CALL *CreoleCharactersHandler)(void* userData, const char* text);

/** Function to be called when placeholder data of the document is read.
 * @param userData User data of the parser.
 * @param text Placeholder text, a 0-terminated string.
 */
typedef void (CREOLE_CALL *CreolePlaceholderHandler)(void* userData, const char* text);

/** Function to be called when plugin data of the document is read.
 * @param userData User data of the parser.
 * @param text Plugin text, a 0-terminated string.
 */
typedef void (CREOLE_CALL *CreolePluginHandler)(void* userData, const char* text);

/** Create parser.
 * The parser should be freed with CreoleFreeParser() when done.
 * @return Handle to the parser created, 0 if out of memory.
 */
CREOLE_API(CreoleParser) CreoleCreateParser();

/** Free parsers data structures.
 * @param parser Parser to be freed.
 */
CREOLE_API(void) CreoleFreeParser(CreoleParser parser);

/** Set user data which is handled to the parsers callback functions.
 * @param parser Parser to which assign the user data.
 * @param userData User data to be handled to the callback functions of the parser.
 */
CREOLE_API(void) CreoleSetUserData(CreoleParser parser, void* userData);

/** Set language features of the parser.
 * @param parser Parser to which assign the user data.
 * @param features Language features of the parser, must be a combination of the 
 *                 #CreoleFeatures flags.
 */
CREOLE_API(void) CreoleSetFeatures(CreoleParser parser, int features);

/** Sets an internal or Wiki link mapping.
 * Internal links are of the form <CODE>[[link]]</CODE>, Wiki links of the form
 * <CODE>[[SomeWiki:link]]</CODE>.
 * This function is used to define base URLs for wiki links or internal links.
 * @param parser Parser for which to set the link mapping.
 * @param wiki Name of the wiki for which to set the URL. May be empty, in this case
 *             the URL for internal links is defined by @a url.
 * @param url URL which should be inserted as the given wikis URL. May be empty, in this
 *            case an existing mapping is removed.
 */
CREOLE_API(void) CreoleSetWikiUrl(CreoleParser parser, const char* wiki, const char* url);

/** Sets a callback function to be called when an element of the document tree starts.
 * @param parser Parser for which to set the callback function.
 * @param handler Callback function to be called when an element of the document tree starts.
 */
CREOLE_API(void) CreoleSetStartElementHandler(CreoleParser parser, CreoleStartElementHandler handler);

/** Sets a callback function to be called when an element of the document tree ends.
 * @param parser Parser for which to set the callback function.
 * @param handler Callback function to be called when an element of the document tree ends.
 */
CREOLE_API(void) CreoleSetEndElementHandler(CreoleParser parser, CreoleEndElementHandler handler);

/** Sets a callback function to be called when character data of the document is read.
 * @param parser Parser for which to set the callback function.
 * @param handler Callback function to be called when character data of the document is read.
 */
CREOLE_API(void) CreoleSetCharactersHandler(CreoleParser parser, CreoleCharactersHandler handler);

/** Sets a callback function to be called when placeholder data of the document is read.
 * Placeholder data is text enclosed <CODE>\<\<\<like this\>\>\></CODE>.
 * @param parser Parser for which to set the callback function.
 * @param handler Callback function to be called when placeholder data of the document is read.
 */
CREOLE_API(void) CreoleSetPlaceholderHandler(CreoleParser parser, CreolePlaceholderHandler handler);

/** Sets a callback function to be called when plugin data of the document is read.
 * Plugin data is text enclosed <CODE>\<\<like this\>\></CODE>. The plugin handler may call
 * CreoleParsePluginText() to insert Wiki text into the output.
 * @param parser Parser for which to set the callback function.
 * @param handler Callback function to be called when plugin data of the document is read.
 */
CREOLE_API(void) CreoleSetPluginHandler(CreoleParser parser, CreolePluginHandler handler);

/** Parse Wiki Creole documents text.
 * @param parser Parser used to parse the input text.
 * @param text Input text, needs @e not be 0-terminated.
 * @param length Length of input text in char's (not including any 0 terminator).
 * @return Error code, 0 if no error occured.
 */
CREOLE_API(CreoleError) CreoleParseDocument(CreoleParser parser, const char* text, int length);

/** Parse text generated by plugin.
 * Must be called by a plugin callback function to inject Wiki text into the output.
 * @param parser Parser used to parse the given text.
 * @param text Plugins text to be inserted, needs @e not be 0-terminated.
 * @param length Length of text in char's (not including any 0 terminator).
 * @return Error code, 0 if no error occured.
 */
CREOLE_API(CreoleError) CreoleParsePluginText(CreoleParser parser, const char* text, int length);

/* @} */

/** @defgroup unicode Unicode Parsing Functions.
 *
 * This group of functions assume that any text is encoded in UTF-16.
 *
 * For documentation please refer to their @ref ascii "ASCII parser" counterparts like 
 * CreoleCreateParser() etc.
 *
 * Internally the functions convert the UTF-16 text to UTF-8 and then use the ASCII
 * functions, so performance may suffer slightly.
 *
 * @{
 */

/** @def CREOLE_UTF16_WCHAR_T
 * This macro can be defined to state that @c wchar_t represents a
 * Unicode character in UTF-16 encoding, as on the Windows platform.
 */
#if defined _WINDOWS || defined WIN32
#  define CREOLE_UTF16_WCHAR_T
#endif

/** The type of a character used for the Unicode API functions.
 */
#ifdef CREOLE_UTF16_WCHAR_T
typedef wchar_t CreoleWChar;
#else
typedef unsigned short CreoleWChar;
#endif

typedef struct CreoleParserImplW* CreoleParserW;

typedef void (CREOLE_CALL *CreoleStartElementHandlerW)(void* userData, const CreoleWChar* name, const CreoleWChar** atts, int numAtts);
typedef void (CREOLE_CALL *CreoleEndElementHandlerW)(void* userData, const CreoleWChar* name);
typedef void (CREOLE_CALL *CreoleCharactersHandlerW)(void* userData, const CreoleWChar* text);
typedef void (CREOLE_CALL *CreolePlaceholderHandlerW)(void* userData, const CreoleWChar* text);
typedef void (CREOLE_CALL *CreolePluginHandlerW)(void* userData, const CreoleWChar* text);

CREOLE_API(CreoleParserW) CreoleCreateParserW();
CREOLE_API(void) CreoleFreeParserW(CreoleParserW parser);
CREOLE_API(void) CreoleSetUserDataW(CreoleParserW parser, void* userData);
CREOLE_API(void) CreoleSetFeaturesW(CreoleParserW parser, int features);
CREOLE_API(void) CreoleSetWikiUrlW(CreoleParserW parser, const CreoleWChar* wiki, const CreoleWChar* url);
CREOLE_API(void) CreoleSetStartElementHandlerW(CreoleParserW parser, CreoleStartElementHandlerW handler);
CREOLE_API(void) CreoleSetEndElementHandlerW(CreoleParserW parser, CreoleEndElementHandlerW handler);
CREOLE_API(void) CreoleSetCharactersHandlerW(CreoleParserW parser, CreoleCharactersHandlerW handler);
CREOLE_API(void) CreoleSetPlaceholderHandlerW(CreoleParserW parser, CreolePlaceholderHandlerW handler);
CREOLE_API(void) CreoleSetPluginHandlerW(CreoleParserW parser, CreolePluginHandlerW handler);
CREOLE_API(CreoleError) CreoleParseDocumentW(CreoleParserW parser, const CreoleWChar* text, int length);
CREOLE_API(CreoleError) CreoleParsePluginTextW(CreoleParserW parser, const CreoleWChar* text, int length);

/* @} */

#ifdef __cplusplus
}
#endif

#endif /* CREOLE_H__ */
