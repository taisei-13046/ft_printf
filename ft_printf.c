#include "ft_printf.h"

size_t	ft_strlen(const char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_checkflag(const char *str, int c)
{
	int i;

	i = 0;
	c = (char)c;
	if (!c)
		return (-1);
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

void	char_print(va_list *ap, t_flag *ans, int *return_num)
{
	char	str;
	int		i;

	//str に文字を格納
	str = va_arg(*ap, int);
	//lenは１
	ans->putlen = 1;
	//field は　ーlen
	if (ans->field != -1)
		ans->field -= ans->putlen;
	//-flagが存在しない
	i = 0;
	if (!ans->flag[1])
		while (i++ < ans->field)
			*return_num += write(1, " ", 1);
	*return_num += write(1, &str, 1);
	//flagが存在する
	if (ans->flag[1])
		while (i++ < ans->field)
			*return_num += write(1, " ", 1);
}

void	word_count(char *str, t_flag *ans)
{
	if (ans->acc == -1 || (size_t)ans->acc >= ft_strlen(str))
		ans->putlen = ft_strlen(str);
	if ((size_t)ans->acc < ft_strlen(str))
		ans->putlen = ans->acc;
}

void	ft_str_field_zero(t_flag *ans, int mode, int *return_num)
{
	int	i;

	i = 0;
	if (mode == 0)
	{
		ans->field -= ans->putlen;
		while (i++ < ans->field)
			*return_num += write(1, "0", 1);
	}
	if (mode == 1)
	{
		while (i++ < ans->field)
			*return_num += write(1, " ", 1);
	}
	if (mode == 2)
	{
		while (i++ < ans->field)
			*return_num += write(1, " ", 1);
	}
}

void	string_print(va_list *ap, t_flag *ans, int *return_num)
{
	char	*str;
	int		i;

	str = va_arg(*ap, char *);
	if (str != NULL)
		word_count(str, ans);
	if (str == NULL)
	{
		str = "(null)";
		if (ans->acc >= 6 || ans->acc == -1)
			ans->putlen = 6;
		else
			ans->putlen = ans->acc;
		ans->specific = -1;
	}
	if (ans->flag[0] && !ans->flag[1] && ans->field > ans->putlen)
		ft_str_field_zero(ans, 0, return_num);
	if (ans->field != -1)
		ans->field -= ans->putlen;
	i = 0;
	if (!ans->flag[1])
		ft_str_field_zero(ans, 1, return_num);
	*return_num += write(1, str, ans->putlen);
	if (ans->flag[1])
		ft_str_field_zero(ans, 2, return_num);
	if (ans->acc != -1)
		ans->putlen += ans->acc;
}

void	percent_print(t_flag *ans, int *return_num)
{
	int	cnt;

	ans->putlen = 1;
	cnt = ans->field;
	if (ans->flag[0] && !ans->flag[1])
		while (cnt-- > 1)
			*return_num += write(1, "0", 1);
	else if (!ans->flag[0] && !ans->flag[1])
		while (cnt-- > 1)
			*return_num += write(1, " ", 1);
	*return_num += write(1, "%%", 1);
	if (ans->flag[1])
		while (cnt-- > 1)
			*return_num += write(1, " ", 1);
	if (ans->field != -1)
		ans->field--;
}

int	int_count(int num)
{
	int	cnt;

	cnt = 0;
	if (num <= 0)
	{
		num *= -1;
		cnt++;
	}
	while (num > 0)
	{
		num /= 10;
		cnt++;
	}
	return (cnt);
}

void	ft_putnbr(t_flag *ans, long long int n, int mode, int *return_num)
{
	char	num;

	if (n == -2147483648)
		*return_num += write(1, "-2147483648", 12);
	if (ans->acc == 0 && ans->field != -1 && n == 0)
		*return_num += write(1, " ", 1);
	else if (ans->acc == 0 && n == 0)
		return ;
	else
	{
		if (n < 0)
		{
			if (mode == 1)
				*return_num += write(1, "-", 1);
			n *= -1;
		}
		if (n >= 10)
			ft_putnbr(ans, n / 10, mode, return_num);
		num = n % 10 + '0';
		*return_num += write(1, &num, 1);
	}
}

void	ft_write_field(t_flag *ans, int mode, int flag, int *return_num)
{
	int	i;

	i = 0;
	//filed > 精度　> putlen の時の空文字
	if ((mode == 0 || mode == 1) && (ans->field > ans->acc && ans->acc > ans->putlen))
	{
		if (flag == 0)
			ans->field -= ans->acc;
		if (flag == 1)
			ans->field -= (ans->acc + 1);
	}
	//filedがputlen以上の時の空文字
	else if (mode == 0 && ans->field > ans->putlen)
		ans->field -= ans->putlen;
	else if (mode == 1 && (ans->field > ans->putlen && ans->acc < ans->field))
		ans->field -= ans->putlen;
	else if (mode == 2 && ans->flag[1] && (ans->field > ans->acc && ans->acc > ans->putlen))
		ans->field -= (ans->acc + ans->putlen);
	else if (mode == 2 && ans->field > ans->putlen)
		ans->field -= ans->putlen;
	else
		ans->field = -1;
	while (i++ < ans->field)
		*return_num += write(1, " ", 1);
}

void	ft_write_zero(t_flag *ans, int mode, int flag, int *return_num)
{
	int	i;

	i = 0;
	if ((mode == 0 || mode == 1 ) && ans->acc > ans->putlen)
	{
		if (flag == 0)
			ans->acc -= ans->putlen;
		else if (flag == 1)
			ans->acc -= (ans->putlen - 1);
		while (i++ < ans->acc)
			*return_num += write(1, "0", 1);
		if (mode == 0)
			ans->putlen += ans->acc;
	}
	else if ((mode == 0 && ans->flag[0] && !ans->flag[1])
		|| (mode == 1 && ans->field > ans->acc && !ans->flag[1]))
	{
		ans->field -= ans->putlen;
		while (i++ < ans->field)
			*return_num += write(1, "0", 1);
	}
	else if (mode == 2)
	{
		ans->acc -= ans->putlen;
		if (!ans->flag[1])
			ans->field -= ans->acc;
		while (i++ < ans->acc)
			*return_num += write(1, "0", 1);
	}
}

void	int_print(va_list *ap, t_flag *ans, int *return_num)
{
	long long	num;
	static int	flag;

	num = va_arg(*ap, int);
	ans->putlen = int_count(num);
	if (CHECK_MINUS)
		flag = 1;
	if (!ans->flag[1] && !ans->flag[0])
		ft_write_field(ans, 0, flag, return_num);
	if (!ans->flag[1] && ans->flag[0] && ans->acc != -1)
		ft_write_field(ans, 1, flag, return_num);
	if (flag == 1)
		*return_num += write(1, "-", 1);
	if (flag == 1)
		ft_write_zero(ans, 0, flag, return_num);
	else if (ans->flag[0])
		ft_write_zero(ans, 1, flag, return_num);
	else if (ans->acc > ans->putlen)
		ft_write_zero(ans, 2, flag, return_num);
	if (flag == 1)
		ft_putnbr(ans, num, 0, return_num);
	else
		ft_putnbr(ans, num, 1, return_num);
	if (ans->flag[1])
		ft_write_field(ans, 2, flag, return_num);
}

int	sixteen_count(unsigned long long num, t_flag *ans)
{
	int	cnt;

	cnt = 0;
	if (ans->specific == 2 && (ans->acc == 0 || num == 0))
	{
		if (ans->acc == 0)
			return (2);
		if (num == 0 && ans->acc != -1)
			return (1);
		if (num == 0)
			return (3);
	}
	if (ans->acc == 0)
		return (0);
	if (num == 0)
		return (1);
	while (num > 0)
	{
		num /= 16;
		cnt++;
	}
	if (ans->specific == 2 && ans->field > ans->acc)
		return (cnt + 2);
	return (cnt);
}

void	ft_putnbr_un(t_flag *ans, unsigned long long n, int *return_num)
{
	char		buf[18];
	const char	change_small[16] = "0123456789abcdef";
	const char	change_large[16] = "0123456789ABCDEF";
	int			i;
	int			j;

	if (ans->acc == 0)
		return ;
	if (n == 0)
		*return_num += write(1, "0", 1);
	i = 0;
	while (n > 0)
	{
		j = n % 16;
		n /= 16;
		if (ans->specific == 6 || ans->specific == 2)
			buf[i] = change_small[j];
		if (ans->specific == 7)
			buf[i] = change_large[j];
		i++;
	}
		buf[i] = '\0';
	while (i-- > 0)
		*return_num += write(1, &buf[i], 1);
}

void	unsigned_print(va_list *ap, t_flag *ans, int *return_num)
{
	long long unsigned	num;

	num = va_arg(*ap, int);
	ans->putlen = sixteen_count(num, ans);
	if (!ans->flag[1] && !ans->flag[0])
		ft_write_field(ans, 0, 0, return_num);
	if (!ans->flag[1] && ans->flag[0] && ans->acc != -1)
		ft_write_field(ans, 1, 0, return_num);
	if (ans->flag[0])
		ft_write_zero(ans, 1, 0, return_num);
	else if (ans->acc > ans->putlen)
		ft_write_zero(ans, 2, 0, return_num);
		ft_putnbr_un(ans, num, return_num);
	if (ans->flag[1])
		ft_write_field(ans, 2, 0, return_num);
}

void	addres_print(va_list *ap, t_flag *ans, int *return_num)
{
	long long unsigned	num;

	num = va_arg(*ap, int);
	ans->putlen = sixteen_count(num, ans);
	if (!ans->flag[1] && !ans->flag[0])
		ft_write_field(ans, 0, 0, return_num);
	if (!ans->flag[1] && ans->flag[0] && ans->acc != -1)
		ft_write_field(ans, 1, 0, return_num);
	*return_num += write(1, "0x", 2);
	if (ans->flag[0])
		ft_write_zero(ans, 1, 0, return_num);
	else if (ans->acc > ans->putlen)
		ft_write_zero(ans, 2, 0, return_num);
		ft_putnbr_un(ans, num, return_num);
	if (ans->flag[1])
		ft_write_field(ans, 2, 0, return_num);
}

int	per_print(const char **format, va_list *ap, t_flag *ans)
{
	int	return_num;

	return_num = 0;
	//指定子がない -> error
	if (ans->specific == -1)
		return (-1);
	//%cの処理
	if (ans->specific == 0)
		char_print(ap, ans, &return_num);
	//%sの処理
	else if (ans->specific == 1)
		string_print(ap, ans, &return_num);
	//%pの処理
	else if (ans->specific == 2)
		addres_print(ap, ans, &return_num);
	//%d %i %uの処理
	else if (ans->specific >= 3 && ans->specific <= 5)
		int_print(ap, ans, &return_num);
	////%x %X の処理
	else if (ans->specific == 6 || ans->specific == 7)
		unsigned_print(ap, ans, &return_num);
	////%%の処理
	else
		percent_print(ans, &return_num);
	(*format)++;
	return (return_num);
}

//mode==0 -> 最小フィールド幅　mode==1 -> 精度　*.*
int	check_field_acc(const char **format, va_list *ap, int mode, t_flag *ans)
{
	int num;

	num = -1;
	//*が来たら次の引数を読み込む
	if (**format == '*')
	{
		num = va_arg(*ap, int);
		//マイナスフラグが立っている
		if (num < 0 && mode == 0)
		{
			num *= -1;
			ans->flag[1] = 1;
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

void	reset_flag(t_flag *ans)
{
	ans->flag[0] = 0; //0
	ans->flag[1] = 0; //-
	ans->field = -1; //-1 -> 存在しない
	ans->acc = -1; //-1 -> 存在しない
	ans->specific = -1; //-1 -> 存在しない
	ans->putlen = 0;
}

//入力部分
void	write_percent(const char **format, int *return_num, va_list *ap)
{
	int		num;
	t_flag	ans;

	num = 0;
	//formatを一つ進める（％の次）
	(*format)++;
	//flagのリセット
	reset_flag(&ans);
	//flagのチェック
	while (num >= 0)
	{
		//0か-があるかの確認
		num = ft_checkflag("0-", **format);
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
	ans.specific = ft_checkflag("cspdiuxX%", **format);
	num = per_print(format, ap, &ans);
	*return_num += num;
}

void	ft_print_str(const char **format, int *return_num)
{
	size_t 		len;
	const char	*str;

	str = *format;
	//formatが％になるまで進める
	while (**format != '%' && **format)
		(*format)++;
	//lenを図る
	len = *format - str;
	write(1, str, len);
	*return_num += len;
}

int ft_printf(const char *format, ...)
{
	va_list 	ap;
	//printfの帰り値は表示の長さ
	int			return_num;

	return_num = 0;
	//va_start
	va_start(ap, format);
	if (!format)
		return_num = -1;
	//formatを最後まで読み込む
	while (return_num >= 0 && *format)
	{
		//"%"がないときの実装
		if (*format != '%')
			ft_print_str(&format, &return_num);
		else
			write_percent(&format, &return_num, &ap);
	}
	va_end(ap);
	return (return_num);
}
/*
int main()
{
	int 	cnt;

	//printf("-------------printf---normal------------\n");
	//cnt = printf("abcd\n");
	//printf("                          %d\n", cnt);

	//printf("-------------ft_printf------------\n");
	//cnt = ft_printf("abcd\n");
	//printf("                          %d\n", cnt);

	//printf("-------------printf------char---------\n");
	//cnt = printf("[%c]\n", 'a');
	//printf("                          %d\n", cnt);
	//cnt = printf("[%10c]\n", 'a');
	//printf("                          %d\n", cnt);
	//cnt = printf("[%-10c]\n", 'a');
	//printf("                          %d\n", cnt);
	//cnt = printf("[%0-10.5c]\n", 'a');
	//printf("                          %d\n", cnt);

	//printf("-------------ft_printf-----char-------\n");
	//cnt = ft_printf("[%c]\n", 'a');
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%10c]\n", 'a');
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%-10c]\n", 'a');
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%0-10.5c]\n", 'a');
	//printf("                          %d\n", cnt);

	//printf("--------------printf---------str--------\n");
	//cnt = printf("[%s]\n", "abc");
	//printf("                          %d\n", cnt);
	//cnt = printf("[%10s]\n", "abc");
	//printf("                          %d\n", cnt);
	//cnt = printf("[%10.5s]\n", "abc");
	//printf("                          %d\n", cnt);
	//cnt = printf("[%-10s]\n", "abc");
	//printf("                          %d\n", cnt);
	cnt = printf("%%04.5i 42 == |%04.5i|", 42);
	printf("                          %d\n", cnt);

	//printf("--------------ft_printf------str--------\n");
	//cnt = ft_printf("[%s]\n", "abc");
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%10s]\n", "abc");
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%10.5s]\n", "abc");
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%-10s]\n", "abc");
	//printf("                          %d\n", cnt);
	cnt = ft_printf("%%04.5i 42 == |%04.5i|", 42);
	printf("                          %d\n", cnt);


	//printf("------------printf----%%----------------\n");
	//cnt = printf("[%%]\n");
	//printf("                          %d\n", cnt);
	//cnt = printf("[%10%]\n");
	//printf("                          %d\n", cnt);
	//cnt = printf("[%-10%]\n");
	//printf("                          %d\n", cnt);
	//cnt = printf("[%10.5%]\n");
	//printf("                          %d\n", cnt);

	//printf("------------ft_printf----%%----------------\n");
	//cnt = ft_printf("[%%]\n");
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%10%]\n");
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%-10%]\n");
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%10.5%]\n");
	//printf("                          %d\n", cnt);

	//printf("------------printf----int----------------\n");
	//cnt = printf("[%d]\n", 123);
	//printf("                          %d\n", cnt);
	//cnt = printf("[%10d]\n", 123);
	//printf("                          %d\n", cnt);
	//cnt = printf("[%-10d]\n", 123);
	//printf("                          %d\n", cnt);
	//cnt = printf("[%10.5d]\n", 123);
	//printf("                          %d\n", cnt);
	//cnt = printf("[%010d]\n", -123);
	//printf("                          %d\n", cnt);
	//cnt = printf("[%10d]\n", -123);
	//printf("                          %d\n", cnt);
	//cnt = printf("[%-10i]\n", 123);
	//printf("                          %d\n", cnt);
	//cnt = printf("[%10.5d]\n", 123);
	//printf("                          %d\n", cnt);
	//cnt = printf("[%8.3d]\n", -8473);
	//printf("                          %d\n", cnt);
	//cnt = printf("[%03.3d]\n", 6983);
	//printf("                          %d\n", cnt);
	//cnt = printf("[%010.5d]\n", -216);
	//printf("                          %d\n", cnt);
	//cnt = printf("[%-3.7d]\n", -2375);
	//printf("                          %d\n", cnt);
	//cnt = printf("[%10.7d]\n", -2375);
	//printf("                          %d\n", cnt);
	//cnt = printf("%08.3d", 8375);
	//printf("                          %d\n", cnt);
	//cnt = printf("%0-8.5d", 34);
	//printf("                          %d\n", cnt);
	//cnt = printf("%0-8.5d", 0);
	//printf("                          %d\n", cnt);
	//cnt = printf("%-8.5d", 34);
	//printf("                          %d\n", cnt);

	//printf("------------ft_printf----int----------------\n");
	//cnt = ft_printf("[%d]\n", 123);
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%10d]\n", 123);
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%-10d]\n", 123);
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%10.5d]\n", 123);
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%010d]\n", -123);
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%10d]\n", -123);
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%-10i]\n", 123);
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%10.5d]\n", 123);
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%8.3d]\n", -8473);
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%03.3d]\n", 6983);
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%010.5d]\n", -216);
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%-3.7d]\n", -2375);
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("%0-8.5d", 34);
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("%0-8.5d", 0);
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("%-8.5d", 34);
	//printf("                          %d\n", cnt);

	//printf("------------printf----unsignedint----------------\n");
	//cnt = printf("[%x]\n", 123);
	//printf("                          %d\n", cnt);
	//cnt = printf("[%10x]\n", 123);
	//printf("                          %d\n", cnt);
	//cnt = printf("[%-10x]\n", 123);
	//printf("                          %d\n", cnt);
	//cnt = printf("[%10.5X]\n", 123);
	//printf("                          %d\n", cnt);
	//cnt = printf("[%010x]\n", 123);
	//printf("                          %d\n", cnt);
	//cnt = printf("[%x]\n", 4294967295);
	//printf("                          %d\n", cnt);
	//cnt = printf("[%-10X]\n", 123);
	//printf("                          %d\n", cnt);
	//cnt = printf("%.5p", 0);
	//printf("                          %d\n", cnt);

	//printf("------------ft_printf----unsignedint----------------\n");
	//cnt = ft_printf("[%x]\n", 123);
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%10x]\n", 123);
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%-10x]\n", 123);
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%10.5X]\n", 123);
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%010x]\n", 123);
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%x]\n", 4294967295);
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%-10X]\n", 123);
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("%.5p", 0);
	//printf("                          %d\n", cnt);

	//printf("------------printf----adress----------------\n");
	//char	*buf;
	//char	a[3] = "aaa";

	//buf = &a;
	//cnt = printf("[%p]\n", buf);
	//printf("                          %d\n", cnt);
	//cnt = printf("[%20p]\n", buf);
	//printf("                          %d\n", cnt);
	//cnt = printf("[%-10p]\n", buf);
	//printf("                          %d\n", cnt);
	//cnt = printf("[%020p]\n", buf);
	//printf("                          %d\n", cnt);

	//printf("------------ft_printf----adress----------------\n");
	//cnt = ft_printf("[%p]\n", buf);
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%20p]\n", buf);
	//printf("                          %d\n", cnt);
	//cnt = ft_printf("[%-10p]\n", buf);
	//printf("                          %d\n", cnt);
}*/

