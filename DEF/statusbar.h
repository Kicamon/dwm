static const char* tempfile = "/home/KicamonIce/.config/dwm/statusbar/temp";
static const char* packages = "/home/KicamonIce/.config/dwm/statusbar/packages";
static const char* colors[][2] = {
    [Icons] = { "^c#fbf1c7^^b#282828^", NULL },
    [Wifi] = { "^c#fbf1c7^^b#282828^", "^c#fbf1c7^^b#282828^" },
    [Cpu] = { "^c#fbf1c7^^b#282828^", "^c#fbf1c7^^b#282828^" },
    [Mem] = { "^c#fbf1c7^^b#282828^", "^c#fbf1c7^^b#282828^" },
    [Date] = { "^c#fbf1c7^^b#282828^", "^c#fbf1c7^^b#282828^" },
    [Light] = { "^c#fbf1c7^^b#282828^", "^c#fbf1c7^^b#282828^" },
    [Vol] = { "^c#fbf1c7^^b#282828^", "^c#fbf1c7^^b#282828^" },
    [Bat] = { "^c#fbf1c7^^b#282828^", "^c#fbf1c7^^b#282828^" },
};
static const char* devs[] = {
    [Wired] = ":enp88s0",
    [Wireless] = ":wlp0s20f3",
    [Temperature] = "/sys/class/hwmon/hwmon5/temp1_input",
    [Capacity] = "/sys/class/power_supply/BAT0/capacity",
    [Charging] = "/sys/class/power_supply/BAT0/status",
    [Plug] = "/sys/class/power_supply/ADP0/online",
};
