@ECHO OFF

TITLE Prepare Wrapping Preview
ECHO Preparing Wrapping Preview
python3 src\PreviewWrap .\UVMaps\UV%1.png .\Intermediate\indexed.png
ECHO previewFile Complete

PAUSE
