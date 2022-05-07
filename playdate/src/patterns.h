#pragma once

static const LCDPattern kColorChekerboard = {0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xaa, 0x55, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};

#define FADE_LEVELS 8
#define PATTERNS 2

static const LCDPattern kPattern0[FADE_LEVELS] = {
    {0,0,0,0,0,0,0,0,
      0b10000000, // 0
      0b01000000,
      0b00100000,
      0b00010000,
      0b00001000,
      0b00000100,
      0b00000010,
      0b00000001,
    },
    {0,0,0,0,0,0,0,0,
      0b10001000, // 1
      0b01000100,
      0b00100010,
      0b00010001,
      0b10001000,
      0b01000100,
      0b00100010,
      0b00010001,
    },
    {0,0,0,0,0,0,0,0,
      0b10101000, // 2
      0b01010100,
      0b00101010,
      0b00010101,
      0b10001010,
      0b01000101,
      0b10100010,
      0b01010001,
    },
    {0,0,0,0,0,0,0,0,
      0b10101010, // 3
      0b01010101,
      0b10101010,
      0b01010101,
      0b10101010,
      0b01010101,
      0b10101010,
      0b01010101,
    },
    {0,0,0,0,0,0,0,0,
      0b10101011, // 4
      0b11010101,
      0b11101010,
      0b01110101,
      0b10111010,
      0b01011101,
      0b10101110,
      0b01010111,
    },
    {0,0,0,0,0,0,0,0,
      0b11101011, // 5
      0b11110101,
      0b11111010,
      0b01111101,
      0b10111110,
      0b01011111,
      0b10101111,
      0b11010111,
    },
    {0,0,0,0,0,0,0,0,
      0b11111011, // 6
      0b11111101,
      0b11111110,
      0b01111111,
      0b10111111,
      0b11011111,
      0b11101111,
      0b11110111,
    },
    {0,0,0,0,0,0,0,0,
      0b11111111, // 7
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
    },
  };

static const LCDPattern kPattern1[FADE_LEVELS] = {
    {0,0,0,0,0,0,0,0,
      0b00001000, // 0
      0b00001000,
      0b00001000,
      0b00001000,
      0b00001000,
      0b00001000,
      0b00001000,
      0b00001000,
    },
    {0,0,0,0,0,0,0,0,
      0b00001000, // 1
      0b00001000,
      0b00001000,
      0b00001000,
      0b11111111,
      0b00001000,
      0b00001000,
      0b00001000,
    },
    {0,0,0,0,0,0,0,0,
      0b10001000, // 2
      0b10001000,
      0b10001000,
      0b10001000,
      0b11111111,
      0b10001000,
      0b10001000,
      0b10001000,
    },
    {0,0,0,0,0,0,0,0,
      0b11111111, // 3
      0b10001000,
      0b10001000,
      0b10001000,
      0b11111111,
      0b10001000,
      0b10001000,
      0b10001000,
    },
    {0,0,0,0,0,0,0,0,
      0b11111111, // 4
      0b10101010,
      0b10101010,
      0b10101010,
      0b11111111,
      0b10101010,
      0b10101010,
      0b10101010,
    },
    {0,0,0,0,0,0,0,0,
      0b11111111, // 5
      0b10101010,
      0b11111111,
      0b10101010,
      0b11111111,
      0b10101010,
      0b11111111,
      0b10101010,
    },
    {0,0,0,0,0,0,0,0,
      0b11111111, // 6
      0b11101110,
      0b11111111,
      0b11101110,
      0b11111111,
      0b11101110,
      0b11111111,
      0b11101110,
    },
    {0,0,0,0,0,0,0,0,
      0b11111111, // 7
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
      0b11111111,
    },
  };