@ECHO OFF
:: This batch file runs pixel by pixel compare
TITLE Cropping And Scaling Render Target
ECHO Cropping And Scaling Render Target
python3 src\cropScaleRenderTarget .\Result\OutputRT.png

ECHO Cropping And Scaling UV Map Mask

python3 src\cropScaleRenderTarget .\Input\UVMap_Mask.png

ECHO Image Cropping and Scaling complete

PAUSE