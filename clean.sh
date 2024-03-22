#!/bin/bash
ffmpeg  -framerate 60 -i out/tmp%d.png -c:v libx264 -crf 25 -vf "scale=1920:1080,format=yuv420p" -movflags +faststart output.mp4
