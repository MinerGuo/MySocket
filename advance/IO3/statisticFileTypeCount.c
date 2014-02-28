/*
 * statisticFileTypeCount.c
 *
 *  Created on: 2013-5-5
 *      Author: miner
 */

#include "include/statisticFileTypeCount.h"
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>


void do_file_type_count_statistic(char * path_name, struct FileStatisticResult * rs) {
	//open dir
	 DIR * cur_dir = opendir(path_name);
	 if(cur_dir == NULL){
		 printf("open dir %s fail!",path_name);
		 return;
	 }
	 rs->m_dir_count++;



	//step1:判断给定的path_name是否是一个目录

	//step2:如果是目录，继续执行本函数，如果不是目录，则调用文件分析函数，进行统计


	 //close dir
	 closedir(cur_dir);
}

void do_stat(struct stat * file_stat, struct FileStatisticResult * rs) {
	if (S_ISREG(file_stat)) {
		rs->m_regular_count++;
	} else if (S_ISCHR(file_stat)) {
		rs->m_char_special_count++;
	} else if (S_ISBLK(file_stat)) {
		rs->m_block_count++;
	} else if (S_ISFIFO(file_stat)) {
		rs->m_fifo_count++;
	} else if (S_ISLNK(file_stat)) {
		rs->m_symbolic_count++;
	} else if (S_ISSOCK(file_stat)) {
		rs->m_socket_count++;
	} else if (S_ISDIR(file_stat)) {
		rs->m_dir_count++;
	} else {
		rs->m_unkown_count++;
	}
}
