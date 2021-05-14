#include "ft_printf.h"

int	per_print(const char **format, va_list *ap, t_flag *ans)
{
	int	return_num;

	return_num = 0;
	if (ans->specific == -1)
		return (-1);
	if (ans->specific == 0)
		char_print(ap, ans, &return_num);
	else if (ans->specific == 1)
		string_print(ap, ans, &return_num);
	else if (ans->specific == 2)
		addres_print(ap, ans, &return_num);
	else if (ans->specific == 3 || ans->specific == 4)
		int_print(ap, ans, &return_num);
	else if (ans->specific == 5)
		unsigned_int_print(ap, ans, &return_num);
	else if (ans->specific == 6 || ans->specific == 7)
		unsigned_print(ap, ans, &return_num);
	else
		percent_print(ans, &return_num);
	(*format)++;
	return (return_num);
}

int	check_field_acc(const char **format, va_list *ap, int mode, t_flag *ans)
{
	int	num;

	num = -1;
	if (**format == '*')
	{
		num = va_arg(*ap, int);
		if (num < 0 && mode == 0)
		{
			num *= -1;
			ans->flag[1] = 1;
		}
		else if (num < 0)
			num = -1;
		(*format)++;
	}
	else if (mode == 1 || ft_isdigit(**format))
	{
		num = 0;
		while (ft_isdigit(**format))
			num = num * 10 + (*((*format)++) - '0');
	}
	return (num);
}

void	reset_flag(t_flag *ans)
{
	ans->flag[0] = 0;
	ans->flag[1] = 0;
	ans->field = -1;
	ans->acc = -1;
	ans->specific = -1;
	ans->putlen = 0;
}

void	write_percent(const char **format, int *return_num, va_list *ap)
{
	int		num;
	t_flag	ans;

	num = 0;
	(*format)++;
	reset_flag(&ans);
	while (num >= 0)
	{
		num = ft_checkflag("0-", **format);
		if (num == -1)
			break ;
		ans.flag[num] = 1;
		(*format)++;
	}
	ans.field = check_field_acc(format, ap, 0, &ans);
	if (**format == '.')
	{
		(*format)++;
		ans.acc = check_field_acc(format, ap, 1, &ans);
	}
	ans.specific = ft_checkflag("cspdiuxX%", **format);
	num = per_print(format, ap, &ans);
	*return_num += num;
}

void	ft_print_str(const char **format, int *return_num)
{
	size_t		len;
	const char	*str;

	str = *format;
	while (**format != '%' && **format)
		(*format)++;
	len = *format - str;
	write(1, str, len);
	*return_num += len;
}
