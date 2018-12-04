/*
 * This file is part of the creole/c parser and HTML converter.
 *
 * Copyright (c) 2009 Klaus Drescher.
 *
 * This file may be used under the terms of the GNU Lesser General Public
 * License version 2.1 as published by the Free Software Foundation and
 * appearing in the file LICENSE.LGPL included in the packaging of this file.
 * Please review the following information to ensure the GNU Lesser General
 * Public License version 2.1 requirements will be met:
 * http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
 */

#ifndef CREOLE_ATOM_H__
#define CREOLE_ATOM_H__

/** A unique string used as identifier (name).
 *
 * Atoms are considered equal if they hold the same char pointer.
 * Their text contents is @e not compared.
 *
 * The user of the class is responsible for creating unique instances
 * and managing their strings.
 */
class Atom
{
public:
    /** Create atom given its text.
     * The text string is @e not copied.
     */
    Atom(const char* text) : text_(text) {}

    /// Copy constructor.
    Atom(const Atom& a) : text_(a.text()) {}

    /// Assignment operator.
    Atom& operator=(const Atom& a) {
        text_ = a.text();
        return *this;
    }

    /// Get text of atom.
    const char* text() const {
        return text_;
    }

    /// Test for equality.
    bool operator==(const Atom& a) const {
        return text_ == a.text();
    }

    /// Test for inequality.
    bool operator!=(const Atom& a) const {
        return text_ != a.text();
    }

    /// Get text of atom.
    operator const char* () const {
        return text_;
    }

private:
    // hidden ctor
    Atom();

    // hidden char* conversion
    operator char* () const;

    /// The text of the atom.
    const char* text_;
};

#endif // CREOLE_ATOM_H__
