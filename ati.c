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

float tvout_reffreq_table[] = {
    29.49892,
    28.63636,
    14.31818,
    27.00000};

const char* tvout_standard_table[] = {
    "NTSC",
    "PAL",
    "PAL-M",
    "PAL-60",
    "NTSC-J",
    "PAL-CN/SCART-RGB",
    "PAL-N",
    "??",
    "??",
    "SCART PAL" };

