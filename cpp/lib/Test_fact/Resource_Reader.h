
#ifndef _RESOURCE_READER_H_
#define _RESOURCE_READER_H_

#include <windows.h>
#include <vector>
#include "lib\std\util.h"

struct ResourceReader
{
  HMODULE Module;
  static std::vector<string> ResTypes;
  static std::vector<string> ResNames;
  static std::vector<WORD> ResLangs;
// Read resources names from the file ModName, ( or from the current file, default )
  ResourceReader(const char *ModName=NULL)
  {
    if (ModName==NULL)  Module=NULL;
    else TestLastError( (Module = LoadLibrary(ModName)) );
  }
  ~ResourceReader(){FreeLibrary(Module); ResTypes.clear();ResNames.clear();ResLangs.clear();}
// Get resource types stored in the module
  void GetTypes()
  {  
    ResTypes.clear(); EnumResourceTypes(Module,(ENUMRESTYPEPROC)EnumTypesFunc,  0); 
  }

// Get names of the resources of given type
  void GetNames(const char *type)
  {
    ResNames.clear();EnumResourceNames(Module,type,(ENUMRESNAMEPROC)EnumNamesFunc,  0); 
  }
// Get languages for the resource of given name
  void GetLangs(const char *type,const char *name)
  {
    ResLangs.clear();
    EnumResourceLanguages(Module,type,name,(ENUMRESLANGPROC)EnumLangsFunc,  0); 
  }
  static HGLOBAL Load(const char *name,const char *type,long &size,HMODULE Mod=NULL)
  {
    HRSRC res=FindResource(Mod,name,type);
    HGLOBAL dat=NULL;
    if (TestLastError(res)) {dat=LoadResource(NULL,res);TestLastError(dat);}
    size=SizeofResource(NULL,res);
    return dat;  
  }
  static HGLOBAL Load(int name,const char *type,long &size,HMODULE Mod=NULL)
  {
    char tmp[256];
    return Load(string("#").append(itoa(name,&tmp[0],10)).c_str(),type,size,Mod);
  }




// Declare callback functions. 


               // module handle // callback function  // extra parameter 
//  EnumResourceTypes(hExe,(ENUMRESTYPEPROC)EnumTypesFunc,  0);                              
//  EnumResourceNames(hModule, lpType, (ENUMRESNAMEPROC)EnumNamesFunc, 0); 
//  EnumResourceLanguages(hModule, lpType, lpName, (ENUMRESLANGPROC)EnumLangsFunc, 0); 
// extra parameter, could be  used for error checking 
  static BOOL EnumTypesFunc( HANDLE hModule, LPTSTR lpType, LONG lParam)
  {
     // Write the resource name to a resource information file. 
     // The name may be a string or an unsigned decimal 
     // integer, so test before printing. 
    if ((ULONG)lpType & 0xFFFF0000) 
      ResTypes.push_back(string(lpType));
    return 1;
  }
  static BOOL EnumNamesFunc( HANDLE hModule, LPCTSTR lpType, LPTSTR lpName, LONG lParam)
  {
     // Write the resource name to a resource information file. 
     // The name may be a string or an unsigned decimal 
     // integer, so test before printing. 
    char tmp[256];
    if ((ULONG)lpName & 0xFFFF0000) 
      ResNames.push_back(string(lpName));
    else 
      ResNames.push_back(string("#").append(itoa((int)lpName,&tmp[0],10)));
    return 1;
  }
 
  static BOOL EnumLangsFunc( HANDLE hModule, LPCTSTR lpType, LPCTSTR lpName, WORD wLang, 
                             LONG lParam) 
  {
    ResLangs.push_back(wLang);
    //HANDLE hResInfo; 
    //hResInfo = FindResourceEx(hModule, lpType, lpName, wLang); 
    //// Write the resource handle and size to buffer. 
    //cbString = sprintf(szBuffer, 
    //    "\t\thResInfo == %lx,  Size == %lu\n\n", 
    //    hResInfo, 
    //    SizeofResource(hModule, hResInfo)); 
    return 1;
  }
};
 




#endif