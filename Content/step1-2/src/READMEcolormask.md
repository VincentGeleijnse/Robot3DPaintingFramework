# color-mask
This is a script that will mask a PNG image with a specified color. The default option will replace all non-transparent pixels with the inputed color, which is handy for editing single color icons. You can also replace a specified color inside the image using the `-r` (*replace*) option.

## Get Started
```
# Setup
pip3 install -r requirements.txt





#actual usage
> Create a mask for each unique color present in the paletted input image
 python3 color-mask PATH_TO_INPUT_FILE PATH_FOR_OUTPUT (no extension on output path BUT do add it on input file)
    (The Input file should be a paletted PNG image)

# old Usage
> Replace non-transparent pixels with an inputed color.
 ./color-mask ./PATH_TO_INPUT_FILE ./PATH_FOR_OUTPUT '#HEXCODE' [OPTIONS]
    (The Input file should be a PNG image)

    * OPTIONS:
      -s            : Show a preview of the new Image at the end.
      -r '#HEXCODE' : Replace only pixels matching the following HEXCODE.
      -rd '#HEXCODE' : Replace pixels darker than the following HEXCODE.
      -rl '#HEXCODE' : Replace pixels lighter than the following HEXCODE.


> List all colors contained in an image.
 ./color-mask colors ./PATH_TO_IMAGE_FILE
    (The Image file should be a PNG image)

```

## Hex Codes
All inputed hexcodes must begin with `#`. A hexcodes can be inputed as a 3, 6 or 8 digit code.

### Hex Code Formats:
 - **3 digits**: `#RGB`
 - **6 digits**: `#RRGGBB`
 - **8 digits**: `#RRGGBBAA`

### Examples:
 - **`#ddd`**      **-->** `rgba(221, 221, 221, 255)`
 - **`#2e2e2e`**   **-->** `rgba(46, 46, 46, 255)`
 - **`#ff0000aa`** **-->** `rgba(255, 0, 0, 170)`
