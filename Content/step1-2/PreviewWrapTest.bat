@ECHO OFF
:: This batch file runs all image processing (cropping/flipping/scaling/masking) on the input and output to prepare for comparison
ECHO scaling inpt
python3 src\PreviewWrap .\Input\UVMap.png .\Intermediate\indexed.png
ECHO scale complete

ECHO Masking Cropped UV Image
python3 src\MaskUV .\Input\UVMap.png
ECHO Masking Complete

ECHO Applying Mask to (Indexed) input image
python3 src\ApplyMask .\Input\UVMap_Mask.png .\Intermediate\indexed_Scaled.png
ECHO Mask Application Complete

ECHO Images ready for comparison!


PAUSE