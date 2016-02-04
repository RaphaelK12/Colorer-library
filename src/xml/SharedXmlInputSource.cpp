#include <xml/SharedXmlInputSource.h>
#include <xercesc/util/BinFileInputStream.hpp>

std::unordered_map<SString, SharedXmlInputSource*>* SharedXmlInputSource::isHash = nullptr;

int SharedXmlInputSource::addref()
{
  return ++ref_count;
}

int SharedXmlInputSource::delref()
{
  ref_count--;
  if (ref_count <= 0) {
    delete this;
    return -1;
  }
  return ref_count;
}

SharedXmlInputSource::SharedXmlInputSource(uXmlInputSource &source)
{
  ref_count = 1;
  input_source = std::move(source);
  std::unique_ptr<xercesc::BinFileInputStream> bfis(static_cast<xercesc::BinFileInputStream*>(input_source->getInputSource()->makeStream()));
  mSize = static_cast<XMLSize_t>(bfis->getSize());
  mSrc.reset(new XMLByte[mSize]);
  bfis->readBytes(mSrc.get(), mSize);
}

SharedXmlInputSource::~SharedXmlInputSource()
{
  DString d_id = DString(input_source->getInputSource()->getSystemId());
  //�� ����� ������� ������, ��������� �� �������. �� � ��� ��� � �����������
  isHash->erase(&d_id);
  if (isHash->size() == 0) {
    delete isHash;
    isHash = nullptr;
  }
}

SharedXmlInputSource* SharedXmlInputSource::getSharedInputSource(const XMLCh* path, const XMLCh* base)
{
  uXmlInputSource tempis = XmlInputSource::newInstance(path, base);

  if (isHash == nullptr) {
    isHash = new std::unordered_map<SString, SharedXmlInputSource*>();
  }

  DString d_id = DString(tempis->getInputSource()->getSystemId());
  auto s = isHash->find(d_id);
  if (s != isHash->end()) {
    SharedXmlInputSource* sis = s->second;
    sis->addref();
    return sis;
  } else {
    SharedXmlInputSource* sis = new SharedXmlInputSource(tempis);
    isHash->insert(std::make_pair(DString(sis->getInputSource()->getSystemId()), sis));
    return sis;
  }
}

xercesc::InputSource* SharedXmlInputSource::getInputSource() const
{
  return input_source->getInputSource();
}
