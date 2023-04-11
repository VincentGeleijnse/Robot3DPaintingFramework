@ECHO OFF
:: This batch file runs pixel by pixel compare
TITLE Comparing two images pixel wise.
ECHO Metric accuracy test (screenshot of image projected directly onto geometry compared with original indexed image)
python3 src\PixelByPixelCompare .\Intermediate\indexed.png .\Result\AccuracyTest.png

ECHO Robot Arm Projection Output test (compare image drawn by robot arm with Projection method with original indexed image)

python3 src\PixelByPixelCompare .\Intermediate\indexed.png .\Result\output.png


ECHO Robot Arm Wrapping Output test (compare image drawn by robot arm with wrapping method with original indexed image)

python3 src\PixelByPixelCompare .\Intermediate\indexed_masked.png .\Result\OutputRT_CropScaled_masked.png

ECHO Image Comparison Complete

PAUSE