static const char *thisdir = "/home/KicamonIce/.config/dwm/statusbar";
static const char *tempfile = "/home/KicamonIce/.config/dwm/statusbar/temp";
static const char *packages = "/home/KicamonIce/.config/dwm/statusbar/packages";
static const char *colors[][2] = {
    [Icons] = {NULL, "^c#2D1B46^^b#5555660x66^"},
    [Wifi] = {"^c#000080^^b#3870560x88^", "^c#000080^^b#3870560x99^"},
    [Cpu] = {"^c#3E206F^^b#6E51760x88^", "^c#3E206F^^b#6E51760x99^"},
    [Mem] = {"^c#3B001B^^b#6873790x88^", "^c#3B001B^^b#6873790x99^"},
    [Date] = {"^c#3B001B^^b#4865660x88^", "^c#3B001B^^b#4865660x99^"},
    [Light] = {"^c#3B102B^^b#6873790x88^", "^c#3B102B^^b#6873790x99^"},
    [Vol] = {"^c#442266^^b#7879560x88^", "^c#442266^^b#7879560x99^"},
    [Bat] = {"^c#3B001B^^b#4865660x88^", "^c#3B001B^^b#4865660x99^"},
};
static char _icons[256] = "", _wifi[256] = "", _cpu[256] = "", _mem[256] = "", _date[256] = "",
            _light[256] = "", _vol[256] = "", _bat[256] = "";
