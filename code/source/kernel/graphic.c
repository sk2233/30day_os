#include "graphic.h"
#include "comm/cpu_instr.h"
#include "tool.h"
#include "file.h"
#include "mem.h"
#include "comm/math.h"

// 备用
//static uint8_t fonts[]={0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c,0x22,0x41,0x55,0x55,0x41,0x41,0x55,0x49,0x22,0x1c,0x0,0x0,0x0,0x0,0x0,0x1c,0x3e,0x7f,0x6b,0x6b,0x7f,0x7f,0x6b,0x77,0x3e,0x1c,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x36,0x7f,0x7f,0x7f,0x3e,0x1c,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8,0x1c,0x3e,0x7f,0x3e,0x1c,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8,0x1c,0x2a,0x7f,0x2a,0x8,0x1c,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8,0x1c,0x3e,0x7f,0x6b,0x8,0x1c,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x18,0x3c,0x3c,0x18,0x0,0x0,0x0,0x0,0x0,0x0,0xff,0xff,0xff,0xff,0xff,0xff,0xe7,0xc3,0xc3,0xe7,0xff,0xff,0xff,0xff,0xff,0xff,0x0,0x0,0x0,0x0,0x0,0x3c,0x66,0x42,0x42,0x66,0x3c,0x0,0x0,0x0,0x0,0x0,0xff,0xff,0xff,0xff,0xff,0xc3,0x99,0xbd,0xbd,0x99,0xc3,0xff,0xff,0xff,0xff,0xff,0x0,0x8,0x1c,0x2a,0x49,0x8,0x8,0x1c,0x22,0x41,0x41,0x41,0x22,0x1c,0x0,0x0,0x0,0x1c,0x22,0x41,0x41,0x41,0x22,0x1c,0x8,0x8,0x7f,0x8,0x8,0x8,0x0,0x0,0x0,0x0,0x30,0x70,0xd0,0xd0,0x50,0x10,0x10,0x18,0x1e,0x1f,0xe,0x0,0x0,0x0,0x0,0x0,0xf8,0xf8,0x88,0x88,0x88,0x88,0x88,0x88,0xee,0xff,0x66,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8,0x2a,0x1c,0x14,0x1c,0x2a,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x3,0x7,0xf,0x1f,0x3f,0x7f,0x3f,0x1f,0xf,0x7,0x3,0x1,0x0,0x0,0x0,0x40,0x60,0x70,0x78,0x7c,0x7e,0x7f,0x7e,0x7c,0x78,0x70,0x60,0x40,0x0,0x0,0x0,0x0,0x8,0x1c,0x2a,0x49,0x8,0x8,0x8,0x49,0x2a,0x1c,0x8,0x0,0x0,0x0,0x0,0x0,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x22,0x0,0x0,0x22,0x22,0x0,0x0,0x0,0x7c,0x52,0x51,0x51,0x51,0x51,0x52,0x5c,0x50,0x50,0x50,0x50,0x50,0x0,0x0,0x3e,0x41,0x2,0x4,0x1c,0x22,0x41,0x41,0x41,0x22,0x1c,0x10,0x20,0x41,0x3e,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7f,0x7f,0x7f,0x0,0x0,0x0,0x0,0x8,0x1c,0x2a,0x49,0x8,0x8,0x8,0x49,0x2a,0x1c,0x8,0x3e,0x0,0x0,0x0,0x8,0x1c,0x2a,0x49,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x0,0x0,0x0,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x49,0x2a,0x1c,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x8,0x10,0x20,0x7f,0x20,0x10,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8,0x4,0x2,0x7f,0x2,0x4,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x1,0x7f,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x14,0x22,0x7f,0x22,0x14,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8,0x8,0x1c,0x1c,0x3e,0x3e,0x7f,0x7f,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7f,0x7f,0x3e,0x3e,0x1c,0x1c,0x8,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x0,0x0,0x8,0x8,0x0,0x0,0x14,0x14,0x14,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x22,0x22,0x22,0x7f,0x22,0x22,0x22,0x22,0x22,0x7f,0x22,0x22,0x22,0x0,0x0,0x8,0x5c,0x6a,0x49,0x49,0x9,0xa,0x1c,0x28,0x48,0x49,0x49,0x2b,0x1d,0x8,0x8,0x46,0x49,0x29,0x29,0x16,0x10,0x8,0x8,0x4,0x34,0x4a,0x4a,0x49,0x31,0x0,0x0,0x0,0xe,0x11,0x11,0x11,0x9,0x6,0xe2,0x45,0x49,0x51,0x21,0x62,0x9c,0x0,0x0,0x20,0x10,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x40,0x20,0x10,0x10,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x10,0x10,0x20,0x40,0x0,0x1,0x2,0x4,0x4,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x4,0x4,0x2,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x8,0x49,0x2a,0x1c,0x2a,0x49,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8,0x8,0x8,0x7f,0x8,0x8,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x18,0x18,0x10,0x10,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7f,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x18,0x18,0x0,0x0,0x40,0x40,0x20,0x20,0x10,0x10,0x10,0x8,0x8,0x4,0x4,0x2,0x2,0x2,0x1,0x1,0x0,0x18,0x24,0x24,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x24,0x18,0x0,0x0,0x0,0x10,0x18,0x14,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x10,0x7c,0x0,0x0,0x0,0x18,0x24,0x42,0x42,0x40,0x20,0x10,0x8,0x4,0x4,0x2,0x2,0x7e,0x0,0x0,0x0,0x18,0x24,0x42,0x40,0x40,0x20,0x18,0x20,0x40,0x40,0x42,0x24,0x18,0x0,0x0,0x0,0x30,0x30,0x30,0x28,0x28,0x28,0x24,0x24,0x22,0x7e,0x20,0x20,0x78,0x0,0x0,0x0,0x3e,0x2,0x2,0x2,0x1a,0x26,0x40,0x40,0x40,0x40,0x42,0x24,0x18,0x0,0x0,0x0,0x18,0x24,0x42,0x2,0x1a,0x26,0x42,0x42,0x42,0x42,0x42,0x24,0x18,0x0,0x0,0x0,0x7e,0x42,0x42,0x20,0x20,0x10,0x10,0x10,0x8,0x8,0x8,0x8,0x1c,0x0,0x0,0x0,0x18,0x24,0x42,0x42,0x42,0x24,0x18,0x24,0x42,0x42,0x42,0x24,0x18,0x0,0x0,0x0,0x18,0x24,0x42,0x42,0x42,0x42,0x42,0x64,0x58,0x40,0x42,0x24,0x18,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x18,0x18,0x0,0x0,0x0,0x0,0x0,0x18,0x18,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x18,0x18,0x0,0x0,0x0,0x0,0x18,0x18,0x10,0x10,0x8,0x0,0x40,0x20,0x10,0x8,0x4,0x2,0x1,0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7f,0x0,0x0,0x7f,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x40,0x20,0x10,0x8,0x4,0x2,0x1,0x0,0x0,0x1c,0x22,0x41,0x41,0x41,0x20,0x10,0x8,0x8,0x0,0x0,0x18,0x18,0x0,0x0,0x0,0x1c,0x22,0x41,0x59,0x55,0x55,0x55,0x55,0x55,0x39,0x1,0x62,0x1c,0x0,0x0,0x0,0x18,0x18,0x18,0x18,0x24,0x24,0x24,0x24,0x7e,0x42,0x42,0x42,0xe7,0x0,0x0,0x0,0xf,0x12,0x22,0x22,0x22,0x12,0x1e,0x22,0x42,0x42,0x42,0x22,0x1f,0x0,0x0,0x0,0x5c,0x62,0x42,0x41,0x1,0x1,0x1,0x1,0x1,0x41,0x42,0x22,0x1c,0x0,0x0,0x0,0x1f,0x22,0x22,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x22,0x22,0x1f,0x0,0x0,0x0,0x7f,0x42,0x42,0x2,0x2,0x22,0x3e,0x22,0x2,0x2,0x42,0x42,0x7f,0x0,0x0,0x0,0x7f,0x42,0x42,0x2,0x2,0x22,0x3e,0x22,0x22,0x2,0x2,0x2,0xf,0x0,0x0,0x0,0x5c,0x62,0x42,0x41,0x1,0x1,0x79,0x41,0x41,0x41,0x42,0x62,0x1c,0x0,0x0,0x0,0xe7,0x42,0x42,0x42,0x42,0x42,0x7e,0x42,0x42,0x42,0x42,0x42,0xe7,0x0,0x0,0x0,0x3e,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x3e,0x0,0x0,0x0,0xf8,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x21,0x12,0xc,0x0,0x0,0xe7,0x42,0x22,0x12,0xa,0xa,0x6,0xa,0xa,0x12,0x22,0x42,0xe7,0x0,0x0,0x0,0xf,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x2,0x42,0x42,0x7f,0x0,0x0,0x0,0xc3,0x42,0x66,0x66,0x66,0x5a,0x5a,0x5a,0x42,0x42,0x42,0x42,0xe7,0x0,0x0,0x0,0xe3,0x42,0x46,0x46,0x4a,0x4a,0x4a,0x52,0x52,0x52,0x62,0x62,0x47,0x0,0x0,0x0,0x1c,0x22,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x22,0x1c,0x0,0x0,0x0,0x1f,0x22,0x42,0x42,0x42,0x22,0x1e,0x2,0x2,0x2,0x2,0x2,0xf,0x0,0x0,0x0,0x1c,0x22,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x49,0x51,0x22,0x5c,0x0,0x0,0x0,0x3f,0x42,0x42,0x42,0x42,0x3e,0x22,0x42,0x42,0x42,0x42,0x42,0xe7,0x0,0x0,0x0,0x5c,0x62,0x41,0x41,0x1,0x2,0x1c,0x20,0x40,0x41,0x41,0x23,0x1d,0x0,0x0,0x0,0x7f,0x49,0x49,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x3e,0x0,0x0,0x0,0xe7,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x42,0x24,0x3c,0x0,0x0,0x0,0xe7,0x42,0x42,0x42,0x42,0x24,0x24,0x24,0x24,0x18,0x18,0x18,0x18,0x0,0x0,0x0,0xe7,0x42,0x42,0x42,0x5a,0x5a,0x5a,0x5a,0x24,0x24,0x24,0x24,0x24,0x0,0x0,0x0,0xe7,0x42,0x42,0x24,0x24,0x24,0x18,0x24,0x24,0x24,0x42,0x42,0xe7,0x0,0x0,0x0,0x77,0x22,0x22,0x22,0x14,0x14,0x14,0x8,0x8,0x8,0x8,0x8,0x3e,0x0,0x0,0x0,0x7f,0x21,0x21,0x10,0x10,0x8,0x8,0x4,0x4,0x2,0x42,0x41,0x7f,0x0,0x0,0x0,0x7c,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x4,0x7c,0x0,0x1,0x1,0x2,0x2,0x4,0x4,0x4,0x8,0x8,0x10,0x10,0x20,0x20,0x20,0x40,0x40,0x0,0x3e,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x3e,0x0,0x0,0x8,0x14,0x22,0x41,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7f,0x0,0x8,0x10,0x20,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xe,0x10,0x20,0x3c,0x22,0x21,0x21,0x31,0x6e,0x0,0x0,0x3,0x2,0x2,0x2,0x2,0x1a,0x26,0x42,0x42,0x42,0x42,0x42,0x26,0x1a,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xc,0x32,0x21,0x21,0x1,0x1,0x41,0x22,0x1c,0x0,0x0,0x30,0x20,0x20,0x20,0x20,0x2c,0x32,0x21,0x21,0x21,0x21,0x21,0x32,0x6c,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c,0x22,0x41,0x41,0x3f,0x1,0x41,0x42,0x3c,0x0,0x0,0x70,0x8,0x8,0x8,0x8,0x3e,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x3e,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x6c,0x32,0x21,0x21,0x21,0x21,0x32,0x2c,0x20,0x20,0x1e,0x3,0x2,0x2,0x2,0x2,0x1a,0x26,0x42,0x42,0x42,0x42,0x42,0x42,0xc7,0x0,0x0,0x0,0x8,0x8,0x0,0x0,0xc,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x1c,0x0,0x0,0x0,0x20,0x20,0x0,0x0,0x30,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x10,0x10,0xc,0x3,0x2,0x2,0x2,0x2,0x72,0x22,0x12,0xa,0x6,0xa,0x12,0x22,0x67,0x0,0x0,0xc,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x1c,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x6f,0x92,0x92,0x92,0x92,0x92,0x92,0x92,0xdb,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1b,0x26,0x42,0x42,0x42,0x42,0x42,0x42,0xc7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1c,0x22,0x41,0x41,0x41,0x41,0x41,0x22,0x1c,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x1b,0x26,0x42,0x42,0x42,0x42,0x42,0x26,0x1a,0x2,0x7,0x0,0x0,0x0,0x0,0x0,0x2c,0x32,0x21,0x21,0x21,0x21,0x21,0x32,0x2c,0x20,0x70,0x0,0x0,0x0,0x0,0x0,0x3b,0x46,0x42,0x2,0x2,0x2,0x2,0x2,0x7,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x5e,0x61,0x41,0x3,0x1c,0x60,0x41,0x43,0x3d,0x0,0x0,0x0,0x0,0x8,0x8,0x8,0x3e,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x70,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x63,0x42,0x42,0x42,0x42,0x42,0x42,0x62,0xdc,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xe7,0x42,0x42,0x42,0x24,0x24,0x24,0x18,0x18,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xe7,0x42,0x42,0x5a,0x5a,0x5a,0x24,0x24,0x24,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x63,0x22,0x14,0x14,0x8,0x14,0x14,0x22,0x63,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xe7,0x42,0x42,0x24,0x24,0x24,0x18,0x18,0x8,0x8,0x6,0x0,0x0,0x0,0x0,0x0,0x7f,0x41,0x21,0x10,0x8,0x4,0x42,0x41,0x7f,0x0,0x0,0x0,0x60,0x10,0x8,0x8,0x8,0x8,0x6,0x8,0x8,0x8,0x8,0x10,0x60,0x0,0x0,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x0,0x6,0x8,0x10,0x10,0x10,0x10,0x60,0x10,0x10,0x10,0x10,0x8,0x6,0x0,0x0,0x0,0x4e,0x31,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8,0x14,0x22,0x41,0x7f,0x41,0x7f,0x0,0x0,0x0,0x0,0x0,0x0,0x1c,0x22,0x41,0x1,0x1,0x1,0x1,0x1,0x1,0x1,0x41,0x22,0x1c,0x8,0x4,0x0,0x0,0x24,0x24,0x0,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x42,0x7c,0x0,0x0,0x30,0x10,0x8,0x0,0x0,0x1c,0x22,0x41,0x41,0x7f,0x1,0x41,0x22,0x1c,0x0,0x0,0x0,0x8,0x14,0x22,0x0,0x1e,0x20,0x20,0x3c,0x22,0x21,0x21,0x22,0x7c,0x0,0x0,0x0,0x0,0x24,0x24,0x0,0x1e,0x20,0x20,0x3c,0x22,0x21,0x21,0x22,0x7c,0x0,0x0,0x8,0x10,0x20,0x0,0x0,0x1e,0x20,0x20,0x3c,0x22,0x21,0x21,0x22,0x7c,0x0,0x0,0x0,0x18,0x24,0x18,0x0,0x1e,0x20,0x20,0x3c,0x22,0x21,0x21,0x22,0x7c,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3c,0x42,0x1,0x1,0x1,0x1,0x1,0x42,0x3c,0x10,0x8,0x0,0x8,0x14,0x22,0x0,0x1c,0x22,0x41,0x41,0x7f,0x1,0x41,0x22,0x1c,0x0,0x0,0x0,0x0,0x24,0x24,0x0,0x1c,0x22,0x41,0x41,0x7f,0x1,0x41,0x22,0x1c,0x0,0x0,0x8,0x10,0x20,0x0,0x0,0x1c,0x22,0x41,0x41,0x7f,0x1,0x41,0x22,0x1c,0x0,0x0,0x0,0x0,0x24,0x24,0x0,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x0,0x0,0x0,0x8,0x14,0x22,0x0,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x0,0x0,0x8,0x10,0x20,0x0,0x0,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x0,0x0,0x24,0x24,0x0,0x1c,0x22,0x41,0x41,0x41,0x41,0x7f,0x41,0x41,0x41,0x41,0x0,0x0,0x0,0x1c,0x22,0x1c,0x22,0x41,0x41,0x41,0x41,0x7f,0x41,0x41,0x41,0x41,0x0,0x0,0x30,0x10,0x8,0x7f,0x1,0x1,0x1,0x1,0x1f,0x1,0x1,0x1,0x1,0x7f,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x6,0x38,0x48,0x4e,0x79,0x9,0x9,0x49,0x36,0x0,0x0,0x30,0x8,0x4,0x14,0x14,0x14,0x7f,0x14,0x14,0x14,0x14,0x14,0x14,0x14,0x0,0x0,0x0,0x8,0x14,0x22,0x0,0x1c,0x22,0x41,0x41,0x41,0x41,0x41,0x22,0x1c,0x0,0x0,0x0,0x0,0x24,0x24,0x0,0x1c,0x22,0x41,0x41,0x41,0x41,0x41,0x22,0x1c,0x0,0x0,0x8,0x10,0x20,0x0,0x0,0x1c,0x22,0x41,0x41,0x41,0x41,0x41,0x22,0x1c,0x0,0x0,0x0,0x8,0x14,0x22,0x0,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x42,0x7c,0x0,0x0,0x8,0x10,0x20,0x0,0x0,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x42,0x7c,0x0,0x0,0x0,0x0,0x24,0x24,0x0,0x41,0x41,0x22,0x22,0x14,0x14,0x8,0x8,0x4,0x4,0x2,0x24,0x24,0x0,0x1c,0x22,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x22,0x1c,0x0,0x0,0x24,0x24,0x0,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x22,0x1c,0x0,0x0,0x0,0x14,0x14,0x14,0x3c,0x56,0x15,0x15,0x15,0x15,0x15,0x56,0x3c,0x14,0x14,0x14,0x0,0x30,0x48,0x4,0x4,0x4,0x3f,0x4,0x4,0x4,0x6,0x5,0x4d,0x32,0x0,0x0,0x0,0x41,0x41,0x22,0x14,0x8,0x7f,0x8,0x8,0x7f,0x8,0x8,0x8,0x8,0x0,0x0,0x0,0x7,0x9,0x11,0x11,0x11,0x29,0x27,0xf9,0x21,0x21,0x21,0x21,0x21,0x0,0x0,0x0,0x30,0x48,0x8,0x8,0x8,0x7f,0x8,0x8,0x8,0x8,0x8,0x9,0x6,0x0,0x0,0x30,0x10,0x8,0x0,0x0,0x1e,0x20,0x20,0x3c,0x22,0x21,0x21,0x22,0x7c,0x0,0x0,0x30,0x10,0x8,0x0,0x0,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x0,0x0,0x30,0x10,0x8,0x0,0x0,0x1c,0x22,0x41,0x41,0x41,0x41,0x41,0x22,0x1c,0x0,0x0,0x30,0x10,0x8,0x0,0x0,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x42,0x7c,0x0,0x0,0x0,0x48,0x54,0x24,0x0,0x1f,0x21,0x41,0x41,0x41,0x41,0x41,0x41,0x41,0x0,0x0,0x48,0x54,0x24,0x0,0x41,0x43,0x43,0x45,0x49,0x49,0x51,0x61,0x61,0x41,0x0,0x0,0x0,0x0,0x0,0x1e,0x20,0x20,0x3c,0x22,0x21,0x21,0x22,0x7c,0x0,0x7f,0x0,0x0,0x0,0x0,0x0,0x1c,0x22,0x41,0x41,0x41,0x41,0x41,0x22,0x1c,0x0,0x7f,0x0,0x0,0x0,0x8,0x8,0x0,0x0,0x8,0x8,0x4,0x22,0x41,0x41,0x41,0x22,0x1c,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7f,0x1,0x1,0x1,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x7f,0x40,0x40,0x40,0x0,0x0,0x0,0x8,0xc,0x8,0x8,0x8,0x0,0x7f,0x0,0x1e,0x20,0x1c,0x2,0x3e,0x0,0x0,0x0,0x8,0xc,0x8,0x8,0x8,0x0,0x7f,0x0,0x18,0x14,0x12,0x3e,0x10,0x0,0x0,0x0,0x8,0x8,0x0,0x0,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x48,0x24,0x12,0x9,0x9,0x12,0x24,0x48,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x9,0x12,0x24,0x48,0x48,0x24,0x12,0x9,0x0,0x0,0x0,0x0,0x88,0x22,0x88,0x22,0x88,0x22,0x88,0x22,0x88,0x22,0x88,0x22,0x88,0x22,0x88,0x22,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xaa,0x55,0xee,0xbb,0xee,0xbb,0xee,0xbb,0xee,0xbb,0xee,0xbb,0xee,0xbb,0xee,0xbb,0xee,0xbb,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0xf,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0xf,0x8,0xf,0x8,0x8,0x8,0x8,0x8,0x8,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x2f,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3f,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xf,0x8,0xf,0x8,0x8,0x8,0x8,0x8,0x8,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x2f,0x20,0x2f,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x3f,0x20,0x2f,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x2f,0x20,0x3f,0x0,0x0,0x0,0x0,0x0,0x0,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x3f,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0xf,0x8,0xf,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xf,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0xf8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0xff,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xff,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0xf8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xff,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0xff,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0xf8,0x8,0xf8,0x8,0x8,0x8,0x8,0x8,0x8,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0xe8,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0xe8,0x8,0xf8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xf8,0x8,0xe8,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0xef,0x0,0xff,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xff,0x0,0xef,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0xe8,0x8,0xe8,0x28,0x28,0x28,0x28,0x28,0x28,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xff,0x0,0xff,0x0,0x0,0x0,0x0,0x0,0x0,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0xef,0x0,0xef,0x28,0x28,0x28,0x28,0x28,0x28,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0xff,0x0,0xff,0x0,0x0,0x0,0x0,0x0,0x0,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0xff,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xff,0x0,0xff,0x8,0x8,0x8,0x8,0x8,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xff,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0xf8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0xf8,0x8,0xf8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xf8,0x8,0xf8,0x8,0x8,0x8,0x8,0x8,0x8,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xf8,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0xef,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x28,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0xff,0x8,0xff,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0xf,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xf8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0x8,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xf0,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
static uint8_t *fonts;

// 没有透明通道
void draw_block(uint8_t *vram, uint16_t vram_w,uint16_t vram_h, int x, int y, uint16_t w, uint16_t h, uint8_t *block){
    int minX= max(x,0);
    int maxX = min(x+w,vram_w);
    int minY=max(y,0);
    int maxY= min(y+h,vram_h);
    if(maxX<=minX||maxY<=minY){
        return;
    }
    for (int ty = minY; ty < maxY; ++ty) {
        mem_copy(vram+ty*vram_w+minX,block+(ty-y)*w+minX-x,maxX-minX);
    }
}

// 有透明通道的，会慢一点
void draw_block_alpha(uint8_t *vram, uint16_t vram_w,uint16_t vram_h, int x, int y, uint16_t w, uint16_t h, const uint8_t *block){
    int minX= max(x,0);
    int maxX = min(x+w,vram_w);
    int minY=max(y,0);
    int maxY= min(y+h,vram_h);
    if(maxX<=minX||maxY<=minY){
        return;
    }
    for (int ty = minY; ty < maxY; ++ty) {
        for (int tx = minX; tx < maxX; ++tx) {
            uint8_t col = block[(ty-y)*w+tx-x];
            if(col!=COL_ALPHA){
                vram[ty* vram_w + tx]=col;
            }
        }
    }
}

// 无透明通道版本
void draw_block_sub(uint8_t *vram, uint16_t vram_w, int minX, int minY, int maxX, int maxY,
                    int x, int y, uint16_t w, uint16_t h, uint8_t *block){
    minX= max(minX,x);
    maxX= min(maxX,x+w);
    minY= max(minY,y);
    maxY= min(maxY,y+h);
    if(maxX<=minX||maxY<=minY){
        return;
    }
    for (int ty = minY; ty < maxY; ++ty) {
        mem_copy(vram+ty*vram_w+minX,block+(ty-y)*w+minX-x,maxX-minX);
    }
}

// 有透明通道版本会慢一些
void draw_block_sub_alpha(uint8_t *vram, uint16_t vram_w, int minX, int minY, int maxX, int maxY,
                    int x, int y, uint16_t w, uint16_t h, const uint8_t *block){
    minX= max(minX,x);
    maxX= min(maxX,x+w);
    minY= max(minY,y);
    maxY= min(maxY,y+h);
    if(maxX<=minX||maxY<=minY){
        return;
    }
    for (int ty = minY; ty < maxY; ++ty) {
        for (int tx = minX; tx < maxX; ++tx) {
            uint8_t col = block[(ty-y)*w+tx-x];
            if(col!=COL_ALPHA){
                vram[ty * vram_w + tx]=col;
            }
        }
    }
}

void fill_alpha(uint8_t *vram, uint16_t w, uint16_t h){
    mem_set(vram,COL_ALPHA,(uint32_t)w*(uint32_t)h);
}

void fill_rect(uint8_t *vram, uint16_t vram_w, uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint8_t col){
    for (int ty = 0; ty < h; ++ty) {
        mem_set(vram+(ty + y) * vram_w + x,col,w);
    }
}

void draw_point(uint8_t *vram, uint16_t vram_w, uint16_t x, uint16_t y,uint8_t col){
    vram[y*vram_w+x]=col;
}

// 注意绘制线要考虑负数
void draw_line(uint8_t *vram, uint16_t vram_w,uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1,uint8_t col){
    uint16_t stepX;
    uint16_t stepY;
    uint16_t count;
    if(abs(x0-x1)> abs(y0-y1)){// 谁大把谁当成1的标准 因为存在小数 这里 * 1024
        stepX=1024;
        stepY=((y1-y0)<<10)/(x1-x0);
        count= abs(x1-x0);
    } else{
        stepY=1024;
        stepX=((x1-x0)<<10)/(y1-y0);
        count= abs(y1-y0);
    }
    for (int i = 0; i < count; ++i) {
        uint16_t x=x0+(stepX*i>>10);
        uint16_t y=y0+(stepY*i>>10);
        vram[y*vram_w+x]=col;
    }
}

void draw_char(uint8_t *vram, uint16_t vram_w, uint16_t x, uint16_t y, char c, uint8_t col){
    int index = c*16;
    for (int ty = 0; ty < 16; ty++) {
        uint8_t b=fonts[index];
        index++;
        for (int tx = 0; tx < 8; ++tx) {
            if(b&(1<<tx)){
                vram[(ty + y) * vram_w + tx + x]=col;
            }
        }
    }
}

void draw_str(uint8_t *vram, uint16_t vram_w, uint16_t x, uint16_t y, char *s, uint8_t col){
    while (*s!=0x00){
        draw_char(vram, vram_w, x, y, *s, col);
        s++;
        x+=8;
    }
}

void set_palette(uint8_t index, const uint8_t *rgbs,uint8_t size){
    uint32_t eflags = read_eflags();	/* 记录中断许可标志的值 */
    cli(); 					/* 将中断许可标志置为0,禁止中断 */
    // 0x03c8设置写入调色板，0x03c7设置读取调色板
    outb(0x03c8, index);  // 设置当前要设置的调色板 默认使用调色板0
    for (int i = 0; i < size; i++) {
        // 依次写入rgb
        outb(0x03c9, rgbs[0] / 4);
        outb(0x03c9, rgbs[1] / 4);
        outb(0x03c9, rgbs[2] / 4);
        rgbs += 3;
    }
    write_eflags(eflags);	/* 复原中断许可标志必须使用复原而不是直接打开，可能原来中断就是关闭的 */
}

void palette_init(){
    // 传递给外部函数使用的值必须使用静态值，否则会在其他方法中被复用被修改
    static uint8_t rgbs[] = {
            0x00, 0x00, 0x00,	/*  0:黑 */
            0xff, 0x00, 0x00,	/*  1:梁红 */
            0x00, 0xff, 0x00,	/*  2:亮绿 */
            0xff, 0xff, 0x00,	/*  3:亮黄 */
            0x00, 0x00, 0xff,	/*  4:亮蓝 */
            0xff, 0x00, 0xff,	/*  5:亮紫 */
            0x00, 0xff, 0xff,	/*  6:浅亮蓝 */
            0xff, 0xff, 0xff,	/*  7:白 */
            0xc6, 0xc6, 0xc6,	/*  8:亮灰 */
            0x84, 0x00, 0x00,	/*  9:暗红 */
            0x00, 0x84, 0x00,	/* 10:暗绿 */
            0x84, 0x84, 0x00,	/* 11:暗黄 */
            0x00, 0x00, 0x84,	/* 12:暗青 */
            0x84, 0x00, 0x84,	/* 13:暗紫 */
            0x00, 0x84, 0x84, /* 14:浅暗蓝 */
            0x84, 0x84, 0x84,	/* 15:暗灰 */
            // img
            0xff,0xc2,0xe6,
            0xde,0x2c,0xb4,
            0xf6,0x75,0xde,
            0xff,0x71,0xde,
            0xde,0x30,0xb4,
            0xf6,0xae,0xbd,
            0xf6,0xca,0xac,
            0xff,0xfa,0xde,
            0xde,0x50,0xbd,
            0xde,0x30,0xac,
            0xf6,0x71,0xd5,
            0xd5,0x30,0xb4,
            0xf6,0xb2,0xbd,
            0xf6,0xc6,0xac,
            0xde,0x2c,0xac,
            0x5a,0xba,0xee,
            0x52,0xba,0xee,
            0xff,0xfa,0xe6,
            0x94,0xff,0xff,
            0xff,0xff,0xff,
            0xe6,0xff,0xff,
            0x9c,0xff,0xff,
            0xff,0xf6,0xe6,
            0xa4,0x85,0xde,
            0xde,0xca,0xff,
            0xde,0xca,0xf6,
            0xde,0xc6,0xff,
            0x9c,0x85,0xde,
            0xa4,0x85,0xd5,
            0xcd,0xb2,0xf6,
            0xa4,0x81,0xe6,
            0xf6,0xe6,0xff,
            0xff,0xbe,0xe6,
            0xc5,0xaa,0xf6,
            0xe6,0xc6,0xff,
            0xc5,0xaa,0xee,
            0x9c,0x61,0x9c,
            0xa4,0x61,0x9c,
            0x5a,0x69,0xa4,
            0x8b,0xa1,0xd5,
            0xd5,0x30,0xac,
            0x94,0xa1,0xd5,
            0xff,0xe2,0xf6,
            0x5a,0x69,0xac,
            0xc5,0xda,0xff,
            0xff,0xee,0xff,
            0xff,0xbe,0xee,
            0x9c,0x65,0x94,
            0x62,0x69,0xa4,
            0xee,0xca,0xac,
            0xff,0xf2,0xff,
            0xff,0xf2,0xf6,
            0xf6,0xca,0xa4,
            0xf6,0xca,0xb4,
            0x52,0xba,0xf6,
            // anim
            0x97,0xa8,0xba,
            0xff,0xff,0xff,
            0xc1,0xce,0xdb,
            0x9a,0x99,0xb3,
            0xc7,0xc6,0xdd,
            0xf0,0xf0,0xfb,
            0xb4,0x60,0x17,
            0x70,0x38,0x0,
            0xda,0xb1,0x8d,
            0x75,0x31,0x84,
            0xf0,0xcf,0xb1,
            0xc3,0x24,0xe5,
            0xfb,0xe0,0xc7,
            0xff,0xcf,0xc3,
            0x13,0x3f,0x6b,
            0x0,0x23,0x3f,
            0x23,0xb3,0x67,
            0x1c,0x87,0x53,
            0x6f,0x83,0x98,
    };
    set_palette(0,rgbs, sizeof(rgbs)/3);
}

void init_graphic(){
    // 加载外部资源
    file_t *file= file_open("fonts.sp",FALSE);
    fonts= mem_alloc(4096);
    file_read(file,4096,fonts);
    file_close(file);
    palette_init();
}
