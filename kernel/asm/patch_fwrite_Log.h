/*
	Filename    : patch_fwrite_Log.bin
	Date created: Sun Feb 14 13:40:18 2021
*/

#define patch_fwrite_Log_size 0x68

const unsigned char patch_fwrite_Log[] = {
	0x7C, 0x85, 0x21, 0xD7, 	0x40, 0x81, 0x00, 0x5C, 	0x3C, 0xE0, 0x93, 0x16, 	0x60, 0xE7, 0x00, 0x00, 
	0x80, 0xC7, 0x00, 0x04, 	0x2C, 0x06, 0x00, 0x00, 	0x40, 0x82, 0x00, 0x48, 	0x54, 0x86, 0xD9, 0x7E, 
	0x38, 0xC6, 0x00, 0x02, 	0x7C, 0xC9, 0x03, 0xA6, 	0x7C, 0x66, 0x1B, 0x78, 	0x54, 0xC6, 0x00, 0x34, 
	0x7C, 0x00, 0x30, 0xAC, 	0x38, 0xC6, 0x00, 0x20, 	0x42, 0x00, 0xFF, 0xF8, 	0x90, 0x67, 0x00, 0x00, 
	0x90, 0x87, 0x00, 0x04, 	0x7C, 0x00, 0x38, 0xAC, 	0x7C, 0x00, 0x04, 0xAC, 	0x80, 0xC7, 0x00, 0x04, 
	0x7C, 0x00, 0x3B, 0xAC, 	0x7C, 0x00, 0x04, 0xAC, 	0x7C, 0x04, 0x30, 0x00, 	0x41, 0x82, 0xFF, 0xF0, 
	0x7C, 0xA3, 0x2B, 0x78, 	0x4E, 0x80, 0x00, 0x20
};
