Framework for simulating painting 3D objects using robot arms\
Made for Embedded Systems Master Graduation Project at TU Eindhoven\
\
Creator: Vincent Geleijnse

## Prerequisites:
- Microsoft Visual Studio 2019 set up for Unreal Engine (https://docs.unrealengine.com/5.0/en-US/setting-up-visual-studio-development-environment-for-cplusplus-projects-in-unreal-engine/)
- Unreal Engine 5.01 (https://www.unrealengine.com/en-US/unreal-engine-5)

## First time setup:
1. Clone or download this repository.
2. Right-click RobotArm.uproject in the main directory and select `Generate Visual Studio project files`
3. After completion open RobotArm.uproject and select OK for any prompts that come up.
4. The Unreal Engine editor should open displaying the robot arm and a canvas.

## Running the framework
1. Open the Unreal Engine project file `RobotArm.uproject`
2. Press the play button
3. Go through the steps in the shown user interface (for the first time I recommend just picking existing image and color palette, to verify if the drawing framework functions, the positioning of the image does have to be adjusted in order to make the robot able to reach the whole path though (i.e. scale image down and center the image on the robot arm and draw on upper half of sphere works for the default canvas position), a custom image and color can be used, however, it is worth to note that the size and complexity of the image and the number of colors chosen can greatly impact the time it takes to draw and the processing time required before drawing.
4. Watch the robot draw
5. 2D representation and results are saved in  `Content/step1-2/UnrealOutput` and `Content/step1-2/Result`

## Input and Output Image Processing:
- Some input and output of the image processing can be found in the `Content/step1-2/` folder.
- `Content/step1-2/Config` contains path generation settings 
- `Content/step1-2/Input` contains the current color palettes and some existing color palettes for benchmark images (only the file named colors.txt is used by the framework)
- `Content/step1-2/Intermediate` contains data about image size and the reduced image
- `Content/step1-2/Output` contains the path generation gcode output and preview images.
- `Content/step1-2/Result` contains the logdata (e.g. jointdata and targetdata) and an overview of previous test specs, (`ConfigData.csv`) keep this file closed while drawing in the framework because it attempts to update after every completed drawing.
- `Content/step1-2/src` contains the source code for the image processing tasks and the open-source LaserGRBL software modified to automatically generate paths using the path generation settings.
- `Content/step1-2/UnrealOutput` contains the 2D representation of drawings the framework completed, and the visualized difference with the original image. 
- `Content/step1-2/UVMaps` contains masks for each canvas used, these masks are used for comparison in the case of wrapping. (only the head canvas has a distinct mask, the other UV mappings are fully mapped on the 2D square texture and are therefore just white images)

## Adjusting parameters in Unreal Engine Project:
some of the parameters can only be adjusted in the Unreal Engine Editor.
- Canvas & Pen: The canvas and pen settings can be adjusted by selecting the canvas in the scene. In the details tab parameters can be adjusted such as pen width, canvas mesh and canvas position/scale.
- Inverse kinematics settings: The inverse kinematics settings can be adjusted by finding the IKsolver actor in the actor outliner, and adjusting the parameters such as max randomizations and max iterations