#ifndef _ARITHMETIC_H_
#define _ARITHMETIC_H_

template <typename T>
void sort_bubble(T* data, int size, bool increase = true)
{
	if (increase)
	{
		for (int i = size - 1; i > 0; --i)
		{
			for (int j = 0; j < i; ++j)
			{
				if (data[j + 1] < data[j])
				{
					T t = data[j + 1];
					data[j + 1] = data[j];
					data[j] = t;
				}
			}
		}
	}
	else
	{
		for (int i = size - 1; i > 0; --i)
		{
			for (int j = 0; j < i; ++j)
			{
				if (data[j] < data[j + 1])
				{
					T t = data[j + 1];
					data[j + 1] = data[j];
					data[j] = t;
				}
			}
		}
	}
}

template <typename T>
void sort_quick_increase(T* data, int begin, int end)
{
	if (begin < end)
	{
		//设置交换位置
		int pos = begin;
		for (int i = begin + 1; i <= end; ++i)
		{
			if (data[i] < data[begin])
			{
				pos += 1;
				if (pos != i)
				{
					T t = data[i];
					data[i] = data[pos];
					data[pos] = t;
				}
			}
		}

		//交换begin和pos
		if (begin != pos)
		{
			T t = data[begin];
			data[begin] = data[pos];
			data[pos] = t;
		}

		//递归左侧、右侧数组
		sort_quick_increase(data, begin, pos - 1);
		sort_quick_increase(data, pos + 1, end);
	}
}

template <typename T>
void sort_quick_decrease(T* data, int begin, int end)
{
	if (begin < end)
	{
		//设置交换位置
		int pos = begin;
		for (int i = begin + 1; i <= end; ++i)
		{
			if (data[begin] < data[i])
			{
				pos += 1;
				if (pos != i)
				{
					T t = data[i];
					data[i] = data[pos];
					data[pos] = t;
				}
			}
		}

		//交换begin和pos
		if (begin != pos)
		{
			T t = data[begin];
			data[begin] = data[pos];
			data[pos] = t;
		}

		//递归左侧、右侧数组
		sort_quick_decrease<T>(data, begin, pos - 1);
		sort_quick_decrease<T>(data, pos + 1, end);
	}
}

template <typename T>
void sort_quick(T* data, int begin, int end, bool increase = true)
{
	if (increase)
		sort_quick_increase(data, begin, end);
	else
		sort_quick_decrease(data, begin, end);
}

#endif