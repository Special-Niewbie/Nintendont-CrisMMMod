/*

Nintendont (Loader) - Playing Gamecubes in Wii mode on a Wii U

Copyright (C) 2018  send0r

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation version 2.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

*/

#include <stdlib.h>
#include <gccore.h>
#include <sys/param.h>
#include <ogc/lwp_watchdog.h>
#include <network.h>

#include "font.h"
#include "menu.h"
#include "global.h"
#include "FPad.h"
#include "exi.h"
#include "ssl.h"
#include "http.h"

#include "ff_utf8.h"
#include "unzip/miniunz.h"
#include "unzip/ioapi.h"

#include "background_png.h"

#include "themes.h"
#include "mysterio.h"

extern char launch_dir[MAXPATHLEN];
extern char loaderPath[MAXPATHLEN];

typedef struct {
	const char *url;
	const char *text;
	const char *filename;
	const u32 max_size;
} downloads_t;

typedef enum {
    //Loader Themes
    LOADER_CONTROLLER = 0,
    LOADER_CLASSIC,
    LOADER_WANNAPLAY,
    LOADER_WARNING,
    LOADER_GAMECUBES,
    LOADER_MMBLUE,
    LOADER_MMCLASSIC,
    LOADER_MMDARK,
    LOADER_MARIOLINK,
    LOADER_KIDSPOWER,
    LOADER_PURPLE,
    LOADER_CARBONIX,
    LOADER_SEXYBLACK,
    LOADER_GCCONTROLLER,
    LOADER_RAINBOW,
    LOADER_MRLUIGI,
    //Dark Themes
    DARK_DOOMY,
    DARK_CARBONIX,
    DARK_ORCHID,
    DARK_OILY,
    DARK_STRIPES,
    DARK_DARKPLANET,
    DARK_RUSTYHELL,
    DARK_INFINITEFLOWER,
    DARK_COSMICPUZZLE,
    DARK_CHIRPYRED,
    DARK_CLOUDY,
    DARK_NINTENDO,
    DARK_MAGICALCUBE,
    DARK_GCLOGO,
    DARK_ALONEINTHEDARK,
    DARK_DARKFOREST,
    //Light Themes
    LIGHT_ORCHID,
    LIGHT_AQUA,
    LIGHT_SUNSHINE,
    LIGHT_SHROOM,
    LIGHT_TRIFORCE,
    LIGHT_GCLOGO,
    LIGHT_RAYS,
    LIGHT_CARBONIX,
    LIGHT_ARCADIA,
    LIGHT_ZELDAOOT,
    LIGHT_DOUBLEDASH,
    LIGHT_BRIGHTSKY,
    LIGHT_WII,
    LIGHT_KIRBY,
    LIGHT_CONTROLLER,
    LIGHT_LUIGI,
    //Gradient Themes
    GRADIENT_AMAZON,
    GRADIENT_FORESTSUN,
    GRADIENT_BLUEDREAM,
    GRADIENT_RAINBOW,
    GRADIENT_SANDY,
    GRADIENT_GOLDEN,
    GRADIENT_PARADISE,
    GRADIENT_PHARAO,
    GRADIENT_NOFEAR,
    GRADIENT_MAGIC,
    GRADIENT_MIGHTY,
    GRADIENT_GREENIES,
    GRADIENT_FOCUS,
    GRADIENT_HIGHLIGHT,
    GRADIENT_NORETURN,
    GRADIENT_ZOMBIE,
    //Black Themes
    BLACK_STORMYNIGHT,
    BLACK_DARKFOG,
    BLACK_NOHOPE,
    BLACK_VERSACE,
    BLACK_OLDASHELL,
    BLACK_NOESCAPE,
    BLACK_MILDHOPE,
    BLACK_LONELYFLOWERS,
    BLACK_POWERGRID,
    BLACK_FUTUREDOOR,
    BLACK_OLDPAPER,
    BLACK_DOOMSHIVE,
    BLACK_CRAZYWALL,
    BLACK_CLASSICCURVES,
    BLACK_SUPERNOVA,
    BLACK_MOSAIC,
    //Unicolor Themes
    UNI_BLACK,
    UNI_EARTHBLUE,
    UNI_DARKFOREST,
    UNI_FIREBRICK,
    UNI_DARKPURPLE,
    UNI_MIDNIGHT,
    UNI_GREENISH,
    UNI_GOLDENDIRT,
    UNI_BLUEDIAMOND,
    UNI_FROGGREEN,
    UNI_BEANRED,
    UNI_VANILLA,
    UNI_MINT,
    UNI_CORNYELLOW,
    UNI_SILVER,
    UNI_WHITE,
    //NinBlack.png
    DOWNLOAD_NINBLACK,
    //Preview.zip
    PREVIEW
} DOWNLOADS;

static const downloads_t Downloads[] = {
    //Loader Themes
	{"http://send0r.lima-city.de/Nintendont/themes/loader/l00.zip", "Downloading Loader Theme", "l00.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/loader/l01.zip", "Downloading Loader Theme", "l01.zip", 0x100000}, // 1MB
	{"http://send0r.lima-city.de/Nintendont/themes/loader/l02.zip", "Downloading Loader Theme", "l02.zip", 0x100000}, // 1MB
	{"http://send0r.lima-city.de/Nintendont/themes/loader/l03.zip", "Downloading Loader Theme", "l03.zip", 0x100000}, // 1MB
	{"http://send0r.lima-city.de/Nintendont/themes/loader/l04.zip", "Downloading Loader Theme", "l04.zip", 0x100000}, // 1MB
	{"http://send0r.lima-city.de/Nintendont/themes/loader/l05.zip", "Downloading Loader Theme", "l05.zip", 0x100000}, // 1MB
	{"http://send0r.lima-city.de/Nintendont/themes/loader/l06.zip", "Downloading Loader Theme", "l06.zip", 0x100000}, // 1MB
	{"http://send0r.lima-city.de/Nintendont/themes/loader/l07.zip", "Downloading Loader Theme", "l07.zip", 0x100000}, // 1MB
	{"http://send0r.lima-city.de/Nintendont/themes/loader/l08.zip", "Downloading Loader Theme", "l08.zip", 0x100000}, // 1MB
	{"http://send0r.lima-city.de/Nintendont/themes/loader/l09.zip", "Downloading Loader Theme", "l09.zip", 0x100000}, // 1MB
	{"http://send0r.lima-city.de/Nintendont/themes/loader/l10.zip", "Downloading Loader Theme", "l10.zip", 0x100000}, // 1MB
	{"http://send0r.lima-city.de/Nintendont/themes/loader/l11.zip", "Downloading Loader Theme", "l11.zip", 0x100000}, // 1MB
	{"http://send0r.lima-city.de/Nintendont/themes/loader/l12.zip", "Downloading Loader Theme", "l12.zip", 0x100000}, // 1MB
	{"http://send0r.lima-city.de/Nintendont/themes/loader/l13.zip", "Downloading Loader Theme", "l13.zip", 0x100000}, // 1MB
	{"http://send0r.lima-city.de/Nintendont/themes/loader/l14.zip", "Downloading Loader Theme", "l14.zip", 0x100000}, // 1MB
	{"http://send0r.lima-city.de/Nintendont/themes/loader/l15.zip", "Downloading Loader Theme", "l15.zip", 0x100000}, // 1MB
    //Dark Themes
    {"http://send0r.lima-city.de/Nintendont/themes/dark/legacy00.zip", "Downloading Dark Theme", "legacy00.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/dark/legacy01.zip", "Downloading Dark Theme", "legacy01.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/dark/legacy02.zip", "Downloading Dark Theme", "legacy02.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/dark/legacy03.zip", "Downloading Dark Theme", "legacy03.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/dark/legacy04.zip", "Downloading Dark Theme", "legacy04.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/dark/legacy05.zip", "Downloading Dark Theme", "legacy05.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/dark/legacy06.zip", "Downloading Dark Theme", "legacy06.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/dark/legacy07.zip", "Downloading Dark Theme", "legacy07.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/dark/legacy08.zip", "Downloading Dark Theme", "legacy08.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/dark/legacy09.zip", "Downloading Dark Theme", "legacy09.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/dark/legacy10.zip", "Downloading Dark Theme", "legacy10.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/dark/legacy11.zip", "Downloading Dark Theme", "legacy11.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/dark/legacy12.zip", "Downloading Dark Theme", "legacy12.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/dark/legacy13.zip", "Downloading Dark Theme", "legacy13.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/dark/legacy14.zip", "Downloading Dark Theme", "legacy14.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/dark/legacy15.zip", "Downloading Dark Theme", "legacy15.zip", 0x100000}, // 1MB
    //Light Themes
    {"http://send0r.lima-city.de/Nintendont/themes/light/back00.zip", "Downloading Light Theme", "back00.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/light/back01.zip", "Downloading Light Theme", "back01.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/light/back02.zip", "Downloading Light Theme", "back02.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/light/back03.zip", "Downloading Light Theme", "back03.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/light/back04.zip", "Downloading Light Theme", "back04.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/light/back05.zip", "Downloading Light Theme", "back05.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/light/back06.zip", "Downloading Light Theme", "back06.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/light/back07.zip", "Downloading Light Theme", "back07.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/light/back08.zip", "Downloading Light Theme", "back08.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/light/back09.zip", "Downloading Light Theme", "back09.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/light/back10.zip", "Downloading Light Theme", "back10.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/light/back11.zip", "Downloading Light Theme", "back11.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/light/back12.zip", "Downloading Light Theme", "back12.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/light/back13.zip", "Downloading Light Theme", "back13.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/light/back14.zip", "Downloading Light Theme", "back14.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/light/back15.zip", "Downloading Light Theme", "back15.zip", 0x100000}, // 1MB
    //Gradient Themes
    {"http://send0r.lima-city.de/Nintendont/themes/gradient/grad00.zip", "Downloading Light Theme", "grad00.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/gradient/grad01.zip", "Downloading Light Theme", "grad01.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/gradient/grad02.zip", "Downloading Light Theme", "grad02.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/gradient/grad03.zip", "Downloading Light Theme", "grad03.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/gradient/grad04.zip", "Downloading Light Theme", "grad04.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/gradient/grad05.zip", "Downloading Light Theme", "grad05.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/gradient/grad06.zip", "Downloading Light Theme", "grad06.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/gradient/grad07.zip", "Downloading Light Theme", "grad07.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/gradient/grad08.zip", "Downloading Light Theme", "grad08.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/gradient/grad09.zip", "Downloading Light Theme", "grad09.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/gradient/grad10.zip", "Downloading Light Theme", "grad10.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/gradient/grad11.zip", "Downloading Light Theme", "grad11.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/gradient/grad12.zip", "Downloading Light Theme", "grad12.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/gradient/grad13.zip", "Downloading Light Theme", "grad13.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/gradient/grad14.zip", "Downloading Light Theme", "grad14.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/gradient/grad15.zip", "Downloading Light Theme", "grad15.zip", 0x100000}, // 1MB
    //Black Themes
    {"http://send0r.lima-city.de/Nintendont/themes/black/000.zip", "Downloading Black Theme", "000.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/black/001.zip", "Downloading Black Theme", "001.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/black/002.zip", "Downloading Black Theme", "002.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/black/003.zip", "Downloading Black Theme", "003.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/black/004.zip", "Downloading Black Theme", "004.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/black/005.zip", "Downloading Black Theme", "005.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/black/006.zip", "Downloading Black Theme", "006.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/black/007.zip", "Downloading Black Theme", "007.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/black/008.zip", "Downloading Black Theme", "008.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/black/009.zip", "Downloading Black Theme", "009.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/black/010.zip", "Downloading Black Theme", "010.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/black/011.zip", "Downloading Black Theme", "011.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/black/012.zip", "Downloading Black Theme", "012.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/black/013.zip", "Downloading Black Theme", "013.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/black/014.zip", "Downloading Black Theme", "014.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/black/015.zip", "Downloading Black Theme", "015.zip", 0x100000}, // 1MB
    //Unicolor Themes
    {"http://send0r.lima-city.de/Nintendont/themes/unicolor/00.zip", "Downloading Unicolor Theme", "00.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/unicolor/01.zip", "Downloading Unicolor Theme", "01.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/unicolor/02.zip", "Downloading Unicolor Theme", "02.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/unicolor/03.zip", "Downloading Unicolor Theme", "03.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/unicolor/04.zip", "Downloading Unicolor Theme", "04.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/unicolor/05.zip", "Downloading Unicolor Theme", "05.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/unicolor/06.zip", "Downloading Unicolor Theme", "06.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/unicolor/07.zip", "Downloading Unicolor Theme", "07.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/unicolor/08.zip", "Downloading Unicolor Theme", "08.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/unicolor/09.zip", "Downloading Unicolor Theme", "09.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/unicolor/10.zip", "Downloading Unicolor Theme", "10.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/unicolor/11.zip", "Downloading Unicolor Theme", "11.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/unicolor/12.zip", "Downloading Unicolor Theme", "12.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/unicolor/13.zip", "Downloading Unicolor Theme", "13.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/unicolor/14.zip", "Downloading Unicolor Theme", "14.zip", 0x100000}, // 1MB
    {"http://send0r.lima-city.de/Nintendont/themes/unicolor/15.zip", "Downloading Unicolor Theme", "15.zip", 0x100000}, // 1MB
    //NinBlack.png
    {"http://send0r.lima-city.de/Nintendont/themes/NinBlack.png", "Downloading Black Screen for Game Boot", "NinBlack.png", 0x1000}, // 4KB
    //Preview.zip
    {"http://send0r.lima-city.de/Nintendont/themes/Preview.zip", "Downloading Preview Files", "Preview.zip", 0x500000} // 5MB
};

//took code from update.c (sorry, but why reinvent the wheel?)
extern void changeToDefaultDrive();
static int UnzipFile(const char *dir, bool useDefaultDrive, DOWNLOADS download_number, const void *buf, size_t fSize) {
	char filepath[20];
	snprintf(filepath,20,"%x+%x",(unsigned int)buf,(unsigned int)fSize);
	unzFile uf = unzOpen(filepath);
	if (!uf) {
		gprintf("Cannot open %s, aborting\r\n", Downloads[download_number].filename);
		return -1;
	}
	gprintf("%s opened\n", Downloads[download_number].filename);

	// Use the default drive or the active drive?
	if (useDefaultDrive) {
		changeToDefaultDrive();
	} else {
		// Use the active drive.
		f_chdrive_char(UseSD ? "sd:" : "usb:");
	}

	f_mkdir_char(dir); // attempt to make dir
	if (f_chdir_char(dir) != FR_OK) {
		gprintf("Error changing into %s, aborting\r\n", dir);
		unzClose(uf);
		return -2;
	}

	int ret = extractZip(uf, 0, 1, 0);
	unzCloseCurrentFile(uf);
	unzClose(uf);

	if (ret != 0) {
		gprintf("Failed to extract %s\r\n", filepath);
		return -3;
	}

	remove(Downloads[download_number].filename);
	changeToDefaultDrive();
	return 1;
}

// Using modified Download()-function
static s32 Download(DOWNLOADS download_number) {
    int line = 5;
	int ret;
	char errmsg[48];
	unsigned int http_status = 0;
	u8* outbuf = NULL;
	unsigned int filesize;
	char filepath[MAXPATHLEN];

	bool dir_argument_exists = (launch_dir[0] != 0);
	const char *dir = (dir_argument_exists ? launch_dir : "/apps/Nintendont/");

	ClearScreen();
	PrintInfo();

	snprintf(filepath, sizeof(filepath), "%s%s", dir, Downloads[download_number].filename);
    PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*line, Downloads[download_number].text);
	UpdateScreen();

	line++;
	gprintf("Downloading %s to %s\r\n", Downloads[download_number].url, filepath);
	ret = net_init();
	if (ret < 0) {
		gprintf("Failed to init network\r\n");
		ret = -1;
		strcpy(errmsg, "Network initialization failed.");
		goto end;
	}
	gprintf("Network Initialized\r\n");
	PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*line, "Network Initialized");
	UpdateScreen();
	ssl_init(); //only once needed
	line++;
	
	PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*line, "Downloading...");
	UpdateScreen();
	line++;

	int i;
	for (i = 0; i <= 10; i++) {
		ret = http_request(Downloads[download_number].url, Downloads[download_number].max_size);
		if (ret) break;
		if (i == 10) {
			gprintf("Error making http request\r\n");
			ret = -2;
			http_get_result(&http_status, NULL, NULL);
			snprintf(errmsg, sizeof(errmsg), "HTTP request failed: %u", http_status);
			goto end;
		}
	}

	ret = http_get_result(&http_status, &outbuf, &filesize); 
	if (((int)*outbuf & 0xF0000000) == 0xF0000000) {
		ret = -3;
		snprintf(errmsg, sizeof(errmsg), "http_get_result() failed: %u", http_status);
		goto end;
	}

	PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*line, "Download Complete");
	UpdateScreen();
	line++;
	if (!dir_argument_exists) {
		gprintf("Creating new directory\r\n");
		f_mkdir_char("/apps");
		f_mkdir_char("/apps/Nintendont");
	}

	// Write the file to disk.
	
	if (download_number != DOWNLOAD_NINBLACK) {
		// Zip-file needs to be decompressed to the loader's drive

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, download_number, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping failed.");
		}
	}
	else
	{
		FIL file;
		FRESULT res = f_open_char(&file, filepath, FA_WRITE|FA_CREATE_ALWAYS);
		if (res != FR_OK) {
			gprintf("File Error\r\n");
			snprintf(errmsg, sizeof(errmsg), "Error opening '%s': %u", filepath, res);
			ret = -4;
			goto end;
		} else {
			// Reserve space in the file.
			f_expand(&file, filesize, 1);

			// Write the file.
			UINT wrote;
			f_write(&file, outbuf, filesize, &wrote);
			f_close(&file);
			FlushDevices();
			ret = 1;
		}
	}
	if (ret == 1) {
		PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*line, "Finished.");
		UpdateScreen();
		line++;
        if (download_number <= LOADER_MRLUIGI) {
            line++;
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*line, "You will see a preview for 3 seconds now.");
            UpdateScreen();
            line++;
        } else if (download_number > LOADER_MRLUIGI && download_number < DOWNLOAD_NINBLACK) {
            line++;
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*line, "Applying theme now.");
            UpdateScreen();
            line++;
        }
	}

end:
	if (ret != 1) {
		PrintFormat(DEFAULT_SIZE, MAROON, MENU_POS_X, MENU_POS_Y + 20*line, "Error: %s", errmsg);
    }
    
	UpdateScreen();
	if (outbuf != NULL) free(outbuf);
	net_deinit();
	sleep(4);
	return ret;
}

void DeleteThemes() {
    char path[MAXPATHLEN];
    bool exists = strlen(launch_dir);
    int i;
    
    //delete every possible loading theme file except custom game themes
    for (i = 0; i < 5; i++){
		if (i != 0){
			memset(path, 0, sizeof(path));
		}
		switch(i){
			case 0:
				snprintf(path, sizeof(path), "%sNinBackLight.png", exists ? launch_dir : "/apps/Nintendont/");
				break;
			case 1:
				snprintf(path, sizeof(path), "%sNinBackDark.png", exists ? launch_dir : "/apps/Nintendont/");;
				break;
			case 2:
				snprintf(path, sizeof(path), "%sNinBackBars.png", exists ? launch_dir : "/apps/Nintendont/");
				break;
			case 3:
				snprintf(path, sizeof(path), "%sNinBackNoBar.png", exists ? launch_dir : "/apps/Nintendont/");
				break;
			case 4:
				snprintf(path, sizeof(path), "%sNinBlack.png", exists ? launch_dir : "/apps/Nintendont/");
				break;
		}
		
		f_unlink_char(path);
    }
}

void DeleteBackground() {
    char path[MAXPATHLEN];
    bool exists = strlen(launch_dir);
    int i;
    //just delete every possible background file
    for (i = 0; i < 4; i++){
		if (i != 0){
			memset(path, 0, sizeof(path));
		}
		switch (i){
			case 0:
				snprintf(path, sizeof(path), "%sNinLegacy.png", exists? launch_dir : "/apps/Nintendont/");
				break;
			case 1:
				snprintf(path, sizeof(path), "%sNinBack.png", exists? launch_dir : "/apps/Nintendont/");
				break;
			case 2:
				snprintf(path, sizeof(path), "%sbackground.png", exists? launch_dir : "/apps/Nintendont/");
				break;
			case 3:
				snprintf(path, sizeof(path), "%sbackground.jpg", exists? launch_dir : "/apps/Nintendont/");
				break;
		}
		
		f_unlink_char(path);
    }
}

bool SetPreview(const bool enabled) {
    bool exists = strlen(launch_dir);
    char path[MAXPATHLEN];
    
    snprintf(path, sizeof(path), "%sPreview/enabled",
             exists ? launch_dir : "/apps/Nintendont/");
    //if enabled delete previewConf
    if (enabled) {
        f_unlink_char(path);
        return false;
    }
    
    //if not enabled, try to write previewConf (empty file)
    FIL previewConf;
    if (f_open_char(&previewConf, path, FA_WRITE | FA_CREATE_ALWAYS) == FR_OK) {
        f_close(&previewConf);
        return true;
    }
    
    //if this fails, return state which was given at function call
    return enabled;
}

bool PreviewEnabled() {
    bool exists = strlen(launch_dir);
    char path[MAXPATHLEN];
    
    snprintf(path, sizeof(path), "%sPreview/enabled",
             exists ? launch_dir : "/apps/Nintendont/");
    // Check if file exists and return true if so
    FIL previewConf;
    if (f_open_char(&previewConf, path, FA_READ | FA_OPEN_EXISTING) == FR_OK) {
        f_close(&previewConf);
        return true;
    }
    
    return false;
}

bool PreviewTest() {
    bool exists = strlen(launch_dir);
    char path[MAXPATHLEN];
    
    snprintf(path, sizeof(path), "%sPreview/black/00.png",
             exists ? launch_dir : "/apps/Nintendont/");
    // Check if file exists and return true if so
    FIL testPic;
    if (f_open_char(&testPic, path, FA_READ | FA_OPEN_EXISTING) == FR_OK) {
        f_close(&testPic);
        return true;
    }
    
    return false;
}

void Preview(const int menuNumber, const int selected) {
    bool exists = strlen(launch_dir);
    char path[MAXPATHLEN];
    char folder[6][8] = {
        "loader/",
        "dark/",
        "light/",
        "grad/",
        "black/",
        "single/"
    };
    
    snprintf(path, sizeof(path), "%sPreview/%s%02d.png",
             exists ? launch_dir : "/apps/Nintendont/",
             folder[menuNumber], selected);
    GRRLIB_texImg *preview = GRRLIB_LoadTextureFromFile(path);
    if (preview != NULL) {
        GRRLIB_DrawImg(400, 120, preview, 0, 1, 1, RGBA(255, 255, 255, 255));
    }
    
    GRRLIB_FreeTexture(preview);
}

void LoaderMenu(const bool previewState) {
    // Using code from void UpdateNintendont(void)
    bool redraw = true;
    int selected = 0;
    u64 delay = ticks_to_millisecs(gettime()) + 500;
    
    while (true) {
        if (redraw) {
            /*
            * Print the Menu
            */
        
            PrintInfo();
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*3, "Loader Theme Download Menu");
            PrintButtonActions("Go Back", "Download", "Go Back", NULL);
        
            // Now the entries
        
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*5, "Download Controller Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*6, "Download Classic Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*7, "Download Wanna-Play Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*8, "Download Warning Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*9, "Download GameCubes Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*10, "Download MasterMod Blue Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*11, "Download MasterMod Classic Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*12, "Download MasterMod Dark Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*13, "Download Mario Link Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*14, "Download Kids Power Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*15, "Download Purple Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*16, "Download Carbonix Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*17, "Download Sexy Black Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*18, "Download GC Controller Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*19, "Download Rainbow Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*20, "Download Mr Luigi Theme");
            
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*(5+selected), ARROW_RIGHT);
            
            if (previewState) {
                Preview(0, selected);
            }
            
            redraw = false;
        
            // Render everything
            UpdateScreen();
            ClearScreen();
        }
        
        VIDEO_WaitVSync();
        FPAD_Update();
        if (delay > ticks_to_millisecs(gettime())) {
            continue;
        }
        
        if (FPAD_OK(0)) {
            DeleteThemes();
            
            unsigned int backupTextColor = text_color;
            int ret = Download(selected);
            int mode = checkTheme();
            
            GRRLIB_texImg *temp;
            temp = GRRLIB_LoadTextureFromFile(loaderPath);
            if (ret == 1 && temp != NULL) {
                GRRLIB_Screen2Texture(0, 0, screen_buffer, GX_FALSE);
                FadeOut(screen_buffer);
                
                FadeIn(temp);
                GRRLIB_DrawImg(0, 0, temp, 0, 1, 1, RGBA(255, 255, 255, 255));
                if (mode == 2) { //show loading bar
                    PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50*1, MENU_POS_Y + 20*19, "\xe2\x96\x88\xe2\x96\x88\xe2\x96\x88\xe2\x96\x88\xe2\x96\x88\xe2\x96\x88\xe2\x96\x88\xe2\x96\x88\xe2\x96\x88\xe2\x96\x88");
                }
                
                UpdateScreen();
                GRRLIB_Screen2Texture(0, 0, screen_buffer, GX_FALSE);
                UpdateScreen();
                sleep(3);
                FadeOut(screen_buffer);
                GRRLIB_FreeTexture(temp);
            } else {
                ClearScreen();
                PrintFormat(DEFAULT_SIZE, MAROON, MENU_POS_X + 50, MENU_POS_Y + 20*10, "SOMETHING WENT WRONG!");
                PrintFormat(DEFAULT_SIZE, MAROON, MENU_POS_X + 50, MENU_POS_Y + 20*11, "PLEASE TRY REDOWNLOADING THE THEME!");
                UpdateScreen();
                sleep(5);
            }
            
            text_color = backupTextColor;
            ClearScreen();
            redraw = true;
        } else if (FPAD_Start(0) || FPAD_Cancel(0)) {
            break;
        } else if (FPAD_Down(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected++;
            if (selected > 15) {
                selected = 0;
            }
            
            redraw = true;
        } else if (FPAD_Up(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected--;
            if (selected < 0) {
                selected = 15;
            }
            
            redraw = true;
        }
    } //while
    
    return;
}

void DarkMenu(const bool previewState) {
    // Using code from void UpdateNintendont(void)
    bool redraw = true;
    int selected = 0;
    u64 delay = ticks_to_millisecs(gettime()) + 500;
    
    while (true) {
        if (redraw) {
            /*
            * Print the Menu
            */
        
            PrintInfo();
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*3, "Dark Theme Download Menu");
            PrintButtonActions("Go Back", "Download", "Go Back", NULL);
        
            // Now the entries
        
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*5, "Download Doomy Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*6, "Download Carbonix Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*7, "Download Orchid Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*8, "Download Oily Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*9, "Download Stripes Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*10, "Download Dark Planet Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*11, "Download Rusty Hell Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*12, "Download Infinite Flower Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*13, "Download Cosmic Puzzle Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*14, "Download Chirpy Red Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*15, "Download Cloudy Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*16, "Download Nintendo Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*17, "Download Magical Cube Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*18, "Download GC Logo Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*19, "Download Alone in The Dark Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*20, "Download Dark Forest Theme");
            
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*(5+selected), ARROW_RIGHT);
            
            if (previewState) {
                Preview(1, selected);
            }
            redraw = false;
        
            // Render everything
            UpdateScreen();
            ClearScreen();
        }
        
        VIDEO_WaitVSync();
        FPAD_Update();
        if (delay > ticks_to_millisecs(gettime())) {
            continue;
        }
        
        if (FPAD_OK(0)) {
            DeleteBackground();
            DeleteThemes();
            
            Download(selected + 1 * 16);
            
            CustomBackground();
            
            ClearScreen();
            redraw = true;
        } else if (FPAD_Start(0) || FPAD_Cancel(0)) {
            break;
        } else if (FPAD_Down(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected++;
            if (selected > 15) {
                selected = 0;
            }
            
            redraw = true;
        } else if (FPAD_Up(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected--;
            if (selected < 0) {
                selected = 15;
            }
            
            redraw = true;
        }
    } //while
    
    return;
}

void LightMenu(const bool previewState) {
    // Using code from void UpdateNintendont(void)
    bool redraw = true;
    int selected = 0;
    u64 delay = ticks_to_millisecs(gettime()) + 500;
    
    while (true) {
        if (redraw) {
            /*
            * Print the Menu
            */
        
            PrintInfo();
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*3, "Light Theme Download Menu");
            PrintButtonActions("Go Back", "Download", "Go Back", NULL);
        
            // Now the entries
        
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*5, "Download Orchid Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*6, "Download Aqua Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*7, "Download Sunshine Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*8, "Download Shroom Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*9, "Download Triforce Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*10, "Download GC Logo Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*11, "Download Rays Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*12, "Download Carbonix Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*13, "Download Arcadia Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*14, "Download Zelda OoT Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*15, "Download Double-Dash Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*16, "Download Bright Sky Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*17, "Download Wii Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*18, "Download Kirby Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*19, "Download Controller Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*20, "Download Luigi Theme");
            
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*(5+selected), ARROW_RIGHT);
            
            if (previewState) {
                Preview(2, selected);
            }
            redraw = false;
        
            // Render everything
            UpdateScreen();
            ClearScreen();
        }
        
        VIDEO_WaitVSync();
        FPAD_Update();
        if (delay > ticks_to_millisecs(gettime())) {
            continue;
        }
        
        if (FPAD_OK(0)) {
            DeleteBackground();
            DeleteThemes();
            
            Download(selected + 2 * 16);
            
            CustomBackground();
            
            ClearScreen();
            redraw = true;
        } else if (FPAD_Start(0) || FPAD_Cancel(0)) {
            break;
        } else if (FPAD_Down(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected++;
            if (selected > 15) {
                selected = 0;
            }
            
            redraw = true;
        } else if (FPAD_Up(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected--;
            if (selected < 0) {
                selected = 15;
            }
            
            redraw = true;
        }
    } //while
    
    return;
}

void GradientMenu(const bool previewState) {
    // Using code from void UpdateNintendont(void)
    bool redraw = true;
    int selected = 0;
    u64 delay = ticks_to_millisecs(gettime()) + 500;
    
    while (true) {
        if (redraw) {
            /*
            * Print the Menu
            */
        
            PrintInfo();
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*3, "Gradient Theme Download Menu");
            PrintButtonActions("Go Back", "Download", "Go Back", NULL);
        
            // Now the entries
        
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*5, "Download Amazon Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*6, "Download Forest Sun Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*7, "Download Blue Dream Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*8, "Download Rainbow Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*9, "Download Sandy Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*10, "Download Golden Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*11, "Download Paradise Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*12, "Download Pharao Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*13, "Download No Fear Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*14, "Download Magic Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*15, "Download Mighty Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*16, "Download Greenies Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*17, "Download Focus Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*18, "Download Highlight Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*19, "Download No return Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*20, "Download Zombie Theme");
            
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*(5+selected), ARROW_RIGHT);
            
            if (previewState) {
                Preview(3, selected);
            }
            redraw = false;
        
            // Render everything
            UpdateScreen();
            ClearScreen();
        }
        
        VIDEO_WaitVSync();
        FPAD_Update();
        if (delay > ticks_to_millisecs(gettime())) {
            continue;
        }
        
        if (FPAD_OK(0)) {
            DeleteBackground();
            DeleteThemes();
            
            Download(selected + 3 * 16);
            
            CustomBackground();
            
            ClearScreen();
            redraw = true;
        } else if (FPAD_Start(0) || FPAD_Cancel(0)) {
            break;
        } else if (FPAD_Down(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected++;
            if (selected > 15) {
                selected = 0;
            }
            
            redraw = true;
        } else if (FPAD_Up(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected--;
            if (selected < 0) {
                selected = 15;
            }
            
            redraw = true;
        }
    } //while
    
    return;
}

void BlackMenu(const bool previewState) {
    // Using code from void UpdateNintendont(void)
    bool redraw = true;
    int selected = 0;
    u64 delay = ticks_to_millisecs(gettime()) + 500;
    
    while (true) {
        if (redraw) {
            /*
            * Print the Menu
            */
        
            PrintInfo();
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*3, "Black Theme Download Menu");
            PrintButtonActions("Go Back", "Download", "Go Back", NULL);
        
            // Now the entries
        
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*5, "Download Stormy Night Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*6, "Download Dark Fog Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*7, "Download No Hope Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*8, "Download Versace Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*9, "Download Old As Hell Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*10, "Download No Esape Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*11, "Download Mild Hope Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*12, "Download Lonely Flowers Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*13, "Download Power Grid Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*14, "Download Future Door Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*15, "Download Old Paper Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*16, "Download Doom's Hive Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*17, "Download Crazy Wall Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*18, "Download Classic Curves Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*19, "Download Super Nova Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*20, "Download Mosaic Theme");
            
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*(5+selected), ARROW_RIGHT);
            
            if (previewState) {
                Preview(4, selected);
            }
            redraw = false;
        
            // Render everything
            UpdateScreen();
            ClearScreen();
        }
        
        VIDEO_WaitVSync();
        FPAD_Update();
        if (delay > ticks_to_millisecs(gettime())) {
            continue;
        }
        
        if (FPAD_OK(0)) {
            DeleteBackground();
            DeleteThemes();
            
            Download(selected + 4 * 16);
            
            CustomBackground();
            
            ClearScreen();
            redraw = true;
        } else if (FPAD_Start(0) || FPAD_Cancel(0)) {
            break;
        } else if (FPAD_Down(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected++;
            if (selected > 15) {
                selected = 0;
            }
            
            redraw = true;
        } else if (FPAD_Up(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected--;
            if (selected < 0) {
                selected = 15;
            }
            
            redraw = true;
        }
    } //while
    
    return;
}

void UnicolorMenu(const bool previewState) {
    // Using code from void UpdateNintendont(void)
    bool redraw = true;
    int selected = 0;
    u64 delay = ticks_to_millisecs(gettime()) + 500;
    
    while (true) {
        if (redraw) {
            /*
            * Print the Menu
            */
        
            PrintInfo();
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*3, "Unicolor Theme Download Menu");
            PrintButtonActions("Go Back", "Download", "Go Back", NULL);
        
            // Now the entries
        
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*5, "Download Black Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*6, "Download Earth Blue Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*7, "Download Dark Forest Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*8, "Download Fire Brick Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*9, "Download Dark Purple Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*10, "Download Midnight Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*11, "Download Greenish Blue Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*12, "Download Golden Dirt Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*13, "Download Blue Diamond Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*14, "Download Frog Green Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*15, "Download Bean Red Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*16, "Download Vanilla Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*17, "Download Mint Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*18, "Download Corn Yellow Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*19, "Download Silver Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*20, "Download White Theme");
            
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*(5+selected), ARROW_RIGHT);
            
            if (previewState) {
                Preview(5, selected);
            }
            redraw = false;
        
            // Render everything
            UpdateScreen();
            ClearScreen();
        }
        
        VIDEO_WaitVSync();
        FPAD_Update();
        if (delay > ticks_to_millisecs(gettime())) {
            continue;
        }
        
        if (FPAD_OK(0)) {
            DeleteBackground();
            DeleteThemes();
            
            Download(selected + 5 * 16);
            
            CustomBackground();
            
            ClearScreen();
            redraw = true;
        } else if (FPAD_Start(0) || FPAD_Cancel(0)) {
            break;
        } else if (FPAD_Down(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected++;
            if (selected > 15) {
                selected = 0;
            }
            
            redraw = true;
        } else if (FPAD_Up(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected--;
            if (selected < 0) {
                selected = 15;
            }
            
            redraw = true;
        }
    } //while
    
    return;
}

void RestoreDefault() {
    ClearScreen();
    PrintInfo();
    PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*5, "Deleting theme files...");
    PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*6, "Text Color Settings won't be changed!");
    UpdateScreen();
    DeleteThemes();
    DeleteBackground();
    CustomBackground();
    // Double ClearScreen to prevent weird black screen
    ClearScreen();
    ClearScreen();
    sleep(4);
}

void ThemeMenu() {
    // Using code from void UpdateNintendont(void)
    bool previewState = PreviewEnabled();
    bool previewDownloaded = PreviewTest();
    bool redraw = true;
    int selected = 0;
    u64 delay = ticks_to_millisecs(gettime()) + 500;
    
    while (true) {
        if (redraw) {
            /*
            * Print the Menu
            */
        
            PrintInfo();
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*3, "Theme Menu");
            PrintButtonActions("Go Back", "Select", "Go Back", NULL);
        
            // Now the entries
        
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*5, "Download Loader Themes");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*6, "Download Dark Themes");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*7, "Download Light Themes");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*8, "Download Gradient Themes");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*9, "Download Black Themes");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*10, "Download Unicolor Themes");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*11, "Set Black Screen on Game Boot");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*12, "Download Preview Pack");
            if (previewDownloaded) {
                if (previewState) {
                    PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*14, "Preview Pack: Enabled");
                } else {
                    PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*14, "Preview Pack: Disabled");
                }
            }
            
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*15, "Restore Default Theme");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*(5+selected), ARROW_RIGHT);
            
            
            redraw = false;
        
            // Render everything
            UpdateScreen();
            ClearScreen();
        }
        
        VIDEO_WaitVSync();
        FPAD_Update();
        if (delay > ticks_to_millisecs(gettime())) {
            continue;
        }
        
        if (FPAD_OK(0)) {
            switch(selected) {
                case 0:
                    LoaderMenu(previewState);
                    break;
                case 1:
                    DarkMenu(previewState);
                    break;
                case 2:
                    LightMenu(previewState);
                    break;
                case 3:
                    GradientMenu(previewState);
                    break;
                case 4:
                    BlackMenu(previewState);
                    break;
                case 5:
                    UnicolorMenu(previewState);
                    break;
                case 6:
                    DeleteThemes();
                    Download(DOWNLOAD_NINBLACK);
                    ClearScreen();
                    break;
                case 7:
                    if (Download(PREVIEW) == 1) {
                        previewDownloaded = true;
                        previewState = PreviewEnabled();
                    }
                    
                    ClearScreen();
                    break;
                case 9:
                    previewState = SetPreview(previewState);
                    break;
                case 10:
                    RestoreDefault();
                    break;
                default:
                    goto exit_while;
            } //switch
            
            redraw = true;
        } else if (FPAD_Start(0) || FPAD_Cancel(0)) {
            break;
        } else if (FPAD_Down(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected++;
            if (selected == 8 && previewDownloaded) {
                selected++;
            } else if (selected == 8 && !previewDownloaded) {
                selected += 2;
            } else if (selected > 10) {
                selected = 0;
            }
            
            redraw = true;
        } else if (FPAD_Up(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected--;
            if (selected == 8 && previewDownloaded) {
                selected--;
            } else if (selected == 9 && !previewDownloaded) {
                selected -= 2;
            } else if (selected < 0) {
                selected = 10;
            }
            
            redraw = true;
        }
    } //while
    
    exit_while:
    return;
}
