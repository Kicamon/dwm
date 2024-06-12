#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>

enum
{
    Icons,
    Wifi,
    Cpu,
    Mem,
    Date,
    Light,
    Vol,
    Bat,
};
enum
{
    Wired,
    Wireless,
    Temperature,
    Capacity,
    Charging,
};

#include "statusbar.h"

#define ull unsigned long long

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

void icons() {
    FILE *fp = NULL;
    char icon[20] = " ";
    char buffer[256] = "";
    fp = popen("nmcli dev | grep 'ap0'", "r");
    if (fp == NULL) {
        return;
    }
    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
        strcat(icon, "󱜠");
    }
    fclose(fp);
    sprintf(_icons, "^sicons^%s %s ", colors[Icons][1], icon);
}

void wifi() {
    FILE *fp = NULL;
    char buffer[256] = "";
    char connected_network[256] = "";
    char *icon = "󰕡";
    int is_connected = 0;

    fp = popen("nmcli -t -f NAME,DEVICE,STATE connection show --active", "r");
    if (fp == NULL) {
        return;
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        if (devs[Wired] != NULL && strstr(buffer, devs[Wired]) != NULL) {
            is_connected = 1;
            char *token = strtok(buffer, ":");
            strncpy(connected_network, token, sizeof(connected_network) - 1);
            connected_network[sizeof(connected_network) - 1] = '\0';
        }
        if (devs[Wireless] != NULL && strstr(buffer, devs[Wireless]) != NULL) {
            is_connected = 1;
            char *token = strtok(buffer, ":");
            strncat(connected_network, token, sizeof(connected_network) - 1);
            connected_network[sizeof(connected_network) - 1] = '\0';
        }
    }
    pclose(fp);

    sprintf(_wifi, "^swifi^%s %s %s %s ", colors[Wifi][0], icon, colors[Wifi][1],
            connected_network);
}

void cpu() {
    char *icon = "";
    FILE *fp = NULL;
    char buffer[256] = "";
    ull user, nice, system, idle, iowait, irq, softirq, steal;
    static ull prev_user, prev_nice, prev_system, prev_idle, prev_iowait, prev_irq, prev_softirq,
        prev_steal;

    fp = fopen("/proc/stat", "r");
    if (fp == NULL) {
        return;
    }

    fgets(buffer, sizeof(buffer), fp);
    fclose(fp);

    sscanf(buffer, "cpu %llu %llu %llu %llu %llu %llu %llu %llu", &user, &nice, &system, &idle,
           &iowait, &irq, &softirq, &steal);

    ull prev_total = prev_user + prev_nice + prev_system + prev_idle + prev_iowait + prev_irq +
                     prev_softirq + prev_steal;
    ull total = user + nice + system + idle + iowait + irq + softirq + steal;

    ull totald = total - prev_total;
    ull idled = idle - prev_idle;

    double usage = (double)(totald - idled) / totald * 100.0;

    fp = fopen(devs[Temperature], "r");
    if (fp == NULL) {
        return;
    }

    fgets(buffer, sizeof(buffer), fp);
    fclose(fp);

    double temperature = atof(buffer) / 1000.0;

    sprintf(_cpu, "^scpu^%s %s %s %.0lf%% %.0lf°C ", colors[Cpu][0], icon, colors[Cpu][1], usage,
            temperature);
}

void mem() {
    FILE *fp = NULL;
    char buffer[256] = "";
    char *icon = "󰍛";
    unsigned long mem_total = 0, mem_available = 0;
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
    double mem_persent = ((double)mem_total - mem_available) / mem_total * 100;
    sprintf(_mem, "^smem^%s %s %s %.0lf%% ", colors[Mem][0], icon, colors[Mem][1], mem_persent);
}

void date() {
    time_t time_now;
    time(&time_now);
    struct tm *time_info = localtime(&time_now);
    char time_str[100], time_h[10], icon[5] = "";
    strftime(time_str, sizeof(time_str), "%m/%d %H:%M", time_info);
    strftime(time_h, sizeof(time_h), "%I", time_info);
    if (strcmp(time_h, "01") <= 0) {
        strncpy(icon, "", sizeof(icon) - 1);
    } else if (strcmp(time_h, "02") <= 0) {
        strncpy(icon, "", sizeof(icon) - 1);
    } else if (strcmp(time_h, "03") <= 0) {
        strncpy(icon, "", sizeof(icon) - 1);
    } else if (strcmp(time_h, "04") <= 0) {
        strncpy(icon, "", sizeof(icon) - 1);
    } else if (strcmp(time_h, "05") <= 0) {
        strncpy(icon, "", sizeof(icon) - 1);
    } else if (strcmp(time_h, "06") <= 0) {
        strncpy(icon, "", sizeof(icon) - 1);
    } else if (strcmp(time_h, "07") <= 0) {
        strncpy(icon, "", sizeof(icon) - 1);
    } else if (strcmp(time_h, "08") <= 0) {
        strncpy(icon, "", sizeof(icon) - 1);
    } else if (strcmp(time_h, "09") <= 0) {
        strncpy(icon, "", sizeof(icon) - 1);
    } else if (strcmp(time_h, "10") <= 0) {
        strncpy(icon, "", sizeof(icon) - 1);
    } else if (strcmp(time_h, "11") <= 0) {
        strncpy(icon, "", sizeof(icon) - 1);
    } else if (strcmp(time_h, "12") <= 0) {
        strncpy(icon, "", sizeof(icon) - 1);
    }
    sprintf(_date, "^sdate^%s %s %s %s ", colors[Date][0], icon, colors[Date][1], time_str);
}

void light() {
    FILE *fp = NULL;
    double light = 0;
    char buffer[256] = "", icon[5] = "󰃠";
    fp = popen("xrandr --verbose | grep Brightnes | awk '{print $2}' | head -n 1", "r");
    if (fp == NULL) {
        return;
    }
    fgets(buffer, sizeof(buffer), fp);
    fclose(fp);
    light = atof(buffer);
    light *= 100;
    if (light >= 95) {
        strncpy(icon, "󰃠", strlen(icon) - 1);
    } else if (light >= 75) {
        strncpy(icon, "󰃝", strlen(icon) - 1);
    } else if (light >= 50) {
        strncpy(icon, "󰃟", strlen(icon) - 1);
    } else if (light >= 25) {
        strncpy(icon, "󰃞", strlen(icon) - 1);
    } else {
        strncpy(icon, "󰃜", strlen(icon) - 1);
    }
    sprintf(_light, "^slight^%s %s %s %.0lf%% ", colors[Light][0], icon, colors[Light][1], light);
}

void vol() {
    FILE *fp = NULL;
    char buffer[256] = "";
    char icon[5] = "󰕾";
    unsigned int vol = 0;
    int muted = 0;
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
    fclose(fp);
    if (strstr(buffer, "[off]") != NULL) {
        muted = 1;
    }
    if (muted) {
        strncpy(icon, "", sizeof(icon) - 1);
        sprintf(_vol, "^svol^%s %s %s -- ", colors[Vol][0], icon, colors[Vol][1]);
    } else {
        if (vol > 50) {
            strncpy(icon, "󰕾", sizeof(icon) - 1);
        } else if (vol > 0) {
            strncpy(icon, "󰖀", sizeof(icon) - 1);
        } else {
            strncpy(icon, "󰕿", sizeof(icon) - 1);
        }
        sprintf(_vol, "^svol^%s %s %s %d%% ", colors[Vol][0], icon, colors[Vol][1], vol);
    }
}

void bat() {
    FILE *fp = NULL;
    char buffer[256] = "";
    int capacity = -1;

    fp = fopen(devs[Capacity], "r");
    if (fp == NULL) {
        return;
    }
    if (fgets(buffer, sizeof(buffer), fp) != NULL) {
        capacity = atoi(buffer);
    }
    fclose(fp);

    if (capacity <= 10) {
        if (!bat_less) {
            bat_less = 1;
            system("bash ~/.config/dwm/statusbar/packages/bat.sh notify");
        }
    } else {
        bat_less = 0;
    }

    char status[20];
    int charging = 0;
    fp = fopen(devs[Charging], "r");
    if (fp == NULL) {
        return;
    }
    if (fgets(status, sizeof(status), fp) != NULL) {
        if (status[0] == 'C') {
            charging = 1;
        }
    }
    fclose(fp);

    char icon[5] = "󰁹";
    if (capacity >= 95) {
        strncpy(icon, charging ? "󰂅" : "󰁹", sizeof(icon) - 1);
    } else if (capacity >= 90) {
        strncpy(icon, charging ? "󰂋" : "󰂂", sizeof(icon) - 1);
    } else if (capacity >= 80) {
        strncpy(icon, charging ? "󰂊" : "󰂁", sizeof(icon) - 1);
    } else if (capacity >= 70) {
        strncpy(icon, charging ? "󰢞" : "󰂀", sizeof(icon) - 1);
    } else if (capacity >= 60) {
        strncpy(icon, charging ? "󰂉" : "󰁿", sizeof(icon) - 1);
    } else if (capacity >= 50) {
        strncpy(icon, charging ? "󰢝" : "󰁾", sizeof(icon) - 1);
    } else if (capacity >= 40) {
        strncpy(icon, charging ? "󰂈" : "󰁽", sizeof(icon) - 1);
    } else if (capacity >= 30) {
        strncpy(icon, charging ? "󰂇" : "󰁼", sizeof(icon) - 1);
    } else if (capacity >= 20) {
        strncpy(icon, charging ? "󰂆" : "󰁻", sizeof(icon) - 1);
    } else if (capacity >= 10) {
        strncpy(icon, charging ? "󰢜" : "󰁺", sizeof(icon) - 1);
    } else {
        strncpy(icon, charging ? "󰢟" : "󰂃", sizeof(icon) - 1);
    }
    sprintf(_bat, "^sbat^%s %s %s %d%% ", colors[Bat][0], icon, colors[Bat][1], capacity);
}

void refresh() {
    char status[2048] = "";
    sprintf(status, "%s%s%s%s%s%s%s%s", _icons, _wifi, _cpu, _mem, _date, _light, _vol, _bat);
    char cmd[2048] = "";
    snprintf(cmd, sizeof(cmd), "xsetroot -name \"%s\"", status);
    system(cmd);
}

// 启动定时更新状态栏
void cron() {
    int i = 0;
    FILE *fp = NULL;
    while (1) {
        fp = fopen(tempfile, "r");
        if (fp != NULL || !i) {
            icons(), wifi(), light(), vol();
            i = 0;
            if (fp != NULL) {
                fclose(fp);
                remove(tempfile);
            }
        }
        cpu(), mem(), date(), bat();
        refresh();
        i++;
        i %= 10;
        usleep(100000);
    }
}

void click(char *signal, char *button) {
    char script[100] = "";
    if (!strcmp(signal, "icons")) {
        sprintf(script, "bash %s/icons.sh click %s", packages, button);
    } else if (!strcmp(signal, "wifi")) {
        sprintf(script, "bash %s/wifi.sh click %s", packages, button);
    } else if (!strcmp(signal, "cpu")) {
        sprintf(script, "bash %s/cpu.sh click %s", packages, button);
    } else if (!strcmp(signal, "mem")) {
        sprintf(script, "bash %s/mem.sh click %s", packages, button);
    } else if (!strcmp(signal, "date")) {
        sprintf(script, "bash %s/date.sh click %s", packages, button);
    } else if (!strcmp(signal, "light")) {
        sprintf(script, "bash %s/light.sh click %s", packages, button);
    } else if (!strcmp(signal, "vol")) {
        sprintf(script, "bash %s/vol.sh click %s", packages, button);
    } else if (!strcmp(signal, "bat")) {
        sprintf(script, "bash %s/bat.sh click %s", packages, button);
    }
    system(script);
}

// 主函数
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
