/*

Nintendont (Loader) - Playing Gamecubes in Wii mode on a Wii U

Copyright (C) 2021  CrisMod

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

#include "mysterio.h"
#include "themes.h"
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
	//Blue Groups Menu
	DOWNLOAD_BLUE = 0,
	DOWNLOAD_CORNFLOWERBLUE,
	DOWNLOAD_DARKBLUE,
	DOWNLOAD_DEEPSKYBLUE,
	DOWNLOAD_DODGERBLUE,
	DOWNLOAD_LIGHTBLUE,
	DOWNLOAD_LIGHTSKYBLUE,
	DOWNLOAD_LIGHTSTEELBLUE,
	DOWNLOAD_MEDIUMBLUE,
	DOWNLOAD_MIDNIGHTBLUE,
	DOWNLOAD_NAVY,
	DOWNLOAD_POWDERBLUE,
	DOWNLOAD_ROYALBLUE,
	DOWNLOAD_SKYBLUE,
	DOWNLOAD_STEELBLUE,
	//Brown Groups Menu
	DOWNLOAD_BISQUE,
	DOWNLOAD_BLANCHEDALMOND,
	DOWNLOAD_BROWN,
	DOWNLOAD_BURLYWOOD,
	DOWNLOAD_CHOCOLATE,
	DOWNLOAD_CORNSILK,
	DOWNLOAD_DARKGOLDENROD,
	DOWNLOAD_GOLDENROD,
	DOWNLOAD_MAROON,
	DOWNLOAD_NAVAJOWHITE,
	DOWNLOAD_PERU,
	DOWNLOAD_ROSYBROWN,
	DOWNLOAD_SADDLEBROWN,
	DOWNLOAD_SANDYBROWN,
	DOWNLOAD_SIENNA,
	DOWNLOAD_TAN,
	DOWNLOAD_WHEAT,
	//Cyan Groups Menu
	DOWNLOAD_AQUA,
	DOWNLOAD_AQUAMARINE,
	DOWNLOAD_CADETBLUE,
	DOWNLOAD_CYAN,
	DOWNLOAD_DARKCYAN,
	DOWNLOAD_DARKTURQUOISE,
	DOWNLOAD_LIGHTCYAN,
	DOWNLOAD_LIGHTSEAGREEN,
	DOWNLOAD_MEDIUMTURQUOISE,
	DOWNLOAD_PALETURQUOISE,
	DOWNLOAD_TEAL,
	DOWNLOAD_TURQUOISE,
	//Green Groups Menu
	DOWNLOAD_DARKGREEN,
	DOWNLOAD_DARKOLIVEGREEN,
	DOWNLOAD_DARKSEAGREEN,
	DOWNLOAD_FORESTGREEN,
	DOWNLOAD_GREEN,
	DOWNLOAD_GREENYELLOW,
	DOWNLOAD_LAWNGREEN,
	DOWNLOAD_LIGHTGREEN,
	DOWNLOAD_LIME,
	DOWNLOAD_LIMEGREEN,
	DOWNLOAD_MEDIUMAQUAMARINE,
	DOWNLOAD_MEDIUMSEAGREEN,
	DOWNLOAD_MEDIUMSPRINGGREEN,
	DOWNLOAD_OLIVE,
	DOWNLOAD_OLIVEDRAB,
	DOWNLOAD_PALEGREEN,
	DOWNLOAD_SEAGREEN,
	DOWNLOAD_SPRINGGREEN,
	//Grey Groups Menu
	DOWNLOAD_BLACK,
	DOWNLOAD_DARKGRAY,
	DOWNLOAD_DARKSLATEGRAY,
	DOWNLOAD_DIMGRAY,
	DOWNLOAD_GAINSBORO,
	DOWNLOAD_GRAY,
	DOWNLOAD_LIGHTGRAY,
	DOWNLOAD_LIGHTSLATEGRAY,
	DOWNLOAD_SILVER,
	DOWNLOAD_SLATEGRAY,
	//Orange Groups Menu
	DOWNLOAD_CORAL,
	DOWNLOAD_DARKORANGE,
	DOWNLOAD_ORANGE,
	DOWNLOAD_ORANGERED,
	DOWNLOAD_TOMATO,
	//Pink Groups Menu
	DOWNLOAD_DEEPPINK,
	DOWNLOAD_HOTPINK,
	DOWNLOAD_LIGHTPINK,
	DOWNLOAD_MEDIUMVIOLETRED,
	DOWNLOAD_PALEVIOLETRED,
	DOWNLOAD_PINK,
	//Purple Groups Menu
	DOWNLOAD_DARKORCHID,
	DOWNLOAD_DARKSLATEBLUE,
	DOWNLOAD_DARKVIOLET,
	DOWNLOAD_FUCHSIA,
	DOWNLOAD_INDIGO,
	DOWNLOAD_LAVENDER,
	DOWNLOAD_MEDIUMORCHID,
	DOWNLOAD_MEDIUMPURPLE,
	DOWNLOAD_MEDIUMSLATEBLUE,
	DOWNLOAD_ORCHID,
	DOWNLOAD_PLUM,
	DOWNLOAD_PURPLE,
	DOWNLOAD_SLATEBLUE,
	DOWNLOAD_THISTLE,
	DOWNLOAD_VIOLET,
	DOWNLOAD_VIOLETBLUE,
	//Red Groups Menu
	DOWNLOAD_CRIMSON,
	DOWNLOAD_DARKRED,
	DOWNLOAD_DARKSALMON,
	DOWNLOAD_FIREBRICK,
	DOWNLOAD_INDIANRED,
	DOWNLOAD_LIGHTCORAL,
	DOWNLOAD_LIGHTSALMON,
	DOWNLOAD_RED,
	DOWNLOAD_SALMON,
	//White Groups Menu
	DOWNLOAD_ALICEBLUE,
	DOWNLOAD_ANTIQUEWHITE,
	DOWNLOAD_AZURE,
	DOWNLOAD_BEIGE,
	DOWNLOAD_FLORALWHITE,
	DOWNLOAD_GHOSTWHITE,
	DOWNLOAD_HONEYDEW,
	DOWNLOAD_IVORY,
	DOWNLOAD_LAVENDERBLUSH,
	DOWNLOAD_LINEN,
	DOWNLOAD_MINTCREAM,
	DOWNLOAD_MISTYROSE,
	DOWNLOAD_OLDLACE,
	DOWNLOAD_SEASHELL,
	DOWNLOAD_SNOW,
	DOWNLOAD_WHITE,
	DOWNLOAD_WHITESMOKE,
	//Yellow Groups Menu
	DOWNLOAD_DARKKHAKI,
	DOWNLOAD_GOLD,
	DOWNLOAD_KHAKI,
	DOWNLOAD_LEMONCHIFFON,
	DOWNLOAD_LIGHTGOLDENRODYELLOW,
	DOWNLOAD_LIGHTYELLOW,
	DOWNLOAD_MOCCASIN,
	DOWNLOAD_PALEGOLDENROD,
	DOWNLOAD_PAPAYAWHIP,
	DOWNLOAD_PEACHPUFF,
	DOWNLOAD_YELLOW,
	DOWNLOAD_YELLOWGREEN,
	//Preview.zip
    PREVIEW,
	//NinBlack.png
    DOWNLOAD_NINBLACK,
	//Boot Loader Themes
	LOADER_MRLUIGI
} DOWNLOADS;

static const downloads_t Downloads[] = {
	//Text Color Customs Menu
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBlue_Groups%2FBlue.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-17T13%3A23%3A58.665Z&signature=ae1073e4a4d2b86b194725894431136d6edddce161f8493919fe10547f05a8c1", "Downloading Blue", "Blue.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBlue_Groups%2FCornflowerBlue.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-18T10%3A45%3A27.032Z&signature=21e7b4edb2817cd4504d7edc1bdc14f57265fb21a70cd0ae33b3af48785ce8b5", "Downloading CornflowerBlue", "CornflowerBlue.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBlue_Groups%2FDarkBlue.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-18T11%3A29%3A00.075Z&signature=e05759799ba32b2c1f946f12a9f98a8b78a6b0f5b5e51848252b4e298cffa3f4", "Downloading DarkBlue", "DarkBlue.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBlue_Groups%2FDeepSkyBlue.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-18T13%3A21%3A35.986Z&signature=93cb34e013a649063c998f1106222b89cac90d7489d037f8ab5f0053e319be71", "Downloading DeepSkyBlue", "DeepSkyBlue.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBlue_Groups%2FDodgerBlue.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-18T13%3A48%3A03.840Z&signature=a9e109d3879477f7e9a89595e0d9f83a513d8163a8cd09eae209f039f6493477", "Downloading DodgerBlue", "DodgerBlue.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBlue_Groups%2FLightBlue.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-18T14%3A03%3A49.966Z&signature=fddd3689786ab9762705d298216b557f433c86012b21dd51c864cbc4cca4b542", "Downloading LightBlue", "LightBlue.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBlue_Groups%2FLightSkyBlue.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-18T14%3A19%3A59.411Z&signature=c6cfefb6102f69bc3da7ee5f4845f22bd26f917ab7442be86dbdda9658050ee7", "Downloading LightSkyBlue", "LightSkyBlue.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBlue_Groups%2FLightSteelBlue.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-18T14%3A53%3A20.277Z&signature=2716e62e68330ac58568d16eea13ce840c4126c3a5b9d0233b3b23822acfac1f", "Downloading LightSteelBlue", "LightSteelBlue.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBlue_Groups%2FMediumBlue.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-18T15%3A00%3A24.243Z&signature=eb3e058f09938a6353fdd0df77dc81c5f9539b11b9fe8aea27e2476ab5db682f", "Downloading MediumBlue", "MediumBlue.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBlue_Groups%2FMidnightBlue.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-18T15%3A05%3A09.320Z&signature=164ddceb4554af2bb872916df08d15192d4609fb7fece86be9aa655dc81a4314", "Downloading MidnightBlue", "MidnightBlue.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBlue_Groups%2FNavy.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-18T15%3A09%3A11.078Z&signature=da227583b0801e182b4c32d9153dd57e6bf49424a4e556bcac3dc26b5d7a24f2", "Downloading Navy", "Navy.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBlue_Groups%2FPowderBlue.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-18T15%3A13%3A51.745Z&signature=1425984d5ee2444908622d2259ba9fc0d3c6d62c3a4f8bae2bfb7cb454d586c2", "Downloading PowderBlue", "PowderBlue.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBlue_Groups%2FRoyalBlue.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-18T15%3A19%3A52.160Z&signature=fae8621f35b4c23b2db56e5ac00a8fe38f0b3ec2765301f59cf78320a3af28a6", "Downloading RoyalBlue", "RoyalBlue.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBlue_Groups%2FSkyBlue.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-18T15%3A20%3A08.647Z&signature=1f58e815afdba990287b5291782e882271e127dca693151cb0a5473112755305", "Downloading SkyBlue", "SkyBlue.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBlue_Groups%2FSteelBlue.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-18T15%3A20%3A21.635Z&signature=fd80474cdc052d2a60f8782ce95fd1c1dfc02a17f78ed637443b0d9090d8067c", "Downloading SteelBlue", "SteelBlue.zip", 0x1000}, // 1KB
	//Brown Groups Menu
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBrown_Groups%2FBisque.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T06%3A07%3A15.931Z&signature=a6299d016420802329d0d2ec6a2a3aa351f391969d09cc3b749728ffab274f14", "Downloading Bisque", "Bisque.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBrown_Groups%2FBlanchedAlmond.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T06%3A07%3A43.129Z&signature=052a3ed4aa62a73ff8f203035db7d4ac6c24ca5a7330512c1d6b1fa5fe454d10", "Downloading BlanchedAlmond", "BlanchedAlmond.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBrown_Groups%2FBrown.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T06%3A08%3A00.717Z&signature=c9a4e3c2e67a3d081df960d7ade43e12a776c134625744f0a1b2cd7193c344a0", "Downloading Brown", "Brown.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBrown_Groups%2FBurlywood.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T06%3A08%3A19.993Z&signature=dda7e008c10845648dea2aab3f81eeeb153146416c25ce2c548b23439d39bef5", "Downloading Burlywood", "Burlywood.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBrown_Groups%2FChocolate.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T06%3A08%3A36.466Z&signature=2e3eff2b1b53d7a79b875aa72d99085239f4daaddc132f00b492cef59f5cc1ab", "Downloading Chocolate", "Chocolate.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBrown_Groups%2FCornsilk.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T06%3A09%3A02.503Z&signature=9a33ba5007c8fd4d3c2f074546df782ed95148a811b807d1c996e50927732df7", "Downloading Cornsilk", "Cornsilk.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBrown_Groups%2FDarkGoldenrod.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T06%3A09%3A20.727Z&signature=c06c98077184e8a405f651bdb7b048687aa6771385a376d9e568883743cbbf43", "Downloading DarkGoldenrod", "DarkGoldenrod.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBrown_Groups%2FGoldenrod.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T06%3A09%3A41.454Z&signature=b21c87c4d90a57f3d207015fb808ea56d8fd4a5dd2dfb12dd506a2bec96ec74a", "Downloading Goldenrod", "Goldenrod.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBrown_Groups%2FMaroon.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T06%3A09%3A57.302Z&signature=f23f324fd06542fec415db93951a5eb271b5a0b0b7c203b4fc038a3d4d049112", "Downloading Maroon", "Maroon.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBrown_Groups%2FNavajoWhite.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T06%3A10%3A45.577Z&signature=de41f300f730eec0b4a495690d604f96f1576f8f0d16a3e50aedd85db5246ad7", "Downloading NavajoWhite", "NavajoWhite.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBrown_Groups%2FPeru.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T06%3A11%3A02.573Z&signature=9f9636a41d180a76bdcd1c41a1d7134176c6e713f9b7fb1a367f20915ddd6ec0", "Downloading Peru", "Peru.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBrown_Groups%2FRosyBrown.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T06%3A11%3A21.362Z&signature=9f9c2dc211327d1675b104c166375606b8182c8e69b72c061c3274f7a459dc46", "Downloading RosyBrown", "RosyBrown.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBrown_Groups%2FSaddleBrown.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T06%3A11%3A40.076Z&signature=e4ca5b38d20a006d5a915d6a017b6087df4c1c25b2e383578fec895fdc4f5482", "Downloading SaddleBrown", "SaddleBrown.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBrown_Groups%2FSandyBrown.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T06%3A11%3A59.921Z&signature=d9d270b07ab9d83af48c5f7158dac752324a53d118d032f8a09f52693614cd24", "Downloading SandyBrown", "SandyBrown.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBrown_Groups%2FSienna.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T06%3A12%3A13.277Z&signature=b3af1830535fa90c0a28218e5057259991bf15a6909acb9d1d5f577632ec8dae", "Downloading Sienna", "Sienna.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBrown_Groups%2FTan.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T06%3A12%3A36.074Z&signature=7ecd48a97e9db1e92e24b115729b0d2619172f3aa20624da9b6f614f0d613c6b", "Downloading Tan", "Tan.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FBrown_Groups%2FWheat.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T06%3A12%3A55.936Z&signature=49edeb540caf3bb958c6690b5c711c175de33b038647e687cb9065c4be49bcea", "Downloading Wheat", "Wheat.zip", 0x1000}, // 1KB
	//Cyan Groups Menu
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FCyan_Groups%2FAqua.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T08%3A43%3A57.836Z&signature=1a07434b8edb8b4c5bd9539f0a75a408ea548f531688e8a18ca917a624dd615a", "Downloading Aqua", "Aqua.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FCyan_Groups%2FAquamarine.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T08%3A44%3A16.052Z&signature=225b0db0cb614175dfa453511e1b45cc32a613bc61727ed984d5e20e4b8bc7a2", "Downloading Aquamarine", "Aquamarine.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FCyan_Groups%2FCadetBlue.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T08%3A44%3A30.495Z&signature=fca6cec254c6dade60ba0f385ec5659b35030a686c07f40f6335eaa0139857ab", "Downloading CadetBlue", "CadetBlue.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FCyan_Groups%2FCyan.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T08%3A44%3A51.181Z&signature=e554be15161bd362b34093ec30a4d6db8c8d49b720af039d2dc8ca335f204bee", "Downloading Cyan", "Cyan.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FCyan_Groups%2FDarkCyan.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T08%3A45%3A16.163Z&signature=3f3261904b89371593a62053c80cd38c3bfa6f42b72e63352c6cb7c327e853b3", "Downloading DarkCyan", "DarkCyan.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FCyan_Groups%2FDarkTurquoise.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T08%3A45%3A33.284Z&signature=4148caf503a0c8ec58e6c098c147a7d3784cd36f654415b3cf6e4234cf22f973", "Downloading DarkTurquoise", "DarkTurquoise.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FCyan_Groups%2FLightCyan.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T08%3A45%3A49.405Z&signature=c90afa9b2a04f211813e0f303e4b9d44afd1960105dbf7eed97e8096cfdcde20", "Downloading LightCyan", "LightCyan.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FCyan_Groups%2FLightSeaGreen.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T08%3A46%3A04.920Z&signature=1f430d6a18e0d08735298922f2d9d9674dade42ab25459188608f670c2c2f0de", "Downloading LightSeaGreen", "LightSeaGreen.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FCyan_Groups%2FMediumTurquoise.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T08%3A46%3A21.801Z&signature=09c8372d752430a0067632f94c6c9a2dbfdd6d64fe424fa6c080f6d903dfa4da", "Downloading MediumTurquoise", "MediumTurquoise.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FCyan_Groups%2FPaleTurquoise.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T08%3A46%3A48.174Z&signature=0d4d75a4e37ed98d3d8f6d3c671bbc35ed639221606379434a6476cd17083f27", "Downloading PaleTurquoise", "PaleTurquoise.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FCyan_Groups%2FTeal.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T08%3A46%3A57.079Z&signature=0699b08624334b3e664b86348ee044d8d62b6baf428553f5c28e718b93e054fc", "Downloading Teal", "Teal.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FCyan_Groups%2FTurquoise.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-20T08%3A47%3A10.093Z&signature=10c9ab22729c8a8536178349a2d036ff67dfbdcfd377efbdaf35a884cb122a82", "Downloading Turquoise", "Turquoise.zip", 0x1000}, // 1KB
	//Green Groups Menu
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGreen_Groups%2FDarkGreen.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T04%3A05%3A59.822Z&signature=b9df7b46ef1594ec4228c52c3f1ef499ae4225ad23bafb60b1f99e5f36f77d0c", "Downloading Darkgreen", "Darkgreen.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGreen_Groups%2FDarkOliveGreen.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T04%3A06%3A11.831Z&signature=b8612fe053d7d24809aebdf87e1164922a0686738018af7dcb20c2f8bc846dbd", "Downloading Darkolivegreen", "Darkolivegreen.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGreen_Groups%2FDarkSeaGreen.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T04%3A06%3A24.197Z&signature=81e9b6d6b4c3bbc42b5cc981fcce60a7e9fd6a15a5999c493d7304ca7c51b963", "Downloading Darkseagreen", "Darkseagreen.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGreen_Groups%2FForestGreen.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T04%3A06%3A37.564Z&signature=a299c78bb64be3dc16d7dfb7ba591af12aaf3e640e8b5a9634b3edc7b0873c89", "Downloading Forestgreen", "Forestgreen.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGreen_Groups%2FGreen.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T04%3A06%3A49.232Z&signature=b476c75e79de7415b6fefb3d7aeb167a3e3e1e236622d2a4b96863e945ffd96d", "Downloading Green", "Green.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGreen_Groups%2FGreenYellow.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T04%3A06%3A58.826Z&signature=c28e5918d9360e50dce55ce42a8ef51bbeee0ee72180121a04fc811a4fb7a80d", "Downloading Greenyellow", "Greenyellow.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGreen_Groups%2FLawnGreen.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T04%3A07%3A18.990Z&signature=82901adbd6955ddd3089b06ba6e8bc099c6887d9ddaef3b280483cf88299e22f", "Downloading Lawngreen", "Lawngreen.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGreen_Groups%2FLightGreen.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T04%3A07%3A34.799Z&signature=d8d79f774c32502e7d42880cfddd9f7abe6d5ab70fdfbf5c5e3a451bc37d9c53", "Downloading Lightgreen", "Lightgreen.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGreen_Groups%2FLime.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T04%3A07%3A44.512Z&signature=2c561b76eebfe9167eacf1be63d8bfbec0d1324fe52ad15bb905ff952409d1e3", "Downloading Lime", "Lime.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGreen_Groups%2FLimeGreen.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T04%3A08%3A02.008Z&signature=d97733c806778a25d8f8dd75b04d25dcf3e9512e3b0161d60db790e2f2465fb5", "Downloading Limegreen", "Limegreen.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGreen_Groups%2FMediumAquamarine.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T04%3A08%3A11.252Z&signature=acb4e277de91a2f87e060ed35ca04975f18c3bbe65e91da4597cbcd31f1bc75c", "Downloading Mediumaquamarine", "Mediumaquamarine.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGreen_Groups%2FMediumSeaGreen.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T04%3A08%3A22.001Z&signature=372f9ebb46bdcef3392e031f3df8585152e581d8e2fa4a6231ca7804f26a2706", "Downloading Mediumseagreen", "Mediumseagreen.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGreen_Groups%2FMediumSpringGreen.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T04%3A08%3A49.959Z&signature=6200e71f0986c00390740a436b853c1580615865df20f68cd4bd10c2a2abbf00", "Downloading Mediumspringgreen", "Mediumspringgreen.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGreen_Groups%2FOlive.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T04%3A09%3A01.083Z&signature=414b3ce185c7b627a058ca33d68dba834e1789fdbb26a7aac25dbd557a8054a6", "Downloading Olive", "Olive.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGreen_Groups%2FOliveDrab.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T04%3A09%3A11.032Z&signature=7b077a40fe345c46251c3aafc565f9781015f2e7a3ee4a7e46d170910da67554", "Downloading Olivedrab", "Olivedrab.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGreen_Groups%2FPaleGreen.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T04%3A09%3A24.377Z&signature=f672a1e3dbe0fbc91bc8758d92e8ed10874b6fdba517f219ebec699ebd79010c", "Downloading Palegreen", "Palegreen.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGreen_Groups%2FSeaGreen.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T04%3A09%3A46.375Z&signature=711059e1a94a9239974aa50cc43de6e826b603cfd1d24114a225d3de5588926f", "Downloading Seagreen", "Seagreen.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGreen_Groups%2FSpringGreen.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T04%3A09%3A55.718Z&signature=59eff29e3f367ea8ca5cad9ac754103cf8b57d503e7588095c7c7bcc8b0a1147", "Downloading Springgreen", "Springgreen.zip", 0x1000}, // 1KB
	//Grey Groups Menu
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGrey_Groups%2FBlack.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T05%3A17%3A51.796Z&signature=03dfc77ae690eba82b8e2a023883167a013afaaf90bdfcabd9b0452cbdb99279", "Downloading Black", "Black.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGrey_Groups%2FDarkGray.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T05%3A18%3A04.324Z&signature=a2e2d56a7111ce17fbc5c98efc4f22c35970dd9360dbeff8dbcf8bfef7bc6e50", "Downloading DarkGray", "DarkGray.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGrey_Groups%2FDarkSlateGray.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T05%3A18%3A14.692Z&signature=2810b4e5a8158a309e5effa16498096a16dec344a42b9f96d163ae14d4f240b1", "Downloading DarkSlateGray", "DarkSlateGray.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGrey_Groups%2FDimGray.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T05%3A18%3A32.903Z&signature=36d6ef7f4b9d1aa29eceeb9b9fb808c3168047072299abe3453a2a04dac72a91", "Downloading DimGray", "DimGray.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGrey_Groups%2FGainsboro.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T05%3A18%3A47.029Z&signature=679967994d982e1089f6ce609c76908d548148ec09ee1f61033d64715a291f31", "Downloading Gainsboro", "Gainsboro.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGrey_Groups%2FGray.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T05%3A18%3A58.316Z&signature=3689e21510939c0a581fa5c9e1511a4b407ec742ad18865f257198698d2c15b1", "Downloading Gray", "Gray.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGrey_Groups%2FLightGray.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T05%3A19%3A05.823Z&signature=20a033ad75c5e0944a4c0a665ce621c9a7664000a2f0bfadcd041dfe782a5008", "Downloading LightGray", "LightGray.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGrey_Groups%2FLightSlateGray.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T05%3A19%3A21.170Z&signature=a7515b8173c680e08d64e5741a9a2e304e0fcf3555d000d6b81ada9674815b76", "Downloading LightSlateGray", "LightSlateGray.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGrey_Groups%2FSilver.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T05%3A19%3A32.668Z&signature=d5b545131892917a9d195ff8a36ea8c4da20d826aea3443df6dd51e6973f26b1", "Downloading Silver", "Silver.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FGrey_Groups%2FSlateGray.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T05%3A19%3A40.602Z&signature=4ec7673cad0060b5a68bc30229e114cf77b1f99ae82e9f425aeb4130fc54b087", "Downloading SlateGray", "SlateGray.zip", 0x1000}, // 1KB
	//Pink Groups Menu
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FOrange_Groups%2FCoral.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T05%3A40%3A42.961Z&signature=33e4f3440e4f254550d0a8441a43793042f85383a4bcce583dd6489adedd657b", "Downloading Coral", "Coral.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FOrange_Groups%2FDarkOrange.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T05%3A40%3A50.810Z&signature=390c55b8aff6c74d54927f7f5ae90e53469d6a72674c07c0e3e5eb64c1ee762c", "Downloading DarkOrange", "DarkOrange.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FOrange_Groups%2FOrange.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T05%3A41%3A00.773Z&signature=1cf3102720bf0d3950d9a0bd234b855814a4f5486dd178a5e4478ec0d7f3197b", "Downloading Orange", "Orange.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FOrange_Groups%2FOrangeRed.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T05%3A41%3A09.614Z&signature=42ff10ffd2a0092ed4697bf98966a1195db0d6f015acca381c3674cff83fc78e", "Downloading OrangeRed", "OrangeRed.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FOrange_Groups%2FTomato.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T05%3A41%3A17.785Z&signature=52be402c74f8dbb18359bceb4fa88f25ef5105564c0409ae9fd34958d157c4a4", "Downloading Tomato", "Tomato.zip", 0x1000}, // 1KB
	//Orange Groups Menu
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FPink_Groups%2FDeepPink.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T06%3A05%3A53.487Z&signature=06a42efe7baa9c52ae62dd05f86abefabdad5c2526d62475e437e436d47932fe", "Downloading DeepPink", "DeepPink.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FPink_Groups%2FHotPink.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T06%3A06%3A10.281Z&signature=0528a5ff253c36a59ad102ee1f44531922b385dbb3116743fffb8d459b1acc4b", "Downloading HotPink", "HotPink.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FPink_Groups%2FLightPink.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T06%3A06%3A20.505Z&signature=d756585235963c5a1f6b7774b4e8b8246a13bc3394c2bbe4e91b529d6297bf54", "Downloading LightPink", "LightPink.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FPink_Groups%2FMediumVioletRed.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T06%3A06%3A45.435Z&signature=e0aed3e424d588ddb1d5c278c5189ce3465fff0cd61eba31c1bb24561c9e32bb", "Downloading MediumVioletRed", "MediumVioletRed.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FPink_Groups%2FPaleVioletRed.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T06%3A06%3A59.769Z&signature=df5aaa45d6f19e4ad35c960bdf95c2254aa9e778b3d24368789707d0707cb5f9", "Downloading PaleVioletRed", "PaleVioletRed.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FPink_Groups%2FPink.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T06%3A07%3A11.281Z&signature=617103d01200d3192b5c881b95eeaa698e62b133d8c7cf6e7aa82979a7a29bac", "Downloading Pink", "Pink.zip", 0x1000}, // 1KB
	//Purple Groups Menu
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FPurple_Groups%2FDarkOrchid.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T08%3A25%3A39.729Z&signature=b639b1071efa3be78249d1c2d1e7419c5598aa79ad6972d0a42f2ea7ca82b614", "Downloading DarkOrchid", "DarkOrchid.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FPurple_Groups%2FDarkSlateBlue.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T08%3A26%3A14.670Z&signature=d8a9a7dd5254a60111a08cb2adb1513daa901719cfe21140306467151a7b9c6f", "Downloading DarkSlateBlue", "DarkSlateBlue.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FPurple_Groups%2FDarkViolet.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T08%3A26%3A30.747Z&signature=21e823faa16d3873ea3244b3b7826e296e7b341d8c8eed28ce8fc7d475b68dcd", "Downloading DarkViolet", "DarkViolet.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FPurple_Groups%2FFuchsia.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T08%3A26%3A41.652Z&signature=7d4c011c742333207ebda9fc9d2ef2b0476df4fa848a6f034832abd57bf1b984", "Downloading Fuchsia", "Fuchsia.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FPurple_Groups%2FIndigo.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T08%3A26%3A55.166Z&signature=a8158ae2b9441ddc30c113dd1ebc1eb6e1aa018eb4ca2fb692d6868a9de85d38", "Downloading Indigo", "Indigo.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FPurple_Groups%2FLavender.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T08%3A27%3A06.386Z&signature=1fa733db7b210307be914acfc311c427b25952267d8be684f259646e8390078c", "Downloading Lavender", "Lavender.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FPurple_Groups%2FMediumOrchid.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T08%3A27%3A18.593Z&signature=b44f04b546d89b516d461624dc735a02da4be5795111de364d142cdd6d61599e", "Downloading MediumOrchid", "MediumOrchid.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FPurple_Groups%2FMediumPurple.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T08%3A27%3A30.162Z&signature=b1da64fe10cb57c7908469e7db3a0c1eafa6278f621fc5f6b64dcd36004ed424", "Downloading MediumPurple", "MediumPurple.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FPurple_Groups%2FMediumSlateBlue.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T08%3A27%3A48.980Z&signature=4dee01137b4a0f2bdf34b96c9ff33f2cf4dd6edf2f00a6c936f3a39ae76f34af", "Downloading MediumSlateBlue", "MediumSlateBlue.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FPurple_Groups%2FOrchid.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T08%3A28%3A00.815Z&signature=3893616088a82a66cb1205fd46282415a64748f502a37847358e835338633d5f", "Downloading Orchid", "Orchid.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FPurple_Groups%2FPlum.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T08%3A28%3A12.533Z&signature=66dd699067a1fa0399438d19149b9e0dfaeb820ad6237e1c3467d2870ec0e41d", "Downloading Plum", "Plum.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FPurple_Groups%2FPurple.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T08%3A28%3A26.628Z&signature=06639b986968fad179046f3e457603e671f21fcfb874adbafaac8cb601771f00", "Downloading Purple", "Purple.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FPurple_Groups%2FSlateBlue.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T08%3A28%3A44.946Z&signature=65e910f62c81df730f921c69d2cb6697eaa857bad490865cfad210b1c333c18b", "Downloading SlateBlue", "SlateBlue.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FPurple_Groups%2FThistle.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T08%3A29%3A00.419Z&signature=fb90fd2e21345a9019583b9c24cb4ca0c36b8e0d9b0e2c82a5349ddf05d57a91", "Downloading Thistle", "Thistle.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FPurple_Groups%2FViolet.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T08%3A29%3A12.237Z&signature=ab7506efc134d72fb03bae28c43578f592cbb94055998b0a30207c5bb15369ef", "Downloading Violet", "Violet.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FPurple_Groups%2FVioletBlue.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T08%3A29%3A23.358Z&signature=0b267b7c86f4624239380d87c7973fa058326b4638c05a02251c6212ddf357fb", "Downloading VioletBlue", "VioletBlue.zip", 0x1000}, // 1KB
	//Red Groups Menu
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FRed_Groups%2FCrimson.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T11%3A07%3A34.888Z&signature=da74439e724c179bd3b61dc0aaee3e8d6c6fb671ff04b49043b58724caba2132", "Downloading Crimson", "Crimson.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FRed_Groups%2FDarkRed.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T11%3A12%3A02.534Z&signature=3c333e47250ed409688af81c671edf92bbe598be511af985ce3dbd7274ff1976", "Downloading DarkRed", "DarkRed.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FRed_Groups%2FDarkSalmon.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T11%3A12%3A19.234Z&signature=f4779b92bdaf0462b84a10ac5d1bc9ce67fed81b05030198529234742f1e1ebc", "Downloading DarkSalmon", "DarkSalmon.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FRed_Groups%2FFirebrick.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T11%3A12%3A35.690Z&signature=aa771febe554f813bfd640da4a0f419ab144951cad9b6b91e2be86927a5294ef", "Downloading Firebrick", "Firebrick.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FRed_Groups%2FIndianRed.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T11%3A12%3A54.584Z&signature=4de18ddd1a18ed6d5f33bd115b73f86c6a8e7d7f19ebdf28ba0fe35f95f0a101", "Downloading IndianRed", "IndianRed.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FRed_Groups%2FLightCoral.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T11%3A13%3A11.428Z&signature=872a86af1e960752bd810d9c49a3f0e0bc8f6c3d73ca40bb01fca0bc0aae0328", "Downloading LightCoral", "LightCoral.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FRed_Groups%2FLightSalmon.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T11%3A13%3A30.493Z&signature=cea52ec3db55462d41c0f10cb47ed91a1e8568e361ba1b43b3b011484edc3cba", "Downloading LightSalmon", "LightSalmon.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FRed_Groups%2FRed.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T11%3A13%3A41.742Z&signature=19c8bac1c9f0dfd1fe1ef5ad8b49c0ec544f8c412954fc6e4803b59682f1c595", "Downloading Red", "Red.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FRed_Groups%2FSalmon.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T11%3A13%3A51.973Z&signature=c70e6f714f9cac68980d9213d5825cb20ddd50a61938a1a690f44984983ce68d", "Downloading Salmon", "Salmon.zip", 0x1000}, // 1KB
	//White Groups Menu
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FWhite_Groups%2FAliceBlue.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T13%3A45%3A32.938Z&signature=112004308fb342e47a822bdf9cbca3d2f7c9c9aa739347ec94a349e1ad9beacd", "Downloading AliceBlue", "AliceBlue.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FWhite_Groups%2FAntiqueWhite.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T13%3A45%3A45.397Z&signature=ea7f68eb52cbd436e320f5a1b3fe0a24cf46ec11bf8044ce54c581219910f222", "Downloading AntiqueWhite", "AntiqueWhite.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FWhite_Groups%2FAzure.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T13%3A46%3A00.978Z&signature=220f1a4a8c935625028c63c79bfa44cb5d22d2ceb3083047f466bd9aeb34be24", "Downloading Azure", "Azure.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FWhite_Groups%2FBeige.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T13%3A46%3A16.160Z&signature=153b99b57e5db9f260e68caf2e4c0638b439d8b7a422bc6aef8be77610601ec9", "Downloading Beige", "Beige.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FWhite_Groups%2FFloralWhite.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T13%3A46%3A39.693Z&signature=764f28a683a426445d991d77d02a3ec27b403e4cf9598c1ecabba7d30f446764", "Downloading FloralWhite", "FloralWhite.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FWhite_Groups%2FGhostWhite.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T13%3A46%3A56.874Z&signature=94ac0bc42b741010933a1b2be7f5f4f840af89541b09622dc3aac9fb2eff484e", "Downloading GhostWhite", "GhostWhite.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FWhite_Groups%2FHoneydew.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T13%3A47%3A10.974Z&signature=b0e4cda1bbd65cc7a1a77318d3e6070a57e05ee50b402315a9748ae8488d0ca0", "Downloading Honeydew", "Honeydew.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FWhite_Groups%2FIvory.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T13%3A47%3A25.174Z&signature=df458e9a82f88c2259f9bae05cd0718b5248a86372392c4311542639a32c5013", "Downloading Ivory", "Ivory.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FWhite_Groups%2FLavenderBlush.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T13%3A47%3A37.234Z&signature=525da9f1d8038b59ee6722c8d033025fc74ea1be4f42f67a6243a938c35ede33", "Downloading LavenderBlush", "LavenderBlush.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FWhite_Groups%2FLinen.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T13%3A47%3A56.113Z&signature=ad6969729f031c932c00e61351d7a3d650fee2bd5242b6fa989d0ac99d8f3ef0", "Downloading Linen", "Linen.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FWhite_Groups%2FMintCream.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T13%3A48%3A15.358Z&signature=73a3f47819e51a77624645f2a286d2c1e808187286d9d5c48c1ac22826a94844", "Downloading MintCream", "MintCream.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FWhite_Groups%2FMistyRose.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T13%3A48%3A30.673Z&signature=86bfe2df88296dbd3a9b6967b598f73054b8e7b1838680a63ba78ba723971547", "Downloading MistyRose", "MistyRose.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FWhite_Groups%2FOldLace.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T13%3A48%3A44.278Z&signature=66fc3ebcd30dc95ea26f132fed219d0f6098ee92db8612ec737d340f1a3c43c2", "Downloading OldLace", "OldLace.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FWhite_Groups%2FSeashell.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T13%3A48%3A57.703Z&signature=ac656674d38d0db2dc6332a1f32cdfd05cb031e37fc791feaf3f8aa0dd7cb1b7", "Downloading Seashell", "Seashell.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FWhite_Groups%2FSnow.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T13%3A49%3A08.865Z&signature=3bf28736593d46e2e82faeba98abd8cd0e61eadc9af8b338c32496a15a7ac82d", "Downloading Snow", "Snow.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FWhite_Groups%2FWhite.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T13%3A49%3A19.784Z&signature=95ebe3088a94eacc9b9a4fa2bb205fe2dd1469638c70f87faf563f154553af6b", "Downloading White", "White.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FWhite_Groups%2FWhiteSmoke.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T13%3A49%3A43.824Z&signature=f4d4adbbbfccb18ff0b9f72de100646e2008257291197545248e5cf9bbbff8b0", "Downloading WhiteSmoke", "WhiteSmoke.zip", 0x1000}, // 1KB
	//Yellow Groups Menu
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FYellow_Groups%2FDarkKhaki.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T15%3A01%3A28.693Z&signature=f9d42d823fb80bd4d3b5d006c9b936d39aafb93478b0df2828183692affc741b", "Downloading DarkKhaki", "DarkKhaki.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FYellow_Groups%2FGold.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T15%3A01%3A39.608Z&signature=9504dd28da9cf73322783551a39a168eae08622f1374a28b5b13205335994419", "Downloading Gold", "Gold.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FYellow_Groups%2FKhaki.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T15%3A01%3A49.463Z&signature=0ef9cbee54b08e3f2bd502f0d06b614487a18499c136bd723c87c257643ee66e", "Downloading Khaki", "Khaki.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FYellow_Groups%2FLemonChiffon.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T15%3A01%3A59.772Z&signature=8dccf324374512ba0a2b6e322fdb9e0880a5e4facecb3d6b82f0d7c54c9402e6", "Downloading LemonChiffon", "LemonChiffon.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FYellow_Groups%2FLightGoldenrodYellow.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T15%3A02%3A11.579Z&signature=3ab9b044026637c1a6c7cf21fdf33098114ed1c5c0de125a6b7e81aed5a98a16", "Downloading LightGoldenrodYellow", "LightGoldenrodYellow.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FYellow_Groups%2FLightYellow.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T15%3A02%3A25.427Z&signature=6f940c9ec2ae5bdaba2324a6c4ddc330cfee7052c2d53bc93ffaf37a5c3fd824", "Downloading LightYellow", "LightYellow.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FYellow_Groups%2FMoccasin.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T15%3A02%3A36.887Z&signature=352b4798fe5ee82f14cded41fe32f7d559c6f808723005883c77c77b75c89bef", "Downloading Moccasin", "Moccasin.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FYellow_Groups%2FPaleGoldenrod.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T15%3A03%3A18.990Z&signature=e878d31b4c1f9a7af15df89e58f49e193ff0076fce8f7ce18c3a339aebc02f9c", "Downloading PaleGoldenrod", "PaleGoldenrod.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FYellow_Groups%2FPapayaWhip.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T15%3A03%3A27.711Z&signature=e4f955e2af2db11134031db2e0be0da7c389f0f325245ffbe31b5c99e815dfd6", "Downloading PapayaWhip", "PapayaWhip.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FYellow_Groups%2FPeachPuff.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T15%3A03%3A38.129Z&signature=ce07cbcefc0781cd4416da70353b579b35910b93261aef5a140b307f0aa9f2f0", "Downloading PeachPuff", "PeachPuff.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FYellow_Groups%2FYellow.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T15%3A03%3A52.699Z&signature=4bda2d6d98ac513cf6399a9f59f582e118d8fe49c3b962d03b8bfc57ae495762", "Downloading Yellow", "Yellow.zip", 0x1000}, // 1KB
	{"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/Text_Color_Custom%2FYellow_Groups%2FYellowGreen.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-21T15%3A04%3A06.185Z&signature=efb56c99a6ed734420cfec1567ab83bda5b32bfa64f7504b6462755bb2f7e0f7", "Downloading YellowGreen", "YellowGreen.zip", 0x1000}, // 1KB
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
            UpdateScreen();
            line++;
        } else if (download_number > LOADER_MRLUIGI && download_number < DOWNLOAD_NINBLACK) {
            line++;
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*line, "Applying theme now.");
            UpdateScreen();
            line++;
        }
	}
	else if (download_number == DOWNLOAD_BLUE) {
		// Blue.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_BLUE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Blue.zip failed.");
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
		line++;
		PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*line, "Applying theme now.");
        UpdateScreen();
        line++;
    }			
	else if (download_number == DOWNLOAD_CORNFLOWERBLUE) {
		// CornflowerBlue.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_CORNFLOWERBLUE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping CornflowerBlue.zip failed.");
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
        UpdateScreen();
        line++;
	}
	else if (download_number == DOWNLOAD_DARKBLUE) {
		// DarkBlue.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_DARKBLUE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping DarkBlue.zip failed.");
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
        UpdateScreen();
        line++;
	}
	else if (download_number == DOWNLOAD_DEEPSKYBLUE) {
		// DeepSkyBlue.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_DEEPSKYBLUE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping DeepSkyBlue.zip failed.");
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
        UpdateScreen();
        line++;
	}
	else if (download_number == DOWNLOAD_DODGERBLUE) {
		// DodgerBlue.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_DODGERBLUE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping DodgerBlue.zip failed.");
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
        UpdateScreen();
        line++;
    }
	else if (download_number == DOWNLOAD_LIGHTBLUE) {
		// LightBlue.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_LIGHTBLUE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping LightBlue.zip failed.");
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
        UpdateScreen();
        line++;
	}
	else if (download_number == DOWNLOAD_LIGHTSKYBLUE) {
		// LightSkyBlue.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_LIGHTSKYBLUE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping LightSkyBlue.zip failed.");
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
        UpdateScreen();
        line++;
	}
	else if (download_number == DOWNLOAD_LIGHTSTEELBLUE) {
		// LightSteelBlue.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_LIGHTSTEELBLUE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping LightSteelBlue.zip failed.");
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
        UpdateScreen();
        line++;
	}
	else if (download_number == DOWNLOAD_MEDIUMBLUE) {
		// MediumBlue.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_MEDIUMBLUE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping MediumBlue.zip failed.");
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
        UpdateScreen();
        line++;
	}
	else if (download_number == DOWNLOAD_MIDNIGHTBLUE) {
		// MidnightBlue.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_MIDNIGHTBLUE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping MidnightBlue.zip failed.");
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
        UpdateScreen();
        line++;
	}
	else if (download_number == DOWNLOAD_NAVY) {
		// Navy.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_NAVY, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Navy.zip failed.");
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
        UpdateScreen();
        line++;
	}
	else if (download_number == DOWNLOAD_POWDERBLUE) {
		// PowderBlue.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_POWDERBLUE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping PowderBlue.zip failed.");
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
        UpdateScreen();
        line++;
	}
	else if (download_number == DOWNLOAD_ROYALBLUE) {
		// RoyalBlue.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_ROYALBLUE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping RoyalBlue.zip failed.");
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
        UpdateScreen();
        line++;
	}
	else if (download_number == DOWNLOAD_SKYBLUE) {
		// SkyBlue.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_SKYBLUE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping SkyBlue.zip failed.");
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
		UpdateScreen();
		line++;
	}
	else if (download_number == DOWNLOAD_STEELBLUE) {
		// SteelBlue.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_STEELBLUE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping SteelBlue.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_BISQUE) {
		// Bisque.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_BISQUE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Bisque.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_BLANCHEDALMOND) {
		// BlanchedAlmond.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_BLANCHEDALMOND, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping BlanchedAlmond.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_BROWN) {
		// Brown.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_BROWN, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Brown.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_BURLYWOOD) {
		// Burlywood.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_BURLYWOOD, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Burlywood.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_CHOCOLATE) {
		// Chocolate.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_CHOCOLATE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Chocolate.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_CORNSILK) {
		// Cornsilk.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_CORNSILK, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Cornsilk.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_DARKGOLDENROD) {
		// DarkGoldenrod.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_DARKGOLDENROD, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping DarkGoldenrod.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_GOLDENROD) {
		// Goldenrod.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_GOLDENROD, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Goldenrod.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_MAROON) {
		// Maroon.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_MAROON, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Maroon.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_NAVAJOWHITE) {
		// NavajoWhite.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_NAVAJOWHITE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping NavajoWhite.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_PERU) {
		// Peru.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_PERU, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Peru.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_ROSYBROWN) {
		// RosyBrown.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_ROSYBROWN, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping RosyBrown.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_SADDLEBROWN) {
		// SaddleBrown.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_SADDLEBROWN, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping SaddleBrown.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_SANDYBROWN) {
		// SandyBrown.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_SANDYBROWN, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping SandyBrown.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_SIENNA) {
		// Sienna.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_SIENNA, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Sienna.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_TAN) {
		// Tan.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_TAN, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Tan.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_WHEAT) {
		// Wheat.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_WHEAT, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Wheat.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_AQUA) {
		// Aqua.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_AQUA, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Aqua.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_AQUAMARINE) {
		// Aquamarine.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_AQUAMARINE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Aquamarine.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_CADETBLUE) {
		// CadetBlue.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_CADETBLUE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping CadetBlue.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_CYAN) {
		// Cyan.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_CYAN, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Cyan.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_DARKCYAN) {
		// DarkCyan.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_DARKCYAN, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping DarkCyan.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_DARKTURQUOISE) {
		// DarkTurquoise.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_DARKTURQUOISE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping DarkTurquoise.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_LIGHTCYAN) {
		// LightCyan.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_LIGHTCYAN, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping LightCyan.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_LIGHTSEAGREEN) {
		// LightSeaGreen.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_LIGHTSEAGREEN, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping LightSeaGreen.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_MEDIUMTURQUOISE) {
		// MediumTurquoise.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_MEDIUMTURQUOISE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping MediumTurquoise.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_PALETURQUOISE) {
		// PaleTurquoise.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_PALETURQUOISE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping PaleTurquoise.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_TEAL) {
		// Teal.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_TEAL, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Teal.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_TURQUOISE) {
		// Turquoise.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_TURQUOISE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Turquoise.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_DARKGREEN) {
		// Darkgreen.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_DARKGREEN, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Darkgreen.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_DARKOLIVEGREEN) {
		// Darkolivegreen.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_DARKOLIVEGREEN, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Darkolivegreen.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_DARKSEAGREEN) {
		// Darkseagreen.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_DARKSEAGREEN, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Darkseagreen.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_FORESTGREEN) {
		// Forestgreen.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_FORESTGREEN, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Forestgreen.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_GREEN) {
		// Green.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_GREEN, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Green.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_GREENYELLOW) {
		// Greenyellow.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_GREENYELLOW, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Greenyellow.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_LAWNGREEN) {
		// Lawngreen.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_LAWNGREEN, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Lawngreen.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_LIGHTGREEN) {
		// Lightgreen.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_LIGHTGREEN, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Lightgreen.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_LIME) {
		// Lime.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_LIME, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Lime.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_LIMEGREEN) {
		// Limegreen.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_LIMEGREEN, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Limegreen.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_MEDIUMAQUAMARINE) {
		// Mediumaquamarine.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_MEDIUMAQUAMARINE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Mediumaquamarine.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_MEDIUMSEAGREEN) {
		// Mediumseagreen.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_MEDIUMSEAGREEN, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Mediumseagreen.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_MEDIUMSPRINGGREEN) {
		// Mediumspringgreen.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_MEDIUMSPRINGGREEN, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Mediumspringgreen.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_OLIVE) {
		// Olive.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_OLIVE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Olive.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_OLIVEDRAB) {
		// Olivedrab.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_OLIVEDRAB, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Olivedrab.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_PALEGREEN) {
		// Palegreen.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_PALEGREEN, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Palegreen.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_SEAGREEN) {
		// Seagreen.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_SEAGREEN, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Seagreen.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_SPRINGGREEN) {
		// Springgreen.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_SPRINGGREEN, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Springgreen.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_BLACK) {
		// Black.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_BLACK, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Black.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_DARKGRAY) {
		// DarkGray.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_DARKGRAY, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping DarkGray.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_DARKSLATEGRAY) {
		// DarkSlateGray.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_DARKSLATEGRAY, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping DarkSlateGray.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_DIMGRAY) {
		// DimGray.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_DIMGRAY, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping DimGray.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_GAINSBORO) {
		// Gainsboro.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_GAINSBORO, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Gainsboro.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_LIGHTGRAY) {
		// LightGray.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_LIGHTGRAY, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping LightGray.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_LIGHTSLATEGRAY) {
		// LightSlateGray.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_LIGHTSLATEGRAY, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping LightSlateGray.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_SILVER) {
		// Silver.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_SILVER, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Silver.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_SLATEGRAY) {
		// SlateGray.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_SLATEGRAY, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping SlateGray.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_CORAL) {
		// Coral.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_CORAL, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Coral.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_DARKORANGE) {
		// DarkOrange.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_DARKORANGE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping DarkOrange.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_ORANGE) {
		// Orange.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_ORANGE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Orange.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_ORANGERED) {
		// OrangeRed.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_ORANGERED, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping OrangeRed.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_TOMATO) {
		// Tomato.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_TOMATO, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Tomato.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_DEEPPINK) {
		// DeepPink.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_DEEPPINK, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping DeepPink.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_HOTPINK) {
		// HotPink.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_HOTPINK, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping HotPink.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_LIGHTPINK) {
		// LightPink.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_LIGHTPINK, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping LightPink.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_MEDIUMVIOLETRED) {
		// MediumVioletRed.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_MEDIUMVIOLETRED, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping MediumVioletRed.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_PALEVIOLETRED) {
		// PaleVioletRed.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_PALEVIOLETRED, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping PaleVioletRed.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_PINK) {
		// Pink.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_PINK, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Pink.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_DARKORCHID) {
		// DarkOrchid.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_DARKORCHID, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping DarkOrchid.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_DARKSLATEBLUE) {
		// DarkSlateBlue.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_DARKSLATEBLUE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping DarkSlateBlue.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_DARKVIOLET) {
		// DarkViolet.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_DARKVIOLET, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping DarkViolet.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_FUCHSIA) {
		// Fuchsia.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_FUCHSIA, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Fuchsia.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_INDIGO) {
		// Indigo.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_INDIGO, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Indigo.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_LAVENDER) {
		// Lavender.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_LAVENDER, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Lavender.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_MEDIUMORCHID) {
		// MediumOrchid.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_MEDIUMORCHID, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping MediumOrchid.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_MEDIUMPURPLE) {
		// MediumPurple.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_MEDIUMPURPLE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping MediumPurple.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_MEDIUMSLATEBLUE) {
		// MediumSlateBlue.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_MEDIUMSLATEBLUE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping MediumSlateBlue.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_ORCHID) {
		// Orchid.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_ORCHID, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Orchid.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_PLUM) {
		// Plum.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_PLUM, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Plum.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_PURPLE) {
		// Purple.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_PURPLE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Purple.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_SLATEBLUE) {
		// SlateBlue.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_SLATEBLUE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping SlateBlue.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_THISTLE) {
		// Thistle.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_THISTLE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Thistle.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_VIOLET) {
		// Violet.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_VIOLET, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Violet.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_VIOLETBLUE) {
		// VioletBlue.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_VIOLETBLUE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping VioletBlue.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_CRIMSON) {
		// Crimson.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_CRIMSON, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Crimson.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_DARKRED) {
		// DarkRed.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_DARKRED, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping DarkRed.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_DARKSALMON) {
		// DarkSalmon.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_DARKSALMON, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping DarkSalmon.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_FIREBRICK) {
		// Firebrick.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_FIREBRICK, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Firebrick.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_INDIANRED) {
		// IndianRed.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_INDIANRED, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping IndianRed.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_LIGHTCORAL) {
		// LightCoral.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_LIGHTCORAL, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping LightCoral.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_LIGHTSALMON) {
		// LightSalmon.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_LIGHTSALMON, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping LightSalmon.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_RED) {
		// Red.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_RED, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Red.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_SALMON) {
		// Salmon.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_SALMON, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Salmon.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_ALICEBLUE) {
		// AliceBlue.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_ALICEBLUE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping AliceBlue.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_ALICEBLUE) {
		// AliceBlue.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_ALICEBLUE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping AliceBlue.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_ANTIQUEWHITE) {
		// AntiqueWhite.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_ANTIQUEWHITE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping AntiqueWhite.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_AZURE) {
		// Azure.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_AZURE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Azure.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_BEIGE) {
		// Beige.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_BEIGE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Beige.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_FLORALWHITE) {
		// FloralWhite.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_FLORALWHITE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping FloralWhite.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_GHOSTWHITE) {
		// GhostWhite.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_GHOSTWHITE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping GhostWhite.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_HONEYDEW) {
		// Honeydew.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_HONEYDEW, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Honeydew.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_IVORY) {
		// Ivory.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_IVORY, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Ivory.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_LAVENDERBLUSH) {
		// LavenderBlush.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_LAVENDERBLUSH, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping LavenderBlush.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_LINEN) {
		// Linen.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_LINEN, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Linen.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_MINTCREAM) {
		// MintCream.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_MINTCREAM, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping MintCream.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_MISTYROSE) {
		// MistyRose.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_MISTYROSE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping MistyRose.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_OLDLACE) {
		// OldLace.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_OLDLACE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping OldLace.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_SEASHELL) {
		// Seashell.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_SEASHELL, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Seashell.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_SNOW) {
		// Snow.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_SNOW, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Snow.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_WHITE) {
		// White.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_WHITE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping White.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_WHITESMOKE) {
		// WhiteSmoke.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_WHITESMOKE, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping WhiteSmoke.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_DARKKHAKI) {
		// DarkKhaki.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_DARKKHAKI, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping DarkKhaki.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_GOLD) {
		// Gold.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_GOLD, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Gold.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_KHAKI) {
		// Khaki.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_KHAKI, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Khaki.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_LEMONCHIFFON) {
		// LemonChiffon.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_LEMONCHIFFON, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping LemonChiffon.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_LIGHTGOLDENRODYELLOW) {
		// LightGoldenrodYellow.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_LIGHTGOLDENRODYELLOW, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping LightGoldenrodYellow.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_LIGHTYELLOW) {
		// LightYellow.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_LIGHTYELLOW, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping LightYellow.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_MOCCASIN) {
		// Moccasin.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_MOCCASIN, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Moccasin.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_PALEGOLDENROD) {
		// PaleGoldenrod.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_PALEGOLDENROD, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping PaleGoldenrod.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_PAPAYAWHIP) {
		// PapayaWhip.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_PAPAYAWHIP, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping PapayaWhip.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_PEACHPUFF) {
		// PeachPuff.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_PEACHPUFF, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping PeachPuff.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_YELLOW) {
		// Yellow.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_YELLOW, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping Yellow.zip failed.");
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
        UpdateScreen();
        line++;

	}
	else if (download_number == DOWNLOAD_YELLOWGREEN) {
		// YellowGreen.zip needs to be decompressed to the
		// loader's drive, since it's used by the verifier.

		// Need a directory without a trailing '/'.
		char *dirNoSlash = strdup(dir);
		size_t len = strlen(dirNoSlash);
		if (len > 1 && dirNoSlash[len-1] == '/')
			dirNoSlash[len-1] = 0;

		ret = UnzipFile(dirNoSlash, true, DOWNLOAD_YELLOWGREEN, outbuf, filesize);
		free(dirNoSlash);

		if (ret != 1) {
			strcpy(errmsg, "Unzipping YellowGreen.zip failed.");
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
        UpdateScreen();
        line++;

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

void DeleteThemes();
  
void DeleteBackground();

void DeleteTextColorCustoms();

bool SetPreview(const bool enabled);

bool PreviewEnabled();

bool PreviewTest();

void Preview(const int menuNumber, const int selected);
 
void BlueGroupsMenu(const bool previewState) {
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
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 220, MENU_POS_Y + 20*3, "Blue Groups Menu");
            PrintButtonActions("Go Back", "Download", "Go Back", NULL);
        
            // Now the entries
			
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*5, "Download Blue");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*6, "Download CornflowerBlue");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*7, "Download DarkBlue");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*8, "Download DeepSkyBlue");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*9, "Download DodgerBlue");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*10, "Download LightBlue");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*11, "Download LightSkyBlue");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*12, "Download LightSteelBlue");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*13, "Download MediumBlue");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*14, "Download MidnightBlue");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*15, "Download Navy");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*16, "Download PowderBlue");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*17, "Download RoyalBlue");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*18, "Download SkyBlue");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*19, "Download SteelBlue");
			
			
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 20, MENU_POS_Y + 20*(5+selected), ARROW_RIGHT);
            
        
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
			DeleteTextColorCustoms();
			switch(selected) {
				case 0:
					Download(DOWNLOAD_BLUE);
					ClearScreen();
                    break;
				case 1:
					Download(DOWNLOAD_CORNFLOWERBLUE);
					ClearScreen();
                    break;
				case 2:
					Download(DOWNLOAD_DARKBLUE);
					ClearScreen();
                    break;
				case 3:
					Download(DOWNLOAD_DEEPSKYBLUE);
					ClearScreen();
                    break;
				case 4:
					Download(DOWNLOAD_DODGERBLUE);
					ClearScreen();
                    break;
				case 5:
					Download(DOWNLOAD_LIGHTBLUE);
					ClearScreen();
                    break;
				case 6:
					Download(DOWNLOAD_LIGHTSKYBLUE);
					ClearScreen();
                    break;
				case 7:
					Download(DOWNLOAD_LIGHTSTEELBLUE);
					ClearScreen();
                    break;
				case 8:
					Download(DOWNLOAD_MEDIUMBLUE);
					ClearScreen();
                    break;
				case 9:
					Download(DOWNLOAD_MIDNIGHTBLUE);
					ClearScreen();
                    break;
				case 10:
					Download(DOWNLOAD_NAVY);
					ClearScreen();
                    break;
				case 11:
					Download(DOWNLOAD_POWDERBLUE);
					ClearScreen();
                    break;
				case 12:
					Download(DOWNLOAD_ROYALBLUE);
					ClearScreen();
                    break;
				case 13:
					Download(DOWNLOAD_SKYBLUE);
					ClearScreen();
                    break;
				case 14:
					Download(DOWNLOAD_STEELBLUE);
					ClearScreen();
                    break;	
		    }//switch
			CustomBackground();
			ClearScreen();
            redraw = true;
        } else if (FPAD_Start(0) || FPAD_Cancel(0)) {
            break;
        } else if (FPAD_Down(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected++;
            if (selected > 14) {
                selected = 0;
            }
            
            redraw = true;
        } else if (FPAD_Up(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected--;
            if (selected < 0) {
                selected = 14;
            }
            
            redraw = true;
        }
    } //while
    
    return;
}

void BrownGroupsMenu(const bool previewState) {
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
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 220, MENU_POS_Y + 20*3, "Brown Groups Menu");
            PrintButtonActions("Go Back", "Download", "Go Back", NULL);
        
            // Now the entries
			
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*4, "Download Bisque");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*5, "Download BlanchedAlmond");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*6, "Download Brown");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*7, "Download Burlywood");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*8, "Download Chocolate");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*9, "Download Cornsilk");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*10, "Download DarkGoldenrod");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*11, "Download Goldenrod");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*12, "Download Maroon");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*13, "Download NavajoWhite");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*14, "Download Peru");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*15, "Download RosyBrown");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*16, "Download SaddleBrown");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*17, "Download SandyBrown");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*18, "Download Sienna");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*19, "Download Tan");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*20, "Download Wheat");
			
			
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 20, MENU_POS_Y + 20*(4+selected), ARROW_RIGHT);
            
        
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
			DeleteTextColorCustoms();
			switch(selected) {
				case 0:
					Download(DOWNLOAD_BISQUE);
					ClearScreen();
                    break;
				case 1:
					Download(DOWNLOAD_BLANCHEDALMOND);
					ClearScreen();
                    break;
				case 2:
					Download(DOWNLOAD_BROWN);
					ClearScreen();
                    break;
				case 3:
					Download(DOWNLOAD_BURLYWOOD);
					ClearScreen();
                    break;
				case 4:
					Download(DOWNLOAD_CHOCOLATE);
					ClearScreen();
                    break;
				case 5:
					Download(DOWNLOAD_CORNSILK);
					ClearScreen();
                    break;
				case 6:
					Download(DOWNLOAD_DARKGOLDENROD);
					ClearScreen();
                    break;
				case 7:
					Download(DOWNLOAD_GOLDENROD);
					ClearScreen();
                    break;
				case 8:
					Download(DOWNLOAD_MAROON);
					ClearScreen();
                    break;
				case 9:
					Download(DOWNLOAD_NAVAJOWHITE);
					ClearScreen();
                    break;
				case 10:
					Download(DOWNLOAD_PERU);
					ClearScreen();
                    break;
				case 11:
					Download(DOWNLOAD_ROSYBROWN);
					ClearScreen();
                    break;
				case 12:
					Download(DOWNLOAD_SADDLEBROWN);
					ClearScreen();
                    break;
				case 13:
					Download(DOWNLOAD_SANDYBROWN);
					ClearScreen();
                    break;
				case 14:
					Download(DOWNLOAD_SIENNA);
					ClearScreen();
                    break;	
				case 15:
					Download(DOWNLOAD_TAN);
					ClearScreen();
                    break;
				case 16:
					Download(DOWNLOAD_WHEAT);
					ClearScreen();
                    break;		
		    }//switch
			CustomBackground();
			ClearScreen();
            redraw = true;
        } else if (FPAD_Start(0) || FPAD_Cancel(0)) {
            break;
        } else if (FPAD_Down(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected++;
            if (selected > 16) {
                selected = 0;
            }
            
            redraw = true;
        } else if (FPAD_Up(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected--;
            if (selected < 0) {
                selected = 16;
            }
            
            redraw = true;
        }
    } //while
    
    return;
}

void CyanGroupsMenu(const bool previewState) {
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
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 220, MENU_POS_Y + 20*3, "Cyan Groups Menu");
            PrintButtonActions("Go Back", "Download", "Go Back", NULL);
        
            // Now the entries
			
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*6, "Download Aqua");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*7, "Download Aquamarine");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*8, "Download CadetBlue");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*9, "Download Cyan");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*10, "Download DarkCyan");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*11, "Download DarkTurquoise");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*12, "Download LightCyan");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*13, "Download LightSeaGreen");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*14, "Download MediumTurquoise");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*15, "Download PaleTurquoise");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*16, "Download Teal");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*17, "Download Turquoise");

			
			
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 20, MENU_POS_Y + 20*(6+selected), ARROW_RIGHT);
            
        
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
			DeleteTextColorCustoms();
			switch(selected) {
				case 0:
					Download(DOWNLOAD_AQUA);
					ClearScreen();
                    break;
				case 1:
					Download(DOWNLOAD_AQUAMARINE);
					ClearScreen();
                    break;
				case 2:
					Download(DOWNLOAD_CADETBLUE);
					ClearScreen();
                    break;
				case 3:
					Download(DOWNLOAD_CYAN);
					ClearScreen();
                    break;
				case 4:
					Download(DOWNLOAD_DARKCYAN);
					ClearScreen();
                    break;
				case 5:
					Download(DOWNLOAD_DARKTURQUOISE);
					ClearScreen();
                    break;
				case 6:
					Download(DOWNLOAD_LIGHTCYAN);
					ClearScreen();
                    break;
				case 7:
					Download(DOWNLOAD_LIGHTSEAGREEN);
					ClearScreen();
                    break;
				case 8:
					Download(DOWNLOAD_MEDIUMTURQUOISE);
					ClearScreen();
                    break;
				case 9:
					Download(DOWNLOAD_PALETURQUOISE);
					ClearScreen();
                    break;
				case 10:
					Download(DOWNLOAD_TEAL);
					ClearScreen();
                    break;
				case 11:
					Download(DOWNLOAD_TURQUOISE);
					ClearScreen();
                    break;
		    }//switch
			CustomBackground();
			ClearScreen();
            redraw = true;
        } else if (FPAD_Start(0) || FPAD_Cancel(0)) {
            break;
        } else if (FPAD_Down(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected++;
            if (selected > 11) {
                selected = 0;
            }
            
            redraw = true;
        } else if (FPAD_Up(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected--;
            if (selected < 0) {
                selected = 11;
            }
            
            redraw = true;
        }
    } //while
    
    return;
}

void GreenGroupsMenu(const bool previewState) {
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
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 340, MENU_POS_Y + 20*3, "Green Groups Menu");
            PrintButtonActions("Go Back", "Download", "Go Back", NULL);
        
            // Now the entries
			
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*3, "Download DarkGreen");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*4, "Download DarkOliveGreen");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*5, "Download DarkSeaGreen");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*6, "Download ForestGreen");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*7, "Download Green");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*8, "Download GreenYellow");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*9, "Download LawnGreen");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*10, "Download LightGreen");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*11, "Download Lime");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*12, "Download LimeGreen");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*13, "Download MediumAquamarine");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*14, "Download MediumSeaGreen");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*15, "Download MediumSpringGreen");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*16, "Download Olive");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*17, "Download OliveDrab");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*18, "Download PaleGreen");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*19, "Download SeaGreen");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*20, "Download SpringGreen");
			
			
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 20, MENU_POS_Y + 20*(3+selected), ARROW_RIGHT);
            
        
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
			DeleteTextColorCustoms();
			switch(selected) {
				case 0:
					Download(DOWNLOAD_DARKGREEN);
					ClearScreen();
                    break;
				case 1:
					Download(DOWNLOAD_DARKOLIVEGREEN);
					ClearScreen();
                    break;
				case 2:
					Download(DOWNLOAD_DARKSEAGREEN);
					ClearScreen();
                    break;
				case 3:
					Download(DOWNLOAD_FORESTGREEN);
					ClearScreen();
                    break;
				case 4:
					Download(DOWNLOAD_GREEN);
					ClearScreen();
                    break;
				case 5:
					Download(DOWNLOAD_GREENYELLOW);
					ClearScreen();
                    break;
				case 6:
					Download(DOWNLOAD_LAWNGREEN);
					ClearScreen();
                    break;
				case 7:
					Download(DOWNLOAD_LIGHTGREEN);
					ClearScreen();
                    break;
				case 8:
					Download(DOWNLOAD_LIME);
					ClearScreen();
                    break;
				case 9:
					Download(DOWNLOAD_LIMEGREEN);
					ClearScreen();
                    break;
				case 10:
					Download(DOWNLOAD_MEDIUMAQUAMARINE);
					ClearScreen();
                    break;
				case 11:
					Download(DOWNLOAD_MEDIUMSEAGREEN);
					ClearScreen();
                    break;
				case 12:
					Download(DOWNLOAD_MEDIUMSPRINGGREEN);
					ClearScreen();
                    break;
				case 13:
					Download(DOWNLOAD_OLIVE);
					ClearScreen();
                    break;
				case 14:
					Download(DOWNLOAD_OLIVEDRAB);
					ClearScreen();
                    break;
				case 15:
					Download(DOWNLOAD_PALEGREEN);
					ClearScreen();
                    break;
				case 16:
					Download(DOWNLOAD_SEAGREEN);
					ClearScreen();
                    break;	
				case 17:
					Download(DOWNLOAD_SPRINGGREEN);
					ClearScreen();
                    break;
		    }//switch
			CustomBackground();
			ClearScreen();
            redraw = true;
        } else if (FPAD_Start(0) || FPAD_Cancel(0)) {
            break;
        } else if (FPAD_Down(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected++;
            if (selected > 17) {
                selected = 0;
            }
            
            redraw = true;
        } else if (FPAD_Up(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected--;
            if (selected < 0) {
                selected = 17;
            }
            
            redraw = true;
        }
    } //while
    
    return;
}

void GreyGroupsMenu(const bool previewState) {
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
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 220, MENU_POS_Y + 20*3, "Grey Groups Menu");
            PrintButtonActions("Go Back", "Download", "Go Back", NULL);
        
            // Now the entries
			
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*5, "Download Black");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*6, "Download DarkGray");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*7, "Download DarkSlateGray");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*8, "Download DimGray");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*9, "Download Gainsboro");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*10, "Download Gray");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*11, "Download LightGray");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*12, "Download LightSlateGray");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*13, "Download Silver");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*14, "Download SlateGray");
			
			
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 20, MENU_POS_Y + 20*(5+selected), ARROW_RIGHT);
            
        
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
			DeleteTextColorCustoms();
			switch(selected) {
				case 0:
					Download(DOWNLOAD_BLACK);
					ClearScreen();
                    break;
				case 1:
					Download(DOWNLOAD_DARKGRAY);
					ClearScreen();
                    break;
				case 2:
					Download(DOWNLOAD_DARKSLATEGRAY);
					ClearScreen();
                    break;
				case 3:
					Download(DOWNLOAD_DIMGRAY);
					ClearScreen();
                    break;
				case 4:
					Download(DOWNLOAD_GAINSBORO);
					ClearScreen();
                    break;
				case 5:
					Download(DOWNLOAD_GRAY);
					ClearScreen();
                    break;
				case 6:
					Download(DOWNLOAD_LIGHTGRAY);
					ClearScreen();
                    break;
				case 7:
					Download(DOWNLOAD_LIGHTSLATEGRAY);
					ClearScreen();
                    break;
				case 8:
					Download(DOWNLOAD_SILVER);
					ClearScreen();
                    break;
				case 9:
					Download(DOWNLOAD_SLATEGRAY);
					ClearScreen();
                    break;	
		    }//switch
			CustomBackground();
			ClearScreen();
            redraw = true;
        } else if (FPAD_Start(0) || FPAD_Cancel(0)) {
            break;
        } else if (FPAD_Down(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected++;
            if (selected > 9) {
                selected = 0;
            }
            
            redraw = true;
        } else if (FPAD_Up(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected--;
            if (selected < 0) {
                selected = 9;
            }
            
            redraw = true;
        }
    } //while
    
    return;
}

void OrangeGroupsMenu(const bool previewState) {
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
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 220, MENU_POS_Y + 20*3, "Orange Groups Menu");
            PrintButtonActions("Go Back", "Download", "Go Back", NULL);
        
            // Now the entries
			
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*5, "Download Coral");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*6, "Download DarkOrange");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*7, "Download Orange");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*8, "Download OrangeRed");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*9, "Download Tomato");
			
			
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 20, MENU_POS_Y + 20*(5+selected), ARROW_RIGHT);
            
        
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
			DeleteTextColorCustoms();
			switch(selected) {
				case 0:
					Download(DOWNLOAD_CORAL);
					ClearScreen();
                    break;
				case 1:
					Download(DOWNLOAD_DARKORANGE);
					ClearScreen();
                    break;
				case 2:
					Download(DOWNLOAD_ORANGE);
					ClearScreen();
                    break;
				case 3:
					Download(DOWNLOAD_ORANGERED);
					ClearScreen();
                    break;
				case 4:
					Download(DOWNLOAD_TOMATO);
					ClearScreen();
                    break;
		    }//switch
			CustomBackground();
			ClearScreen();
            redraw = true;
        } else if (FPAD_Start(0) || FPAD_Cancel(0)) {
            break;
        } else if (FPAD_Down(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected++;
            if (selected > 4) {
                selected = 0;
            }
            
            redraw = true;
        } else if (FPAD_Up(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected--;
            if (selected < 0) {
                selected = 4;
            }
            
            redraw = true;
        }
    } //while
    
    return;
}

void PinkGroupsMenu(const bool previewState) {
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
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 220, MENU_POS_Y + 20*3, "Pink Groups Menu");
            PrintButtonActions("Go Back", "Download", "Go Back", NULL);
        
            // Now the entries
			
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*5, "Download DeepPink");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*6, "Download HotPink");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*7, "Download LightPink");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*8, "Download MediumVioletRed");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*9, "Download PaleVioletRed");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*10, "Download Pink");
			
			
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 20, MENU_POS_Y + 20*(5+selected), ARROW_RIGHT);
            
        
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
			DeleteTextColorCustoms();
			switch(selected) {
				case 0:
					Download(DOWNLOAD_DEEPPINK);
					ClearScreen();
                    break;
				case 1:
					Download(DOWNLOAD_HOTPINK);
					ClearScreen();
                    break;
				case 2:
					Download(DOWNLOAD_LIGHTPINK);
					ClearScreen();
                    break;
				case 3:
					Download(DOWNLOAD_MEDIUMVIOLETRED);
					ClearScreen();
                    break;
				case 4:
					Download(DOWNLOAD_PALEVIOLETRED);
					ClearScreen();
                    break;
				case 5:
					Download(DOWNLOAD_PINK);
					ClearScreen();
                    break;
		    }//switch
			CustomBackground();
			ClearScreen();
            redraw = true;
        } else if (FPAD_Start(0) || FPAD_Cancel(0)) {
            break;
        } else if (FPAD_Down(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected++;
            if (selected > 5) {
                selected = 0;
            }
            
            redraw = true;
        } else if (FPAD_Up(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected--;
            if (selected < 0) {
                selected = 5;
            }
            
            redraw = true;
        }
    } //while
    
    return;
}

void PurpleGroupsMenu(const bool previewState) {
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
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 220, MENU_POS_Y + 20*3, "Purple Groups Menu");
            PrintButtonActions("Go Back", "Download", "Go Back", NULL);
        
            // Now the entries
			
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*5, "Download DarkOrchid");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*6, "Download DarkSlateBlue");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*7, "Download DarkViolet");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*8, "Download Fuchsia");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*9, "Download Indigo");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*10, "Download Lavender");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*11, "Download MediumOrchid");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*12, "Download MediumPurple");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*13, "Download MediumSlateBlue");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*14, "Download Orchid");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*15, "Download Plum");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*16, "Download Purple");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*17, "Download SlateBlue");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*18, "Download Thistle");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*19, "Download Violet");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*20, "Download VioletBlue");
			
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 20, MENU_POS_Y + 20*(5+selected), ARROW_RIGHT);
            
        
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
			DeleteTextColorCustoms();
			switch(selected) {
				case 0:
					Download(DOWNLOAD_DARKORCHID);
					ClearScreen();
                    break;
				case 1:
					Download(DOWNLOAD_DARKSLATEBLUE);
					ClearScreen();
                    break;
				case 2:
					Download(DOWNLOAD_DARKVIOLET);
					ClearScreen();
                    break;
				case 3:
					Download(DOWNLOAD_FUCHSIA);
					ClearScreen();
                    break;
				case 4:
					Download(DOWNLOAD_INDIGO);
					ClearScreen();
                    break;
				case 5:
					Download(DOWNLOAD_LAVENDER);
					ClearScreen();
                    break;
				case 6:
					Download(DOWNLOAD_MEDIUMORCHID);
					ClearScreen();
                    break;
				case 7:
					Download(DOWNLOAD_MEDIUMPURPLE);
					ClearScreen();
                    break;
				case 8:
					Download(DOWNLOAD_MEDIUMSLATEBLUE);
					ClearScreen();
                    break;
				case 9:
					Download(DOWNLOAD_ORCHID);
					ClearScreen();
                    break;
				case 10:
					Download(DOWNLOAD_PLUM);
					ClearScreen();
                    break;
				case 11:
					Download(DOWNLOAD_PURPLE);
					ClearScreen();
                    break;
				case 12:
					Download(DOWNLOAD_SLATEBLUE);
					ClearScreen();
                    break;
				case 13:
					Download(DOWNLOAD_THISTLE);
					ClearScreen();
                    break;
				case 14:
					Download(DOWNLOAD_VIOLET);
					ClearScreen();
                    break;
				case 15:
					Download(DOWNLOAD_VIOLETBLUE);
					ClearScreen();
                    break;
		    }//switch
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

void RedGroupsMenu(const bool previewState) {
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
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 220, MENU_POS_Y + 20*3, "Red Groups Menu");
            PrintButtonActions("Go Back", "Download", "Go Back", NULL);
        
            // Now the entries
			
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*5, "Download Crimson");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*6, "Download DarkRed");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*7, "Download DarkSalmon");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*8, "Download Firebrick");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*9, "Download IndianRed");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*10, "Download LightCoral");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*11, "Download LightSalmon");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*12, "Download Red");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*13, "Download Salmon");
			
			
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 20, MENU_POS_Y + 20*(5+selected), ARROW_RIGHT);
            
        
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
			DeleteTextColorCustoms();
			switch(selected) {
				case 0:
					Download(DOWNLOAD_CRIMSON);
					ClearScreen();
                    break;
				case 1:
					Download(DOWNLOAD_DARKRED);
					ClearScreen();
                    break;
				case 2:
					Download(DOWNLOAD_DARKSALMON);
					ClearScreen();
                    break;
				case 3:
					Download(DOWNLOAD_FIREBRICK);
					ClearScreen();
                    break;
				case 4:
					Download(DOWNLOAD_INDIANRED);
					ClearScreen();
                    break;
				case 5:
					Download(DOWNLOAD_LIGHTCORAL);
					ClearScreen();
                    break;
				case 6:
					Download(DOWNLOAD_LIGHTSALMON);
					ClearScreen();
                    break;
				case 7:
					Download(DOWNLOAD_RED);
					ClearScreen();
                    break;
				case 8:
					Download(DOWNLOAD_SALMON);
					ClearScreen();
                    break;
		    }//switch
			CustomBackground();
			ClearScreen();
            redraw = true;
        } else if (FPAD_Start(0) || FPAD_Cancel(0)) {
            break;
        } else if (FPAD_Down(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected++;
            if (selected > 8) {
                selected = 0;
            }
            
            redraw = true;
        } else if (FPAD_Up(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected--;
            if (selected < 0) {
                selected = 8;
            }
            
            redraw = true;
        }
    } //while
    
    return;
}

void WhiteGroupsMenu(const bool previewState) {
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
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 220, MENU_POS_Y + 20*3, "White Groups Menu");
            PrintButtonActions("Go Back", "Download", "Go Back", NULL);
        
            // Now the entries
			
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*4, "Download AliceBlue");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*5, "Download AntiqueWhite");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*6, "Download Azure");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*7, "Download Beige");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*8, "Download FloralWhite");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*9, "Download GhostWhite");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*10, "Download Honeydew");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*11, "Download Ivory");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*12, "Download LavenderBlush");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*13, "Download Linen");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*14, "Download MintCream");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*15, "Download MistyRose");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*16, "Download OldLace");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*17, "Download Seashell");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*18, "Download Snow");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*19, "Download White");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*20, "Download WhiteSmoke");
			
			
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 20, MENU_POS_Y + 20*(4+selected), ARROW_RIGHT);
            
        
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
			DeleteTextColorCustoms();
			switch(selected) {
				case 0:
					Download(DOWNLOAD_ALICEBLUE);
					ClearScreen();
                    break;
				case 1:
					Download(DOWNLOAD_ANTIQUEWHITE);
					ClearScreen();
                    break;
				case 2:
					Download(DOWNLOAD_AZURE);
					ClearScreen();
                    break;
				case 3:
					Download(DOWNLOAD_BEIGE);
					ClearScreen();
                    break;
				case 4:
					Download(DOWNLOAD_FLORALWHITE);
					ClearScreen();
                    break;
				case 5:
					Download(DOWNLOAD_GHOSTWHITE);
					ClearScreen();
                    break;
				case 6:
					Download(DOWNLOAD_HONEYDEW);
					ClearScreen();
                    break;
				case 7:
					Download(DOWNLOAD_IVORY);
					ClearScreen();
                    break;
				case 8:
					Download(DOWNLOAD_LAVENDERBLUSH);
					ClearScreen();
                    break;
				case 9:
					Download(DOWNLOAD_LINEN);
					ClearScreen();
                    break;
				case 10:
					Download(DOWNLOAD_MINTCREAM);
					ClearScreen();
                    break;
				case 11:
					Download(DOWNLOAD_MISTYROSE);
					ClearScreen();
                    break;
				case 12:
					Download(DOWNLOAD_OLDLACE);
					ClearScreen();
                    break;
				case 13:
					Download(DOWNLOAD_SEASHELL);
					ClearScreen();
                    break;
				case 14:
					Download(DOWNLOAD_SNOW);
					ClearScreen();
                    break;
				case 15:
					Download(DOWNLOAD_WHITE);
					ClearScreen();
                    break;
				case 16:
					Download(DOWNLOAD_WHITESMOKE);
					ClearScreen();
                    break;	
		    }//switch
			CustomBackground();
			ClearScreen();
            redraw = true;
        } else if (FPAD_Start(0) || FPAD_Cancel(0)) {
            break;
        } else if (FPAD_Down(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected++;
            if (selected > 16) {
                selected = 0;
            }
            
            redraw = true;
        } else if (FPAD_Up(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected--;
            if (selected < 0) {
                selected = 16;
            }
            
            redraw = true;
        }
    } //while
    
    return;
}

void YellowGroupsMenu(const bool previewState) {
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
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 220, MENU_POS_Y + 20*3, "Yellow Groups Menu");
            PrintButtonActions("Go Back", "Download", "Go Back", NULL);
        
            // Now the entries
			
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*5, "Download DarkKhaki");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*6, "Download Gold");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*7, "Download Khaki");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*8, "Download LemonChiffon");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*9, "Download LightGoldenrodYellow");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*10, "Download LightYellow");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*11, "Download Moccasin");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*12, "Download PaleGoldenrod");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*13, "Download PapayaWhip");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*14, "Download PeachPuff");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*15, "Download Yellow");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*16, "Download YellowGreen");

			
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 20, MENU_POS_Y + 20*(5+selected), ARROW_RIGHT);
            
        
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
			DeleteTextColorCustoms();
			switch(selected) {
				case 0:
					Download(DOWNLOAD_DARKKHAKI);
					ClearScreen();
                    break;
				case 1:
					Download(DOWNLOAD_GOLD);
					ClearScreen();
                    break;
				case 2:
					Download(DOWNLOAD_KHAKI);
					ClearScreen();
                    break;
				case 3:
					Download(DOWNLOAD_LEMONCHIFFON);
					ClearScreen();
                    break;
				case 4:
					Download(DOWNLOAD_LIGHTGOLDENRODYELLOW);
					ClearScreen();
                    break;
				case 5:
					Download(DOWNLOAD_LIGHTYELLOW);
					ClearScreen();
                    break;
				case 6:
					Download(DOWNLOAD_MOCCASIN);
					ClearScreen();
                    break;
				case 7:
					Download(DOWNLOAD_PALEGOLDENROD);
					ClearScreen();
                    break;
				case 8:
					Download(DOWNLOAD_PAPAYAWHIP);
					ClearScreen();
                    break;
				case 9:
					Download(DOWNLOAD_PEACHPUFF);
					ClearScreen();
                    break;
				case 10:
					Download(DOWNLOAD_YELLOW);
					ClearScreen();
                    break;
				case 11:
					Download(DOWNLOAD_YELLOWGREEN);
					ClearScreen();
                    break;	
		    }//switch
			CustomBackground();
			ClearScreen();
            redraw = true;
        } else if (FPAD_Start(0) || FPAD_Cancel(0)) {
            break;
        } else if (FPAD_Down(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected++;
            if (selected > 11) {
                selected = 0;
            }
            
            redraw = true;
        } else if (FPAD_Up(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected--;
            if (selected < 0) {
                selected = 11;
            }
            
            redraw = true;
        }
    } //while
    
    return;
}

extern void RestoreDefault();

void TextColorCustomMenu() {
    // Using code from void UpdateNintendont(void)
    bool previewState = PreviewEnabled();
    bool redraw = true;
    int selected = 0;
    u64 delay = ticks_to_millisecs(gettime()) + 500;
    
    while (true) {
        if (redraw) {
            /*
            * Print the Menu
            */
        
            PrintInfo();
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 195, MENU_POS_Y + 20*3, "Text Color Custom Menu");
            PrintButtonActions("Go Back", "Select", "Go Back", NULL);
        
            // Now the entries
        
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*6, "Blue Groups   Menu");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*7, "Brown Groups  Menu");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*8, "Cyan Groups   Menu");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*9, "Green Groups  Menu");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*10, "Grey Groups   Menu");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*11, "Orange Groups Menu");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*12, "Pink Groups   Menu");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*13, "Purple Groups Menu");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*14, "Red Groups    Menu");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*15, "White Groups  Menu");
			PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*16, "Yellow Groups Menu");

            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*(6+selected), ARROW_RIGHT);
            
            
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
                    BlueGroupsMenu(previewState);
                    break;
                case 1:
                    BrownGroupsMenu(previewState);
                    break;
                case 2:
                    CyanGroupsMenu(previewState);
                    break;
                case 3:
                    GreenGroupsMenu(previewState);
                    break;
                case 4:
                    GreyGroupsMenu(previewState);
                    break;
                case 5:
                    OrangeGroupsMenu(previewState);
                    break;
                case 6:
				    PinkGroupsMenu(previewState);
                    break;
				case 7:
                    PurpleGroupsMenu(previewState);
					break;
                case 8:
				    RedGroupsMenu(previewState);
					break;
                case 9:
                    WhiteGroupsMenu(previewState);
                    break;
                case 10:
                    YellowGroupsMenu(previewState);
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
            if (selected > 10) {
                selected = 0;
            }
            
            redraw = true;
        } else if (FPAD_Up(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected--;
            if (selected < 0) {
                selected = 10;
            }
            
            redraw = true;
        }
    } //while
    
    exit_while:
    return;
}
