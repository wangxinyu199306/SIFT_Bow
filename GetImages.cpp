

#include "stdafx.h"
#include "GetImages.h"
#include "opencv.hpp"

using namespace std;

/*
typedef struct _WIN32_FIND_DATA
{
	DWORD dwFileAttributes; //�ļ�����
	FILETIME ftCreationTime; // �ļ�����ʱ��
	FILETIME ftLastAccessTime; // �ļ����һ�η���ʱ��
	FILETIME ftLastWriteTime; // �ļ����һ���޸�ʱ��
	DWORD nFileSizeHigh; // �ļ����ȸ�32λ
	DWORD nFileSizeLow; // �ļ����ȵ�32λ
	DWORD dwReserved0; // ϵͳ����
	DWORD dwReserved1; // ϵͳ����
	TCHAR cFileName[MAX_PATH]; // ���ļ��������ɴ� 255 ���ַ��ĳ��ļ���������������չ��
	TCHAR cAlternateFileName[14]; //8.3��ʽ�ļ��������ǰ�����8���ַ�������չ����������3���ַ���
} WIN32_FIND_DATA, *PWIN32_FIND_DATA;
*/

void Wchar_tToString(std::string& szDst, wchar_t *wchar)
{
	wchar_t * wText = wchar;
	DWORD dwNum = WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, NULL, 0, NULL, FALSE);// WideCharToMultiByte������
	char *psText; // psTextΪchar*����ʱ���飬��Ϊ��ֵ��std::string���м����
	psText = new char[dwNum];
	WideCharToMultiByte(CP_OEMCP, NULL, wText, -1, psText, dwNum, NULL, FALSE);// WideCharToMultiByte���ٴ�����
	szDst = psText;// std::string��ֵ
	delete[]psText;// psText�����
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