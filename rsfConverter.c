#include "EV3_RobotControl/btcomm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define debug(...) fprintf(stderr, __VA_ARGS__)

char str[1024], name[100], buffer[65535];

int main(int argc, char const *argv[])
{
    if (argc != 2 && argc != 3)
    {
        debug("Error: Invalid argc!\n");
        return -1;
    }
    strcpy(name, argv[1]);
    int name_len = strlen(name);
    int dot = name_len - 1;
    while (dot >= 0 && name[dot] != '.')
        dot -= 1;
    if (dot < 0)
        dot = name_len;
    name[dot] = 0;
    sprintf(str, "ffmpeg -i %s -acodec pcm_u8 -f u8 -ac 1 -ar 8000 %s.raw", argv[1], name);
    if (system(str) != 0)
    {
        debug("Error: Cannot convert the sound file to .raw file.\n");
        debug("Please check the file name, and whether ffmpeg is correctly installed.\n");
        return -1;
    }
    sprintf(str, "%s.raw", name);
    int in_fd = open(str, O_RDONLY);
    if (in_fd < 0)
    {
        debug("Error: Cannot open .raw file.\n");
        return -1;
    }
    int segment_cnt = 0, size;
    while ((size = read(in_fd, buffer, sizeof(buffer))) > 0)
    {
        segment_cnt += 1;
        sprintf(str, "%s_%d.rsf", name, segment_cnt);
        int out_fd = open(str, O_CREAT | O_WRONLY, 0644);
        if (out_fd < 0)
        {
            debug("Error: Cannot open output file.\n");
            close(in_fd);
            return -1;
        }
        str[0] = 0x01;
        str[1] = 0x00;
        str[2] = size >> 8;
        str[3] = size & ((1 << 8) - 1);
        str[4] = 0x1f;
        str[5] = 0x40;
        str[6] = 0x00;
        str[7] = 0x00;
        write(out_fd, str, 8);
        write(out_fd, buffer, size);
        close(out_fd);
    }
    close(in_fd);
    if (argc == 3)
    {
        if (BT_open(argv[2]) != 0)
        {
            debug("Error: Cannot connect to EV3.\n");
            return -1;
        }
        for (int i = 1; i <= segment_cnt; i += 1)
        {
            debug("Uploading segment #%d...\n", i);
            char *str2 = str + sizeof(str) / 2;
            sprintf(str2, "%s_%d.rsf", name, i);
            sprintf(str, "/home/root/lms2012/prjs/sound/%s", str2);
            debug("%s\n%s\n", str, str2);
            BT_upload_file(str, str2);
        }
        BT_close();
    }
    return 0;
}