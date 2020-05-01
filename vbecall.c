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
#include <assert.h>

#include "vbecall.h"

int vbe_call(struct LRMI_regs *r) {

    assert(r);

#ifdef DEBUG
    fprintf(stderr, "Calling VBE with ax=0x%04x, bx=0x%04x, cx=0x%04x, dx=0x%04x.\n", r->eax & 0xFFFF, r->ebx & 0xFFFF, r->ecx & 0xFFFF, r->edx & 0xFFFF);
#endif 
    
    if (!LRMI_int(0x10, r)) {
        fprintf(stderr, "Interrupt call failed.\n");
        return -1;
    }

#ifdef DEBUG
    fprintf(stderr, "VBE call returned with ax=0x%04x, bx=0x%04x, cx=0x%04x, dx=0x%04x.\n", r->eax & 0xFFFF, r->ebx & 0xFFFF, r->ecx & 0xFFFF, r->edx & 0xFFFF);
#endif 


    
    if (r->eax & 0xff00) {
        fprintf(stderr, "VBE call failed.\nMaybe this command is not supported by your graphics adapter?\nDid your parameters (if you specified some) really make sense?\nPlease try all other available commands before complaining!\n");
        return -2;
    }

    return 0;
}
