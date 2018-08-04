

#include <opencv.hpp>
#include <iostream>
#include "nonfree/nonfree.hpp"
#include "nonfree/features2d.hpp"
using namespace std;

class Featuredetector
{
public:
	//compute the sift point for a batch of images, <images.size()> = batch_size
	//<keypoints.size()> = batch_size
	//<keypoints[0].size()> = number of sift points in a image
	//<descriptors.size()> = batch_size
	//<descriptors[0].rows> = number of sift points in a image
	//<descriptors[0].cols> = 128
	void get_sift_points(vector<cv::Mat> images, vector<vector<cv::KeyPoint>>& keypoints, vector<cv::Mat>& descriptors);

	//<descriptors.size()> = batch_size
	//<descriptors[0].rows> = number of sift points in a image
	//<descriptors[0].cols> = 128
	//<clusters_num> = number of classes intened to cluster, example:200
	//<dictionary>: the center point of each class
	//<dictionary.rows> = <clusters_num>
	//<dictionary.cols> = 128
	void feature_cluster(vector<cv::Mat>& descriptors, int clusters_num, cv::Mat& dictionary);

	//compute the bow vector for each image
	//<pointIdxsOfClusters.size()> = batch_size
	//<pointIdxsOfClusters[0].size()> = <clusters_num>
	//<pointIdxsOfClusters[0][0].size()> = the count of sift points in this image shows in this image
	void bow_for_each_image(cv::Mat dictionary, vector<cv::Mat> images, vector<vector<cv::KeyPoint>>& keypoints, vector<cv::Mat>& BOWdescriptors, vector<vector<vector<int>>>& pointIdxsOfClusters);
};