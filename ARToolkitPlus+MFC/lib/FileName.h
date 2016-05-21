#ifndef __CFileName__
#define __CFileName__


typedef struct {
		CString		name;
		int			type;
}  FILETYPE;


class CFileName
{
public:
		CFileName();
		CFileName(CString);
		//~CFileName();

public:
		CString GetPath();
		CString GetFileName();
		CString	GetFileExt();
		CString GetFileType();
		CString GetNoExtName();
		
		// GetFileName() = "babo002" 
		// GetName() = babo
		// GetNum()  = 002;
		CString GetName();
		CString	GetNum();
		CString NO(int,int order=0);

		CString GetPath(CString);
		CString GetFileName(CString);
		CString	GetFileExt(CString);

		BOOL Copy( CString,CString );
		BOOL IsSubOf(CString s);	// is this the sub folder of s?
public:
		CString m_FullPath;
		CString	m_FileName;
		CString m_FileExt;
		CString m_FilePath;
		CString m_FileType;

		CString m_NoNum;
		CString m_Num;
		int		nNum;

public:
	static void	MakeFileNameWithDateTime(CString *rtn);
	static CString GetFileNameS(CString);

protected:
		void	FindNum();		
};


#endif 