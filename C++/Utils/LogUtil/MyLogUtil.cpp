#include "MyLogUtil.h"

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



/*		LOGGER INITIALIZATION	*/

int main()
{
  time_t unixTimeInt; time(&unixTimeInt);
  struct tm curTimeStruct;  localtime_r(&unixTimeInt, &curTimeStruct);
  char timeStampBuf[20];  strftime(timeStampBuf, 20, "%Y_%m_%d_%H_%M_%S",&curTimeStruct);

	LogObjHolder::InitObject();
	LogObjHolder::GetObject()->logObjects  = new std::map<std::string,MyDebugLogger*>;
	MyDebugLogger *newDebLogger;

	unsigned int logItemSize = 0;
	for(unsigned int logItem =0; logItem <= logItemSize; logItem++)
	{
		std::string debugFilePath = "log.txt";
		std::string logId = "001";
		newDebLogger =  new MyDebugLogger;
		newDebLogger->InitializeLogger(debugFilePath);
		LogObjHolder::GetObject()->logObjects->insert(std::make_pair<std::string,MyDebugLogger*>(logId,newDebLogger));
	}

	DebugLog("001","Hi ya, Wassup");
}
