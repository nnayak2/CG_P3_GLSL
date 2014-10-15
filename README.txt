Usage:

Project and solution files are VS 2013. Static linking used for debug and release executables, so it should work without any dependency.
EXE files are in debug and release folders.(along with dependency glut32.dll)

Initial setup:
On the console, please enter parameters as asked. (how many textures and each texture name completely. ex: "xyz.bmp")
Right click in the window to get a menu and pick a texture from the options

Files:
OBJ and MTL files have to be in current directory. (NOTE: It wasn't getting loaded correctly from the inputs directory strangely.)
view.txt, lights.txt, window.txt and images should be in inputs directory, and input directory should be in current working directory.

Sample files provided: (code was tested with these files)
capsule.obj and capsule.bmp
cube-textures.obj with 1.bmp and 1.png and 2.bmp

Key Mapping:
Light toggle ->              l
Translate X and Y ->         Arrow keys
Translate Z ->               [ and ]
Scale ->                     q and w
RIGHT CLICK ->				 Get the menu to select the texture 

Limitations:
Loading obj without normals and tex coords causes not so nice output and no textures.


Credits to external sources:

1. Code used:

a. Tiny Obj Loader: (with minor modifications so materials are loaded correctly)
For parsing obj files.
https://github.com/syoyo/tinyobjloader

b. GLUT:
For creating a window and rendering the pixels from my colorbuffer.

c. GLM:
For all vector calculations.

d. SOIL
For loading all image files.
http://www.lonesock.net/soil.html

e. Healey's webpage for CSC 461
For GLUT menu code for texture selection

2. Other sources:

nehe.gamedev.net - helped me clear some doubts related to initial GL setup.

Code from my ray tracer project reused at some places, like file parsing, obj loading.