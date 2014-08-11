#include "MyLogUtil.h"
#include <sys/stat.h>

void MyDebugLogger::InitializeLogger(std::string logFileName)
{
	this->logOutStream.open(logFileName.c_str(),std::ios::out|std::ios::app);
};

void MyDebugLogger::WriteLog(std::string logMsg)
{
	time_t unixTimeInt; time(&unixTimeInt);
	struct tm curTimeStruct;  localtime_r(&unixTimeInt, &curTimeStruct);
	char timeStampBuf[20];  strftime(timeStampBuf, 20, "%Y_%m_%d %H:%M:%S",&curTimeStruct);

	std::string timeStampStr = timeStampBuf;

	std::string msgToLog = timeStampStr;
	msgToLog.append("	:");
	msgToLog.append(logMsg);
	msgToLog.append(":	###");

	if(this->logOutStream)
	{
		this->logOutStream << msgToLog.c_str() << std::endl;
	}
};

LogObjHolder* LogObjHolder::logHolderObj = NULL;

LogObjHolder::LogObjHolder()
{
	//Default Private Constructor
};

LogObjHolder::~LogObjHolder()
{
	// Private Destructor [ SINGLETON - No Need to destroy the Object ]
};

void LogObjHolder::InitObject()
{
	logHolderObj = new LogObjHolder;
};

LogObjHolder* LogObjHolder::GetObject()
{
	return logHolderObj;
};

void LogObjHolder::CreateNewLogObj(std::string logId,std::string logFilePath)
{
	MyDebugLogger *newDebLogger =  new MyDebugLogger;
	newDebLogger->InitializeLogger(logFilePath);
	this->logObjects->insert(std::make_pair<std::string,MyDebugLogger*>(logId,newDebLogger));
};

/*		LOGGER INITIALIZATION	*/

int main()
{
	LogObjHolder::InitObject();
	LogObjHolder::GetObject()->logObjects  = new std::map<std::string,MyDebugLogger*>;

	time_t unixTimeInt; time(&unixTimeInt);
	struct tm curTimeStruct;  localtime_r(&unixTimeInt, &curTimeStruct);
	char timeStampBuf[20];  strftime(timeStampBuf, 20, "%Y_%m_%d_%H_%M_%S",&curTimeStruct);

	std::string logDir = "./debugLog_"; logDir.append(timeStampBuf); logDir.append("/");
	mkdir(logDir.c_str(),0777);

	std::string errLog1 = logDir; errLog1.append("errLog1.txt");
	std::string logId001 = "LOG_001";
	LogObjHolder::GetObject()->CreateNewLogObj(logId001,errLog1);

	std::string errLog2 = logDir; errLog2.append("errLog2.txt");
	std::string logId002 = "LOG_002";
	LogObjHolder::GetObject()->CreateNewLogObj(logId002,errLog2);

	DebugLog("LOG_001","Hi ya, Wassup");
}

/*	*/

