#include "msgpack_parser.h"

MsgpackParser::MsgpackParser(char* data, size_t size) {
  DataStream data_stream(data, size);

  int count = 0;
  while(data_stream) {
    // create key value map
    std::unordered_map<std::string, double> key_value_map;
    ParseData(data_stream, "", key_value_map);

    // add to time_series for key & value
    time_series_.timestamps.push_back(count);
    for(auto itr = key_value_map.cbegin(); itr != key_value_map.cend(); itr++) {
      const std::string& key = itr->first;
      const double& val = itr->second;

      const auto& time_series_itr = time_series_.data.find(key);
      if( time_series_itr != time_series_.data.end() ) {
        // if key is defined, add value
        time_series_itr->second.push_back(val);
      } else {
        // if key is not defined, add key & value
        time_series_.data[key].push_back(val);
      }
    }
    count++;
  }
  std::cout << "count : " << count << std::endl;
}

const MsgpackParser::Timeseries& MsgpackParser::RefTimeseries() {
  return time_series_;
}

void MsgpackParser::PrintTimeseries() {

}

void MsgpackParser::ParseData(DataStream& data_stream, const std::string& prefix, std::unordered_map<std::string, double>& key_value) {
  char format = *(data_stream.read(1));
  if ((format & 0x80) == FormatType::POSITIVE_FIX_INT) {
    double val = static_cast<double>(format);
    // insert
    key_value.emplace(prefix, val);
  } else if ((format & 0xF0) == FormatType::FIX_MAP) {
    size_t map_size = format & 0x0F;
    for (size_t i = 0; i < map_size; i++) {
      // recursive
      ParseData(data_stream, prefix, key_value);
    }
  } else if ((format & 0xF0) == FormatType::FIX_STR) {
    size_t str_size = format & 0x0F;
    std::string str = data_stream.str(str_size);
    // recursive
    ParseData(data_stream, prefix + '/' + str, key_value);
  } else if ((format & 0xF0) == FormatType::FIX_ARRAY) {
    const size_t array_size = format & 0x0F;
    for (size_t i = 0; i < array_size; i++) {
      // recursive
      ParseData(data_stream, prefix + '.' + std::to_string(i), key_value);
    }
  } else if ((format & 0xFF) == FormatType::FLOAT_64) {
    double val = data_stream.float64();
    // insert
    key_value.emplace(prefix, val);
  } else {
    std::cout << "!!! undefined !!!" << std::endl;
  }
}

