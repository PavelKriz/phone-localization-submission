====================PARAMETERS JSON====================
{
	"output_type" : "CONSOLE", 			// possible values: FILE, CONSOLE
	"preview_result" : true, 			// possible values: true, false
	"timing_optimalisation" : false,		// possible values: true, false(recommended)
	"detection_method" : "SIFT",			// possible values: SIFT, ORB
	"description_method" : "SIFT",			// possible values: SIFT, ORB, RootSIFT, Precise_RootSIFT, BEBLID
	"features_limit" : 1000,			// possible recommended value ranges: for SIFT detection <500, 5000> (or 0 which is disabling limits, <750, 2000> recommended), for ORB detection <500, 5000> ( <750, 2000> recommended)
	"matching_method" : "BF_matching",		// possible values: BF_matching, FLANN_matching
	"ratio_test_alpha" : 0.7,			// possible values in ranges: <0.5, 1.0> (<0.7, 0.8> recommended)
	"standing_person_optimalisation" : true,	// possible values: true(recommended), false
	"find_projection_from_3D" : true,	 	// possible values: true(recommended), false
	"find_GPS" : true 				// possible values: true(recommended), false
}
Some explained values:
preview_resul - determines whether images will be created
timing_optimalisation - determines if timing optimalisations (such that lower the possibility of time changes across multiple tests, it also lowers the output richness)
features_limit - limit of detected features
ratio_test_alpha - the alpha of the Lowe's ratio test
standing_person_optimalisation - turns on/off the accuracy optimalistion in calculation of global location (GPS). It is recommended to be enabled for scenes with flat ground, which is everytime for the default image database.
find_projection_from_3D - determines if part of the output will be the volume recognision image (projected guessed bounding box of the building)
find_GPS - determines if the global location (GPS) calculation will take place
====================SCENES JSON====================
{
	"scene_index" : 0, 						// possible range: <0, N>
	"scenes" : [ 
				"filepath/scn_img0.jpg",
				"filepath/scn_img1.jpg",
				...
				"filepath/scn_imgN.jpg"
				]
}
The "scene_index" number is an index in the  "scenes" array, which contains all prepared scene images. The index determines the chosen scene image for processing.
====================REFERENCES JSON====================
{
	"filepaths" : [ 
				"filepath/ref_img0.jpg",
				"filepath/ref_img1.jpg",
				...
				"filepath/ref_imgN.jpg"
				]
}
That JSON has one array "filepaths" that contains file paths to all reference images.
====================NOTE====================
All file paths have to be relative to the directory where the application runs (exe is the default).
