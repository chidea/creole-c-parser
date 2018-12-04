#
# A very simplistic sample using the creole/c parser from Python 3.
#
from ctypes import *

class Converter:
    """Converts Wiki Creole to HTML."""

    def _CharactersHandler(self, userData, text):
        print(text, end="")

    def _StartElementHandler(self, userData, name, atts, numAtts):
        print("<" + name, end="")
        for i in range(0, numAtts, 2):
            print(" " + atts[i] + "=\"" + atts[i+1] + "\"", end="")
        print (">", end="")

    def _EndElementHandler(self, userData, name):
        print("</" + name + ">", end="")

    def _PluginHandler(self, userData, text):
        text = "**" + text.upper() + "**"
        self._creole.CreoleParsePluginTextW(self._parser, text, len(text))

    def __init__(self):
        self._creole = LibraryLoader(CDLL).LoadLibrary(r"../../bin/creole.dll")
        self._parser = self._creole.CreoleCreateParserW()

        charactersHandler = CFUNCTYPE(None, c_void_p, c_wchar_p)
        self._charactersHandlerFunc = charactersHandler(self._CharactersHandler)
        self._creole.CreoleSetCharactersHandlerW(self._parser, self._charactersHandlerFunc)

        startElementHandler = CFUNCTYPE(None, c_void_p, c_wchar_p, POINTER(c_wchar_p), c_int)
        self._startElementHandlerFunc = startElementHandler(self._StartElementHandler)
        self._creole.CreoleSetStartElementHandlerW(self._parser, self._startElementHandlerFunc)

        endElementHandler = CFUNCTYPE(None, c_void_p, c_wchar_p)
        self._endElementHandlerFunc = endElementHandler(self._EndElementHandler)
        self._creole.CreoleSetEndElementHandlerW(self._parser, self._endElementHandlerFunc)

        pluginHandler = CFUNCTYPE(None, c_void_p, c_wchar_p)
        self._pluginHandler = pluginHandler(self._PluginHandler)
        self._creole.CreoleSetPluginHandlerW(self._parser, self._pluginHandler)

        self._creole.CreoleSetWikiUrlW(self._parser, "", r"http://www.mywiki.org/")

    def __del__():
        self._creole.CreoleFreeParser(self._parser)

    def Reset():
        """Reset converters state."""
        self._creole.CreoleResetParser(self._parser)

    def ParseDocument(self, text):
        """Parse Wiki input text block."""
        self._creole.CreoleParseDocumentW(self._parser, text, len(text))


if __name__ == "__main__":
    text = r"This is **bold //italic//** text with an {{image.png?100x50|image}} image.\n\
We also have an __internal__ [[link]] and a <<plugin>> here.\n"
    converter = Converter()
    converter.ParseDocument(text)
