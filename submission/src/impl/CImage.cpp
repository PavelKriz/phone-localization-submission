#include "CImage.h"

//=================================================================================================

CImage::CDetectorExtractor::CDetectorExtractor(const SProcessParams& params) {
	if (params.detectMethod_ == params.describeMethod_) {
		detector_ = getDetectorExtractor(params);
		extractor_ = detector_;
	}
	else {
		detector_ = getDetector(params);
		extractor_ = getExtractor(params);
	}
}

Ptr<Feature2D> CImage::CDetectorExtractor::getDetector(const SProcessParams& params)
{
	Ptr<Feature2D> detector;
	switch (params.detectMethod_)
	{
	case EAlgorithm::ALG_ROOTSIFT:
	case EAlgorithm::ALG_PRECISE_ROOTSIFT:
	case EAlgorithm::ALG_SIFT:
		detector = SIFT::create(params.siftParams_.nfeatures_,
								params.siftParams_.nOctaveLayers_,
								params.siftParams_.contrastThreshold_,
								params.siftParams_.edgeTreshold_,
								params.siftParams_.sigma_);
		break;
	case EAlgorithm::ALG_ORB:
		detector = ORB::create(params.orbParams_.nfeatures_,
							   params.orbParams_.scaleFactor_,
							   params.orbParams_.nlevels_,
							   params.orbParams_.edgeTreshold_,
							   params.orbParams_.firstLevel_,
							   params.orbParams_.WTA_K_,
							   params.orbParams_.scoreType_,
							   params.orbParams_.patchSize_,
							   params.orbParams_.fastTreshold_);
		break;
	default:
		throw invalid_argument("Error feature detecting method was used! (non recognized method)");
		break;
	}
	return detector;
}

Ptr<Feature2D> CImage::CDetectorExtractor::getExtractor(const SProcessParams& params)
{
	Ptr<Feature2D> extractor;
	switch (params.describeMethod_)
	{
	case EAlgorithm::ALG_ROOTSIFT:
	case EAlgorithm::ALG_PRECISE_ROOTSIFT:
	case EAlgorithm::ALG_SIFT:
		extractor = SIFT::create(params.siftParams_.nfeatures_,
								 params.siftParams_.nOctaveLayers_,
								 params.siftParams_.contrastThreshold_,
								 params.siftParams_.edgeTreshold_,
								 params.siftParams_.sigma_);
		break;
	case EAlgorithm::ALG_ORB:
		extractor = ORB::create(params.orbParams_.nfeatures_,
								params.orbParams_.scaleFactor_,
								params.orbParams_.nlevels_,
								params.orbParams_.edgeTreshold_,
								params.orbParams_.firstLevel_,
								params.orbParams_.WTA_K_,
								params.orbParams_.scoreType_,
								params.orbParams_.patchSize_,
								params.orbParams_.fastTreshold_);
		break;
#ifdef COMPILE_EXPERIMENTAL_MODULES_ENABLED
	case EAlgorithm::ALG_BEBLID:
		extractor = xfeatures2d::BEBLID::create(params.beblidParams_.scale_factor_,
												params.beblidParams_.n_bits_);
		break;
#endif
	default:
		throw invalid_argument("Error feature detecting method was used! (non recognized method)");
		break;
	}
	return extractor;
}

Ptr<Feature2D> CImage::CDetectorExtractor::getDetectorExtractor(const SProcessParams& params)
{
	Ptr<Feature2D> detectorExtractor;
	switch (params.detectMethod_)
	{
	case EAlgorithm::ALG_ROOTSIFT:
	case EAlgorithm::ALG_PRECISE_ROOTSIFT:
	case EAlgorithm::ALG_SIFT:
		detectorExtractor = SIFT::create(params.siftParams_.nfeatures_,
										 params.siftParams_.nOctaveLayers_,
										 params.siftParams_.contrastThreshold_,
										 params.siftParams_.edgeTreshold_,
										 params.siftParams_.sigma_);
		break;
	case EAlgorithm::ALG_ORB:
		detectorExtractor = ORB::create(params.orbParams_.nfeatures_,
										params.orbParams_.scaleFactor_,
										params.orbParams_.nlevels_,
										params.orbParams_.edgeTreshold_,
										params.orbParams_.firstLevel_,
										params.orbParams_.WTA_K_,
										params.orbParams_.scoreType_,
										params.orbParams_.patchSize_,
										params.orbParams_.fastTreshold_);
		break;
	default:
		throw invalid_argument("Error feature detecting method was used! (non recognized method)");
		break;
	}
	return detectorExtractor;
}

void CImage::detectDescribeFeatures(const SProcessParams & params, Ptr<CLogger>& logger, const Ptr<CDetectorExtractor>& detectorExtractor)
{
	if (params.detectMethod_ == params.describeMethod_ ||
		(params.detectMethod_ == EAlgorithm::ALG_SIFT && params.describeMethod_ == EAlgorithm::ALG_ROOTSIFT)) {
		if (detectorExtractor.empty()) {
			throw invalid_argument("Method detectDescribeFeatures was called with empty pointer to CDetectorExtractor");
		}
		detectorExtractor->detector_->detectAndCompute(image_, noArray(), imageKeypoints_, keypointsDescriptors_);
		logger->endl().log("  Detection and description done, keypoints count: ").log(to_string(imageKeypoints_.size())).endl();
		wasProcessed_ = true;
	}
	//bit slower then previous version (OpenCV implementation is slower when it is done splited - first detecting and then extracting)
	else {
		detectorExtractor->detector_->detect(image_, imageKeypoints_);
		detectorExtractor->extractor_->compute(image_, imageKeypoints_, keypointsDescriptors_);
		logger->endl().log("  Detection and description done, keypoints count: ").log(to_string(imageKeypoints_.size())).endl();
		wasProcessed_ = true;
	}
}

//=================================================================================================

void CImage::processCLAHE(Ptr<CLogger>& logger)
{
	//TODO think about the parameters for clahe
	Ptr<CLAHE> clahePtr = createCLAHE();
	//wikipedia:  Common values limit the resulting amplification to between 3 and 4. 
	//from a test I got a better result with 3
	clahePtr->setClipLimit(3);
	clahePtr->apply(image_, image_);

	logger->log("  CLAHE was done on the image.");
}

//=================================================================================================

void CImage::fastRootSiftDescriptorsAdjust(Ptr<CLogger>& logger)
{
	double eps = 1e-7;
	//for every descriptor do some processing
	for (size_t i = 0; i < keypointsDescriptors_.rows; ++i) {

		//L1 normalization
		double norm1Sum = 0;
		for (size_t j = 0; j < keypointsDescriptors_.cols; ++j) {
			//the values in the descriptor vector should be in range [0,1]
			norm1Sum += keypointsDescriptors_.at<float>(i, j);
		}

		double norm2Sum = 0;

		//converting the descriptor to the Hellinger kernel
		for (size_t j = 0; j < keypointsDescriptors_.cols; ++j) {
			//no problem here with the sqrt since the descriptopr will be always positive
			(keypointsDescriptors_.at<float>(i, j) = sqrt((keypointsDescriptors_.at<float>(i, j) + eps) / norm1Sum));
			norm2Sum += (keypointsDescriptors_.at<float>(i, j) * keypointsDescriptors_.at<float>(i, j));
		}

		norm2Sum += eps;

		//L2 normalisation
		for(size_t j = 0; j < keypointsDescriptors_.cols; ++j) {
			keypointsDescriptors_.at<float>(i, j) = keypointsDescriptors_.at<float>(i, j) / norm2Sum;
		}
	}

	logger->log("the descriptors have been adjusted with the rootSIFT processing").endl();
}

void CImage::preciseRootSiftDescriptorsAdjust(Ptr<CLogger>& logger)
{
	double eps = 1e-7;

	Mat floatDescriptors;
	keypointsDescriptors_.convertTo(floatDescriptors, CV_64F);

	//for every descriptor do some processing
	for (size_t i = 0; i < floatDescriptors.rows; ++i) {

		double norm1Sum = 0;
		for (size_t j = 0; j < floatDescriptors.cols; ++j) {
			//the values in the descriptor vector should be in range [0,1]
			norm1Sum += floatDescriptors.at<double>(i, j);
		}

		double norm2Sum = 0;
		//converting the descriptor to the Hellinger kernel
		for (size_t j = 0; j < floatDescriptors.cols; ++j) {
			//no problem here with the sqrt since the descriptopr will be always positive
			(floatDescriptors.at<double>(i, j) = sqrt((floatDescriptors.at<double>(i, j) + eps) / norm1Sum));
			norm2Sum += (floatDescriptors.at<double>(i, j) * floatDescriptors.at<double>(i, j));
		}

		norm2Sum += eps;

		//L2 normalisation
		for (size_t j = 0; j < floatDescriptors.cols; ++j) {
			//the values in the descriptor vector should be in range [0,1]
			keypointsDescriptors_.at<float>(i, j) = (float)floatDescriptors.at<double>(i, j) / norm2Sum;
		}
	}

	logger->log("the descriptors have been adjusted with the rootSIFT processing").endl();
}

//=================================================================================================

CImage::CImage(const string& filePath, const sm::SGcsCoords& rightBaseGc, const sm::SGcsCoords& leftBaseGc)
	:
	filePath_(filePath),
	rightBaseGc_(rightBaseGc),
	leftBaseGc_(leftBaseGc)
{
	image_ = imread(filePath, CV_8U);
	if (image_.empty())
	{
		throw ios_base::failure("Can't load image with file path: " + filePath);
	}
}

Ptr<CImage::CDetectorExtractor> CImage::createDetectorExtractor(const SProcessParams& params)
{
	return Ptr<CImage::CDetectorExtractor>(new CImage::CDetectorExtractor(params));
}

void CImage::process(const SProcessParams& params, Ptr<CLogger>& logger, const Ptr<CDetectorExtractor>& detectorExtractor)
{
	logger->log("Image with filepath: " + filePath_ + " is being processed.").endl();
	processCLAHE(logger);
	detectDescribeFeatures(params, logger, detectorExtractor);
	if (params.describeMethod_ == EAlgorithm::ALG_ROOTSIFT) {
		fastRootSiftDescriptorsAdjust(logger);
	}
	else if (params.describeMethod_ == EAlgorithm::ALG_PRECISE_ROOTSIFT) {
		preciseRootSiftDescriptorsAdjust(logger);
	}
}

const vector<KeyPoint>& CImage::getKeypoints() const
{
	if (!wasProcessed_) {
		throw logic_error("CImage - to get keypoints first the process function has to be called.");
	}
	return imageKeypoints_;
}

const Mat& CImage::getDescriptors() const
{
	if (!wasProcessed_) {
		throw logic_error("CImage - to get descriptors of the keypoints first the process function has to be called.");
	}
	return keypointsDescriptors_;
}

sm::SGcsCoords CImage::getRightBaseGc() const
{
	return rightBaseGc_;
}

sm::SGcsCoords CImage::getLeftBaseGc() const
{
	return leftBaseGc_;
}

//=================================================================================================
