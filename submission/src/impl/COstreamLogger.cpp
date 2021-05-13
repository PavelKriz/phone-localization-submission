#include "COstreamLogger.h"

CLogger& COstreamLogger::logSection(const string& name, unsigned int level) {
	//crop max level
	if (level > 2)
		level = 2;
	currentSectionLevel_ = level;

	//make the name in upper case
	string upperName = name;
	if (level < 2) {
		for (auto& c : upperName)
			c = toupper(c);
	}

	out() << std::endl;

	for (size_t i = 0; i < (size_t)level + 1; ++i) {
		out() << "===";
	}

	out() << upperName;

	for (size_t i = 0; i < (size_t)level + 1; ++i) {
		out() << "===";
	}

	endl();

	return *this;
}

CLogger& COstreamLogger::log(const string& toLog) {
	out() << toLog;
	return *this;
}

CLogger& COstreamLogger::log(const Mat& toLog)
{
	//checking if the Mat is vector
	if ((toLog.rows == 1 || toLog.cols == 1) && toLog.dims == 2) {
		if (toLog.cols == 1) {
			//vector has to be transposed
			out() << toLog.t() << "^T";
		}
		else {
			out() << toLog.t();
		}
	}
	//it is some non-vector matrix
	else {
		stringstream sOut;
		sOut << toLog;
		string line;
		vector<string> lines;
		while (std::getline(sOut, line)) {
			lines.push_back(line);
		}
		for (size_t i = 0; i < lines.size(); ++i) {
			log("[").log(lines[i]);
			if (i < lines.size() - 1) {
				endl();
			}
		}
	}

	return *this;
}

CLogger& COstreamLogger::log(const Point2d& toLog)
{
	out() << toLog;
	return *this;
}

CLogger& COstreamLogger::log(const Point3d& toLog)
{
	out() << toLog;
	return *this;
}

CLogger& COstreamLogger::log(const sm::SGcsCoords& toLog)
{
	streamsize prevPrecision = out().precision();
	out() << setprecision(9); //commonly used precsion for global coordinates
	out() << "[ latitude, longitude]: " << toLog.latitude_ << ", " << toLog.longitude;
	out().precision(prevPrecision);
	return *this;
}

CLogger& COstreamLogger::logError(const string& toLog)
{
	emergencyStart();
	int tmpLvl = currentSectionLevel_;
	logSection("Error", 0);
	log(toLog);
	out() << std::endl;
	log("===========================");
	
	currentSectionLevel_ = tmpLvl;
	endl();

	emergencyEnd();
	return *this;
}

CLogger& COstreamLogger::endl() {
	out() << std::endl;
	for (int i = 0; i < currentSectionLevel_; ++i) out() << "    ";
	return *this;
}

void COstreamLogger::putImage(const Mat& image, const string& name)
{
	if (putImages_) {
		images_.push_back(make_pair(image, name));
	}
}

