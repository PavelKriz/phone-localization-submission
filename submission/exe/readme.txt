====================APPLICATION RUN====================
The application is ready to run.

====================NOTE TO DLL====================
This DLL is the version compiled version of OpenCV with xfeatures2D module.

====================MAIN CONFIGURATION JSON====================
{
    "reference_images" : "config/references.json", 		//File path to the JSON for the reference images filepath. (explanation of that JSON is in exe/config/readme.txt) - do not change for default configuration
    "scene_images" : "config/scenes.json",				//File path to the JSON for the scene images filepaths. (explanation of that JSON is in exe/config/readme.txt) - do not change for default configuration
    "parameters" : "config/parameters.json",			//File path to the JSON with the processing parameters (explanation of that JSON is in exe/config/readme.txt)
	"output_root" : "output/outputTesting12.5",			//File path of a output directory where files would be saved in case that FILE type output is chosen in parameters JSON.
	"run_name" : "Run1"									//Name of the application run that is used in the logs and in the names of saved files.
}
====================NOTE====================
All file paths have to be relative to the directory where the application runs (exe is the default).
The smooth running of the prototype was tested and is guaranteed only on Ascii file paths (same as the default configuration uses).