
#include "stdafx.h"
#include "FeatureDetector.h"
#include "nonfree/nonfree.hpp"
#include "nonfree/features2d.hpp"

void Featuredetector::get_sift_points(vector<cv::Mat> images, vector<vector<cv::KeyPoint>>& keypoints, vector<cv::Mat>& descriptors)
{

	for (int i = 0; i < images.size(); i++)
	{
		cv::SiftFeatureDetector detector;
		cv::SiftDescriptorExtractor extractor;
		vector<cv::KeyPoint> tempFeaturePoints;
		tempFeaturePoints.resize(100);
		cv::Mat tempDescriptor;
		detector.detect(images[i], tempFeaturePoints);
		extractor.compute(images[i], tempFeaturePoints, tempDescriptor);
		descriptors.push_back(tempDescriptor);
		keypoints.push_back(tempFeaturePoints);
	}
}


void Featuredetector::feature_cluster(vector<cv::Mat>& descriptors, int clusters_num, cv::Mat& dictionary)
{
	cv::BOWKMeansTrainer trainer(clusters_num);
	for (int i = 0; i < descriptors.size(); i++)
	{
		if (descriptors[i].empty())
		{
			cout << 0 << endl;
		}
		else
			trainer.add(descriptors[i]);
	}
	dictionary = trainer.cluster();
}

void Featuredetector::bow_for_each_image(cv::Mat dictionary, vector<cv::Mat> images, vector<vector<cv::KeyPoint>>& keypoints, vector<cv::Mat>& BOWdescriptors, vector<vector<vector<int>>>& pointIdxsOfClusters)
{
	cv::Ptr<cv::DescriptorExtractor> extractor = cv::DescriptorExtractor::create("SIFT");
	cv::Ptr<cv::DescriptorMatcher> matcher = cv::DescriptorMatcher::create("BruteForce");
	cv::BOWImgDescriptorExtractor bowOp(extractor, matcher);
	bowOp.setVocabulary(dictionary);
	for (int i = 0; i < images.size(); i++)
	{
		cv::Mat bowDescriptor;
		vector<vector<int>> point_label;
		bowOp.compute(images[i], keypoints[i], bowDescriptor, &point_label);
		pointIdxsOfClusters.push_back(point_label);
		normalize(bowDescriptor, bowDescriptor, 1.0, 0.0, cv::NORM_MINMAX);
		BOWdescriptors.push_back(bowDescriptor);
	}
}