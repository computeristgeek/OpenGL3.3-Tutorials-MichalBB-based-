#include "mainheader.h"

//====================

string fileName(string strPath)
{
	while(1)
	{
		GLint iPos = (int)strPath.find('\\');
		if(iPos == -1)break;
		strPath = strPath.substr(iPos+1, strPath.length()-iPos-1);
	}
	return strPath;
}

//====================

char* readline(FILE* fp)
{
	char* strLine = new char[255];
	do fgets(strLine, 255, fp);
	while(strLine[0] == '\n' || strLine[0] == '/');
	strLine[strlen(strLine)-1] = '\0'; // Remove newline character
	return strLine;
}

//====================

char* fileDir(char* strFileName)
{
	string strPath = strFileName;
	reverse(strPath.begin(), strPath.end());
	GLint iSlash = (int)strPath.find('\\');
	if(iSlash == -1)return "";
	else
	{
		string strSub = strPath.substr(iSlash, strPath.length() - iSlash);
		reverse(strSub.begin(), strSub.end());
		char* ret = new char[strSub.length() + 1];
		strcpy(ret, strSub.c_str());
		return ret;
	}
}

//====================

GLvoid changeDir(char* strNew)
{
	static char strDef[256] = {0};
	if(strDef[0] == 0)GetCurrentDirectory(256, strDef);
	if(strlen(strNew) == 0)SetCurrentDirectory(strDef);
	else SetCurrentDirectory(strNew);
}