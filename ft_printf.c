#include "ft_printf.h"

void	ft_print_str(const char **str, const char **format, int *return_num)
{
	size_t len;

	//formatが％になるまで進める
	while (**format != '%' && **format)
		(*format)++;
	//lenを図る
	len = *format - *str;
	write(1, *str, len);
	*return_num += len;
}

int ft_printf(const char *format, ...)
{
	va_list 	ap;
	const char	*str;
	int			return_num;

	return_num = 0;
	//va_start
	va_start(ap, format);
	if (!format)
		return_num = -1;
	//formatを最後まで読み込む
	while (return_num >= 0 && *format)
	{
		//lenを図るためにstrにアドレスを渡す　format自身のアドレスは動かさない
		str = format;
		//"%"がないときの実装
		if (*str != '%')
			ft_print_str(&str, &format, &return_num);
		else
			return (0);
	}
	va_end(ap);
	return (return_num);
}

int main()
{
	ft_printf("-------------printf---------------\n");
	printf("abcd\n");
	ft_printf("-------------ft_printf------------\n");
	ft_printf("abcd\n");
}
