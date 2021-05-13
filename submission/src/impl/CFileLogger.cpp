#include "CFileLogger.h"


string CFileLogger::correctOutputRoot(const string& toCorrect)
{
	if (toCorrect.back() != '/' && toCorrect.back() != '\\' ) {
		return toCorrect + "/";
	}
	else {
		return toCorrect;
	}
}

void CFileLogger::checkAndCreateOutputDir()
{
	try {
		boost::filesystem::create_directories(outputRoot_);
	}
	catch (exception& e) {
		throw ios_base::failure(string("File output failed! System message: ") + e.what());
	}
}

CFileLogger::CFileLogger(const string& outputRoot, const string& runName, bool timing)
	: COstreamLogger(timing),
		out_(outputRoot + runName + ".txt", ofstream::trunc),
	outputRoot_(correctOutputRoot(outputRoot)),
	runName_(runName)
{
}

void CFileLogger::flush()
{
	//write text to the file
	ofstream txtOut;

	string outTxtName = outputRoot_ + runName_ + ".txt";
	if (wasFlushed_) {
		checkAndCreateOutputDir();
		txtOut.open(outTxtName, ofstream::app);
	}
	else {
		checkAndCreateOutputDir();
		txtOut.open(outTxtName, ofstream::trunc);
	}

	if (txtOut.good()) {
		txtOut << out_.str();
		//flush is called on close automatically
	}
	else {
		string text = string("ERROR: Error occured when trying to open a file.\n") +
			"Problem occured in file: )" + outTxtName;
		logError(text);
	}

	//flush is called on close automatically
	txtOut.close();
	out_.str(std::string());


	//write images to the files (OpenCV functions)
	for(size_t i = 0; i < images_.size(); ++i){
		string outImageName = outputRoot_ + runName_ + "_img" + to_string(i) + ".jpg";
		int result = imwrite(outImageName, images_[i].first);
		//handle errors
		if (!result) {
			string text = string("ERROR: Error occured when trying to save an image.\n") +
				"Problem occured when saving image: )" + outImageName;
			logError(text);
		}
	}
	
	images_.clear();
	wasFlushed_ = true;
}