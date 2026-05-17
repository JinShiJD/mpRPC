#include "logger.h"
#include <time.h>
#include <iostream>

// 获取日志的单例
Logger& Logger::GetInstance()
{
    static Logger logger;
    return logger;
}

Logger::Logger()
{
    // 启动专门的写线程日志
    std::thread writeLogTask([&](){
        for(;;)
        {
            // 获取当前的日期，然后获取日志信息，写入相应的日志文件中
            time_t now = time(nullptr);
            tm *nowtm = localtime(&now);

            char fileName[128];
            sprintf(fileName, "%d-%d-%d-log.txt", nowtm->tm_year+1900, nowtm->tm_mon+1, nowtm->tm_mday);

            FILE *pf = fopen(fileName, "a+");
            if(pf == nullptr)
            {
                std::cout << "logger file : " << fileName << " open error!" << std::endl;
                exit(EXIT_FAILURE);
            }

            std::string msg = m_lckQue.Pop();

            // 信息时间戳
            char timeBuf[127] = {0};
            sprintf(timeBuf, "%d:%d:%d => [%s]", 
                    nowtm->tm_hour, 
                    nowtm->tm_min, 
                    nowtm->tm_sec,
                    (m_loglevel == LogLevel::INFO ? "info" : "error"));
            msg.insert(0, timeBuf);
            msg.append("\n");


            fputs(msg.c_str(), pf);
            fclose(pf);

        }
    });
    // 设置分离线程，守护线程
    writeLogTask.detach();
}

// 设置日志级别
void Logger::SetLogLevel(LogLevel level)
{
    m_loglevel = level;
}

// 写日志, 把日志信息写入lockqueue缓冲区中
void Logger::Log(std::string msg)
{
    m_lckQue.push(msg);
}
