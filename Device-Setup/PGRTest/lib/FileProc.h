
#ifndef __File_Proc__
#define __File_Proc__

class FileProc
{
public:
	static void MakeFileNameWithDateTime(CString *rtn);
	static CString CreateFolder(CString name,BOOL bRelativePath=FALSE);
	static CString GetPath( CString path);
	static CString GetNoExtName(CString fn);
	static CString GetFileName( CString path);

};

#endif