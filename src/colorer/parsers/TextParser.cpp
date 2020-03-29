#include <colorer/parsers/TextParserImpl.h>
#include <colorer/parsers/FileTypeImpl.h>

TextParser::TextParser() : pimpl(new TextParser::Impl()) {}

void TextParser::breakParse()
{
  pimpl->breakParse();
}

void TextParser::clearCache()
{
  pimpl->clearCache();
}

int TextParser::parse(int from, int num, TextParseMode mode)
{
  return pimpl->parse(from, num, mode);
}

void TextParser::setFileType(FileType* type)
{
  pimpl->setFileType(type);
}

void TextParser::setLineSource(LineSource* lh)
{
  pimpl->setLineSource(lh);
}

void TextParser::setRegionHandler(RegionHandler* rh)
{
  pimpl->setRegionHandler(rh);
}