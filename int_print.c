#include "ft_printf.h"

void	ft_putnbr(t_flag *ans, long long int n, int mode, int *return_num)
{
	char	num;

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
	if ((mode == 0 || mode == 1)
		&& (ans->field > ans->acc && ans->acc > ans->putlen))
	{
		if (flag == 0)
			ans->field -= ans->acc;
		if (flag == 1)
			ans->field -= (ans->acc + 1);
	}
	else if (mode == 0 && ans->field > ans->putlen)
		ans->field -= ans->putlen;
	else if (mode == 1 && (ans->field > ans->putlen && ans->acc < ans->field))
		ans->field -= ans->putlen;
	else if (mode == 2 && ans->flag[1]
		&& (ans->field > ans->acc && ans->acc > ans->putlen))
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
