### Nintendont
A Wii Homebrew Project to play GC Games on Wii and vWii on Wii U

### Features:
* Works on Wii and Wii U (in vWii mode)
* GameCube BIOS Animation.
* Custom Background
* Loader Themes
* Custom Game Loader Themes
* Custom Text Color
* Added Preview Pack for loader and app themes (can be enabled or disabled)
* Want to try other mods? Original Nintendont FIX94 right from the update menu.
* Full-speed loading from a USB device or an SD card.
* Loads 1:1 and shrunken .GCM/.ISO disc images.
* Loads games as extracted files (FST format)
* Loads CISO-format disc images. (uLoader CISO format)
* Memory card emulation
* Play audio via disc audio streaming
* Bluetooth controller support (Classic Controller (Pro), Wii U Pro Controller)
* HID controller support via USB
* Custom button layout when using HID controllers
* Cheat code support
* Changeable configuration of various settings
* Reset/Power off via button combo (R + Z + Start) (R + Z + B + D-Pad Down)
* Advanced video mode patching, force progressive and force 16:9 widescreen
* Auto boot from loader
* Disc switching
* Use the official Nintendo GameCube controller adapter
* BBA Emulation (see [BBA Emulation Readme](BBA_Readme.md))

### Features: (Wii only)
* Play retail discs
* Play backups from writable DVD media (Old Wii only)
* Use real memory cards
* GBA-Link cable
* WiiRd
* Allow use of the Nintendo GameCube Microphone

### What Nintendont will never support:
* Game Boy Player
* Playstation 5

### Quick Installation:
1. Get the [loader.dol](loader/loader.dol?raw=true), rename it to boot.dol and put it in /apps/Nintendont/ along with the files [meta.xml](nintendont/meta.xml?raw=true) and [icon.png](nintendont/icon.png?raw=true). For custom text colour, write the color's hexadecimal RGBA value in the blank [textcolor.ini](nintendont/textcolor.ini?raw=true)
2. Copy your GameCube games to the /games/ directory. Subdirectories are optional for 1-disc games in ISO/GCM and CISO format.
   * For 2-disc games, you should create a subdirectory /games/MYGAME/ (where MYGAME can be anything), then name disc 1 as "game.iso" and disc 2 as "disc2.iso".
   * For extracted FST, the FST must be located in a subdirectory, e.g. /games/FSTgame/sys/boot.bin .
3. Connect your storage device to your Wii or Wii U and start The Homebrew Channel.
4. Select Nintendont.

### Guide how to personalize your Nintendont: 
* GameCube Loader Animation https://www.youtube.com/watch?v=XOfJtJ03_As = CrisMMMod renders an animation which looks like the original GameCube BIOS start-up before every game launch. It's perfect if you can't find the BIOS files or want to feel the nostalgia of GameCube on vWii when starting PAL games.
                                                                          ** To activate it, simply go to Settings --> Skip IPL: and select "No"

* Custom Background =  To use a custom background in Nintendont's menus, just place a background.png or background.jpg inside your /apps/Nintendont/ folder with a resolution of 640x480 pixels. Or you can simply download from avialable Nintendont's Settings Menu.
                      ** To download the custom Themes directly from Nintendont's, please go to Settings --> Update --> and scroll down till "Theme Menu" --> then sroll down to "Download Preview Pack".
					  ** Wait till the downloaded of the previews will be done, and then navigate to the different Downloads Themes to choose the Background that you like more. Done!
					  

* Custom Game Loader ![](nintendont/example_loader.png) = CrisMMMods custom game background loader themes are now working for ALL GameCube games. They will appear when the game boots and come with or without a loading bar. (If you will download the themes by the Nintendont app it will create all the new Folder automatically) Create a folder named 'CustomGameLoader' inside your Nintendont folder and place files named after the following scheme: GAMEID3.png for themes without loading bar, and GAMEID3X.png for themes with loading bar (for example GZLX.png for Wind Waker theme with loading bar).
                                                          ** To download the custom game background Settings --> Update Menu --> and scroll down till "Theme Menu" --> Download Loader Theme. Done!
														  
* The exactly list to consult for the over 135 Text custom colors, it's here: https://en.wikipedia.org/wiki/Web_colors
  -If you cannot find in between of over 135 Text custom color tones buiold-in you can customize the Nintendont Text Menu Color = Download in this repository the blank textcolor.ini file and specify a custom text color by placing the color's hexadecimal RGBA value.
  -You can copy the hexadecimal RGBA value (example: 0x33FFFF - copy the code without 0x and paste like that --> 33FFFF add other two FF in the end will be like that --> 33FFFFFF ) from this site Hexadecimal RGBA Value site click here. Once pasted the hexadecimal RGBA value in the textcolor.ini click here to download save it, and place the file into the “/apps/Nintendont/” folder.​

* Upgrade the Games Titles = If will be some misspelling of some games, once it will be upgraded the list file you can upgrade it by Nintendont Menu Settings without to do it manually.
                             ** Settings --> Update Menu --> Download titles.txt. Done!
							 
* Upgrade to the latest Controllers Supports for Nintendont = You can download the latest controllers.zip file from Nintendont. And place your particolar unzipped controller.ini file into the root of your SD card to work with Nintendont.
                                                             ** Settings --> Update Menu --> Download controller.zip. Done!
                                                             							 
* Upgrade to the latest ID Games list file = You can download the latest gcn_md5.txt file from Nintendont. once it will be upgraded the list file you can upgrade it by Nintendont Menu Settings without to do it manually.
                                         ** Settings --> Update Menu --> Download gcn_md5. Done!

  
  
  

### Notes
* The Wii and Wii U SD card slot is known to be slow. If you're using an SD card and are having performance issues, consider to buy a recent SD high speed card either using a USB Pendriver/ USB SD reader or a USB hard drive.
* USB flash drives are known to be problematic.
* Nintendont runs best with storage devices formatted with 32 KB clusters. (Use either FAT32 or exFAT.)
