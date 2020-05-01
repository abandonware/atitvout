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

#include <stdio.h>
#include <string.h>

#include <lrmi.h>
#include <vbe.h>

#include "vbeinfo.h"

int vbe_info(int *vh, int *vl, char *oe, int s) {
    struct LRMI_regs r;
    struct vbe_info_block *vib;
    
    if (!(vib = LRMI_alloc_real(sizeof(struct vbe_info_block)))) {
        fprintf(stderr, "Can't alloc real mode memory.\n");
        return -1;
    }
    
    memset(&r, 0, sizeof(r));

    r.eax = 0x4f00;
    r.es = (unsigned int) vib >> 4;
    r.edi = 0;
        
    memcpy(vib->vbe_signature, "VBE2", 4);

    if (!LRMI_int(0x10, &r)) {
        fprintf(stderr, "Can't get VESA info (vm86 failure)\n");
        LRMI_free_real(vib);
        return -2;
    }

    if ((r.eax & 0xffff) != 0x4f || strncmp(vib->vbe_signature, "VESA", 4) != 0) {
        fprintf(stderr, "No VESA bios\n");
        LRMI_free_real(vib);
        return -3;
    }

    if (vh)
        *vh = (vib->vbe_version >> 8) & 0xff;

    if (vl)
        *vl = vib->vbe_version & 0xff;

    if (oe && s) {
        int i;
        if (s > sizeof(vib->oem_data))
            s = sizeof(vib->oem_data);
        
        for (i = 0; i < s; i++)
            if (vib->oem_data[i] >= ' ' && vib->oem_data[i] < 127)
                oe[i] = vib->oem_data[i];
            else {
                oe[i] = 0;
                break;
            }
    }
    
    LRMI_free_real(vib);
    
    return 0;
}
