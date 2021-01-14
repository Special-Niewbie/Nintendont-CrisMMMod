/*

Nintendont (Loader) - Playing Gamecubes in Wii mode on a Wii U

Copyright (C) 2014  JoostinOnline

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

#include <gccore.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <malloc.h>
#include <network.h>
#include <zlib.h>
#include <ogc/lwp_watchdog.h>
#include <sys/param.h>

#include "exi.h"
#include "FPad.h"
#include "font.h"
#include "global.h"
#include "http.h"
#include "ssl.h"
#include "menu.h"
#include "update.h"
#include "../../common/include/NintendontVersion.h"

#include "ff_utf8.h"
#include "unzip/miniunz.h"
#include "unzip/ioapi.h"

#include "themes.h"
#include "mysterio.h"

extern char launch_dir[MAXPATHLEN];

typedef struct {
	const char *url;
	const char *text;
	const char *filename;
	const u32 max_size;
} downloads_t;

typedef enum {
	DOWNLOAD_MYSTERIO = 0,
	DOWNLOAD_MASTERMOD,
	DOWNLOAD_NINTENDONT,
	DOWNLOAD_TITLES,
	DOWNLOAD_CONTROLLERS,
	DOWNLOAD_GAMECUBE_MD5,
	DOWNLOAD_VERSION
} DOWNLOADS;

static const downloads_t Downloads[] = {
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/boot.dol?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T03%3A27%3A58.283Z&signature=920cf6e46db81df6a9d2a6bc8fe07f7535452814909ce6244759bab3de9b588b", "Downloading CrisMMMod", "boot.dol", 0x700000}, // 7MB
	{"http://send0r.lima-city.de/Nintendont/MasterMod/boot.dol", "Downloading MasterMod", "boot.dol", 0x400000}, // 4MB
	{"https://raw.githubusercontent.com/FIX94/Nintendont/master/loader/loader.dol", "Updating Nintendont", "boot.dol", 0x400000}, // 4MB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/titles.txt?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-14T01%3A40%3A55.387Z&signature=f8456a323aeeedd87d265ea84430f8bc8e5b68baf38214a2a0e7d949356b721e", "Updating titles.txt", "titles.txt", 0x80000}, // 512KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/controllers.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-14T01%3A44%3A59.694Z&signature=54a53322e50e1215f6c3de95b2dc2331c1dd4532d3968d02011e41d146887039", "Updating controllers.zip", "controllers.zip", 0x8000}, // 32KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/gcn_md5.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-14T01%3A42%3A03.787Z&signature=1f7126712abb6162391204e79a1dc8724bf434529c66d0de082f62b6382d9f4f", "Updating gcn_md5.txt", "gcn_md5.zip", 0x20000}, // 128 KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/NintendontVersion.h?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T03%3A21%3A42.887Z&signature=56c355708239e87a8a153776aba516e7eaba62e28991a0f540254688eeb3ffba", "Checking Latest Version", "", 0x400} // 1KB
};

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

static inline bool LatestVersion(int *major, int *minor, int *current_line) {
	unsigned int http_status = 0;
	u8* outbuf = NULL;
	unsigned int filesize;
	int line = *current_line;

	PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*line, Downloads[DOWNLOAD_VERSION].text);
	UpdateScreen();
	line++;
	if(!http_request(Downloads[DOWNLOAD_VERSION].url, Downloads[DOWNLOAD_VERSION].max_size)) {
		PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*line, "Failed to retrieve version");
		UpdateScreen();
		*current_line = line;
		return false;
	}
	

	http_get_result(&http_status, &outbuf, &filesize);
	
	if (((int)*outbuf & 0xF0000000) == 0xF0000000) 
	{
		if (outbuf != NULL) free(outbuf);
		*current_line = line;
		return false;
	}
	sscanf((char*)outbuf, " #ifndef %*s #define %*s #define NIN_MAJOR_VERSION %i #define NIN_MINOR_VERSION %i", major, minor);
	gprintf("major = %i, minor = %i\r\n", *major, *minor);
	if (outbuf != NULL) free(outbuf);
	if ((*major <= NIN_MAJOR_VERSION) && (*minor <= NIN_MINOR_VERSION)) {
		bool still_download = true;
		PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*line, "You already have the latest version");
		line++;
		PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*line, "Download anyway? (A: Yes, B: No)");
		line++;
		UpdateScreen();
		while(true) {
			DrawBuffer();
			FPAD_Update();
			if (FPAD_Cancel(0)) {
				gprintf("Cancelling download\n");
				still_download = false;
				break;
			}
			if (FPAD_OK(0)) {
				gprintf("okay\n");
				break;
			}
			GRRLIB_Render();
		}
		if(!still_download) {
			*current_line = line;
			return false;
		}
	}
	*current_line = line;
	return (*major + *minor) > 0;

}

static s32 Download(DOWNLOADS download_number)  {
	int line = 5;
	int ret, major = 0, minor = 0;
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
	if (download_number == DOWNLOAD_MYSTERIO) {
		ret = LatestVersion(&major, &minor, &line);
		if (!ret) {
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*line, "Download Cancelled");
			UpdateScreen();
			if (outbuf != NULL) free(outbuf);
			net_deinit();
			sleep(4);
			return 0;
		}

		PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*line, "Downloading Nintendont v%i.%i", major, minor);
		UpdateScreen();
		line++;
	}
	
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
	if (download_number == DOWNLOAD_CONTROLLERS) {
		// controllers.zip needs to be decompressed to the
		// active drive, since the kernel uses it.
		ret = UnzipFile("/controllers", false, DOWNLOAD_CONTROLLERS, outbuf, filesize);
		if (ret != 1) {
			strcpy(errmsg, "Unzipping controllers.zip failed.");
		}
	}
	else if (download_number == DOWNLOAD_GAMECUBE_MD5) {
		// gcn_md5.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_GAMECUBE_MD5, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping gcn_md5.zip failed.");
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
		PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*line, "Update Complete");
		UpdateScreen();
		line++;
	}

end:
	if (ret != 1) {
		PrintFormat(DEFAULT_SIZE, MAROON, MENU_POS_X, MENU_POS_Y + 20*line, "Update Error: %s", errmsg);
	} else {
		PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*line, "Restart Nintendont to complete update");
	}
	UpdateScreen();
	if (outbuf != NULL) free(outbuf);
	net_deinit();
	sleep(4);
	return ret;
}

void UpdateNintendont(void) {
	bool redraw = true;
	int selected = 0;
	u64 delay = ticks_to_millisecs(gettime()) + 500;

	while (true) {
		if (redraw) {
			PrintInfo();
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*3, "Update Menu");
			PrintButtonActions("Go Back", "Select", NULL, NULL);

			// Update menu.
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*5, "Update CrisMMMod");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*6, "Download MasterMod (discontinued)");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*7, "Download official Nintendont");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*8, "Download titles.txt");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*9, "Download controllers.zip");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*10, "Download gcn_md5.txt");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*12, "Theme Menu");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*(5+selected), ARROW_RIGHT);
			redraw = false;

			// Render the screen here to prevent a blank frame
			// when returning from Download().
			UpdateScreen();
			ClearScreen();
		}

		VIDEO_WaitVSync();
		FPAD_Update();
		if (delay > ticks_to_millisecs(gettime())) continue;

		if (FPAD_OK(0)) {
			if (selected <= DOWNLOAD_GAMECUBE_MD5) {
				Download(selected);
				ClearScreen();
			} else {
				ThemeMenu();
			}
			
			redraw = true;
		} else if (FPAD_Start(0)) {
			break;
		} else if (FPAD_Down(1)) {
			delay = ticks_to_millisecs(gettime()) + 150;
			selected++;
            if (selected == 6) {
                selected++;
            } else if (selected > 7) {
                selected = 0;
            }
            
			redraw = true;
		} else if (FPAD_Up(1)) {
			delay = ticks_to_millisecs(gettime()) + 150;
			selected--;
            if (selected == 6) {
                selected--;
            } else if (selected < 0) {
                selected = 7;
            }
            
			redraw = true;
		}
	}

	return;
}
