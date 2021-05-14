#include "ft_printf.h"

int	ft_printf(const char *format, ...)
{
	va_list		ap;
	int			return_num;

	return_num = 0;
	va_start(ap, format);
	if (!format)
		return_num = -1;
	while (return_num >= 0 && *format)
	{
		if (*format != '%')
			ft_print_str(&format, &return_num);
		else
			write_percent(&format, &return_num, &ap);
	}
	va_end(ap);
	return (return_num);
}
