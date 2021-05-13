//----------------------------------------------------------------------------------------
/**
 * \file       COstreamLogger.h
 * \author     Pavel Kriz
 * \date       9/4/2021
 * \brief      Contains virtual class for logging output from the app using streams
 *
 *  It implements the text printing methods to the stream that is given by virtual method.
 *
*/
//----------------------------------------------------------------------------------------

#pragma once

#include <iostream>
#include <iomanip>
#include <utility>
//core functions
#include <opencv2/core.hpp>

#include "CLogger.h"

using namespace std;

/**
 * @brief Class implementing part of the CLogger,
 * 
 * It implements the part of printing the text. It prints the text to the stream given by virtual method.
 * The stream for images is implemented as a vector that is a sort of buffer for the images (the order is not guaranteed)
 * 
*/
class COstreamLogger : public CLogger {
protected:
	unsigned int currentSectionLevel_ = 0; ///< determines in what level is the current state of printing
	vector<pair<Mat, string>> images_; ///< printing que with images - it 

	/**
	 * @brief Virtual method that is used to get the output stream (from standard library) to print text into it
	 * @return the output stream for printing
	*/
	inline virtual ostream& out() = 0;
	/**
	 * @brief method that allows implementations to handle start of the error
	*/
	virtual void emergencyStart() { return; }
	/**
	 * @brief method that allows implementations to handle end of the error
	*/
	virtual void emergencyEnd() { return; }
public:
	/**
	 * @brief Constructor of the class
	 * @param timing information whether the timing optimalization will take place and the images wont be saved
	*/
	COstreamLogger(bool timing) : CLogger(timing){}
	/**
	 * @brief default virtual destructor
	*/
	virtual ~COstreamLogger() override { }
	/**
	 * @brief Logs the begening of the section (prints its name). It should be used when starting a section
	 * 
	 * It prints using the stream given by its child method implementation
	 * 
	 * @param name name of the section
	 * @param level level of the section and the header. There are three options (highest level > 0,1,2 < lowest level), 0,1 are printed with capitals
	 * @return returns reference on this CLogger
	*/
	virtual CLogger& logSection(const string& name, unsigned int level = 0) override;
	/**
	 * @brief Logs in the current section with the right indent.
	 * 
	 * It prints using the stream given by its child method implementation
	 * 
	 * @param toLog string with text to be logged
	 * @return reference on this CLogger
	*/
	virtual CLogger& log(const string& toLog) override;
	/**
	 * @brief method used for logging matrices
	 *
	 * if the matrix is column vector, then the vector is transposed in printing
	 *
	 * @param toLog matrix that has to be logged
	 * @return reference on this CLogger
	*/
	virtual CLogger& log(const Mat& toLog) override;
	/**
	 * @brief method used for logging 2d opencv points
	 * @param toLog point that has to be logged
	 * @return reference on this CLogger
	*/
	virtual CLogger& log(const Point2d& toLog) override;
	/**
	 * @brief method used for logging 3d opencv points
	 * @param toLog point that has to be logged
	 * @return reference on this CLogger
	*/
	virtual CLogger& log(const Point3d& toLog) override;
	/**
	 * @brief method used for logging global coordinates (SGcsCoords structs)
	 * @param toLog coordinates that has to be logged
	 * @return reference on this CLogger
	*/
	virtual CLogger& log(const sm::SGcsCoords& toLog) override;
	/**
	 * @brief Method specialised for logging errors
	 * @param toLog string with error message to be logged
	 * @return reference on this CLogger
	*/
	virtual CLogger& logError(const string& toLog) override;
	/**
	 * @brief Does the right end of the line in the style of the logger.
	 * 
	 * It prints using the stream given by its child method implementation
	 * 
	 * @return reference on this CLogger
	*/
	virtual CLogger& endl() override;
	/**
	 * @brief Puts the image to the output queue to be logged (printed or saved)
	 * @param image image to be logged (printed or saved)
	 * @param outputPath filepath in regard to the place where the app runs
	*/
	virtual void putImage(const Mat& image, const string& name) override;
	/**
	 * @brief Flushes every images or text to the output (interface)
	*/
	virtual void flush() override = 0;
};

