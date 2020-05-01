#ifndef fooatihfoo
#define fooatihfoo

/*
 * This file is part of atitvout. 
 *
 * asd is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * asd is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with atitvout; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
 */

#define ATI_DISPLAY_LCD 1
#define ATI_DISPLAY_CRT 2
#define ATI_DISPLAY_TV 4
#define ATI_DISPLAY_AUTOSWITCH 8

#define ATI_TV_COMPOSITE 1
#define ATI_TV_SVIDEO 2



#define ATI_TVOUT_NOTDETECTED 0
#define ATI_TVOUT_NOTSUPPORTED 1
#define ATI_TVOUT_DISABLED 2
#define ATI_TVOUT_ENABLED 3

#define ATI_TVOUT_REFFREQ29_49892 0
#define ATI_TVOUT_REFFREQ28_63636 1
#define ATI_TVOUT_REFFREQ14_31818 2
#define ATI_TVOUT_REFFREQ27_00000 3

extern float tvout_reffreq_table[];
#define TVOUT_REFFREQ(x) ((x >= 0) && (x <= 3) ? tvout_reffreq_table[x] : 0.0)

#define ATI_TVOUT_STANDARD_NTSC 0
#define ATI_TVOUT_STANDARD_PAL 1
#define ATI_TVOUT_STANDARD_PALM 2
#define ATI_TVOUT_STANDARD_PAL60 3
#define ATI_TVOUT_STANDARD_NTSCJ 4
#define ATI_TVOUT_STANDARD_PALCN 5
#define ATI_TVOUT_STANDARD_PALN 6
#define ATI_TVOUT_STANDARD_SCARTPAL 9

extern const char* tvout_standard_table[];
#define TVOUT_STANDARD_NAME(x) ((x >= 0) && (x <= 9) ? tvout_standard_table[x] : "??")


#endif
