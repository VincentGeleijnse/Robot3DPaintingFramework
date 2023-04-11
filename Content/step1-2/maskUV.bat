@ECHO OFF
:: This batch file runs stuff
TITLE Create mask from UV map
ECHO Cropping And Scaling
python3 src\MaskUV 

ECHO Masking UV Complete

PAUSE