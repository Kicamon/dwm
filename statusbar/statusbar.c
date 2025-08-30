#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include "statusbar.h"

static char _icons[256] = "", _wifi[256] = "", _cpu[256] = "", _mem[256] = "", _date[256] = "",
            _light[256] = "", _vol[256] = "", _bat[256] = "";

static void refresh();
static void cron();
static void click(char *signal, char *button);
static void icons();
static void mem();
static void cpu();
static void wifi();
static void date();
static void light();
static void vol();
static void bat();
static int pscanf(const char *path, const char *fmt, ...);
static long double as_ld(const long double *array, size_t size, size_t start_pos);

int pscanf(const char *path, const char *fmt, ...) {
        FILE *fp = NULL;
        va_list ap;
        int n;

        if (!(fp = fopen(path, "r"))) {
                return -1;
        }
        va_start(ap, fmt);
        n = vfscanf(fp, fmt, ap);
        va_end(ap);
        fclose(fp);

        return (n == EOF) ? -1 : n;
}

long double as_ld(const long double *array, size_t size, size_t start_pos) {
        long double sum = 0;
        size += start_pos;
        for (size_t i = start_pos; i < size; ++i) {
                sum += array[i];
        }
        return sum;
}

void icons() {
        char buffer[256];
        char icon[20] = "󰊠";

        FILE *fp = NULL;
        fp = popen("amixer get Capture | grep off", "r");
        if (fp == NULL) {
                return;
        }
        if (fgets(buffer, sizeof(buffer), fp) != NULL) {
                strncpy(icon, "󰧵", sizeof(icon) - 1);
        }
        pclose(fp);

        sprintf(_icons, "^sicons^%s %s ", colors[Icons][0], icon);
}

void wifi() {
        char buffer[256];
        char connected_network[20] = "";
        char icon[5] = "󰕡";
        int is_wired = 0, is_wireless = 0;

        FILE *fp = NULL;
        fp = popen("nmcli -t -f NAME,DEVICE,STATE connection show --active", "r");
        if (fp == NULL) {
                return;
        }
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
                if (devs[Wired] != NULL && strstr(buffer, devs[Wired]) != NULL) {
                        is_wired = 1;
                        char *token = strtok(buffer, ":");
                        snprintf(connected_network, sizeof(connected_network), "%s", token);
                        connected_network[sizeof(connected_network) - 1] = '\0';
                }
                if (devs[Wireless] != NULL && strstr(buffer, devs[Wireless]) != NULL) {
                        is_wireless = 1;
                        char *token = strtok(buffer, ":");
                        snprintf(connected_network, sizeof(connected_network), "%s", token);
                        connected_network[sizeof(connected_network) - 1] = '\0';
                }
        }
        pclose(fp);

        if (is_wired) {
                strncpy(icon, "󰕡", sizeof(icon) - 1);
        } else if (is_wireless) {
                strncpy(icon, "󰖩", sizeof(icon) - 1);
        } else {
                strncpy(icon, "󱈨", sizeof(icon) - 1);
        }

        sprintf(_wifi, "^swifi^%s %s%s %s ", colors[Wifi][0], icon, colors[Wifi][1],
                connected_network);
}

void cpu() {
        char *icon = "󰍛";

        static long double a[7] = { 0 };
        long double b[7], sum;

        memcpy(b, a, sizeof(b));
        if (pscanf("/proc/stat", "%*s %Lf %Lf %Lf %Lf %Lf %Lf %Lf", &a[0], &a[1], &a[2], &a[3],
                   &a[4], &a[5], &a[6]) != 7) {
                return;
        }

        if (b[0] == 0) {
                return;
        }

        sum = as_ld(b, 7, 0) - as_ld(a, 7, 0);

        int usage = 100.0 * (sum - as_ld(b, 2, 3) + as_ld(a, 2, 3)) / sum;

        int temperature;
        if (pscanf(devs[Temperature], "%d", &temperature) != 1) {
                return;
        }
        temperature /= 1000;

        sprintf(_cpu, "^scpu^%s %s%s %02d%% %02d°C ", colors[Cpu][0], icon, colors[Cpu][1], usage,
                temperature);
}

void mem() {
        char buffer[256];
        char *icon = "󰟖";
        unsigned long mem_total = 0, mem_available = 0;

        FILE *fp = NULL;
        fp = fopen("/proc/meminfo", "r");
        if (fp == NULL) {
                return;
        }
        while (fgets(buffer, sizeof(buffer), fp)) {
                if (sscanf(buffer, "MemTotal:%lu kB", &mem_total) == 1) {
                        continue;
                }
                if (sscanf(buffer, "MemAvailable:%lu kB", &mem_available) == 1) {
                        continue;
                }
        }
        fclose(fp);

        int mem_persent = ((double)mem_total - mem_available) / mem_total * 100;

        sprintf(_mem, "^smem^%s %s%s %02d%% ", colors[Mem][0], icon, colors[Mem][1], mem_persent);
}

void date() {
        time_t time_now;
        time(&time_now);
        struct tm *time_info = localtime(&time_now);
        char time_n[6], icon[5] = "󱑖";
        int time_h;

        strftime(time_n, sizeof(time_n), "%H:%M", time_info);
        time_h = time_info->tm_hour % 12;

        strncpy(icon, time_icons[time_h], sizeof(icon) - 1);

        sprintf(_date, "^sdate^%s %s%s %s ", colors[Date][0], icon, colors[Date][1], time_n);
}

void vol() {
        char buffer[256];
        char icon[5] = "󰕾";
        unsigned int vol = 0;
        int muted = 0, headphones = 0;

        FILE *fp = NULL;
        fp = popen("amixer get Master", "r");
        if (fp == NULL) {
                return;
        }
        while (fgets(buffer, sizeof(buffer) - 1, fp) != NULL) {
                char *vol_pr = strstr(buffer, "[");
                if (vol_pr != NULL) {
                        vol_pr++;
                        vol = atoi(vol_pr);
                }
        }
        pclose(fp);

        if (strstr(buffer, "[off]") != NULL) {
                muted = 1;
        }

        fp = popen("pactl list sinks | grep -E '活动端口.*headphones'", "r");
        if (fp == NULL) {
                return;
        }
        if (fgets(buffer, sizeof(buffer) - 1, fp) != NULL) {
                headphones = 1;
        }
        pclose(fp);

        if (muted) {
                strncpy(icon, "󰝟", sizeof(icon) - 1);
                sprintf(_vol, "^svol^%s %s%s ", colors[Vol][0], icon, colors[Vol][1]);
                return;
        }
        if (headphones) {
                strncpy(icon, "󰋋", sizeof(icon) - 1);
        } else {
                strncpy(icon, vol_icons[(vol + 49) / 50], sizeof(icon) - 1);
        }
        sprintf(_vol, "^svol^%s %s%s %d%% ", colors[Vol][0], icon, colors[Vol][1], vol);
}

void bat() {
        char buffer[256];
        int capacity = -1;

        FILE *fp = NULL;
        fp = fopen(devs[Capacity], "r");
        if (fp == NULL) {
                return;
        }
        if (fgets(buffer, sizeof(buffer), fp) != NULL) {
                capacity = atoi(buffer);
        }
        fclose(fp);

        int charging = 0;
        fp = fopen(devs[Charging], "r");
        if (fp == NULL) {
                return;
        }
        if (fgets(buffer, sizeof(buffer), fp) != NULL) {
                size_t len = strlen(buffer);
                if (len > 1 && buffer[len - 1] == '\n') {
                        buffer[len - 1] = '\0';
                }
                if (!strcmp(buffer, "Charging")) {
                        charging = 1;
                }
        }
        fclose(fp);

        int plugin = 0;
        fp = fopen(devs[Plug], "r");
        if (fp == NULL) {
                return;
        }
        if (fgets(buffer, sizeof(buffer), fp) != NULL) {
                if (buffer[0] == '1') {
                        plugin = 1;
                }
        }
        fclose(fp);

        char icon[5] = "󰁹";
        if (plugin && !charging) {
                strncpy(icon, "󱘖", sizeof(icon) - 1);
        } else {
                strncpy(icon, bat_icons[charging][(capacity + 5) / 10], sizeof(icon) - 1);
        }

        sprintf(_bat, "^sbat^%s %s%s %2d%% ", colors[Bat][0], icon, colors[Bat][1], capacity);
}

void refresh() {
        char status[2048] = "", cmd[2048] = "";
        sprintf(status, "%s%s%s%s%s%s%s", _icons, _wifi, _cpu, _mem, _date, _vol, _bat);
        snprintf(cmd, sizeof(cmd), "xsetroot -name \"%s\"", status);
        system(cmd);
}

void cron() {
        FILE *fp = NULL;
        int i = 0;
        while (1) {
                fp = fopen(tempfile, "r");
                if (fp != NULL || !i) {
                        icons(), wifi(), vol(), cpu(), mem(), bat();
                        if (fp != NULL) {
                                fclose(fp);
                                remove(tempfile);
                        }
                }
                date(), refresh();
                i = (i + 1) % 10;
                usleep(100000);
        }
}

void click(char *signal, char *button) {
        char script[256] = "";
        snprintf(script, sizeof(script), "bash %s/%s.sh click %s", packages, signal, button);
        system(script);
}

int main(int argc, char *argv[]) {
        if (argc > 1) {
                if (argc == 2 && strcmp(argv[1], "cron") == 0) {
                        cron();
                } else if (argc == 3) {
                        click(argv[1], argv[2]);
                }
        }

        return 0;
}
