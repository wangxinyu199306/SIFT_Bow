

#include "stdafx.h"
#include "GetImages.h"
#include "opencv.hpp"

using namespace std;

/*
typedef struct _WIN32_FIND_DATA
{
	DWORD dwFileAttributes; //文件属性
	FILETIME ftCreationTime; // 文件创建时间
	FILETIME ftLastAccessTime; // 文件最后一次访问时间
	FILETIME ftLastWriteTime; // 文件最后一次修改时间
	DWORD nFileSizeHigh; // 文件长度高32位
	DWORD nFileSizeLow; // 文件长度低32位
	DWORD dwReserved0; // 系统保留
	DWORD dwReserved1; // 系统保留
	TCHAR cFileName[MAX_PATH]; // 长文件名（最多可达 255 个字符的长文件名），带句点和扩展名
	TCHAR cAlternateFileName[14]; //8.3格式文件名（句点前最多有8个字符，而扩展名最多可以有3个字符）
} WIN32_FIND_DATA, *PWIN32_FIND_DATA;
*/

void Wchar_tToString(std::string& szDst, wchar_t *wchar)
{
	wchar_t * wText = wchar;
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte的运用
	char *psText; // psText为char*的临时数组，作为赋值给std::string的中间变量
	psText = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte的再次运用
	szDst = psText;// std::string赋值
	delete[]psText;// psText的清除
}

int str2int(const string &string_temp)
{
	int int_temp;
	stringstream stream(string_temp);
	stream >> int_temp;
	return int_temp;
}

void FileProcess::get_files_under_folder(char* folderPath, vector<string> &fileList, vector<string>& prefixList)
{

	char szFind[MAX_PATH];
	WIN32_FIND_DATA FindFileData;

	strcpy(szFind, folderPath);
	strcat(szFind, "\\*.*");

	HANDLE hFind = FindFirstFile(szFind, &FindFileData);
	if (INVALID_HANDLE_VALUE == hFind)  
		return;

	while (true)
	{
		if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
		{
			if (FindFileData.cFileName[0] != '.')
			{
				char szFile[MAX_PATH];
				strcpy(szFile, folderPath);
				strcat(szFile, "\\");
				strcat(szFile, (char*)(FindFileData.cFileName));
				get_files_under_folder(szFile, fileList, prefixList);
			}
		}
		else
		{
			//std::cout << FindFileData.cFileName << std::endl;
			string root = folderPath;
			root = (root[root.size() - 1] == '\\' || root[root.size() - 1] == '/') ? root : root + "\\";
			prefixList.push_back(root);
			fileList.push_back(FindFileData.cFileName);
		}
		if (!FindNextFile(hFind, &FindFileData)) 
			break;
	}
	FindClose(hFind);
}

void FileProcess::get_gray_images_and_labels(vector<string> prefixList, vector<string> fileList, vector<cv::Mat>& imageList, vector<int>& labels)
{

	for (int i = 0; i < fileList.size(); i++)
	{
		string fullPath = prefixList[i] + fileList[i];
		IplImage*iplImg = cvLoadImage(fullPath.c_str(), CV_LOAD_IMAGE_GRAYSCALE);
		cv::Mat tempImage(iplImg, true);
		labels.push_back(str2int(fileList[i].substr(0, 1)));
		if (!fileList.empty())
		{
			imageList.push_back(tempImage);
		}
	}
}