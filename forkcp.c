#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<sys/wait.h>

int main(int argc,const char* argv[])                //0本程序 1源文件 2目的文件 3线程数量
{
	//校验参数
	int num=0;                                       //文件操作数
	num=open(argv[1],O_RDONLY);                      //打开被复制文件
	if(num==-1||argc<3||argc>4)                      //判断是否存在目的文件，参数个数是否正确
	{
		printf("参数不正确无法进行拷贝！\n");
		exit(-1);
	}
	if(4==argc&&(atoi(argv[3])<=0||atoi(argv[3])>20))
	{
		printf("进程参数不正确无法进行拷贝！\n");
		exit(-2);
	}
	int count=0;                                    //线程的个数
	if(3==argc)                                     //判断是否有进程参数
	{
		count=5;
	}
	else
	{
		count=atoi(argv[3]);
	}

	//计算任务量

	//把光标移到最开头位置
	int size=0;                               //文件大小
	lseek(num,0,SEEK_SET);                    //防止光标不在开始位置导致数据大小计算错误
	size=lseek(num,0,SEEK_END);               //记录文件大小

	//计算每个进程拷贝多少数据
	int fnum=0;                               //每个线程拷贝多少
	fnum=size%count;                          //先判断是否有余数
	if(fnum>0)
	{
		fnum=(size/count)+1;                    //有余数的情况下，每个线程多拷贝一字节
	}
	else
	{
		fnum=size/count;
	}
	lseek(num,0,SEEK_SET);                   //防止光标不在开始位置导致数据大小计算错误
	close(num);                              //关闭文件

	//创建线程
	pid_t pid;                              //标记每个进程的pid
	int i=0;
	for(i;i<count;i++)                      //创建申请的进程数
	{
		pid=fork();
		if(0==pid)
		{
			break;
		}
	}

	//经典的主进程和子进程判断
	if(pid>0)
	{
		pid_t zpid;                              //子进程id
		int status=0;                            //进程退出值
		int* ops=(int*)malloc(sizeof(int)*count);//进程值存储
		while((zpid=wait(&status))!=-1)          //等待所有进程都结束
		{
			int op=0;
			if(WIFEXITED(status))                //如果正常退出返回退出值
			{
				op=WEXITSTATUS(status);
				printf("进程%d结束,返回值：%d\n",zpid,op);
			}
			if(WIFSIGNALED(status))              //如果正常退出返回退出值
			{
				op=WTERMSIG(status);
				printf("进程%d被信号杀死,返回值：%d\n",zpid,op);
				printf("进程%d拷贝失败\n",i);
			}
			ops[i]=op;                           //把每个进程的状态值保存下来
		}
		for(int t=0;t<count;t++)                 //验尸
		{
			if(ops[t]<0)                         //暂时可以确定是copy.c文件运行不成功不再进行拷贝
			{
				printf("文件拷贝失败\n");
				unlink(argv[2]);                 //删除新创建的文件
				exit(-4);
			}
		}
		printf("拷贝成功\n");
		exit(0);
	}
	else if(0==pid)
	{
		int y_n=0;
		char par_3[10],par_4[10];                //存储参数
		sprintf(par_3, "%d", i*fnum);            //将int转化为char*
		sprintf(par_4, "%d", fnum);
		y_n=execl("/home/colin/wxy/fockcp/copy","copy",argv[1],argv[2],par_3,par_4,NULL); //重载拷贝程序copy
		if(-1==y_n)
		{
			perror("启动copy程序失败");
			exit(-5);
		}
		exit(1);
	}
	else
	{
		perror("进程启动失败！");
		exit(-3);
	}


	return 0;
}