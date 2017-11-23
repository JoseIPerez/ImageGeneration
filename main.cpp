#include <iostream>
#include <stdio.h>
#include "Frame.h"
#include "Image.h"
#include <fstream>

typedef unsigned char byte;

using namespace std;

const double frames_per_second = 30;
const int duration_in_seconds = 3;

Frame frame(720, 480);

int main(int argc, char * argv[]) {
    // Construct the ffmpeg command to run.
    const char * cmd =
    "ffmpeg              "
    "-y                  "
    "-hide_banner        "
    "-f rawvideo         " // input to be raw video data
    "-pixel_format rgb24 "
    "-video_size 720x480 "
    "-r 60               " // frames per second
    "-i -                " // read data from the standard input stream
    "-pix_fmt yuv420p    " // to render with Quicktime
    "-vcodec mpeg4       "
    "-an                 " // no audio
    "-q:v 5              " // quality level; 1 <= q <= 32
    "output.mp4          ";
    
    // Run the ffmpeg command and get pipe to write into its standard input stream.
    FILE * pipe = popen(cmd, "w");
    if (pipe == 0) {
        cout << "error: " << strerror(errno) << endl;
        return 1;
    }
    Image img1;
    Image img2;
    Image img3;
    
    img2.getX(300.0);
    img2.getY(150.0);
    img3.getX(0.0);
    img3.getY(150.0);
    
    img1.setSpeed(100, 100);
    img2.setSpeed(0, 100);
    img3.setSpeed(100, 0);
    
    img2.load("penguin.bmp", 150, 150);
    img1.load("tinyfin.bmp", 150, 150);
    img3.load("wisp.bmp", 150, 150);
    
    int num_frames = duration_in_seconds * frames_per_second;
    for (int i = 0; i < num_frames; ++i) {
        double dt = 1.0 / frames_per_second;
        frame.clear();
        img2.draw();
        img1.draw();
        img3.draw();
        img1.update(dt);
        img2.update(dt);
        img3.update(dt);
        
        frame.write(pipe);
    }
    
    fflush(pipe);
    pclose(pipe);
    
    cout << "num_frames: " << num_frames << endl;
    cout << "Done." << endl;
    
    return 0;
}
