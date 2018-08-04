// BowTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <windows.h>
#include "opencv.hpp"
#include "GetImages.h"
#include "FeatureDetector.h"
#include "nonfree/nonfree.hpp"
#include "nonfree/features2d.hpp"


using namespace std;

FileProcess fp;
Featuredetector fd;



int _tmain(int argc, _TCHAR* argv[])
{
	char *trainFolder = "D:\\DataSet\\CIFAR10_DATA\\train";
	vector<string> fileList;
	vector<string> prefixList; //prefix path
	vector<cv::Mat> grayImageList;
	vector<int> labels;
	fp.get_files_under_folder(trainFolder, fileList, prefixList);
	fp.get_gray_images_and_labels(prefixList, fileList, grayImageList, labels);


	vector<vector<cv::KeyPoint>> keypoints;
	vector<cv::Mat> descriptors;
	int clusters_num = 200;
	cv::Mat dictionary;
	vector<cv::Mat> BOWdescriptors;
	vector<vector<vector<int>>> pointIdxsOfClusters;
	fd.get_sift_points(grayImageList, keypoints, descriptors);
	cout << descriptors.size() << endl;
	fd.feature_cluster(descriptors, clusters_num, dictionary);
	fd.bow_for_each_image(dictionary, grayImageList, keypoints, descriptors, pointIdxsOfClusters);
	cout << pointIdxsOfClusters.size() << " " << pointIdxsOfClusters[0].size() << " " << pointIdxsOfClusters[0][0].size() << endl;

	return 0;
}

