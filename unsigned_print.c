#include "ft_printf.h"

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
	unsigned long long	num;

	num = va_arg(*ap, unsigned int);
	ans->putlen = sixteen_count(num, ans);
	if (!ans->flag[1] && !ans->flag[0])
		ft_write_field(ans, 0, 0, return_num);
	if (!ans->flag[1] && ans->flag[0] && ans->acc != -1)
		ft_write_field(ans, 1, 0, return_num);
	if (ans->flag[0])
		ft_write_zero(ans, 1, 0, return_num);
	else if (ans->acc > ans->putlen)
		ft_write_zero_acc(ans, return_num);
	ft_putnbr_un(ans, num, return_num);
	if (ans->flag[1])
		ft_write_field(ans, 2, 0, return_num);
}

void	addres_print(va_list *ap, t_flag *ans, int *return_num)
{
	long long unsigned	num;

	num = va_arg(*ap, unsigned long long);
	ans->putlen = sixteen_count(num, ans);
	if (!ans->flag[1] && !ans->flag[0])
		ft_write_field(ans, 0, 0, return_num);
	if (!ans->flag[1] && ans->flag[0] && ans->acc != -1)
		ft_write_field(ans, 1, 0, return_num);
	*return_num += write(1, "0x", 2);
	if (ans->flag[0])
		ft_write_zero(ans, 1, 0, return_num);
	else if (ans->acc > ans->putlen)
		ft_write_zero_acc(ans, return_num);
	ft_putnbr_un(ans, num, return_num);
	if (ans->flag[1])
		ft_write_field(ans, 2, 0, return_num);
}

void	unsigned_int_print(va_list *ap, t_flag *ans, int *return_num)
{
	long long	num;
	int			flag;

	flag = 0;
	num = va_arg(*ap, unsigned int);
	ans->putlen = int_count(num);
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
		ft_write_zero_acc(ans, return_num);
	if (flag == 1)
		ft_putnbr(ans, num, 0, return_num);
	else
		ft_putnbr(ans, num, 1, return_num);
	if (ans->flag[1])
		ft_write_field(ans, 2, flag, return_num);
}
