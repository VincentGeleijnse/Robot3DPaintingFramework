@ECHO OFF
:: This batch file runs pixel by pixel compare
TITLE Cropping And Scaling Render Target
ECHO Cropping And Scaling Render Target
python3 src\QuantizeIndexLab .\Intermediate\indexed_masked.png .\Intermediate\indexed_maskedeeeded.png


ECHO Image Cropping and Scaling complete

PAUSE