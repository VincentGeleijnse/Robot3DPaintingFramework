@ECHO OFF
:: This batch file runs the substeps of step 1 sequentially to create colormasks from an input image
TITLE Step1 Separate image into color masks.
ECHO ==========================
ECHO SEPARATE IMAGE INTO COLOR MASKS
ECHO ==========================
ECHO Please wait, indexing image...

php src\index.php
ECHO Image indexing complete!
ECHO Please wait, creating color masks...
python3 src\color-mask .\Intermediate\indexed.png .\Output\colorMask
ECHO Color masks complete!
ECHO Vectorizing and Generating G-Code using settings stored in \Config\config.txt...

for %%j in (Output\*.png) do (
	ECHO Generating G-Code for %%j
 
	src\LaserGRBL\LaserGRBL\bin\Debug\LaserGRBL.exe %%j
	
	
)
ECHO G-Code files generated! they can be found as .txt files in the Output folder. 
ECHO G-Code paths can be previewed in the Preview\GCode folder.
PAUSE