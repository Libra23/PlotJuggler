#include "msgpack_parser.h"

static int count;

MsgpackParser::MsgpackParser(char* data, size_t size) {
  DataStream data_stream(data, size);

  while(data_stream) {
    parseData(data_stream);
  }
}

void MsgpackParser::parseData(DataStream& data_stream) {
  char format = *(data_stream.read(1));
  if ((format & 0x80) == FormatType::POSITIVE_FIX_INT) {
    char val = format;
    std::cout << "val = " << static_cast<int>(val) << std::endl;
  } else if ((format & 0xF0) == FormatType::FIX_MAP) {
    size_t map_size = format & 0x0F;
    for (size_t i = 0; i < map_size; i++) {
      // recursive
      parseData(data_stream);
    }
  } else if ((format & 0xF0) == FormatType::FIX_STR) {
    size_t str_size = format & 0x0F;
    std::string str = data_stream.str(str_size);
    // recursive
    parseData(data_stream);
  } else if ((format & 0xF0) == FormatType::FIX_ARRAY) {
    const size_t array_size = format & 0x0F;
    for (size_t i = 0; i < array_size; i++) {
      // recursive
      parseData(data_stream);
    }
  } else if ((format & 0xFF) == FormatType::FLOAT_64) {
    double val = data_stream.float64();
    std::cout << "val = " << val << std::endl;
  } else {
    std::cout << "!!! undefined !!!" << std::endl;
  }
}

