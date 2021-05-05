#include "ft_printf.h"

int	ft_strchr(const char *str, int c)
{
	int i;

	i = 0;
	c = (char)c;
	while (str[i] != c)
	{
		if (str[i] == '\0')
			return (-1);
		i++;
	}
	return (i);
}

int	ft_isdigit(int c)
{
	if ('0' <= c && c <= '9')
		return (1);
	return (0);
}

//mode==0 -> 最小フィールド幅　mode==1 -> 精度
int	check_field_acc(const char **format, va_list *ap, int mode, t_flag *ans)
{
	int num;

	num = -1;
	//*が来たら次の引数を読み込む
	if (**format == '*')
	{
		num = va_arg(*ap, int);
		if (num < 0 && mode == 0)
		{
			num *= -1;
			ans->flag[2] = 1;
		}
		//精度がマイナス
		else if (num < 0)
			num = -1;
		(*format)++;
	}
	//フィールド値、精度のnum
	else if (mode == 1 || ft_isdigit(**format))
	{
		num = 0;
		//Atoi
		while (ft_isdigit(**format))
			num = num * 10 + (*((*format)++) - '0');
	}
	return (num);
}

void	reset_flag(t_flag *ans, int return_num)
{
	ans->flag[0] = 0; //0
	ans->flag[1] = 0; //-
	ans->flag[2] = 0; //.*
	ans->field = -1; //-1 -> 存在しない
	ans->acc = -1; //-1 -> 存在しない
	ans->specific = -1; //-1 -> 存在しない
	ans->putlen = 0;
}

void	write_percent(const char **str, const char **format, int *return_num, va_list *ap)
{
	int		num;
	t_flag	ans;

	num = 0;
	//formatを一つ進める
	(*format)++;
	//flagのリセット
	reset_flag(&ans, *return_num);
	//flagのチェック
	while (num >= 0)
	{
		//0か-があるかの確認
		num = ft_strchr("0-", **format);
		if (num == -1)
			break;
		ans.flag[num] = 1;
		(*format)++;
	}
	//fieldのチェック
	ans.field = check_field_acc(format, ap, 0, &ans);
	if (**format == '.')
	{
		//次の要素を見る
		(*format)++;
		ans.acc = check_field_acc(format, ap, 1, &ans);
	}
	//どの変換指定子が立っているか
	ans.specific = ft_strchr("cspdiuxX%", **format);
	num = per_print(str, format, ap, &ans);
	if (*return_num == 0 || num >= 0)
		*return_num = num;
}

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
			write_percent(&str, &format, &return_num, &ap);
	}
	va_end(ap);
	return (return_num);
}

int main()
{
	printf("-------------printf---------------\n");
	printf("abcd\n");
	printf("-------------ft_printf------------\n");
	ft_printf("abcd\n");
	printf("-------------printf---------------\n");
	printf("abcd\n");
	printf("-------------ft_printf------------\n");
	ft_printf("abcd\n");
}
