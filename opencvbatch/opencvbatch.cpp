// opencvbatch.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "cv.h"
#include "highgui.h"
#include "cxcore.h"
#include "io.h"
#include "vector"
#include "windows.h"
#include "string"
 using  namespace std;
 /*
 *获取目录中的文件
 *path 指定的目录
 *files 文件的全路径
 */
void getFiles( string path, vector<string>& files )
{
	//文件句柄
	long   hFile   =   0;
	//文件信息
	struct _finddata_t fileinfo;
	string p;
	if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)
	{
		do
		{
			//如果是目录,迭代之
			//如果不是,加入列表
			if((fileinfo.attrib &  _A_SUBDIR))
			{
				//if(strcmp(fileinfo.name,".") != 0  &&  strcmp(fileinfo.name,"..") != 0)
					//getFiles( p.assign(path).append("\\").append(fileinfo.name), files );
			}
			else
			{
				files.push_back(p.assign(path).append("\\").append(fileinfo.name) );
			}
		}while(_findnext(hFile, &fileinfo)  == 0);
		_findclose(hFile);
	}
}

/*
*写视频帧
*fullpathname	读取视频的全路径
*dstdir         保存目录
*/
void VedioWrite(string fullpathname,string dstdir)
{
	CvCapture *capture=cvCreateFileCapture(fullpathname.c_str());
	IplImage *frame;
	int w,h,fps,pos;
	string name;
	pos=fullpathname.find_last_of('\\');                               //找到最后一个‘\’的位置
	name=fullpathname.substr(pos+1,fullpathname.length()-3-1-pos)+"avi";  //成员方法substr，参数为：初始位置，长度

	w=cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH);
	h=cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT);			//获取视频的属性
	fps=cvGetCaptureProperty(capture,CV_CAP_PROP_FPS);
	CvVideoWriter *writer=cvCreateVideoWriter((dstdir+"\\"+name).c_str(),-1,fps,cvSize(w,h));//成员方法c_str(),把string变成char*
	while(1)
	{
		frame=cvQueryFrame(capture);
		if(!frame) break;
		else
			cvWriteFrame(writer,frame);

	}
	cvReleaseCapture(&capture);
	cvReleaseVideoWriter(&writer);
}

wstring s2ws(const std::string& s)
{
    setlocale(LC_ALL, "chs"); 
    const char* _Source = s.c_str();
    size_t _Dsize = s.size() + 1;
    wchar_t *_Dest = new wchar_t[_Dsize];
    wmemset(_Dest, 0, _Dsize);
    mbstowcs(_Dest,_Source,_Dsize);
    std::wstring result = _Dest;
    delete []_Dest;
    setlocale(LC_ALL, "C");
    return result;
}

string ws2s(const std::wstring& ws)
{
    std::string curLocale = setlocale(LC_ALL, NULL);        // curLocale = "C";
    setlocale(LC_ALL, "chs");
    const wchar_t* _Source = ws.c_str();
    size_t _Dsize = 2 * ws.size() + 1;
    char *_Dest = new char[_Dsize];
    memset(_Dest,0,_Dsize);
    wcstombs(_Dest,_Source,_Dsize);
    std::string result = _Dest;
    delete []_Dest;
    setlocale(LC_ALL, curLocale.c_str());
    return result;
}
int _tmain(int argc, _TCHAR* argv[])
{
	vector<string> files;
	string path="./dir";
	string dstdir="./result";
	string call;
	CreateDirectory(s2ws(dstdir).c_str(),NULL);         //创建目录
	getFiles(path,files);
	int n=files.size();
	for(int i=0;i<n;i++)
	{
		VedioWrite(files[i],dstdir);
	}
	return 0;
}

