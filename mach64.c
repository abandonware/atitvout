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
#include "mach64.h"
#include "vbecall.h"

int mach64_detect_attached_displays(unsigned *tv) {
    struct LRMI_regs r;
    memset(&r, 0, sizeof(r));

    r.eax = 0xa083;
    r.ecx = 0x0700;

    if (vbe_call(&r) != 0)
        return -1;

    if (tv)
        *tv = r.ecx >> 4 & 3;
    
    return ((r.ecx & 3) ? ATI_DISPLAY_CRT : 0) | ((r.ecx >> 4 & 3) ? ATI_DISPLAY_TV : 0) | ((r.ecx & 4) ? ATI_DISPLAY_LCD : 0);
}

int mach64_get_active_displays() {
    struct LRMI_regs r;
    memset(&r, 0, sizeof(r));

    r.eax = 0xa084;
    r.ebx = 0x0000;

    if (vbe_call(&r) != 0)
        return -1;
    
    return r.ebx & 7;
}

int mach64_set_active_displays(int v) {
    struct LRMI_regs r;
    memset(&r, 0, sizeof(r));

    r.eax = 0xa084;
    r.ebx = 0x0100;
    r.ecx = v & 7;

    if (vbe_call(&r) != 0)
        return -1;

    return 0;
}

int mach64_get_tvout_config(unsigned *rev, int *freq) {
    struct LRMI_regs r;

    memset(&r, 0, sizeof(r));

    r.eax = 0xa070;
    r.ebx = 0x0000; 

    if (vbe_call(&r) != 0)
        return -1;

    if ((r.ebx & 0xFFFF) == 0)
        return (r.edx & 0xFFFF) == 0 ? ATI_TVOUT_NOTDETECTED : ATI_TVOUT_NOTSUPPORTED;

    if ((r.ecx & 0xFF)== 0)
        return ATI_TVOUT_DISABLED;
    
    if (rev)
        *rev = (r.edx >> 8) & 0xff;

    if (freq)
        *freq = (r.ecx >> 8) & 0xff;

    return ATI_TVOUT_ENABLED;
}

int mach64_get_tvout_standard() {
    struct LRMI_regs r;

    memset(&r, 0, sizeof(r));

    r.eax = 0xa071;
    r.ebx = 0x0000; 

    if (vbe_call(&r) != 0)
        return -1;

    return r.ecx & 0x00ff;
}

int mach64_set_tvout_standard(int v) {
    struct LRMI_regs r;

    memset(&r, 0, sizeof(r));

    r.eax = 0xa070;
    r.ebx = 0x0003;
    r.ecx = v & 0xFF;

    if (vbe_call(&r) != 0)
        return -1;

    return r.ecx & 0x00ff;
}

