/*

Nintendont (Loader) - Playing Gamecubes in Wii mode on a Wii U

Copyright (C) 2017  send0r

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
#include <sys/param.h>
#include <asndlib.h>
#include <mp3player.h>

#include "global.h"

#include "ff_utf8.h"

#include "bootanim_audio_mp3.h"
#include "gc0_jpg.h"
#include "gc1_jpg.h"
#include "gc2_jpg.h"
#include "gc3_jpg.h"
#include "gc4_jpg.h"
#include "gc5_jpg.h"
#include "gc6_jpg.h"
#include "gc7_jpg.h"
#include "gc8_jpg.h"
#include "gc9_jpg.h"
#include "gc10_jpg.h"
#include "gc11_jpg.h"
#include "gc12_jpg.h"
#include "gc13_jpg.h"
#include "gc14_jpg.h"
#include "gc15_jpg.h"
#include "gc16_jpg.h"
#include "gc17_jpg.h"
#include "gc18_jpg.h"
#include "gc19_jpg.h"
#include "gc20_jpg.h"
#include "gc21_jpg.h"
#include "gc22_jpg.h"
#include "gc23_jpg.h"
#include "gc24_jpg.h"
#include "gc25_jpg.h"
#include "gc26_jpg.h"
#include "gc27_jpg.h"
#include "gc28_jpg.h"
#include "gc29_jpg.h"
#include "gc30_jpg.h"
#include "gc31_jpg.h"
#include "gc32_jpg.h"
#include "gc33_jpg.h"
#include "gc34_jpg.h"
#include "gc35_jpg.h"
#include "gc36_jpg.h"
#include "gc37_jpg.h"
#include "gc38_jpg.h"
#include "gc39_jpg.h"
#include "gc40_jpg.h"
#include "gc41_jpg.h"
#include "gc42_jpg.h"
#include "gc43_jpg.h"
#include "gc44_jpg.h"
#include "gc45_jpg.h"
#include "gc46_jpg.h"
#include "gc47_jpg.h"
#include "gc48_jpg.h"
#include "gc49_jpg.h"
#include "gc50_jpg.h"
#include "gc51_jpg.h"
#include "gc52_jpg.h"
#include "gc53_jpg.h"
#include "gc54_jpg.h"
#include "gc55_jpg.h"
#include "gc56_jpg.h"
#include "gc57_jpg.h"
#include "gc58_jpg.h"
#include "gc59_jpg.h"
#include "gc60_jpg.h"
#include "gc61_jpg.h"
#include "gc62_jpg.h"
#include "gc63_jpg.h"
#include "gc64_jpg.h"
#include "gc65_jpg.h"
#include "gc66_jpg.h"
#include "gc67_jpg.h"
#include "gc68_jpg.h"
#include "gc69_jpg.h"
#include "gc70_jpg.h"
#include "gc71_jpg.h"
#include "gc72_jpg.h"
#include "gc73_jpg.h"
#include "gc74_jpg.h"
#include "gc75_jpg.h"
#include "gc76_jpg.h"
#include "gc77_jpg.h"
#include "gc78_jpg.h"
#include "gc79_jpg.h"
#include "gc80_jpg.h"
#include "gc81_jpg.h"
#include "gc82_jpg.h"
#include "gc83_jpg.h"
#include "gc84_jpg.h"
#include "gc85_jpg.h"
#include "gc86_jpg.h"
#include "gc87_jpg.h"
#include "gc88_jpg.h"
#include "gc89_jpg.h"
#include "gc90_jpg.h"
#include "gc91_jpg.h"
#include "gc92_jpg.h"
#include "gc93_jpg.h"
#include "gc94_jpg.h"
#include "gc95_jpg.h"
#include "gc96_jpg.h"
#include "gc97_jpg.h"
#include "gc98_jpg.h"
#include "gc99_jpg.h"
#include "gc100_jpg.h"
#include "gc101_jpg.h"
#include "gc102_jpg.h"
#include "gc103_jpg.h"
#include "gc104_jpg.h"
#include "gc105_jpg.h"
#include "gc106_jpg.h"
#include "gc107_jpg.h"
#include "gc108_jpg.h"
#include "gc109_jpg.h"
#include "gc110_jpg.h"
#include "gc111_jpg.h"
#include "gc112_jpg.h"
#include "gc113_jpg.h"
#include "gc114_jpg.h"
#include "gc115_jpg.h"
#include "gc116_jpg.h"
#include "gc117_jpg.h"
#include "gc118_jpg.h"
#include "gc119_png.h"
#include "gc120_png.h"
#include "gc121_png.h"
#include "gc122_png.h"
#include "gc123_png.h"
#include "gc124_png.h"
#include "gc125_png.h"
#include "gc126_png.h"
#include "gc127_png.h"
#include "gc128_png.h"
#include "gc129_png.h"
#include "gc130_png.h"
#include "gc131_png.h"
#include "gc132_png.h"
#include "gc133_png.h"
#include "gc134_png.h"
#include "gc135_png.h"
#include "gc136_png.h"
#include "gc137_png.h"
#include "gc138_png.h"
#include "gc139_png.h"
#include "gc140_png.h"
#include "gc141_png.h"
#include "gc142_png.h"
#include "gc143_png.h"
#include "gc144_png.h"
#include "gc145_png.h"
#include "gc146_png.h"
#include "gc147_png.h"
#include "gc148_png.h"
#include "gc149_png.h"
#include "gc150_png.h"
#include "gc151_png.h"
#include "gc152_jpg.h"

#include "background_png.h"

#include "mysterio.h"

#define SCREEN_HEIGHT	rmode->efbHeight
#define SCREEN_WIDTH	rmode->fbWidth

unsigned int text_color = BLACK;
bool cColor = false;

extern char launch_dir[MAXPATHLEN];

void FadeIn(GRRLIB_texImg *tex){
	int a;
	for (a=0; a<255; a +=5) // Fade image in from black screen
	{
		GRRLIB_DrawImg(0, 0, tex, 0, 1, 1, RGBA(255, 255, 255, a)); // Opacity increases as a does
		GRRLIB_Render();
	}
	GRRLIB_DrawImg(0, 0, tex, 0, 1, 1, RGBA(255, 255, 255, 255));
	GRRLIB_Render();
}

void FadeOut(GRRLIB_texImg *tex){
	int a;
	for (a=255; a>0; a -=5) // Fade out image
	{
		GRRLIB_DrawImg(0, 0, tex, 0, 1, 1, RGBA(255, 255, 255, a)); // Opacity decreases as a does
		GRRLIB_Render();
	}
	GRRLIB_FillScreen(BLACK);
	GRRLIB_Render();
}

void CustomBackground() {
	GRRLIB_texImg *tempbg;
	char path[MAXPATHLEN];
	bool exists = strlen(launch_dir);
	int i;
	for (i = 0; i < 5; i++){
		if (i != 0){
			memset(path, 0, sizeof(path));
		}
		switch (i){	//set the different paths before trying to load
			case 0:
				snprintf(path, sizeof(path), "%sNinLegacy.png", exists? launch_dir : "/apps/Nintendont/");
				text_color = WHITE;
                cColor = true;
				break;
			case 1:
				snprintf(path, sizeof(path), "%sNinBack.png", exists? launch_dir : "/apps/Nintendont/");
				text_color = BLACK;
                cColor = true;
				break;
			case 2:
				snprintf(path, sizeof(path), "%sbackground.png", exists? launch_dir : "/apps/Nintendont/");
				break;
			case 3:
				snprintf(path, sizeof(path), "%sbackground.jpg", exists? launch_dir : "/apps/Nintendont/");
				break;
            default:
                if (background != NULL) {
                    GRRLIB_FreeTexture(background);
                }
                
                background = GRRLIB_LoadTexturePNG(background_png);
                break;
		}
		
		//load background if file exists
		if (i != 4) {
			tempbg = GRRLIB_LoadTextureFromFile(path);
        }
		
        if (tempbg != NULL){
			GRRLIB_FreeTexture(background);
			background = tempbg;
//FIXME:	GRRLIB_FreeTexture(tempbg); freezes nintendont lol
			break;
		}
	}
	
	//try loading text color from file. !!this overwrites existing values of previous background!!
	memset(path, 0, sizeof(path));
	snprintf(path, sizeof(path), "%stextcolor.ini", exists ? launch_dir : "/apps/Nintendont/");
	FIL themefile;
	char readColor[8];
	if (f_open_char(&themefile, path, FA_READ|FA_OPEN_EXISTING) == FR_OK){
		UINT read;
		f_read(&themefile, &readColor, 8, &read);
		f_close(&themefile);
		text_color = (unsigned int)strtoul(readColor, NULL, 16);
		cColor = true;
	}
}

char loaderPath[MAXPATHLEN];
int checkTheme(){
    GRRLIB_texImg *loader;
	extern NIN_CFG *ncfg;
	char gameID3[4];
	bool exists = strlen(launch_dir);
    int mode;
    int colorBackup = text_color;
    
    // get current GAMEID3 and force terminating the string.
    memcpy(gameID3, &(ncfg->GameID), 3);
    gameID3[3] = '\0';
    
	//now create the paths
	int i;
    for (i = 0; i < 8; i++){
		if (i != 0){
			GRRLIB_FreeTexture(loader);
			memset(loaderPath, 0, sizeof(loaderPath));
		}
		switch(i){//attempt to load MasterMod loader themes
			case 0:
				snprintf(loaderPath, sizeof(loaderPath), "%sCustomGameLoader/%sX.png", exists ? launch_dir : "/apps/Nintendont/", gameID3);
				text_color = 0x45587C00; //PURPLE
				mode = 2;	
				break;
			case 1:
				snprintf(loaderPath, sizeof(loaderPath), "%sCustomGameLoader/%s.png", exists ? launch_dir : "/apps/Nintendont/", gameID3);
				text_color = WHITE;
				mode = 1;				
				break;
			case 2:
				snprintf(loaderPath, sizeof(loaderPath), "%sNinBackLight.png", exists ? launch_dir : "/apps/Nintendont/");
				text_color = 0x45587C00; //PURPLE
				mode = 2;
				break;
			case 3:
				snprintf(loaderPath, sizeof(loaderPath), "%sNinBackDark.png", exists ? launch_dir : "/apps/Nintendont/");
				text_color = 0xC0C0C000; //SILVER
				mode = 2;
				break;
			case 4:
				snprintf(loaderPath, sizeof(loaderPath), "%sNinBackBars.png", exists ? launch_dir : "/apps/Nintendont/");
				text_color = WHITE;
				mode = 2;
				break;
			case 5:
				snprintf(loaderPath, sizeof(loaderPath), "%sNinBackNoBar.png", exists ? launch_dir : "/apps/Nintendont/");
				text_color = WHITE;
				mode = 1;
				break;
			case 6:
				snprintf(loaderPath, sizeof(loaderPath), "%sNinBlack.png", exists ? launch_dir : "/apps/Nintendont/");
				text_color = WHITE;
				mode = 1;
				break;
			default:
				text_color = colorBackup;
                mode = 0;
				break;
		}
        
		if (i != 7){ //load new texture
			loader = GRRLIB_LoadTextureFromFile(loaderPath);
		}
		
		if (loader != NULL) {
            GRRLIB_FreeTexture(loader);
			break;
        }
	}
    return mode; //0 = default, 1 = nothing but errors, 2 = loading bar;
}

void loaderTheme(bool autoboot, int mode) {
    UpdateScreen();
    GRRLIB_Screen2Texture(0, 0, screen_buffer, GX_FALSE);
    UpdateScreen();
    if (!autoboot) { //only fade out when using nintendonts GUI
        FadeOut(screen_buffer);
    }
    
    if (mode != 0) {
        GRRLIB_texImg *temp;
        temp = GRRLIB_LoadTextureFromFile(loaderPath);
        if (temp != NULL) {
            //not how safe I wanted it to be. But doing it the safer way corrupts the image.
            //checking for empty file in next if-statement.
            GRRLIB_FreeTexture(temp);
            GRRLIB_FreeTexture(background);
            background = GRRLIB_LoadTextureFromFile(loaderPath);
        }
    }
    
    if (background == NULL) {
        CustomBackground();
    }
    
    FadeIn(background);
    
    ClearScreen();
    GRRLIB_Render();
}

void Animation() {	
	//Boot Animation
	const u8 *gcbootanim[] = {gc0_jpg
				, gc1_jpg
				, gc2_jpg
				, gc3_jpg
				, gc4_jpg
				, gc5_jpg
				, gc6_jpg
				, gc7_jpg
				, gc8_jpg
				, gc9_jpg
				, gc10_jpg
				, gc11_jpg
				, gc12_jpg
				, gc13_jpg
				, gc14_jpg
				, gc15_jpg
				, gc16_jpg
				, gc17_jpg
				, gc18_jpg
				, gc19_jpg
				, gc20_jpg
				, gc21_jpg
				, gc22_jpg
				, gc23_jpg
				, gc24_jpg
				, gc25_jpg
				, gc26_jpg
				, gc27_jpg
				, gc28_jpg
				, gc29_jpg
				, gc30_jpg
				, gc31_jpg
				, gc32_jpg
				, gc33_jpg
				, gc34_jpg
				, gc35_jpg
				, gc36_jpg
				, gc37_jpg
				, gc38_jpg
				, gc39_jpg
				, gc40_jpg
				, gc41_jpg
				, gc42_jpg
				, gc43_jpg
				, gc44_jpg
				, gc45_jpg
				, gc46_jpg
				, gc47_jpg
				, gc48_jpg
				, gc49_jpg
				, gc50_jpg
				, gc51_jpg
				, gc52_jpg
				, gc53_jpg
				, gc54_jpg
				, gc55_jpg
				, gc56_jpg
				, gc57_jpg
				, gc58_jpg
				, gc59_jpg
				, gc60_jpg
				, gc61_jpg
				, gc62_jpg
				, gc63_jpg
				, gc64_jpg
				, gc65_jpg
				, gc66_jpg
				, gc67_jpg
				, gc68_jpg
				, gc69_jpg
				, gc70_jpg
				, gc71_jpg
				, gc72_jpg
				, gc73_jpg
				, gc74_jpg
				, gc75_jpg
				, gc76_jpg
				, gc77_jpg
				, gc78_jpg
				, gc79_jpg
				, gc80_jpg
				, gc81_jpg
				, gc82_jpg
				, gc83_jpg
				, gc84_jpg
				, gc85_jpg
				, gc86_jpg
				, gc87_jpg
				, gc88_jpg
				, gc89_jpg
				, gc90_jpg
				, gc91_jpg
				, gc92_jpg
				, gc93_jpg
				, gc94_jpg
				, gc95_jpg
				, gc96_jpg
				, gc97_jpg
				, gc98_jpg
				, gc99_jpg
				, gc100_jpg
				, gc101_jpg
				, gc102_jpg
				, gc103_jpg
				, gc104_jpg
				, gc105_jpg
				, gc106_jpg
				, gc107_jpg
				, gc108_jpg
				, gc109_jpg
				, gc110_jpg
				, gc111_jpg
				, gc112_jpg
				, gc113_jpg
				, gc114_jpg
				, gc115_jpg
				, gc116_jpg
				, gc117_jpg
				, gc118_jpg
				, gc119_png
				, gc120_png
				, gc121_png
				, gc122_png
				, gc123_png
				, gc124_png
				, gc125_png
				, gc126_png
				, gc127_png
				, gc128_png
				, gc129_png
				, gc130_png
				, gc131_png
				, gc132_png
				, gc133_png
				, gc134_png
				, gc135_png
				, gc136_png
				, gc137_png
				, gc138_png
				, gc139_png
				, gc140_png
				, gc141_png
				, gc142_png
				, gc143_png
				, gc144_png
				, gc145_png
				, gc146_png
				, gc147_png
				, gc148_png
				, gc149_png
				, gc150_png
				, gc151_png
				, gc152_jpg
	};

	GRRLIB_texImg *p1, *nextbootframe = NULL; //FIXME: p1 cannot be freed without crash

	ASND_Init();
	MP3Player_Init();
			
	MP3Player_PlayBuffer(bootanim_audio_mp3, bootanim_audio_mp3_size, NULL);
			
	int c;
	p1 = GRRLIB_LoadTexture(gcbootanim[0]);
	for (c = 0; c < 153; c++) {
		if (nextbootframe != NULL) GRRLIB_FreeTexture(nextbootframe);
		GRRLIB_FillScreen(0x000000FF);
		GRRLIB_DrawImg((SCREEN_WIDTH-p1->w)/2, (SCREEN_HEIGHT-p1->h)/2+20, p1, 0, 1, 1, RGBA(255, 255, 255, 255));
		nextbootframe = p1;
		if (c != 152) {
			p1 = GRRLIB_LoadTexture(gcbootanim[c+1]);
		}
		GRRLIB_Render();
	}
	int sf = 250;
	while (sf > 0) {
		GRRLIB_FillScreen(0x000000FF);
		GRRLIB_DrawImg((SCREEN_WIDTH-nextbootframe->w)/2, (SCREEN_HEIGHT-nextbootframe->h)/2+20, nextbootframe, 0, 1, 1, RGBA(255, 255, 255, sf));
		GRRLIB_Render();
		sf -= 10;
	}
	GRRLIB_Render();
	GRRLIB_FreeTexture(nextbootframe);
}
