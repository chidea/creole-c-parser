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

/** @mainpage The Wiki Creole Parser

The @c creole/c parser is a simple event driven parser, similar to
XML parsers like Expat or SAX parsers. (In fact the parsers API 
resembles that of Expat.) Callback handlers are registered with
the parser which are called when events occur during parsing.
The parser events are:

- The start or end of an element is read.
- Charater data is read.
- Placeholder text ist read.
- Plugin text ist read.

For this the parser transforms the Wiki text into an @e imaginary
document tree, where the inner nodes are elements and the leaves are
elements, character data and plugin text. This tree is similar to
the DOM tree of an XML document. The parser then calls the callback
handlers according to the structure of the tree.

I call this tree @e imaginary because the parser does not
build such a tree in memory, it is just a model for the order in
which the events occur.

The structure of the imaginary document tree is according to a DTD
which is a small subset of HTML. This DTD describes which elements
and attribute can occur and when they can occur.

@subpage dtd "The imaginary document trees DTD"

The creole/c parser may be linked either as static or as dynamic library.
One @e must define the macro @c CREOLE_LIB when including the
libraries header file @c creole.h in order to use the static
library.

The parsers API comes in both an ASCII and an Unicode flavor.
The conditions for the relevant text encodings are described in the 
corresponding sections:

@ref ascii "ASCII parsing function"

@ref unicode "Unicode parsing functions"

@page dtd The Imaginary Document Trees DTD

Take this DTD as description of the elements and attributes which may
occur during parsing and when they may occur.

@htmlinclude creole.dtd.html

 */

