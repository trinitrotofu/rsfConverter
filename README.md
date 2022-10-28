# rsfConverter

Play any audio file with your Lego EV3.

This program can convert your audio files (including `.mp3`, `.mp4`, `.wav` ...) to Lego `.rsf` files so that your EV3 can play it. If the original audio file is longer than the limitation of EV3 (about 8 seconds), then it will split it into many segments, then you can play them one by one.

This program is using [FFmpeg](https://ffmpeg.org/) for format conversion. And thank [tiebing.blogspot.com](http://tiebing.blogspot.com/2019/09/lego-ev3-sound-file-rsf-format.html) for providing the definition of `.rsf` file.

And of course, this program is using [EV3_RobotControl API](https://github.com/pacothepaco/EV3_RobotControl).

## Usage

Compile:

```shell
./compile.sh
```

Convert `test.mp3` to `.rsf` files and upload it to the EV3 with HEX id `00:16:53:56:55:D9`:

```shell
./rsfConverter test.mp3 00:16:53:56:55:D9
```

The audio files are named as `test_1.rsf`, `test_2.rsf`, ..., `test_n.rsf`. Here, `n` is the number of segments.

To play `test.mp3` (please replace `n` with the actual number of segments, and `volumn` with the volumn value between 1 and 100):

```shell
./rsfPlayer 00:16:53:56:55:D9 test n volumn
```

By the way, if you want, you can do the `.rsf` conversion without uploading to your EV3, for example:

```shell
./rsfConverter test.mp3
```
