#ifndef _COLORER_CONSOLETOOLS_H_
#define _COLORER_CONSOLETOOLS_H_

#include <colorer/parsers/ParserFactory.h>

/** Writer interface wrapper, which
    allows escaping of XML markup characters (& and <)
    @ingroup colorer_exe
*/
class HtmlEscapesWriter : public Writer
{
public:
  HtmlEscapesWriter(Writer* writer)
  {
    this->writer = writer;
  };
  void write(wchar c)
  {
    if (c == '&') {
      writer->write(CString("&amp;"));
    } else if (c == '<') {
      writer->write(CString("&lt;"));
    } else {
      writer->write(c);
    }
  };
protected:
  Writer* writer;
};

/**
    Console colorer application.
    Implements command-line interface, and allows to generate
    colored html sources, view text files in console window(win32 only)
    and to check HRC database integrity.
    @ingroup colorer_viewer
*/
class ConsoleTools
{
public:

  ConsoleTools();
  ~ConsoleTools();

  /// Is copyright header present in html output.
  void setCopyrightHeader(bool use);
  /// Is HTML characters escaping done by HtmlEscapesWriter class.
  void setHtmlEscaping(bool use);
  /// Is BOM outputted into result stream.
  void setBomOutput(bool use);
  /// Is HTML text header and footer are printed in HTML result.
  void setHtmlWrapping(bool use);

  /// Alternative HRC type description for type selection
  void setTypeDescription(const String &str);
  /// File name, used as input source. Could be URL.
  void setInputFileName(const String &str);
  /// Optional file name, used for output
  void setOutputFileName(const String &str);
  /// Input Characters Encoding
  void setInputEncoding(const String &str);
  /// Output Characters Encoding
  void setOutputEncoding(const String &str);
  /// Optional path to base catalog.xml
  void setCatalogPath(const String &str);
  /// Optional HRD instance name, used to perform parsing
  void setHRDName(const String &str);
  /// Sets linking datasource into this filename
  void setLinkSource(const String &str);
  /// If true, result file will have line numbers before each line
  void addLineNumbers(bool add);

  /** Regular Expressions tests.
      Reads RE and expression from stdin,
      and checks expression against RE.
      Outputs all matched brackets.
  */
  void RETest();

  /** Runs parser in profile mode. Does everything, -h makes, but
      makes possible repeat loops and produces no output.
      Prints into standard output number of msecs, used.

      @param loopCount Number of times to repeat file's parsing.
  */
  void profile(int loopCount);

  /** Lists all available HRC types and
      optionally tries to load them.
  */
  void listTypes(bool load, bool useNames);


  FileType* selectType(HRCParser* hrcParser, LineSource* lineSource);


  /** Views file in console window, using TextConsoleViewer class
  */
  void viewFile();

  /** Simply forwards input text to output, using passed encoding information.
  */
  void forward();

  /** Generates HTML-ized output of file.
      Uses @c 'rgb' HRD class to dynamically create
      HTML tags, or @c 'text' HRD class to use specified in
      HRD file starting and ending sequences.
  */
  void genOutput(bool useTokens = false);

  /*
   *  No HRD input is used, but direct tokenized output is produced with region names, as names of tokens.
   */
  void genTokenOutput();
private:
  bool copyrightHeader;
  bool htmlEscaping;
  bool bomOutput;
  bool htmlWrapping;
  bool lineNumbers;

  int inputEncodingIndex;
  int outputEncodingIndex;
  std::unique_ptr<String> inputEncoding;
  std::unique_ptr<String> outputEncoding;

  std::unique_ptr<String> typeDescription;
  std::unique_ptr<String> catalogPath;
  std::unique_ptr<String> hrdName;
  std::unique_ptr<String> outputFileName;
  std::unique_ptr<String> inputFileName;

  std::unordered_map<SString, String*> docLinkHash;
};

#endif
