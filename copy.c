#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
int main(int argc,const char* argv[])                        //0 程序本身 1源文件名 2目的文件名 3光标位置 4拷贝大小
{
	//设置缓冲区为任务量大小
	//char* buffer=(char*)malloc(atoi(argv[4]+1));             //申请空间+'\0'->在虚拟机上很容易出错
	int blk=atoi(argv[4]);
	char buffer[blk];                                         //在gcc上可以编译成功vs上不可以
	//打开被复制的文件
	int num=0;                                               //文件操作符
	num=open(argv[1],O_RDWR);                                //打开被复制文件
	if(num==-1)
	{
		printf("文件打开失败无法拷贝\n");                    //当这个程序可以正常运行后注释此行
		exit(-1);
	}

	//把光标移到最开头位置
	lseek(num,0,SEEK_SET);                                   //防止光标不在开始位置导致代码出错
	lseek(num,atoi(argv[3]),SEEK_SET);                       //偏移到需要拷贝的地方

	//把文件内容拷贝到字符串里
	int f=0;
	f=read(num,buffer,atoi(argv[4]));                        //读取目标文件的目标数据
	if((f-atoi(argv[4]))>1)
	{
		printf("文件复制有误无法拷贝\n");                      //当这个程序可以正常运行后注释此行
		exit(-2);
	}
	close(num);                                              //关闭文件

	//创建目标文件
	num=open(argv[2],O_RDWR|O_CREAT,0666);                   //打开目的文件
	if(num==-1)
	{
		printf("创建目的文件失败无法拷贝\n");                  //当这个程序可以正常运行后注释此行
		exit(-3);
	}

	//把光标移到最开头位置
	lseek(num,0,SEEK_SET);                                  //防止光标不在开始位置导致代码出错
	lseek(num,atoi(argv[2]),SEEK_SET);                      //偏移到需要拷贝的地方
	
	//把文件内容拷贝到字符串里
	f=write(num,buffer,atoi(argv[4]));                      //读取目标文件的目标数据
	if(f!=atoi(argv[4]))
	{
		printf("文件复制有误无法拷贝\n");                     //当这个程序可以正常运行后注释此行
		exit(-4);
	}
	close(num);                                             //关闭文件
	return 0;
}