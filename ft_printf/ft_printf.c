#include <stdarg.h>
#include <unistd.h>
#include <stdlib.h>

#define PRECISION format->precision
#define WIDTH format->width
#define IS_PREC format->is_precision
#define SKIP format->skip
#define TYPE format->type
#define MAX(a,b) ((a > b ? a : b))
#define MIN(a,b) ((a < b ? a : b))

typedef struct s_props
{
	int width;
	int is_precision;
	int precision;
	int skip;
	char type;
} t_props;

int ft_strlen(char *str)
{
	int i = 0;
	while(str[i])
		i++;
	return (i);
}

int ft_isdigit(char c)
{
	return ((int)c >= 48 && (int)c <= 57);
}

void ft_parsef(const char *str, t_props *format)
{
	int isprec = 0, i = 0;
	while(ft_isdigit(str[i]) || str[i] == '.')
	{
		if (str[i] == '.')
		{		isprec = 1; IS_PREC = 1;}
		else if (!isprec)
			WIDTH = WIDTH * 10 + str[i] - '0';
		else
			PRECISION = PRECISION * 10 + str[i] - '0';
		i++;
	}
	if (str[i] == 's' || str[i] == 'd'|| str[i] == 'x')
		TYPE = str[i++];
	SKIP = i;
}

size_t ft_process_s(t_props *format, char *str)
{
	size_t ret = 0;
	if (!str)
		str = "(null)";
	int len = ft_strlen(str);
	if (IS_PREC)
	{
		while(WIDTH > MIN(len, PRECISION))
		{
			ret+=write(1, " ", 1);
			WIDTH--;
		}
		ret+=write(1, str, MIN(len, PRECISION));
	}
	else
	{
		while(WIDTH > len)
		{
			ret+=write(1, " ", 1);
			WIDTH--;
		}
		ret+=write(1, str, len);

	}
	return (ret);
}

char *ft_itoa_base(unsigned int num, int base)
{
	char *ret;
	int numlen = 0;
	unsigned int tmp = num;
	char *basename = "0123456789abcdef";
	while (tmp){
		tmp /= base; numlen++;}
	ret = (char*)malloc(numlen + (!num) + 1);
	ret[numlen + (!num)] = 0;
	if (!numlen)
		ret[0] = '0';
	while(num)
	{
		ret[--numlen] = basename[num % base];
		num /= base;
	}
	return(ret);
}

size_t ft_process_d(t_props *format, int num)
{
	unsigned int n = (num < 0 ? -num : num);
	char *str = ft_itoa_base(n, 10);
	int len = ft_strlen(str);
	size_t ret = 0;
	if (IS_PREC && PRECISION)
	{
		while(WIDTH > MAX(len + (num < 0), PRECISION))
		{
			ret+=write(1, " ", 1);
			WIDTH--;
		}
		if (num < 0)
			ret+=write(1, "-", 1);
		while(PRECISION > len)
		{
			ret+=write(1, "0", 1);
			PRECISION--;
		}
		ret+=write(1, str, len);
	}
	else if (IS_PREC && !PRECISION && !num) //not printing digit
	{
		while(WIDTH > 0)
		{
			ret+=write(1, " ", 1);
			WIDTH--;
		}
	}
	else
	{
		while(WIDTH > len + (num <0))
		{
			ret+=write(1, " ", 1);
			WIDTH--;
		}
		if (num < 0)
			ret+=write(1, "-", 1);
		ret+=write(1, str, len);
	}
	free(str);
	return(ret);
}
size_t ft_process_x(t_props *format, unsigned int num)
{
	char *str = ft_itoa_base(num, 16);
	int len = ft_strlen(str);
	size_t ret = 0;
	if (IS_PREC && PRECISION)
	{
		while(WIDTH > MAX(len + (num < 0), PRECISION))
		{
			ret+=write(1, " ", 1);
			WIDTH--;
		}
		if (num < 0)
			ret+=write(1, "-", 1);
		while(PRECISION > len)
		{
			ret+=write(1, "0", 1);
			PRECISION--;
		}
		ret+=write(1, str, len);
	}
	else if (IS_PREC && !PRECISION && !num) //not printing digit
	{
		while(WIDTH > 0)
		{
			ret+=write(1, " ", 1);
			WIDTH--;
		}
	}
	else
	{
		while(WIDTH > len + (num <0))
		{
			ret+=write(1, " ", 1);
			WIDTH--;
		}
		if (num < 0)
			ret+=write(1, "-", 1);
		ret+=write(1, str, len);
	}
	free(str);
	return(ret);
}

size_t ft_process(t_props *format, va_list ap)
{
	size_t ret = 0;
	if (TYPE == 's')
		ret = ft_process_s(format, va_arg(ap, char *));
	else if (TYPE == 'd')
		ret = (ft_process_d(format, va_arg(ap, int)));
	else if (TYPE == 'x')
		ret =  (ft_process_x(format, va_arg(ap, unsigned int)));
	WIDTH = 0; PRECISION = 0; IS_PREC = 0; TYPE = 0; SKIP = 0;
	return (ret);
}

int ft_printf(const char *str, ... )
{
	va_list ap;
	t_props format = {0};
	int n = 0, ret = 0;
	
	va_start(ap, str);
	while(str[n])
	{
		if(str[n] != '%')
			ret += write(1, &str[n], 1);
		else
		{
			ft_parsef(&str[n + 1], &format);
			n += format.skip;
			ret += ft_process(&format, ap);
		}
		n++;
	}
	va_end(ap);
	return (ret);
}
