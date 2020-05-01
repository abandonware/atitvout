#ifndef foomach64hfoo
#define foomach64hfoo

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

// Support for Rage Mobility and 3D Rage Pro LT

int mach64_detect_attached_displays(unsigned *tv);
int mach64_get_active_displays();
int mach64_set_active_displays(int v);
int mach64_get_tvout_config(unsigned *rev, int *freq);
int mach64_get_tvout_standard();
int mach64_set_tvout_standard(int v);

#endif
