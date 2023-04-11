<?php
/**
 * An example of GDIndexedColorConverter.
 *
 * Licensed under The MIT License
 *
 * @author Jeremy Yu
 * @copyright Copyright (c) 2014 Jeremy Yu
 * @license https://github.com/ccpalettes/gd-indexed-color-converter/blob/master/LICENSE
 */

require_once('GDIndexedColorConverter.php');






// the image file path
//$file_path = $argv[1];

//open a file dialog
$command = "python3 src/getInput";
$file_pathold = shell_exec($command);
$file_path = substr_replace(str_replace('/','\\',$file_pathold),"",-1);



echo "fetching color palette from colors.txt...\n";
$paletteFile = fopen(".\Input\colors.txt", "r") or die("Unable to open file!");
// Output one line until end-of-file


for ($x = 0;!feof($paletteFile); $x++) {
  $colorstr = explode(",",fgets($paletteFile));
  if(count($colorstr) == 3){	
	 $palette[$x] = array($colorstr[0],$colorstr[1],$colorstr[2]);
	  
  }
}
fclose($paletteFile);


echo "Indexing image from palette...\n";

// get image information




$image_info = getimagesize($file_path);
if (!$image_info) {
	exit('Fail to get image information.');
}


// dither amounts
$dithers = array(0.0); // we do no dither, because we dont want dots.


$image_type = $image_info[2];

if ($image_type === IMAGETYPE_JPEG || $image_type === IMAGETYPE_JPEG2000) {
	// create image
	$image = imagecreatefromjpeg($file_path);
} elseif($image_type === IMAGETYPE_PNG) {
	
	$image = imagecreatefrompng($file_path);
	$bg = imagecreatetruecolor(imagesx($image), imagesy($image));
	imagefill($bg, 0, 0, imagecolorallocate($bg, 255, 255, 255));
	imagealphablending($bg, TRUE);
	imagecopy($bg, $image, 0, 0, 0, 0, imagesx($image), imagesy($image));
	imagedestroy($image);
	$quality = 50; // 0 = worst / smaller file, 100 = better / bigger file 
	imagejpeg($bg, 'Intermediate\jpgver' . ".jpg", $quality);
	imagedestroy($bg);
	$file_path = 'Intermediate\jpgver.jpg';
	$image = imagecreatefromjpeg($file_path);

	
}
else {
	exit('The image is not JPEG or PNG format');
}

if ($image) {
	
	$width = imagesx($image);
	$height = imagesy($image);
	$imgprops = fopen('Input\ImageProperties.txt', "w");
	$widthText = "width=".strval($width)."\r\n";
	$heightText = "height=".strval($height)."\r\n";
	fwrite($imgprops, $widthText);
	fwrite($imgprops, $heightText);
	fclose($imgprops);
	
	
	// indexed color converter
	$converter = new GDIndexedColorConverter();

	// convert the image into indexed color mode
	foreach($dithers as $dither_amount) {
		$new_image = $converter->convertToIndexedColor($image, $palette, $dither_amount);

		// save the new image
		imagepng($new_image, 'Intermediate\indexed.png', 0);

		// free memory of the new image
		imagedestroy($new_image);
	}


	// free memory
	imagedestroy($image);
} else {
	exit('Fail to load the image.');
}

