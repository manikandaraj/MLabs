#ifndef __MY_DEBUG_LOGGER__
#define __MY_DEBUG_LOGGER__

#include <fstream>
#include <string>
#include <map>

#define DebugLog(logObj,mesg)  LogObjHolder::GetObject()->logObjects->find(logObj)->second->WriteLog(mesg)

class MyDebugLogger
{
	private:
		std::ofstream logOutStream;

	public:
		void InitializeLogger(std::string logFileName);
		void WriteLog(std::string logMsg);
};

class LogObjHolder
{
	/* Singleton Template Methods	*/
	private:
		static LogObjHolder *logHolderObj;
		LogObjHolder();
		~LogObjHolder();

	public:
		static void InitObject();
		static LogObjHolder* GetObject();
	/* Singleton Template Methods	*/

	public:
		std::map<std::string, MyDebugLogger*> *logObjects;	

};

#endif

