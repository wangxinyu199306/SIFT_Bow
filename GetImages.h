

//Notification: class FileProcess is for geting all the images under a giving path, including the subfolder of this path
//Author: Xinyu, Wang
//Date: 2018/7/22


#include <iostream>
#undef UNICODE
#include <windows.h>
#include "opencv.hpp"

using namespace std;

class FileProcess
{
public:

	//traverse all the files in <folderpath>, and return the filename list<fileList> and the corresponding path of this file<prefixList>
	//access a file by (prefixList[i] + fileList[i]) 
	void get_files_under_folder(char* folderPath, vector<string> &fileList, vector<string>& prefixList);

	//read the image by <fileList>
	//<labels> is the first character of filename, modify if it isn't
	void get_gray_images_and_labels(vector<string> prefixList, vector<string> fileList, vector<cv::Mat>& imageList, vector<int>& labels);
};