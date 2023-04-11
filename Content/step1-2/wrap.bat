@ECHO OFF
:: This batch file runs all image processing (cropping/flipping/scaling/masking) on the input and output to prepare for comparison
TITLE Prepare Wrapped Output image
ECHO Flipping UV Image
python3 src\FlipUV .\UVMaps\UV%2.png
ECHO Flipping Complete

ECHO Cropping and scaling  Flipped UV Image
python3 src\cropScaleImage .\UVMaps\UV%2_Flipped.png
ECHO Cropping and scaling Complete

ECHO Masking Cropped UV Image
python3 src\MaskUV .\UVMaps\UV%2_Flipped_CropScaled.png
ECHO Masking Complete

ECHO Cropping and scaling Output Drawing
python3 src\cropScaleImage .\UnrealOutput\%1.png
ECHO Cropping and scaling Complete

ECHO Applying Mask to Output Drawing
python3 src\ApplyMask .\UVMaps\UV%2_Flipped_CropScaled_Mask.png .\UnrealOutput\%1_CropScaled.png
ECHO Mask Application Complete

ECHO Applying Mask to (Indexed) input image
python3 src\ApplyMask .\UVMaps\UV%2_Flipped_CropScaled_Mask.png .\Intermediate\indexed.png
ECHO Mask Application Complete

ECHO Images ready for comparison!

ECHO Robot Arm Wrap Output test (compare image drawn by robot arm with Wrap method with original indexed image)

python3 src\PixelByPixelCompare .\Intermediate\indexed_masked.png .\UnrealOutput\%1_CropScaled_masked.png

ECHO Image Comparison Complete

ECHO Robot Arm Wrap Output test (Weighted compare image drawn by robot arm with Wrap method with original indexed image)

python3 src\PixelByPixelCompareWeighted .\Intermediate\indexed_masked.png .\UnrealOutput\%1_CropScaled_masked.png

ECHO Image Comparison Complete


PAUSE