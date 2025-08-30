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
#include <alsa/asoundlib.h>

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
        char icon[20] = " 󰊠";

        snd_mixer_t *handle;
        snd_mixer_selem_id_t *sid;
        snd_mixer_elem_t *elem;

        // 打开混音器
        if (snd_mixer_open(&handle, 0) < 0) {
                fprintf(stderr, "Failed to open mixer\n");
                return;
        }

        if (snd_mixer_attach(handle, "default") < 0) {
                fprintf(stderr, "Failed to attach mixer\n");
                snd_mixer_close(handle);
                return;
        }

        if (snd_mixer_selem_register(handle, NULL, NULL) < 0) {
                fprintf(stderr, "Failed to register mixer\n");
                snd_mixer_close(handle);
                return;
        }

        if (snd_mixer_load(handle) < 0) {
                fprintf(stderr, "Failed to load mixer\n");
                snd_mixer_close(handle);
                return;
        }

        // 查找捕获控制
        snd_mixer_selem_id_alloca(&sid);
        snd_mixer_selem_id_set_index(sid, 0);
        snd_mixer_selem_id_set_name(sid, "Capture");

        elem = snd_mixer_find_selem(handle, sid);
        if (elem) {
                int switch_val;
                if (snd_mixer_selem_get_capture_switch(elem, SND_MIXER_SCHN_FRONT_LEFT,
                                                       &switch_val) >= 0) {
                        if (!switch_val) {
                                strncpy(icon, " 󰧵", sizeof(icon) - 1);
                        }
                }
        }

        snd_mixer_close(handle);
        sprintf(_icons, "^sicons^%s %s ", colors[Icons][0], icon);
}

void wifi() {
        char icon[5] = "󰕡";
        char connected_network[20] = "";
        int is_wired = 0, is_wireless = 0;

        // 检查网络接口状态
        FILE *fp = fopen("/proc/net/dev", "r");
        if (fp == NULL) {
                perror("Failed to open /proc/net/dev");
                return;
        }

        char line[256];
        fgets(line, sizeof(line), fp); // 跳过前两行标题
        fgets(line, sizeof(line), fp);

        while (fgets(line, sizeof(line), fp)) {
                char iface[20];
                unsigned long long rbytes, tbytes;

                if (sscanf(line, "%19[^:]: %llu %*u %*u %*u %*u %*u %*u %*u %llu", iface, &rbytes,
                           &tbytes) >= 2) {
                        // 去除接口名称末尾的空格
                        size_t len = strlen(iface);
                        while (len > 0 && iface[len - 1] == ' ') {
                                iface[--len] = '\0';
                        }

                        // 检查是否有活动
                        if (rbytes > 0 || tbytes > 0) {
                                if (devs[Wired] != NULL && strcmp(iface, devs[Wired]) == 0) {
                                        is_wired = 1;
                                        snprintf(connected_network, sizeof(connected_network),
                                                 "Wired");
                                } else if (devs[Wireless] != NULL &&
                                           strcmp(iface, devs[Wireless]) == 0) {
                                        is_wireless = 1;

                                        // 尝试获取无线网络SSID
                                        char path[256];
                                        snprintf(path, sizeof(path), "/proc/net/wireless");
                                        FILE *wifi_fp = fopen(path, "r");
                                        if (wifi_fp) {
                                                char wifi_line[256];
                                                fgets(wifi_line, sizeof(wifi_line),
                                                      wifi_fp); // 跳过标题行
                                                fgets(wifi_line, sizeof(wifi_line), wifi_fp);

                                                while (fgets(wifi_line, sizeof(wifi_line),
                                                             wifi_fp)) {
                                                        char wifi_iface[20];
                                                        int status, link_quality;

                                                        if (sscanf(wifi_line, "%19[^:]: %d %d.",
                                                                   wifi_iface, &status,
                                                                   &link_quality) >= 2) {
                                                                // 去除接口名称末尾的空格
                                                                size_t wlen = strlen(wifi_iface);
                                                                while (wlen > 0 &&
                                                                       wifi_iface[wlen - 1] ==
                                                                               ' ') {
                                                                        wifi_iface[--wlen] = '\0';
                                                                }

                                                                if (strcmp(wifi_iface, iface) ==
                                                                            0 &&
                                                                    link_quality > 0) {
                                                                        // 这里无法直接从/proc获取SSID，需要其他方法
                                                                        snprintf(
                                                                                connected_network,
                                                                                sizeof(connected_network),
                                                                                "Wi-Fi");
                                                                }
                                                        }
                                                }
                                                fclose(wifi_fp);
                                        }
                                }
                        }
                }
        }
        fclose(fp);

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

        FILE *fp = fopen("/proc/stat", "r");
        if (fp == NULL) {
                perror("Failed to open /proc/stat");
                return;
        }

        int n = fscanf(fp, "%*s %Lf %Lf %Lf %Lf %Lf %Lf %Lf", &a[0], &a[1], &a[2], &a[3], &a[4],
                       &a[5], &a[6]);
        fclose(fp);

        if (n != 7) {
                fprintf(stderr, "Failed to parse /proc/stat\n");
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
        char buffer[256] = { 0 };
        char icon[5] = "󰕾";
        unsigned int vol = 0;
        int muted = 0, headphones = 0;

        // 获取主音量信息
        FILE *fp = popen("amixer get Master", "r");
        if (fp == NULL) {
                fprintf(stderr, "Failed to get volume info\n");
                return;
        }

        // 安全读取和处理输出
        while (fgets(buffer, sizeof(buffer) - 1, fp) != NULL) {
                // 查找音量百分比
                char *percent_start = strstr(buffer, "[");
                if (percent_start != NULL) {
                        percent_start++; // 跳过 '['
                        char *percent_end = strstr(percent_start, "%]");
                        if (percent_end != NULL) {
                                // 提取百分比数字
                                char percent_str[10] = { 0 };
                                size_t len = percent_end - percent_start;
                                if (len >= sizeof(percent_str)) {
                                        len = sizeof(percent_str) - 1;
                                }
                                strncpy(percent_str, percent_start, len);
                                percent_str[len] = '\0';

                                // 安全转换字符串为整数
                                char *endptr;
                                long temp_vol = strtol(percent_str, &endptr, 10);
                                if (endptr != percent_str && *endptr == '\0' && temp_vol >= 0) {
                                        vol = (unsigned int)temp_vol;
                                }
                        }
                }

                // 检查是否静音
                if (strstr(buffer, "[off]") != NULL) {
                        muted = 1;
                }
        }
        pclose(fp);

        // 检查是否有耳机插入
        fp = popen("pactl list sinks | grep -E '活动端口.*headphones'", "r");
        if (fp != NULL) {
                if (fgets(buffer, sizeof(buffer) - 1, fp) != NULL) {
                        headphones = 1;
                }
                pclose(fp);
        }

        // 确保音量值在合理范围内 (0-200% 或更高，根据实际需要调整)
        if (vol > 200) {
                vol = 200; // 限制最大值为200%，您可以根据需要调整
        }

        if (muted) {
                strncpy(icon, "󰝟", sizeof(icon) - 1);
                sprintf(_vol, "^svol^%s %s%s ", colors[Vol][0], icon, colors[Vol][1]);
                return;
        }

        if (headphones) {
                strncpy(icon, "󰋋", sizeof(icon) - 1);
        } else {
                // 计算图标索引，确保不超出数组范围
                int icon_index = (vol + 5) / 10;
                if (icon_index < 0)
                        icon_index = 0;
                if (icon_index >= sizeof(vol_icons) / sizeof(vol_icons[0])) {
                        icon_index = sizeof(vol_icons) / sizeof(vol_icons[0]) - 1;
                }
                strncpy(icon, vol_icons[icon_index], sizeof(icon) - 1);
        }

        sprintf(_vol, "^svol^%s %s%s %u%% ", colors[Vol][0], icon, colors[Vol][1], vol);
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
