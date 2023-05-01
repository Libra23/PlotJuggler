#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

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

    uint8_t uint8() {
      // restore return address
      uint8_t ret = *ptr_data;
      // update address
      ptr_data += 1;
      offset += 1;
      return ret;
    }

    uint16_t uint16() {
      // endian
      std::vector<char> tmp(ptr_data, ptr_data+2);
      std::reverse(tmp.begin(), tmp.end());
      // restore return address
      uint16_t ret = *((uint16_t*)tmp.data());
      // update address
      ptr_data += 2;
      offset += 2;
      return ret;
    }

    uint32_t uint32() {
      // endian
      std::vector<char> tmp(ptr_data, ptr_data+2);
      std::reverse(tmp.begin(), tmp.end());
      // restore return address
      uint32_t ret = *((uint32_t*)tmp.data());
      // update address
      ptr_data += 4;
      offset += 4;
      return ret;
    }

    uint64_t uint64() {
      // endian
      std::vector<char> tmp(ptr_data, ptr_data+2);
      std::reverse(tmp.begin(), tmp.end());
      // restore return address
      uint64_t ret = *((uint64_t*)tmp.data());
      // update address
      ptr_data += 8;
      offset += 8;
      return ret;
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
  struct Timeseries {
    std::vector<double> timestamps;
    std::unordered_map<std::string, std::vector<double>> data;
  };

  MsgpackParser(char* data, size_t size);

  const Timeseries& RefTimeseries() { return time_series_; }

  const std::unordered_set<std::string>& RefTimeseriesKeys() { return time_series_keys_; }

  void PrintTimeseries();
private:
  static void ParseData(DataStream& data_stream, const std::string& prefix, std::unordered_map<std::string, double>& key_value);
    Timeseries time_series_;
    std::unordered_set<std::string> time_series_keys_;
};