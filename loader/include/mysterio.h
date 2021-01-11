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
#ifndef __MYSTERIO_H__
#define __MYSTERIO_H__

#include <sys/param.h>

void FadeIn(GRRLIB_texImg *tex);
void FadeOut(GRRLIB_texImg *tex);
void CustomBackground();
int checkTheme();
void loaderTheme(bool autoboot, int mode);
void Animation();

extern unsigned int text_color;
extern bool cColor;

extern char loaderPath[MAXPATHLEN];

#endif
