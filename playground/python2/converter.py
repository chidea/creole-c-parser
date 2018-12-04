#
# A very simplistic sample using the creole/c parser from Python 2.6.
#
from __future__ import print_function
from ctypes import *

class Converter:
    """Converts Wiki Creole to HTML."""

    def _CharactersHandler(self, userData, text):
        print(text, end="")

    def _StartElementHandler(self, userData, name, atts, numAtts):
        print("<" + name, end="")
        for i in range(0, numAtts, 2):
            print(" " + atts[i] + "=\"" + atts[i+1] + "\"", end="")
        print(">", end="")

    def _EndElementHandler(self, userData, name):
        print("</" + name + ">", end="")

    def _PluginHandler(self, userData, text):
        text = "**" + text.upper() + "**"
        self._creole.CreoleParsePluginText(self._parser, text, len(text))

    def __init__(self):
        self._creole = LibraryLoader(CDLL).LoadLibrary("../../bin/creole.dll")
        self._parser = self._creole.CreoleCreateParser()

        charactersHandler = CFUNCTYPE(None, c_void_p, c_char_p)
        self._charactersHandlerFunc = charactersHandler(self._CharactersHandler)
        self._creole.CreoleSetCharactersHandler(self._parser, self._charactersHandlerFunc)

        startElementHandler = CFUNCTYPE(None, c_void_p, c_char_p, POINTER(c_char_p), c_int)
        self._startElementHandlerFunc = startElementHandler(self._StartElementHandler)
        self._creole.CreoleSetStartElementHandler(self._parser, self._startElementHandlerFunc)

        endElementHandler = CFUNCTYPE(None, c_void_p, c_char_p)
        self._endElementHandlerFunc = endElementHandler(self._EndElementHandler)
        self._creole.CreoleSetEndElementHandler(self._parser, self._endElementHandlerFunc)

        pluginHandler = CFUNCTYPE(None, c_void_p, c_char_p)
        self._pluginHandler = pluginHandler(self._PluginHandler)
        self._creole.CreoleSetPluginHandler(self._parser, self._pluginHandler)

        self._creole.CreoleSetWikiUrl(self._parser, "", "http://www.mywiki.org/")

    def __del__():
        self._creole.CreoleFreeParser(self._parser)

    def ParseDocument(self, text):
        """Parse Wiki documents text."""
        self._creole.CreoleParseDocument(self._parser, text, len(text))


if __name__ == "__main__":
    text = "This is **bold //italic//** text with an {{image.png?100x50|image}} image.\n\
We also have an __internal__ [[link]] and a <<plugin>> here.\n"
    converter = Converter()
    converter.ParseDocument(text)
