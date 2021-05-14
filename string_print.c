#include "ft_printf.h"

void	char_print(va_list *ap, t_flag *ans, int *return_num)
{
	char	str;
	int		i;

	str = va_arg(*ap, int);
	ans->putlen = 1;
	if (ans->field != -1)
		ans->field -= ans->putlen;
	i = 0;
	if (!ans->flag[1])
		while (i++ < ans->field)
			*return_num += write(1, " ", 1);
	*return_num += write(1, &str, 1);
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
