#include "msgpack_parser.h"

MsgpackParser::MsgpackParser(char* data, size_t size) {
  DataStream data_stream(data, size);

  //while(data_stream) {
    ParseData(data_stream, "", key_value_map_);
  //}
  for (auto it = key_value_map_.cbegin(); it != key_value_map_.cend(); ++it) {
    std::cout << "{" << (*it).first << ": " << (*it).second << "}\n";
  }
}

void MsgpackParser::ParseData(DataStream& data_stream, const std::string& prefix, std::map<std::string, double>& key_value) {
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

