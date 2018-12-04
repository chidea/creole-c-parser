{{creole_c.png}}

[[http://www.wikicreole.org/|Wiki Creole]] is a common wiki markup language
to be used across different wikis.

**creole/c** is a Wiki Creole parser and a HTML converter. It implements
[[http://www.wikicreole.org/wiki/Creole1.0|Wiki Creole 1.0]] and almost all of
its [[http://www.wikicreole.org/wiki/CreoleAdditions|additions]].
There is a [[doc/creole_cheat_sheet.pdf|cheat sheet]] available in PDF format.

The parser is written in C++ using [[http://flex.sourceforge.net/|##flex##]]
and has a simple and flexible event driven plain C API.  The converter is a
stand-alone console application, needing no additional runtime installation.

==== What's Inside?

The binary distribution currently is for Windows only.
It is linked against the static C run-time lib, thus no further installation 
should be needed. It contains:
; bin/creole.exe : The Wiki Creole to HTML converter. This is a console application,
see [[doc/manpage.html]] for a manual page.
; bin/creole.dll : The creole/c parser as a DLL.
; lib/creole.lib : The link library for the ##creole.dll##.
; lib/creolelib.lib : The creole/c parser as a static link library.
; include/creole.h : The header file needed for the creole/c parser. 
The parsers API is documented in [[doc/api/creole.chm]].

It shouldn't be difficult to build the converter and parser on other platforms
than Windows, please refer to [[hacking.html]].

==== License

creole/c may be used under the terms of the GNU Lesser General Public
License version 2.1 as published by the Free Software Foundation and
appearing in the file [[LICENSE.LGPL]] included in the packaging of this file.
Please review the following information to ensure the GNU Lesser General
Public License version 2.1 requirements will be met:
http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.

