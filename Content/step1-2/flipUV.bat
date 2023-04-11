@ECHO OFF
:: This batch file runs stuff
TITLE Flip UV image
ECHO Flipping
python3 src\FlipUV .\Input\UVMap.png

ECHO Flipping Complete

PAUSE