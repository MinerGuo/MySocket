/*
 * sort.h
 *
 *  Created on: Mar 19, 2013
 *      Author: mguo
 */

#ifndef SORT_H_
#define SORT_H_

void change_value(int * a, int * b) {
	*a = *a + *b;
	*b = *a - *b;
	*a = *a - *b;
}

/**
 * 对给定的int数组，执行快速排序
 */
void quick_sort(int * value, size_t start, size_t end) {
	if (end - start < 1) {
		return;
	}

	//拿第一个元素，作为基数
	size_t base_pos = start;
	size_t mv_start = start;
	size_t mv_end = end;


	//查找的起始和结束区间没有碰撞，就继续下去
	while (mv_start !=   mv_end) {

		//因为基数位于第一位，所以，我们应该把右边的所有小于它的数据，都放到它的左边去
		//于是，就从右往左找第一个小于它的数字，如果找到，就把两个数调换位置，
		//因为基数被调换了，左边也有数据了，这时候就需要查找左边比它大的数，再调换位置
		//如此反复，直到左右两边都到达基数所在的位置



		//从后往前，找第一个小于它的数字
		for (size_t i = mv_end; i >= base_pos; i--) {
			mv_start = i;
			if (*(value + i) < *(value + base_pos)) {
				//交换二者的值
				change_value(value + i, value + base_pos);
				//基数要取自当前i所在的位置
				base_pos = i;
				break;
			}
		}

		//从前往后找，找第一个大于它的数字
		for (size_t i = mv_start; i <= base_pos; i++) {
			mv_start = i;
			if (*(value + i) > *(value + base_pos)) {
				//i位置的数据，比基数要小，则交换二者的值
				change_value(value + i, value + base_pos);
				//基数要取自当前i所在的位置
				base_pos = i;
				break;
			}
		}

	}

	quick_sort(value, start, base_pos);
	quick_sort(value, base_pos, end);
}

#endif /* SORT_H_ */
