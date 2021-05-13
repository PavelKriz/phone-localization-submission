====================FILE STRUCTURE====================

readme.txt................brief description of the data storage contents
exe....................directory with executable form of implementation
    image_database......................................image database
    config...................................directory with configuration	
src
    impl .................................... implementation source codes
    thesis .......................... source form of work in LATEX format
text..........................................................thesis text
    thesis.pdf................................thesis text in PDF format
	
More information is placed in dedicated readme.txt files in the directory.

====================THESIS====================
The exact latex file and all files used to create the pdf file are placed in the src/thesis directory. 

====================EXECUTABLE APPLICATION====================
The "BP_Pavel_Kriz_2021.exe" executable binary file is placed in the exe directory.
The directory is already prepared in a way that the application can be run in meaningful configuration.

In the same directory is also placed the OpenCV library, which is necessary for the application to run.

The config.json file is also necessary. It serves the purpose of linking other parts of the configuration.
The default configuration files are placed in the exe/config directory.

There is also a default image database in the directory image_database, which is linked in the default configuration.

====================HOW TO USE THE APPLICATION====================
(CONSOLE output behavior) 
The application pops up a console window and image windows.
To open the next window push any key (when another window would open, it is needed to click back on the current window).
It can be then closed by pushing the standard X button in the right top corner of the console window.

(FILE output behavior)
Run the application and all will be done automatically.
Information about the process's success or any errors is displayed in the console window, which pops up on application start.
It can be then closed by pushing the standard X button in the right top corner of the console window.

====================SOURCE CODE====================
The source code from which the executable binary was build is placed in the src/impl directory.
The source code is commented in the Doxygen style (documentation generator).