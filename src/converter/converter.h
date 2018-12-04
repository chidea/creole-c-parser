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

#ifndef CONVERTER_H__
#define CONVERTER_H__

#include "creole.h"
#include <stdio.h>

/** Converter from Wiki Creole to HTML using the creole/c parser.
 */
class Converter
{
public:
    /// Constructor.
    Converter();

    /// Convert input file, returns error code.
    int convert();

    /// Get text of last error.
    const char* lastError() const;

    /** @name Set options.
     */
    //@{

    /// Set name of input file, must be given.
    void setInputFileName(const char* fileName);

    /// Set name of output file (default is stdout).
    void setOutputFileName(const char* fileName);

    /// Set name of style sheet (optional).
    void setStyleSheet(const char* styleSheet);

    /// Set page title (optional).
    void setPageTitle(const char* title);

    /// Set URL for internal links (optional).
    void setInternalLinkUrl(const char* url);

    /// Set flag to output body element only.
    void setOutputBodyOnly(bool bodyOnly);

    //@}

private:
    /// Parser callback handler for start of element.
    static void startElementHandler(void* userData, const char* name, const char** atts, int numAtts);

    /// Parser callback handler for end of element.
    static void endElementHandler(void* userData, const char* name);

    /// Parser callback handler for character data.
    static void charactersHandler(void* userData, const char* text);

    /// Parser callback handler for plugin data.
    static void pluginHandler(void* userData, const char* text);

    /// Eventually handles start of element.
    void startElement(const char* name, const char** atts, int numAtts);

    /// Eventually handles end of element.
    void endElement(const char* name);

    /// Eventually handles character data.
    void characters(const char* text);

    /// Eventually handles plugin text.
    void plugin(const char* text);

    /// Set last error text.
    void setError(const char* errorText);

    /// Test if input file starts with UTF-8 BOM. If yes skip it.
    static bool hasUtf8Bom(FILE* inFile);

    /// Get flag to output body element only.
    bool outputBodyOnly() const;

    const char* inputFileName_;
    const char* outputFileName_;
    const char* styleSheet_;
    const char* pageTitle_;
    const char* internalUrl_;
    bool bodyOnly_;
    const char* lastError_;

    FILE* outputFile_;
};

// Get flag to output body element only.
inline bool Converter::outputBodyOnly() const
{
    return bodyOnly_;
}

#endif // CONVERTER_H__
