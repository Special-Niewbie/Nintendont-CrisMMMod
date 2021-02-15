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
#include <mp3player.h>

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
#include "sample_mp3.h"

#include "themes.h"
#include "themes2.h"
#include "mysterio.h"
#include "crismmmod.h"

extern char launch_dir[MAXPATHLEN];
extern char loaderPath[MAXPATHLEN];

typedef struct {
	const char *url;
	const char *text;
	const char *filename;
	const u32 max_size;
} downloads_t;

typedef enum {
    //Boot Loader Themes
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
    //NinBlack.png
    DOWNLOAD_NINBLACK,
    //Preview.zip
    PREVIEW
} DOWNLOADS;

static const downloads_t Downloads[] = {
    //Boot Loader Themes
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Floader%2Fl00.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T18%3A09%3A39.354Z&signature=ee66add241ee05a3d0514a3adb4b91d0dbd44a91d61166bbbd7dfd7ef583feca", "Downloading Loader Theme", "l00.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Floader%2Fl01.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T18%3A11%3A03.938Z&signature=ffc3dbb51ed665697792673e3c4aecf0beabe81e4ab9d98a1efd3379bcde79ff", "Downloading Loader Theme", "l01.zip", 0x100000}, // 1MB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Floader%2Fl02.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T18%3A11%3A29.681Z&signature=020c77e98503d9a94453c9d735d8eccca7e00140b726e299f951b0b029c1e5ad", "Downloading Loader Theme", "l02.zip", 0x100000}, // 1MB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Floader%2Fl03.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T18%3A11%3A56.355Z&signature=53b74b632550c2e04f5f5ff304c0f1d982cc76979fbe7a31bed6ed2048fe641f", "Downloading Loader Theme", "l03.zip", 0x100000}, // 1MB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Floader%2Fl04.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T18%3A12%3A20.831Z&signature=a7c9a42b4e20bfb48e90c4ae5e44724dc68ed7d3bcf09b9636ed97b6a57ac44a", "Downloading Loader Theme", "l04.zip", 0x100000}, // 1MB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Floader%2Fl05.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T18%3A12%3A41.312Z&signature=1ac66b14fd4e1c0b5f6b1207c773fa21141099b96053779b78c469f515be601a", "Downloading Loader Theme", "l05.zip", 0x100000}, // 1MB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Floader%2Fl06.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T18%3A12%3A56.908Z&signature=d6ac11203bd4f971fdc96afa6cf8208862e50e779938ea3e7012477797bf7d60", "Downloading Loader Theme", "l06.zip", 0x100000}, // 1MB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Floader%2Fl07.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T18%3A13%3A13.470Z&signature=45fff97379db0504aea0ad13283ad5932b883fc456f2ce06053cf90c494d106d", "Downloading Loader Theme", "l07.zip", 0x100000}, // 1MB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Floader%2Fl08.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T18%3A13%3A39.201Z&signature=3b9b19d5ba35fe5789fdc2e3f8d6b51d66a0704095b1dbdb99ecf6ba73a86893", "Downloading Loader Theme", "l08.zip", 0x100000}, // 1MB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Floader%2Fl09.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T18%3A13%3A56.232Z&signature=fe4340b3189431b1022f911093053312f58525d6cd41c0ac7f996d423976127d", "Downloading Loader Theme", "l09.zip", 0x100000}, // 1MB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Floader%2Fl10.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T18%3A14%3A11.964Z&signature=3321d39556a3d4ed095aad5c355597ce4cf1fe42042704dc80d01205e073d251", "Downloading Loader Theme", "l10.zip", 0x100000}, // 1MB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Floader%2Fl11.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T18%3A15%3A11.289Z&signature=8be113728d431b1528630f46b4eb8ad6b7c1bd5f5214e373ed4372604217469a", "Downloading Loader Theme", "l11.zip", 0x100000}, // 1MB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Floader%2Fl12.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T18%3A15%3A28.855Z&signature=110a3beae73a880d3abe20e0befc2e29f4864beaeee6cae25a82a1f480672c34", "Downloading Loader Theme", "l12.zip", 0x100000}, // 1MB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Floader%2Fl13.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T18%3A15%3A43.565Z&signature=513947347b850958c35b51ff46bd6c387b96c8d856a79d1893637c2cd8259aea", "Downloading Loader Theme", "l13.zip", 0x100000}, // 1MB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Floader%2Fl14.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T18%3A15%3A57.275Z&signature=37f97fea36755f34adb962f92363669c369f828162758066a276cb67717eff4e", "Downloading Loader Theme", "l14.zip", 0x100000}, // 1MB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Floader%2Fl15.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T18%3A16%3A16.252Z&signature=02036a9df285b5a34e4bfbe39d39030f5ddadf8e0f98832149cd3d17871e49a4", "Downloading Loader Theme", "l15.zip", 0x100000}, // 1MB
    //NinBlack.png
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2FNinBlack.png?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T04%3A45%3A06.780Z&signature=c71f3ff7af984f98f2008a65b571ae75eecab2646cafaf62fbc804ee3ebe5db4", "Downloading Black Screen for Game Boot", "NinBlack.png", 0x1000}, // 4KB
    //Preview.zip
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2FPreview.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T21%3A53%3A18.058Z&signature=7d75562d2865182f887e252e0c04a2c0630af0da466d5ded898654393e7887c0", "Downloading Preview Files", "Preview.zip", 0x500000} // 5MB
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
    } else {
		PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*line, "Restart Nintendont to complete update");
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

void DeleteTextColorCustoms() {
    char path[MAXPATHLEN];
    bool exists = strlen(launch_dir);
    int i;
    //just delete every possible textcolor file
    for (i = 0; i < 1; i++){
		if (i != 0){
			memset(path, 0, sizeof(path));
		}
		switch (i){
			case 0:
				snprintf(path, sizeof(path), "%stextcolor.ini", exists? launch_dir : "/apps/Nintendont/");
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
		"A/"
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
    	MP3Player_PlayBuffer(sample_mp3, sample_mp3_size, NULL);
        if (redraw) {
            /*
            * Print the Menu
            */
        
            PrintInfo();
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*3, "Boot Loader Download Themes");
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
    	MP3Player_PlayBuffer(sample_mp3, sample_mp3_size, NULL);
        if (redraw) {
            /*
            * Print the Menu
            */
        
            PrintInfo();
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*3, "Theme Menu");
            PrintButtonActions("Go Back", "Select", "Go Back", NULL);
        
            // Now the entries
        
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*5, "Boot Loader Download Themes");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*6, "Background Menu Downloader 2018");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*7, "Background Menu Downloader 2021 (coming soon)");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*8, "Boot Loader Set Black Screen");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*9, "Text Color Custom Menu");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*10, "Download Preview Pack");
            if (previewDownloaded) {
                if (previewState) {
                    PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*12, "Preview Pack: Enabled");
                } else {
                    PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*12, "Preview Pack: Disabled");
                }
            }
            
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*13, "Restore Default Theme");
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
                    BackgroundMenuDownloader2018();
                    break;
                case 2:
                    //BackgroundMenuDownloader2021();
                    break;
                case 3:
                    DeleteThemes();
                    Download(DOWNLOAD_NINBLACK);
                    ClearScreen();
                    break;
				case 4:
                    TextColorCustomMenu();
					break;
                case 5:
                    if (Download(PREVIEW) == 1) {
                        previewDownloaded = true;
                        previewState = PreviewEnabled();
                    }
                    
                    ClearScreen();
                    break;
                case 7:
                    previewState = SetPreview(previewState);
                    break;
                case 8:
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
            if (selected == 6 && previewDownloaded) {
                selected++;
            } else if (selected == 6 && !previewDownloaded) {
                selected += 2;
            } else if (selected > 8) {
                selected = 0;
            }
            
            redraw = true;
        } else if (FPAD_Up(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected--;
            if (selected == 6 && previewDownloaded) {
                selected--;
            } else if (selected == 7 && !previewDownloaded) {
                selected -= 2;
            } else if (selected < 0) {
                selected = 8;
            }
            
            redraw = true;
        }
    } //while
    
    exit_while:
    return;
}
