#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <map>

#define FORMAT_TYPE_LEN 1

enum FormatType {
  POSITIVE_FIX_INT = 0x00,
  FIX_MAP = 0x80,
  FIX_ARRAY = 0x90,
  FIX_STR = 0xA0,
  FLOAT_64 = 0xCB
};

class MsgpackParser {
public:
  struct DataStream {
    const char* ptr_data;
    const size_t length;
    size_t offset;

    DataStream(const char* ptr, size_t len) : ptr_data(ptr), length(len), offset(0) {}

    const char* read(size_t len=0) {
      // restore return address
      const char* const ret = ptr_data;
      // update address
      ptr_data += len;
      offset += len;
      return ret;
    }

    std::string str(size_t len) {
      // restore return string
      std::string str(ptr_data, ptr_data+len);
      // update address
      ptr_data += len;
      offset += len;
      return str;
    }

    double float64() {
      // endian
      std::vector<char> tmp(ptr_data, ptr_data+8);
      std::reverse(tmp.begin(), tmp.end());
      // restore return double
      double ret = *((double*)(tmp.data()));
      // update address
      ptr_data += 8;
      offset += 8;
      return ret;
    }

    operator bool() {
      return offset < length;
    }
  };
  MsgpackParser(char* data, size_t size);
private:
  static void ParseData(DataStream& data_stream, const std::string& prefix, std::map<std::string, double>& key_value);
  std::map<std::string, double> key_value_map_;
};