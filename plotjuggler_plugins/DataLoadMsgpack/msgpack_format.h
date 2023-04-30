enum FormatType {
  POSITIVE_FIX_INT = 0b00000000,
  FIX_MAP          = 0b10000000,
  FIX_ARRAY        = 0b10010000,
  FIX_STR          = 0b10100000,
  NIL              = 0xC0,
  FALSE            = 0xC2,
  TRUE             = 0xC3,
  BIN_8            = 0xC4,
  BIN_16           = 0xC5,
  BIN_32           = 0xC6,
  EXT_8            = 0xC7,
  EXT_16           = 0xC8,
  EXT_32           = 0xC9,
  FLOAT_32         = 0xCB,
  FLOAT_64         = 0xCB,
  UINT_8           = 0xCC,
  UINT_16          = 0xCD,
  UINT_32          = 0xCE,
  UINT_64          = 0xCF,
  INT_8            = 0xD0,
  INT_16           = 0xD1,
  INT_32           = 0xD2,
  INT_64           = 0xD3,
  FIXEXT1          = 0xD4,
  FIXEXT2          = 0xD5,
  FIXEXT4          = 0xD6,
  FIXEXT8          = 0xD7,
  FIXEXT16         = 0xD8,
  STR8             = 0xD9,
  STR16            = 0xDA,
  STR32            = 0xDB,
  ARRAY16          = 0xDC,
  ARRAY32          = 0xDD,
  MAP16            = 0xDE,
  MAP32            = 0xDF,
  NEGATIVE_FIX_INT = 0b11100000
};

enum FormaShift {
  POSITIVE_FIX_INT_SHIFT = 7,
  FIX_MAP_SHIFT = 4,
  FIX_ARRAY_SHIFT = 4,
  FIX_STR_SHIFT = 5,
  NEGATIVE_FIX_INT_SHIFT = 5
};

static inline bool IsFormat(char format, char mask, char shift = 0) {
  return ~(format^mask)>>shift;
}