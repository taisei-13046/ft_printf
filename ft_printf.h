#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include <unistd.h>
# include <stdio.h>

# define CHECK_MINUS (num < 0 && ((ans->flag[0] && ans->field > ans->putlen) || (ans->acc > ans->putlen)))

typedef struct s_flag
{
	int	flag[2];
	int	field;
	int	acc;
	int	specific;
	int	putlen;
}				t_flag;

int		ft_printf(const char *format, ...);
size_t	ft_strlen(const char *str);
int		ft_checkflag(const char *str, int c);
int		ft_isdigit(int c);
int		int_count(int num);
void	ft_putnbr(t_flag *ans, long long int n, int mode, int *return_num);
void	ft_write_field(t_flag *ans, int mode, int flag, int *return_num);
void	ft_write_zero(t_flag *ans, int mode, int flag, int *return_num);
void	ft_print_str(const char **format, int *return_num);
void	write_percent(const char **format, int *return_num, va_list *ap);
void	percent_print(t_flag *ans, int *return_num);
void	int_print(va_list *ap, t_flag *ans, int *return_num);
void	unsigned_int_print(va_list *ap, t_flag *ans, int *return_num);
void	unsigned_print(va_list *ap, t_flag *ans, int *return_num);
void	addres_print(va_list *ap, t_flag *ans, int *return_num);
void	char_print(va_list *ap, t_flag *ans, int *return_num);
void	string_print(va_list *ap, t_flag *ans, int *return_num);

#endif
