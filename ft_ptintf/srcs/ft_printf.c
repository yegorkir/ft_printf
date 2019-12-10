/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mdeanne <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/05 22:57:32 by mdeanne           #+#    #+#             */
/*   Updated: 2019/12/05 22:57:37 by mdeanne          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_ptintf.h"

void	param_processing(char **format, t_prsng *tools)
{
	(*format)++;
	if (!parsing(format, tools))
		;
	organozation_by_flags_to_buff(tools);
}

void	zeroing_tools(t_prsng *tools, _Bool zeroing_counter)
{
	size_t i;

	tools->flags = 0;
	tools->field = 0;
	tools->precision = -1;
	tools->modifiers = 0;
	tools->type = 0;
	if (zeroing_counter)
	{
		tools->counter = 0;
		i = 0;
		while (i++ < BUFF_SIZE)
			tools->buff[i] = 0;
	}
}

int		ft_printf(const char *format, ...)
{
	t_prsng	tools;
	char	*f_not_const;

	f_not_const = (char*)format;
	zeroing_tools(&tools, 1);
	tools.buff[0] = '\0';
	va_start(tools.ap, format);
	while (*f_not_const)
	{
		if (*f_not_const == '%')
		{
			zeroing_tools(&tools, 0);
			param_processing(&f_not_const, &tools);
		}
		else
			add_str_to_buff(&f_not_const, &tools);
	}
	va_end(tools.ap);
	buffer_managment(&tools, NULL, 0, LAST);
	return (tools.counter);
}