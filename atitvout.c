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
#include <sys/io.h>
#include <string.h>
#include <assert.h>


#include <lrmi.h>
#include <vbe.h>

#include "vbeinfo.h"
#include "ati.h"
#include "mach64.h"
#include "radeon.h"

#ifndef VERSION
#define VERSION "?.?"
#endif

char* basename(char* fn) {
    char *c;
    if (!fn)
        return "atitvout";
    
    if ((c = strrchr(fn, '/')))
        return c+1;

    return fn;
}

int radeon = 0;

void help(char *fn) {
    char *c = basename(fn);
    
    printf("%s %s :: ATI Rage/Radeon TV Out configuration program\n"
           "    Lennart Poettering <mz617469@poettering.de> 2001,2002\n"
           "    http://www.stud.uni-hamburg.de/users/lennart/projects/atitvout/\n"
           "    Licensed under the GNU General Public License, \n"
           "         http://www.gnu.org/licenses/gpl.html#SEC1\n\n"
           "Supported commands:\n"
           "    help: Shows this help.\n"
           "    vbe: Show some information about the VESA BIOS Extension (VBE).\n"
           "    tvout: Shows some information about the TV Out connector.\n"
           "    standard: Shows some information about the used TV standard.\n"
           "    pal*: Sets PAL mode.\n"
           "    ntsc*: Sets NTSC mode.\n"
           "    detect: Shows a list of attached displays.\n"
           "    active: Shows a list of all active displays.\n"
           "    auto: Activates all attached displays, deactivates all others.\n"
           "    off: Deactives all displays.\n"
           "    l,c,t,lc,lt,ct,lct: Activates the specified display, deactivates all\n"
           "                        others. (l = LCD; c = CRT; t = TV)\n\n"
           "Use the flags -f and -r for forcing Rage Mobility/Rage LT resp. Radeon/R128 mode.\n"
           "* Not available on Radeon/Rage 128.\n\n"
           "You may specifiy more than one command on the command line. An example:\n\n"
           "    %s pal auto\n\n"
           "This command line automatically enables an attached TV and sets it to PAL mode.\n\n"
           "PLEASE NOTE: When using the ati driver of XFree 4.1 you should rerun this\n"
           "utility always after having changed the screen resolution with C-A-+ or C-A--.\n"
           "Otherwise the displays may go nuts.\n\n"
           "PLEASE NOTE: Not all ATI adapters support all of these commands. Please try them\n"
           "all before complaining.\n\n"
           "PLEASE NOTE: Most cards do not allow to activate non attached displays. All calls\n"
           "to atitvout will fail if you try to enable displays which do not show up in\n"
           "'%s detect'.\n\n"
           "PLEASE NOTE: Not all adapters seem to support simultaneously using TV and LCD from\n"
           "within Linux. Thus 'lt' may fail, while 'l' and 't' succeed. In this case 'auto'\n"
           "will fail too. In fact 'auto' works only on very few adapters.\n",
           c, VERSION, c, c);
}

int main(int argc, char* argv[]) {
    int r;
    char **c = argv+1;
    char oem[256];
    int vh, vl;

    if (argc <= 1) {
        help(argv[0]);
        return 0;
    }
    
    ioperm(0, 1024, 1);
    iopl(3);

    if (!LRMI_init()) {
        fprintf(stderr, "Could not initialise LRMI.\n");
        return 1;
    }

    if (vbe_info(&vh, &vl, oem, sizeof(oem)) != 0)
        return 2;
    
    if (strcmp(argv[1], "-f") == 0) {
        radeon = 0;
        fprintf(stderr, "Forcing Rage Mobility/Rage 3D Pro LT mode\n");
    } else if (strcmp(argv[1], "-r") == 0) {
        radeon = 1;
        fprintf(stderr, "Forcing Radeon/Rage 128 mode\n");
    } else {
        if (strncmp(oem, "ATI MACH64", 10) == 0)
            radeon = 0;
        else if (strncmp(oem, "ATI RAGE128", 11) == 0)
            radeon = 1;
        /* Note: Also it's possible to have here 'RG6' for first radeons */
        else if (strstr(oem, "RADEON") != 0)
            radeon = 1;
        else {
            fprintf(stderr, "Failed to detect adapter type.\nUse either -f or -r for forcing either Rage Mobility/Rage 3D Pro LT or Radeon/Rage 128 mode.");
            return 2;
        }
    }

    for (; argc > 1; argc--, c++) {
        int standard = -1;
        int display = -1;
        char *a = *c;
        
        assert(a);

        if (strcmp(a, "detect") == 0) {
            unsigned tv;
            
            if(!radeon) {
                if ((r = mach64_detect_attached_displays(&tv)) < 0)
                    return 3;
            } else {
                if ((r = radeon_detect_attached_displays(&tv)) < 0)
                    return 3;
            }

            if (r == 0)
                printf("No display is attached.\n");
            else {
                if (r & ATI_DISPLAY_CRT) printf("CRT is attached.\n");
                if (r & ATI_DISPLAY_LCD) printf("LCD is attached.\n");
                if (r & ATI_DISPLAY_TV) {
                    if (tv & ATI_TV_COMPOSITE) printf("TV is attached via Composite.\n");
                    if (tv & ATI_TV_SVIDEO) printf("TV is attached via S-Video.\n");
                }
            }
        } else if (strcmp(a, "vbe") == 0) {
            printf("VBE Version: %d.%d\n", vh, vl);
            printf("VBE OEM Identification: %s\n", oem);
            
        } else if (!radeon && strcmp(a, "auto") == 0) {
            unsigned tv;

            if ((r = mach64_detect_attached_displays(&tv)) < 0)
                return 3;
            
            mach64_set_active_displays(ATI_DISPLAY_LCD);

            display = r & 0x7;
        } else if (radeon && strcmp(a, "auto") == 0) {
            unsigned tv;
            
            if ((r = radeon_detect_attached_displays(&tv)) < 0)
                return 3;
            
            radeon_set_active_displays(ATI_DISPLAY_CRT);

            display = ATI_DISPLAY_CRT | ((r & 3) != 0 ? ATI_DISPLAY_TV : 0);
        } else if (strcmp(a, "active") == 0) {
            if (!radeon) {
                if ((r = mach64_get_active_displays()) < 0)
                    return 6;
            } else {
                if ((r = radeon_get_active_displays()) < 0)
                    return 6;
            }
                
            if (r == 0)
                printf("No display is active.\n");
            else {
                if (r & ATI_DISPLAY_LCD) printf("LCD is active.\n");
                if (r & ATI_DISPLAY_CRT) printf("CRT is active.\n");
                if (r & ATI_DISPLAY_TV) printf("TV is active.\n");
                if (r & ATI_DISPLAY_AUTOSWITCH) printf("Autoswitch is active.\n");
            }
        } else if (strcmp(a, "tvout") == 0) {
            unsigned rev, freq;

            if (!radeon) {
                if ((r = mach64_get_tvout_config(&rev, &freq)) < 0)
                    return 7;
            } else {
                if ((r = radeon_get_tvout_config(&rev, &freq)) < 0)
                    return 7;
            }
            
            
            switch (r) {
                case ATI_TVOUT_NOTDETECTED:
                    printf("No TV Out detected.\n");
                    break;
                case ATI_TVOUT_NOTSUPPORTED:
                    printf("TV Out not supported.\n");
                    break;
                case ATI_TVOUT_DISABLED:
                    printf("TV Out is disabled.\n");
                    break;
                case ATI_TVOUT_ENABLED:
                    printf("TV Out is enabled.\n");
                    if (!radeon)
                        printf("TV Out subsystem release is %d.\n", rev);
                    printf("Reference frequency is %2.5f Hz\n", TVOUT_REFFREQ(freq));
                    break;
                default:
                    printf("Unknown TV Out config.\n");
                    break;
            }
        } else if (strcmp(a, "standard") == 0) {
            if (!radeon) {
                if ((r = mach64_get_tvout_standard()) < 0)
                    return 8;
            } else {
                if ((r = radeon_get_tvout_standard()) < 0)
                    return 8;
            }
            
            printf("Current standard is %s.\n", TVOUT_STANDARD_NAME(r));
        } else if (strcmp(a, "help") == 0 || strcmp(a, "--help") == 0 || strcmp(a, "-h") == 0)
            help(argv[0]);
        else if (strcmp(a, "ntsc") == 0) 
            standard = ATI_TVOUT_STANDARD_NTSC;
        else if (strcmp(a, "ntscj") == 0) 
            standard = ATI_TVOUT_STANDARD_NTSCJ;
        else if (strcmp(a, "pal") == 0)
            standard = ATI_TVOUT_STANDARD_PAL;
        else if (strcmp(a, "pal60") == 0)
            standard = ATI_TVOUT_STANDARD_PAL60;
        else if (strcmp(a, "palm") == 0)
            standard = ATI_TVOUT_STANDARD_PALM;
        else if (strcmp(a, "off") == 0)
            display = 0;
        else if (strcmp(a, "-f") != 0 && strcmp(a, "-r") != 0) {
            char *p;

            display = 0;

            for (p = a; *p; p++)
                if (*p == 'c')
                    display |= ATI_DISPLAY_CRT;
                else if (*p == 't')
                    display |= ATI_DISPLAY_TV;
                else if (*p == 'l')
                    display |= ATI_DISPLAY_LCD;
                else {
                    fprintf(stderr, "Unknown command: %s\n", a);
                    return 9;
                }
        }

        if (standard != -1) {
            if (!radeon) {
                if (mach64_set_tvout_standard(standard) < 0)
                    return 10;
            } else {
                if (radeon_set_tvout_standard(standard) < 0)
                    return 10;
            }
        }
        
        if (display != -1) {
            if (!radeon) {
                if (mach64_set_active_displays(display) < 0)
                    return 11;
            } else {
                if (radeon_set_active_displays(display) < 0)
                    return 11;
            }
        }
    }

    return 0;
}
