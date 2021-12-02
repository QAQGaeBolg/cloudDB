#include <iostream>
#include "lo_logs.hpp"

using namespace std;

int main()
{
	int i = 0;
	LoLogs::WriteLog("author : zhangzhihua\n");
	LoLogs::WriteLog("editor : caohaitao\n");
	LoLogs::WriteLog("email  : caohaitao_linux@163.com\n");
#ifdef WIN32
	system("pause");
#endif
	return 0;
}