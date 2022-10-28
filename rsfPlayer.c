#include "EV3_RobotControl/btcomm.h"

#define debug(...) fprintf(stderr, __VA_ARGS__)

char str[1024];

int main(int argc, char const *argv[])
{
    if (argc != 5)
    {
        debug("Error: Invalid argc!\n");
        return -1;
    }
    if (BT_open(argv[1]) != 0)
    {
        debug("Error: Cannot connect to EV3.\n");
        return -1;
    }
    int segment_cnt, volumn;
    sscanf(argv[3], "%d", &segment_cnt);
    sscanf(argv[4], "%d", &volumn);
    for (int i = 1; i <= segment_cnt; i += 1)
    {
        sprintf(str, "/home/root/lms2012/prjs/sound/%s_%d", argv[2], i);
        BT_play_sound_file(str, volumn);
        if (i < segment_cnt)
            sleep(8);
    }
    BT_close();
    return 0;
}
