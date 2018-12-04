/*
 * This is a simple example on how to use the creole/c parser from C#.
 */

using System;
using System.Runtime.InteropServices;
using System.IO;
using System.Text;

namespace Creole
{
    /* Types of delegates for callback functions of creole/c parser.
     */
    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    delegate void CreoleStartElementHandlerW(IntPtr userData, string name, [MarshalAs(UnmanagedType.LPArray, SizeParamIndex = 3)] string[] atts, int numAtts);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    delegate void CreoleEndElementHandlerW(IntPtr userData, string name);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
    delegate void CreoleCharactersHandlerW(IntPtr userData, string text);

    /// <summary>
    /// Delegate for start element event raised by Creole parser.
    /// </summary>
    /// <param name="name">Name of element.</param>
    /// <param name="atts">Attribute/value pairs of element.</param>
    public delegate void StartElementEvent(string name, string[] atts);

    /// <summary>
    /// Delegate for end element event raised by Creole parser.
    /// </summary>
    /// <param name="name">Name of element.</param>
    public delegate void EndElementEvent(string name);

    /// <summary>
    /// Delegate for character data event raised by Creole parser.
    /// </summary>
    /// <param name="text">Character data read.</param>
    public delegate void CharactersEvent(string text);

    /// <summary>
    /// Wiki Creole parser.
    /// </summary>
    /// <remarks>
    /// Quite simplistic, error handling is missing for example.
    /// </remarks>
    class CreoleParser : IDisposable
    {
        /* Functions of creole/c parser DLL bound.
         */
        [DllImport("creole.dll", CallingConvention=CallingConvention.Cdecl)]
        private static extern IntPtr CreoleCreateParserW();

        [DllImport("creole.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void CreoleFreeParserW(IntPtr parser);

        [DllImport("creole.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void CreoleSetUserDataW(IntPtr parser, IntPtr userData);

        [DllImport("creole.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void CreoleSetStartElementHandlerW(IntPtr parser, CreoleStartElementHandlerW handler);

        [DllImport("creole.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void CreoleSetEndElementHandlerW(IntPtr parser, CreoleEndElementHandlerW handler);

        [DllImport("creole.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void CreoleSetCharactersHandlerW(IntPtr parser, CreoleCharactersHandlerW handler);

        [DllImport("creole.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        private static extern int CreoleParseDocumentW(IntPtr parser, char[] text, int length);

        /* Callback functions eventually called by creole/c parser.
         * Raise appropriate CLR events.
         */
        private static void StartElementHandler(IntPtr userData, string name, string[] atts, int numAtts)
        {
            CreoleParser parser = (CreoleParser)GCHandle.FromIntPtr(userData).Target;
            if (parser.StartElement != null)
            {
                parser.StartElement(name, atts);
            }
        }

        private static void EndElementHandler(IntPtr userData, string name)
        {
            CreoleParser parser = (CreoleParser)GCHandle.FromIntPtr(userData).Target;
            if (parser.EndElement != null)
            {
                parser.EndElement(name);
            }
        }

        private static void CharactersHandler(IntPtr userData, string text)
        {
            CreoleParser parser = (CreoleParser)GCHandle.FromIntPtr(userData).Target;
            if (parser.Characters != null)
            {
                parser.Characters(text);
            }
        }

        // The creole/c parser object.
        private IntPtr parser_ = new IntPtr(0);

        // GC handle used to handle CreoleParser object as user data of creole/c parser.
        private GCHandle this_;

        /* The callback function delegates.
         */
        private CreoleStartElementHandlerW startElementHandler_ = new CreoleStartElementHandlerW(StartElementHandler);
        private CreoleEndElementHandlerW endElementHandler_ = new CreoleEndElementHandlerW(EndElementHandler);
        private CreoleCharactersHandlerW charactersHandler_ = new CreoleCharactersHandlerW(CharactersHandler);

        /// <summary>
        /// Constructor.
        /// </summary>
        public CreoleParser()
        {
            parser_ = CreoleCreateParserW();
            // Use 'this' as user data for parser.
            this_ = GCHandle.Alloc(this);
            CreoleSetUserDataW(parser_, GCHandle.ToIntPtr(this_));
            // Set up callback functions.
            CreoleSetStartElementHandlerW(parser_, startElementHandler_);
            CreoleSetEndElementHandlerW(parser_, endElementHandler_);
            CreoleSetCharactersHandlerW(parser_, charactersHandler_);
        }

        ~CreoleParser()
        {
            Dispose(false);
        }

        /// <summary>
        /// Dispose parser if no longer needed.
        /// </summary>
        public void Dispose()
        {
            Dispose(true);
        }

        private void Dispose(bool disposing)
        {
            if (disposing && this_.IsAllocated)
            {
                this_.Free();
            }
            if (parser_.ToInt32() != 0)
            {
                CreoleFreeParserW(parser_);
                parser_ = new IntPtr(0);
            }
        }

        /// <summary>
        /// Parse contents given by text.
        /// </summary>
        /// <param name="reader">Text holding Creole input.</param>
        public void Parse(string text)
        {
	    // TODO: handle errors returned
	    CreoleParseDocumentW(parser_, text, text.Length);
        }

        /// <summary>
        /// Emit event when new element starts.
        /// </summary>
        public event StartElementEvent StartElement;

        /// <summary>
        /// Emit event when element ends.
        /// </summary>
        public event EndElementEvent EndElement;

        /// <summary>
        /// Emit event when character data is read.
        /// </summary>
        public event CharactersEvent Characters;
    }

    /// <summary>
    /// Simple converter from Wiki Creole to HTML.
    /// </summary>
    class Converter
    {
        static void OnStartElement(string name, string[] atts)
        {
            StringBuilder text = new StringBuilder();
            text.Append("<");
            text.Append(name);
            for (int i = 0; i < atts.Length; i += 2)
            {
                text.Append(" ");
                text.Append(atts[i]);
                text.Append("=\"");
                text.Append(atts[i+1]);
                text.Append("\"");
            }
            text.Append(">");
            Console.Write(text);
        }

        static void OnEndElement(string name)
        {
            Console.Write("</" + name + ">");
        }
        
        static void OnCharacters(string text)
        {
            Console.Write(text.Replace("&", "&amp;").Replace("<", "&lt;"));
        }

        /// <summary>
        /// Main function. Output is written to console.
        /// </summary>
        /// <param name="args">Program arguments, must be input file name.</param>
        static void Main(string[] args)
        {
            using (CreoleParser parser = new CreoleParser())
            using (StreamReader reader = new StreamReader(args[0]))
            {
                parser.StartElement += OnStartElement;
                parser.EndElement += OnEndElement;
                parser.Characters += OnCharacters;

                parser.Parse(reader.ReadToEnd());
            }
        }
    }
}
