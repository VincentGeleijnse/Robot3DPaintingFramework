@ECHO OFF
:: This batch file runs the substeps of step 1 sequentially to create colormasks from an input image
TITLE Step1 Separate image into color masks.
ECHO ==========================
ECHO SEPARATE IMAGE INTO COLOR MASKS
ECHO ==========================
ECHO Please wait, indexing image...

php src\indexRGB.php
ECHO Image indexing complete!
ECHO Please wait, creating color masks...
PAUSE