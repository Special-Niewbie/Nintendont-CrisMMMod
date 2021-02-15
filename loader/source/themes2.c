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

#include "themes2.h"
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
    //Nintendont Menu Dark Themes
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
    //Nintendont Menu Light Themes
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
    //Nintendont Menu Gradient Themes
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
    //Nintendont Menu Black Themes
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
    //Nintendont Menu Unicolor Themes
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
    PREVIEW,
} DOWNLOADS;

static const downloads_t Downloads[] = {
    //Boot Loader Themes
    {"", "Downloading Loader Theme", "l00.zip", 0x100000}, // 1MB
    {"", "Downloading Loader Theme", "l01.zip", 0x100000}, // 1MB
    {"", "Downloading Loader Theme", "l02.zip", 0x100000}, // 1MB
    {"", "Downloading Loader Theme", "l03.zip", 0x100000}, // 1MB
    {"", "Downloading Loader Theme", "l04.zip", 0x100000}, // 1MB
    {"", "Downloading Loader Theme", "l05.zip", 0x100000}, // 1MB
    {"", "Downloading Loader Theme", "l06.zip", 0x100000}, // 1MB
    {"", "Downloading Loader Theme", "l07.zip", 0x100000}, // 1MB
    {"", "Downloading Loader Theme", "l08.zip", 0x100000}, // 1MB
    {"", "Downloading Loader Theme", "l09.zip", 0x100000}, // 1MB
    {"", "Downloading Loader Theme", "l10.zip", 0x100000}, // 1MB
    {"", "Downloading Loader Theme", "l11.zip", 0x100000}, // 1MB
    {"", "Downloading Loader Theme", "l12.zip", 0x100000}, // 1MB
    {"", "Downloading Loader Theme", "l13.zip", 0x100000}, // 1MB
    {"", "Downloading Loader Theme", "l14.zip", 0x100000}, // 1MB
    {"", "Downloading Loader Theme", "l15.zip", 0x100000}, // 1MB
    //Nintendont Menu Dark Themes
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fdark%2Flegacy00.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A11%3A25.907Z&signature=490646c8b7d0c156aee498bee471891229663ec60be7f5b6b44c36fbc284338a", "Downloading Dark Theme", "legacy00.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fdark%2Flegacy01.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A11%3A44.570Z&signature=c7e51b4ab53fe91d19e58b3b6c115357f4b0b5c5b1cdf272c4f08b038ecbfa01", "Downloading Dark Theme", "legacy01.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fdark%2Flegacy02.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A12%3A52.545Z&signature=a872c1fcb261ccdc9a1e75f1385f606c28a8839c5635757fca41f13d522be5c9", "Downloading Dark Theme", "legacy02.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fdark%2Flegacy03.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A13%3A37.301Z&signature=ed9c46f5ff0255fc7d7286b13897729227626cf97c24245ce359a8121f06e6ad", "Downloading Dark Theme", "legacy03.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fdark%2Flegacy04.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A13%3A55.651Z&signature=693c2849a30970743c67c7d24d44f2414a6bbd9370be2e5f12e95299d5ff78c9", "Downloading Dark Theme", "legacy04.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fdark%2Flegacy05.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A14%3A14.177Z&signature=c69f1eb4a85de1d5a06ae4c7634a052e3a6d493fa82b394eea3447b09f8df9ff", "Downloading Dark Theme", "legacy05.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fdark%2Flegacy06.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A14%3A32.533Z&signature=24724e39b8128be044ab099ae689e6c31efa963551efa189c57dd6638f419fc3", "Downloading Dark Theme", "legacy06.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fdark%2Flegacy07.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A14%3A53.205Z&signature=5ca9971b5ca30404eba14b2a3b049859cb548ecf6bc1b431f520c84328feb137", "Downloading Dark Theme", "legacy07.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fdark%2Flegacy08.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A15%3A08.054Z&signature=7a1a52f0e1315cfb11defa4d6f69a2a3266f689dfc1f6e0ed17009978aef2fae", "Downloading Dark Theme", "legacy08.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fdark%2Flegacy09.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A15%3A27.970Z&signature=691f4e21507f935b1cc8babb708328d5a542b884a7e40b872177ec65bd89f033", "Downloading Dark Theme", "legacy09.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fdark%2Flegacy10.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A15%3A42.897Z&signature=30e054b200d4647414a7d0be2b199e3b61c46e5830de9243d9e71cd08f7ee83d", "Downloading Dark Theme", "legacy10.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fdark%2Flegacy11.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A16%3A02.101Z&signature=b0480f8b26f350f8ef30f7c5452f1dde89882cbe3e1fd6ac657373b2062e0855", "Downloading Dark Theme", "legacy11.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fdark%2Flegacy12.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A16%3A19.687Z&signature=0c116843decc6cd76d30d1cf1bba655402febab561acc7f11ca2fd2bb30715da", "Downloading Dark Theme", "legacy12.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fdark%2Flegacy13.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A16%3A40.485Z&signature=b004523d2de8342f859f6097745effd873a34c9267b0ba9114fd5fe266703ca9", "Downloading Dark Theme", "legacy13.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fdark%2Flegacy14.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A16%3A57.289Z&signature=a95b89eed7160c3a6f503a5b63c9262919f26e152df1b365219b14fb5ac0716d", "Downloading Dark Theme", "legacy14.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fdark%2Flegacy15.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A17%3A15.240Z&signature=78b5c3a1feab4aa417e7f457c90a8804d052aa862f2238792f4612cdf887416e", "Downloading Dark Theme", "legacy15.zip", 0x100000}, // 1MB
    //Nintendont Menu Light Themes
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Flight%2Fback00.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A18%3A12.533Z&signature=f2e8f3457c0a1b8aa19785e14e056f046729f8cb57ec7dd9fa05f4e523feaa89", "Downloading Light Theme", "back00.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Flight%2Fback01.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A18%3A57.571Z&signature=a7b8b617f302546c3611700dd2238bd022caa76dab05533a12c6a66136558584", "Downloading Light Theme", "back01.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Flight%2Fback02.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A19%3A10.439Z&signature=175009eb2fd68bc36917acaa555614777854554016e77742e8f1e303789561bd", "Downloading Light Theme", "back02.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Flight%2Fback03.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A19%3A30.597Z&signature=d11213c0f90691c0ac893f3570b469945a8989d54cd0c1cdd6bae4ce3af1fb9c", "Downloading Light Theme", "back03.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Flight%2Fback04.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A19%3A48.566Z&signature=15e0303fb8334f911442a2cc75a05b7aa90b65d9415274c0073c5b4d2e94d3a7", "Downloading Light Theme", "back04.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Flight%2Fback05.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A20%3A07.560Z&signature=5cf67c52fadbabe321bc0b31954234148ce617bbee1514cff023827c3b4cdbb8", "Downloading Light Theme", "back05.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Flight%2Fback06.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A20%3A22.755Z&signature=f2da909f605554bdc463b1ffaddd6b4aecc76c8baac75ef9ca3677a150c5bab8", "Downloading Light Theme", "back06.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Flight%2Fback07.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A20%3A41.050Z&signature=168b14f647bfdc55f1ebf17fb19608e160803e8517b22bf64874567a9609ccd9", "Downloading Light Theme", "back07.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Flight%2Fback08.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A20%3A56.243Z&signature=2102b37cf566193a89cd30f66fe41276b1b7407a8102ca485afd5461df3900a0", "Downloading Light Theme", "back08.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Flight%2Fback09.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A21%3A11.017Z&signature=f537050a5483e3015ef980f0f0ca56bb80ef3e7f724365acfef13f94859306e4", "Downloading Light Theme", "back09.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Flight%2Fback10.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A21%3A27.098Z&signature=fca486d75d75f2904ce7a66e9a50fd9d2fcd06b6e7e7f38911077aec5169dfb2", "Downloading Light Theme", "back10.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Flight%2Fback11.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A21%3A43.135Z&signature=e21dea9a0c855bc18b12d043ba231788e77924437eebe365a6150205ff8c95b2", "Downloading Light Theme", "back11.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Flight%2Fback12.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A21%3A59.085Z&signature=fc27e1e3314ba9e07aaa2685e14e11cc8ae398b7fe541fdd4afc871d693f893b", "Downloading Light Theme", "back12.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Flight%2Fback13.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A22%3A16.586Z&signature=ffcaac009077bcc6d1957e696dc049a063777f592aedd09cd32a8397f839f8dd", "Downloading Light Theme", "back13.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Flight%2Fback14.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A22%3A37.547Z&signature=ed6786043062cb471fa7c3a4123bc3da6dbacc4cd2b71214364c9139ff703ff4", "Downloading Light Theme", "back14.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Flight%2Fback15.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A22%3A53.768Z&signature=af6f635b8330f6db59aad1bd874570a3d2f13146c3e3e72831d8ea60c4e21e23", "Downloading Light Theme", "back15.zip", 0x100000}, // 1MB
    //Nintendont Menu Gradient Themes
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fgradient%2Fgrad00.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A23%3A47.964Z&signature=19b985b4298a63c4b729ceeb8baff1afb7c88215bed0531dbb12cbada3916707", "Downloading Light Theme", "grad00.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fgradient%2Fgrad01.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A24%3A37.248Z&signature=a3b4b534aed5e8ab9eeb2a2cb39892df4862d02763eb3d95731d7aa2cb34bf35", "Downloading Light Theme", "grad01.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fgradient%2Fgrad02.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A24%3A57.567Z&signature=4133b5328a12fe4f01c70d58d73c6859a7e9f0d72c5e0bcd237548e6c1a4c1f3", "Downloading Light Theme", "grad02.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fgradient%2Fgrad03.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A25%3A16.354Z&signature=b09be7a2d4c1710851204ccdaf985c403619938890e7a4c2ea08ce692d4cf319", "Downloading Light Theme", "grad03.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fgradient%2Fgrad04.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A25%3A35.138Z&signature=04564a2f16b4aec42b16fdba01592575701ddb9ed30a8cf77d97a6375ce3b3e3", "Downloading Light Theme", "grad04.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fgradient%2Fgrad05.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A25%3A51.576Z&signature=5d472ea9949927bba4d1e1217a17db9067351b6694cad0e61e2adea11ba9f75d", "Downloading Light Theme", "grad05.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fgradient%2Fgrad06.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A26%3A09.598Z&signature=df3fa3ed35cb1318a879c0d37a71938c58c13d4d7305962538844e228c1a0b5d", "Downloading Light Theme", "grad06.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fgradient%2Fgrad07.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A26%3A33.070Z&signature=38a300b8bab2229bd043599890a95dd81d663ea423d34095e6400c490bb01b96", "Downloading Light Theme", "grad07.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fgradient%2Fgrad08.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A26%3A47.186Z&signature=e1e81c348e436afce2c617d53e08f0053b016ff58ce15d061797b76850e40dfb", "Downloading Light Theme", "grad08.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fgradient%2Fgrad09.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A27%3A00.351Z&signature=120943e469cc465d0ebce22289e7a0802e891e91ab777d049d51a5b368b517e8", "Downloading Light Theme", "grad09.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fgradient%2Fgrad10.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A27%3A13.256Z&signature=a36e041907cf971fcb186d959ccf08bc1b8bf37f00f4c6cf4816161de38f9a6b", "Downloading Light Theme", "grad10.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fgradient%2Fgrad11.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A27%3A30.073Z&signature=6f432d2591a224a6dd8ba667b597725ff7fbd8b1561a3336f42f5816225aee7d", "Downloading Light Theme", "grad11.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fgradient%2Fgrad12.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A27%3A52.034Z&signature=016f9b3ee97e44456288027256fff85cf65e54d32002cd0a409f74125eb33cc3", "Downloading Light Theme", "grad12.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fgradient%2Fgrad13.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A28%3A12.187Z&signature=05b45af6830d9da4dd91a44855553a52790091c4e409e6bf760e92b65bde54a0", "Downloading Light Theme", "grad13.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fgradient%2Fgrad14.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A28%3A28.116Z&signature=3ec1b629879f6c5a242277e18cf6447179e000fc5989509de89dc9a60d0c3ffe", "Downloading Light Theme", "grad14.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fgradient%2Fgrad15.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A28%3A40.669Z&signature=5e7be5a1c5f7c347647a72f1a2c2bdd7ce2448959488d1edc11cfde8dcfee54b", "Downloading Light Theme", "grad15.zip", 0x100000}, // 1MB
    //Nintendont Menu Black Themes
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fblack%2F000.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A30%3A01.585Z&signature=36ee847b0af56608ee7186b70f42259691b44668197f3b6080958f1c06b8ad7a", "Downloading Black Theme", "000.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fblack%2F001.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A30%3A17.327Z&signature=5b3b7b9ecd0d74c4632bb0aae51151558694995ade0e06dc781163a898101622", "Downloading Black Theme", "001.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fblack%2F002.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A33%3A01.125Z&signature=73c22f11b3c7ca143cf700b8ad309150a7f03cc7d012c4c97017b389c8f3d9ff", "Downloading Black Theme", "002.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fblack%2F003.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A33%3A19.338Z&signature=8c45458d91a4662351488336bdf6f14d5774b26dfee0ec8fd35fbad6af0acac0", "Downloading Black Theme", "003.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fblack%2F004.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A33%3A38.380Z&signature=e69b964a460ac27c2d3bc0ad7cb6876c1e829bb4d49cabf4306da14df199efd5", "Downloading Black Theme", "004.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fblack%2F005.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A33%3A55.567Z&signature=fd726971e7bf561a42ff28211ac35a7ea49cc04aeb0c9b0d61175952bae415af", "Downloading Black Theme", "005.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fblack%2F006.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A34%3A17.935Z&signature=b64e51dcec930258f48969f0eee907b5f3a18a97b97b158b7190ff6cc64b2514", "Downloading Black Theme", "006.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fblack%2F007.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A34%3A33.947Z&signature=3b1f89e836ae05c05275250fddc202887aa99e2b86df2ff8524e69c122f88c17", "Downloading Black Theme", "007.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fblack%2F008.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A34%3A56.955Z&signature=8ef59d3870741e10820252649619b25a02f4a234ad9f6289585c3b6dcc50d9ba", "Downloading Black Theme", "008.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fblack%2F009.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A35%3A14.812Z&signature=8988c1664629fb63f78b5bff2ede571ac2ec153060b8ad7f851e3ac58b360610", "Downloading Black Theme", "009.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fblack%2F010.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A35%3A30.706Z&signature=edda25e43ddc68920481f8b5a7b0a9e8d433105d26b9d115493e37cc896addab", "Downloading Black Theme", "010.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fblack%2F011.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A35%3A45.100Z&signature=3aeae33eb578d15707d83d6a00f1b0b367d7cd99e8911dcfece6041ae045aac6", "Downloading Black Theme", "011.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fblack%2F012.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A36%3A01.733Z&signature=62dfe4f85e37d6c08d344749b5343a8f1513fd6fe649c3f37caa9e9d5a3a2c65", "Downloading Black Theme", "012.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fblack%2F013.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A36%3A14.621Z&signature=76a16801b584495fe8c55d1174fc233e6fd22b375f34dad46f264d05e7405a11", "Downloading Black Theme", "013.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fblack%2F014.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A36%3A29.971Z&signature=fb817cce077ef11ac5b41c62de7d1d87251955ab73d95ec9e7621637e7ca8ae9", "Downloading Black Theme", "014.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Fblack%2F015.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A36%3A45.565Z&signature=2e4cf726c34ecfb3bacffa1f0bc96515eff9578c0f06520ee125fa2a10ba4f51", "Downloading Black Theme", "015.zip", 0x100000}, // 1MB
    //Nintendont Menu Unicolor Themes
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Funicolor%2F00.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A37%3A29.060Z&signature=a8479878e6c5864ee8eb5cb094bcf0708009e52462bc83d25a3b9ba7c3dcb85d", "Downloading Unicolor Theme", "00.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Funicolor%2F01.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A37%3A59.168Z&signature=d3f94ba4a5a59b030fc3c0dcec24c4947637c4b635ca4cab57b66446da4f9851", "Downloading Unicolor Theme", "01.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Funicolor%2F02.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A38%3A17.872Z&signature=6ff6f30b4abe9992bd5c4116c618627968722278a42248be37a5e3a3d6cb2c92", "Downloading Unicolor Theme", "02.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Funicolor%2F03.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A38%3A50.776Z&signature=08d5668643306ac4a79b6b4cbd0f778e7e4c5fd6d47db592995ced04c7b33a63", "Downloading Unicolor Theme", "03.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Funicolor%2F04.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A39%3A57.693Z&signature=30a9c72e6a27c8e4527d4543d7c8080ff6d7f4f97db15642c4ef192f5b4e2235", "Downloading Unicolor Theme", "04.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Funicolor%2F05.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A40%3A18.614Z&signature=55048fa8f8d60bf519b688963911f3910a0403760ca3beda07c167ed87fc554d", "Downloading Unicolor Theme", "05.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Funicolor%2F06.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A40%3A32.434Z&signature=14d8b58a17fa00d9bdaf4d51a7344f9a3c19109fb80fa346a1db1af78df80916", "Downloading Unicolor Theme", "06.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Funicolor%2F07.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A40%3A47.555Z&signature=21e5f6365e9b8b5e26147824913b137cedf6bc165904d57d809d1cf339df6381", "Downloading Unicolor Theme", "07.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Funicolor%2F08.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A41%3A10.054Z&signature=c6d722e97c75dc250a9cccf1a8807894f6b774961cc8bd742e8cbdd649451c51", "Downloading Unicolor Theme", "08.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Funicolor%2F09.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A41%3A22.644Z&signature=e9fd99f432ff216485695b12a022a95aadbbe18be43f1fe1ee3af5026b7a3c20", "Downloading Unicolor Theme", "09.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Funicolor%2F10.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A41%3A38.346Z&signature=872b5c2df9e85c95dd5863a32bc4243532d5be89efe0585d95c23efc2b525593", "Downloading Unicolor Theme", "10.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Funicolor%2F11.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A41%3A53.212Z&signature=8511c383fa98721b5737704485a81642e12eec42e0ee8ff5675f7936cb7c8721", "Downloading Unicolor Theme", "11.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Funicolor%2F12.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A42%3A23.264Z&signature=b62589539f8b642ae594c71017956cca063387b3177d9e7bd568cef874c6ef6f", "Downloading Unicolor Theme", "12.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Funicolor%2F13.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A42%3A37.401Z&signature=55a13980164b76b305665f95f9ccea5174fc03dcfc31d099f920d39defcc90a0", "Downloading Unicolor Theme", "13.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Funicolor%2F14.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A43%3A37.761Z&signature=f79eb7e7bc46e75fb9800207be9a5076fbe6e72799727fa1aaa911c3a2a60ac6", "Downloading Unicolor Theme", "14.zip", 0x100000}, // 1MB
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2Funicolor%2F15.zip?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T19%3A43%3A55.066Z&signature=fdb4e375305e3e616aeb2accc227a2b37868ce893d4b182408e0db15f0e3ec3e", "Downloading Unicolor Theme", "15.zip", 0x100000}, // 1MB
    //NinBlack.png
    {"https://helixteamhub.cloud/api/projects/nintendont-crismmmod/repositories/nintendont-crismmmodweb/commits/webdav/files/themes%2FNinBlack.png?api_signing_key=cc53b4ea-688d-40a0-bd89-99ef16ce604c&expires_at=2121-01-13T04%3A45%3A06.780Z&signature=c71f3ff7af984f98f2008a65b571ae75eecab2646cafaf62fbc804ee3ebe5db4", "Downloading Black Screen for Game Boot", "NinBlack.png", 0x1000} // 4KB
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

void DeleteThemes2() {
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

void DeleteBackground2() {
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

bool SetPreview2(const bool enabled) {
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

bool PreviewEnabled2() {
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

bool PreviewTest2() {
    bool exists = strlen(launch_dir);
    char path[MAXPATHLEN];
    
    snprintf(path, sizeof(path), "%sPreview/dark/00.png",
             exists ? launch_dir : "/apps/Nintendont/");
    // Check if file exists and return true if so
    FIL testPic;
    if (f_open_char(&testPic, path, FA_READ | FA_OPEN_EXISTING) == FR_OK) {
        f_close(&testPic);
        return true;
    }
    
    return false;
}

void Preview2(const int menuNumber, const int selected) {
    bool exists = strlen(launch_dir);
    char path[MAXPATHLEN];
    char folder[6][8] = {
    	"A/",
        "B/",
		"C/",
		"D/",
		"E/",
		"F/"
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

void LoaderMenu2(const bool previewState) {
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
                Preview2(0, selected);
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
            DeleteThemes2();
            
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
        MP3Player_PlayBuffer(sample_mp3, sample_mp3_size, NULL);
        if (redraw) {
            /*
            * Print the Menu
            */
        
            PrintInfo();
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*3, "Background Menu Download Dark Themes");
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
                Preview2(1, selected);
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
            DeleteBackground2();
            DeleteThemes2();
            
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
        MP3Player_PlayBuffer(sample_mp3, sample_mp3_size, NULL);
        if (redraw) {
            /*
            * Print the Menu
            */
        
            PrintInfo();
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*3, "Background Menu Download Light Themes");
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
                Preview2(2, selected);
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
            DeleteBackground2();
            DeleteThemes2();
            
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
        MP3Player_PlayBuffer(sample_mp3, sample_mp3_size, NULL);
        if (redraw) {
            /*
            * Print the Menu
            */
        
            PrintInfo();
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*3, "Background Menu Download Gradient Themes");
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
                Preview2(3, selected);
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
            DeleteBackground2();
            DeleteThemes2();
            
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
        MP3Player_PlayBuffer(sample_mp3, sample_mp3_size, NULL);
        if (redraw) {
            /*
            * Print the Menu
            */
        
            PrintInfo();
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*3, "Background Menu Download Black Themes");
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
                Preview2(4, selected);
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
            DeleteBackground2();
            DeleteThemes2();
            
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
        MP3Player_PlayBuffer(sample_mp3, sample_mp3_size, NULL);
        if (redraw) {
            /*
            * Print the Menu
            */
        
            PrintInfo();
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*3, "Background Menu Download Unicolor Themes");
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
                Preview2(5, selected);
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
            DeleteBackground2();
            DeleteThemes2();
            
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

void RestoreDefault();

void BackgroundMenuDownloader2018() {
    // Using code from void UpdateNintendont(void)
	bool previewState = PreviewEnabled2();
	bool previewDownloaded = PreviewTest2();
    bool redraw = true;
    int selected = 1;
    u64 delay = ticks_to_millisecs(gettime()) + 500;
    
    while (true) {
        MP3Player_PlayBuffer(sample_mp3, sample_mp3_size, NULL);
        if (redraw) {
            /*
            * Print the Menu
            */
        
            PrintInfo();
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X, MENU_POS_Y + 20*3, "Theme Menu 2018");
            PrintButtonActions("Go Back", "Select", "Go Back", NULL);
        
            // Now the entries
        
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*4, " ");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*5, "Background Menu Download Dark Themes");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*6, "Background Menu Download Light Themes");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*7, "Background Menu Download Gradient Themes");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*8, "Background Menu Download Black Themes");
            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*9, "Background Menu Download Unicolor Themes");
            if (previewDownloaded) {
                if (previewState) {
                    PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 50, MENU_POS_Y + 20*12, "Preview Pack: Enabled");
                }
            }

            PrintFormat(DEFAULT_SIZE, text_color, MENU_POS_X + 35, MENU_POS_Y + 20*(4+selected), ARROW_RIGHT);
            
            
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
                LoaderMenu2(previewState);
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
                default:
                    goto exit_while;
            } //switch
            
            redraw = true;
        } else if (FPAD_Start(0) || FPAD_Cancel(0)) {
            break;
        } else if (FPAD_Down(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected++;
            if (selected > 5) {
                selected = 1;
            }
            
            redraw = true;
        } else if (FPAD_Up(1)) {
            delay = ticks_to_millisecs(gettime()) + 150;
            selected--;
            if (selected < 1) {
                selected = 5;
            }
            
            redraw = true;
        }
    } //while
    
    exit_while:
    return;
}
