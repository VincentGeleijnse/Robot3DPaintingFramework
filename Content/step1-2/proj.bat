@ECHO OFF
:: This batch file runs pixel by pixel compare
TITLE Comparing two images pixel wise.


ECHO Robot Arm Projection Output test (compare image drawn by robot arm with Projection method with original indexed image)

python3 src\PixelByPixelCompare .\UnrealOutput\%1_original.png .\UnrealOutput\%1.png

ECHO Image Comparison Complete

ECHO Robot Arm Projection Weighted Output test (Weighted compare image drawn by robot arm with Projection method with original indexed image)

python3 src\PixelByPixelCompareWeighted .\UnrealOutput\%1_original.png .\UnrealOutput\%1.png

ECHO Image Comparison Complete

PAUSE