static const char *tempfile = "/home/KicamonIce/.config/dwm/statusbar/temp";
static const char *packages = "/home/KicamonIce/.config/dwm/statusbar/packages";
static const char *colors[][2] = {
    [Icons] = {"^c#2D1B46^^b#5555660x66^", NULL},
    [Wifi]  = {"^c#000080^^b#3870560x88^", "^c#000080^^b#3870560x99^"},
    [Cpu]   = {"^c#3E206F^^b#6E51760x88^", "^c#3E206F^^b#6E51760x99^"},
    [Mem]   = {"^c#3B001B^^b#6873790x88^", "^c#3B001B^^b#6873790x99^"},
    [Date]  = {"^c#3B001B^^b#4865660x88^", "^c#3B001B^^b#4865660x99^"},
    [Light] = {"^c#3B102B^^b#6873790x88^", "^c#3B102B^^b#6873790x99^"},
    [Vol]   = {"^c#442266^^b#7879560x88^", "^c#442266^^b#7879560x99^"},
    [Bat]   = {"^c#3B001B^^b#4865660x88^", "^c#3B001B^^b#4865660x99^"},
};
static const char *devs[] = {
    [Wired]       = ":enp88s0",
    [Wireless]    = ":wlp0s20f3",
    [Temperature] = "/sys/class/hwmon/hwmon5/temp1_input",
    [Capacity]    = "/sys/class/power_supply/BAT0/capacity",
    [Charging]    = "/sys/class/power_supply/BAT0/status",
    [Plug]        = "/sys/class/power_supply/ADP0/online",
};
