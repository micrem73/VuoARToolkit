# VuoARToolkit
Vuo node that implements ARToolkit markers detection.

Actually it's just a node skeleton correctly importing Vuo Node SDK and ARToolkit SDK.
The main purpose of this repository is to look for contributors to reach the goal joining forces.
We're looking ad simpleLite ARToolkit example trying to port it in a Vuo node, ( https://artoolkit.org/documentation/doku.php?id=7_Examples:example_simplelite )
Ideally we'll get a result similar to Kineme ARTool Quartz Composer node: https://kineme.net/ARTools

INSTALL PROCEDURES:

- Create a 'SDKs' folder in your home folder
- Download Vuo SDK and install in ~/SDKs/vuo-1.2.5-sdk
- Download ARToolkit5 SDK ( http://www.artoolkit.org/dist/artoolkit5/5.3/ARToolKit5-bin-5.3.2r1-OSX.tar.gz ) and install it in  ~/SDKs/ARToolKit5-bin-5.3.2r1-OSX
- Download this repository and install it in ~/SDKs/vuo-1.2.5-sdk/example/node/detect
- Open detect.ARToolkit.pro file with QT creator. When it asks you which kit use, choose "Desktop".


Don't expect it can run neither compile!'