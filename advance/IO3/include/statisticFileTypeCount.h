/*
 * statisticFileTypeCount.h
 *
 *  Created on: 2013-5-5
 *      Author: miner
 */

#ifndef STATISTICFILETYPECOUNT_H_
#define STATISTICFILETYPECOUNT_H_

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

struct FileStatisticResult {
	int m_dir_count;
	int m_regular_count;
	int m_block_count;
	int m_char_special_count;
	int m_fifo_count;
	int m_symbolic_count;
	int m_socket_count;
	int m_unkown_count;
};

void do_file_type_count_statistic(char * path_name, struct FileStatisticResult * rs);

void do_stat(struct stat * file_stat,struct FileStatisticResult * rs);

#endif /* STATISTICFILETYPECOUNT_H_ */
