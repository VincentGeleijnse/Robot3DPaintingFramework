@ECHO OFF
:: This batch file runs all image processing (cropping/flipping/scaling/masking) on the input and output to prepare for comparison
TITLE Prepare Wrapped Output image
ECHO Flipping UV Image
python3 src\FlipUV .\Input\UVMap.png
ECHO Flipping Complete

ECHO Cropping and scaling  Flipped UV Image
python3 src\cropScaleImage .\Input\UVMap_Flipped.png
ECHO Cropping and scaling Complete

ECHO Masking Cropped UV Image
python3 src\MaskUV .\Input\UVMap_Flipped_CropScaled.png
ECHO Masking Complete

ECHO Cropping and scaling Output Drawing
python3 src\cropScaleImage .\Result\OutputRT.png
ECHO Cropping and scaling Complete

ECHO Applying Mask to Output Drawing
python3 src\ApplyMask .\Input\UVMap_Flipped_CropScaled_Mask.png .\Result\OutputRT_CropScaled.png
ECHO Mask Application Complete

ECHO Applying Mask to (Indexed) input image
python3 src\ApplyMask .\Input\UVMap_Flipped_CropScaled_Mask.png .\Intermediate\indexed.png
ECHO Mask Application Complete

ECHO Images ready for comparison!


PAUSE