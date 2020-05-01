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

#include <lrmi.h>
#include <stdio.h>

#include "ati.h"
#include "radeon.h"
#include "vbecall.h"

// This call seems to do more than just detect attached TVs: it
// enables all those displays which have been enabled with the last
// call to radeon_set_active_displays()
int radeon_detect_attached_displays(unsigned *tv) {
    int retval,tv_out;
    struct LRMI_regs r;
    memset(&r, 0, sizeof(r));

    r.eax = 0xa014;
    r.ecx = 0x0700;

    if (vbe_call(&r) != 0)
        return -1;
    retval = 0;
    tv_out = ((r.ecx>>8)&0xFF);
    /* FIXME: doc says that tvout info is returned in CH
       but CH on my Radeon VE is always 0. But such info
       is present in high part of CL !!! */
    if(!tv_out && (r.ecx>>4)&0xF) tv_out = (r.ecx>>4)&0xF;
    if(tv_out)
    {
        *tv = tv_out;
        retval |= ATI_DISPLAY_TV;
    }
/*
  Note: Here we can return more precise info:
  cl = 0 - no CRT connected
  cl = 1 - monochrome CRT is connected
  cl = 2 - color monitor is connected
*/
    if (r.ecx&0xFF) retval |= ATI_DISPLAY_CRT;
    if ((r.ebx&0xFF) == 1) retval |= ATI_DISPLAY_LCD;
    return retval;
}

int radeon_get_active_displays() {
    struct LRMI_regs r;
    memset(&r, 0, sizeof(r));

    r.eax = 0xa015;
    r.ecx = 0x0000;

    if (vbe_call(&r) != 0)
        return -1;
    
    return ((r.ecx & 1) ? ATI_DISPLAY_CRT : 0) |
        ((r.ecx & 2) ? ATI_DISPLAY_TV : 0) |
        ((r.ecx & 4) ? ATI_DISPLAY_LCD : 0) |
        ((r.ecx & 8) ? ATI_DISPLAY_AUTOSWITCH : 0);
}

int radeon_set_active_displays(int v) {
    struct LRMI_regs r;
    memset(&r, 0, sizeof(r));

    r.eax = 0xa015;
    r.ecx = 0x0100;

    if (v & ATI_DISPLAY_CRT) r.ecx |= 1;
    if (v & ATI_DISPLAY_TV) r.ecx |= 2;
    if (v & ATI_DISPLAY_LCD) r.ecx |= 4;
    if (v & ATI_DISPLAY_AUTOSWITCH) r.ecx |= 8;

    if (vbe_call(&r) != 0)
        return -1;
    
    return 0;
}

int radeon_get_tvout_config(unsigned *rev, int *freq) {
    struct LRMI_regs r;
    memset(&r, 0, sizeof(r));

    r.eax = 0xA017;
    r.edi = 0x00;

    if (vbe_call(&r) != 0)
        return -1;

    if (rev)
        *rev = (r.ecx >> 8) & 0xff;

    if (freq)
        *freq = r.ecx & 0xff;
    
    if (r.edx & 0x3)
        printf("TVOut chip is detected but not supported in BIOS\n");
    else if (r.edx & 0x1)
        printf("TVOut chip is detected and supported in BIOS\n");
    else
        printf("no TVOut chip is detected\n");
    
    return ATI_TVOUT_ENABLED;
}

int radeon_get_tvout_standard() {
    struct LRMI_regs r;
    memset(&r, 0, sizeof(r));
    
    r.eax = 0xa016;
    r.ecx = 0x0000;
    
    if (vbe_call(&r) != 0)
        return -1;
/*
  ch = current active standard value
  1 - ntsc
  2 - pal
  3 - palm
  4 - pal60
  5 - ntscj
  6 - scartrgb

  cl = mask of standards supported on the fly    
  0x01 - ntsc
  0x02 - pal
  0x04 - palm
  0x08 - pal60
  0x10 - ntscj
  0x20 - scartrgb
*/
    printf("Can set on the fly:\n");
    if (r.ecx & 0x01) printf(" ntsc\n");
    if (r.ecx & 0x02) printf(" pal\n");
    if (r.ecx & 0x04) printf(" palm\n");
    if (r.ecx & 0x08) printf(" pal60\n");
    if (r.ecx & 0x10) printf(" ntscj\n");
    if (r.ecx & 0x20) printf(" scartrgb\n");
    
    return ((r.ecx >> 8) & 0xff)-1;
}

int radeon_set_tvout_standard(int v) {
    struct LRMI_regs r;
    memset(&r, 0, sizeof(r));

    r.eax = 0xa016;
/*
  cl = new standard value
  1 - ntsc
  2 - pal
  3 - palm
  4 - pal60
  5 - ntscj
  6 - scartrgb
*/
    r.ecx = 0x0100|((v+1)&0xFF);

    if (vbe_call(&r) != 0)
        return -1;
    
    return 0; /* always no error */
}
