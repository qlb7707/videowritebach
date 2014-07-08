// opencvbatch.cpp : �������̨Ӧ�ó������ڵ㡣
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
 *��ȡĿ¼�е��ļ�
 *path ָ����Ŀ¼
 *files �ļ���ȫ·��
 */
void getFiles( string path, vector<string>& files )
{
	//�ļ����
	long   hFile   =   0;
	//�ļ���Ϣ
	struct _finddata_t fileinfo;
	string p;
	if((hFile = _findfirst(p.assign(path).append("\\*").c_str(),&fileinfo)) !=  -1)
	{
		do
		{
			//�����Ŀ¼,����֮
			//�������,�����б�
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
*д��Ƶ֡
*fullpathname	��ȡ��Ƶ��ȫ·��
*dstdir         ����Ŀ¼
*/
void VedioWrite(string fullpathname,string dstdir)
{
	CvCapture *capture=cvCreateFileCapture(fullpathname.c_str());
	IplImage *frame;
	int w,h,fps,pos;
	string name;
	pos=fullpathname.find_last_of('\\');                               //�ҵ����һ����\����λ��
	name=fullpathname.substr(pos+1,fullpathname.length()-3-1-pos)+"avi";  //��Ա����substr������Ϊ����ʼλ�ã�����

	w=cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH);
	h=cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT);			//��ȡ��Ƶ������
	fps=cvGetCaptureProperty(capture,CV_CAP_PROP_FPS);
	CvVideoWriter *writer=cvCreateVideoWriter((dstdir+"\\"+name).c_str(),-1,fps,cvSize(w,h));//��Ա����c_str(),��string���char*
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
	CreateDirectory(s2ws(dstdir).c_str(),NULL);         //����Ŀ¼
	getFiles(path,files);
	int n=files.size();
	for(int i=0;i<n;i++)
	{
		VedioWrite(files[i],dstdir);
	}
	return 0;
}

