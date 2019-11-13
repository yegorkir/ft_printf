//
// Created by Mort Deanne on 2019-08-25.
//

#include "ft_ptintf.h"


void	fill_union_diouxx(t_mkfld *field, t_prsng *tools)
{
	field->number.ull = 0;

	if (tools->modifiers == 0 && tools->type == 'u')
		field->number.ui = va_arg(tools->ap, unsigned int);
	else if (tools->modifiers == 0)
		field->number.i = va_arg(tools->ap, int);
	else if (tools->modifiers & M_LL && tools->type == 'u')
		field->number.ull = va_arg(tools->ap, unsigned long long);
	else if (tools->modifiers & M_LL)
		field->number.ll = va_arg(tools->ap, long long);
	else if (tools->modifiers & M_L && tools->type == 'u')
		field->number.ul = va_arg(tools->ap, unsigned long);
	else if (tools->modifiers & M_L)
		field->number.l = va_arg(tools->ap, long);
	else if (tools->modifiers & M_H && tools->type == 'u')
		field->number.ush = (unsigned short)va_arg(tools->ap, unsigned int);
	else if (tools->modifiers & M_H)
		field->number.sh = (short)va_arg(tools->ap, int);
	//////////////////////////??????
	else if (tools->modifiers & M_H && tools->type == 'u')
		field->number.uc = (unsigned char)va_arg(tools->ap, unsigned int);
	else if (tools->modifiers & M_H)
		field->number.c = (char)va_arg(tools->ap, int);
	/////////////////////////??????
}

void	fill_union_csp(t_mkfld *field, t_prsng *tools)
{
	field->number.ull = 0;

	if (tools->type == 'c')
		field->number.c = (char)va_arg(tools->ap, int);
	else if (tools->type == 's')
		field->number.cptr = va_arg(tools->ap, char*);
	else if (tools->type == 'p')
		field->number.ull = va_arg(tools->ap, unsigned long long);
}

void	fill_union_aaeeffgg(t_mkfld *field, t_prsng *tools)
{
	field->number.db = 0.0;

	if (tools->modifiers & M_UPPER_L)
		field->number.ldb = va_arg(tools->ap, long double);
	else
		field->number.db = va_arg(tools->ap, double);
}

void	set_flags(t_mkfld *field, t_prsng *tools)
{
	if (tools->flags & M_SHARP && tools->type == 'o' && field->lennum >= (size_t)tools->precision) {
		field->str[field->len - 1] = '0';
		field->len--;
	}
	else if ((tools->flags & M_SHARP) && !(tools->flags & M_ZERO) &&
			(((tools->type == 'x' || tools->type == 'X') && field->number.i)))
	{
		field->str[field->len - 2] = '0';
		field->str[field->len - 1] = tools->type;
		field->len -= 2;
	}
	else if ((tools->flags & M_SHARP) && tools->type == 'p')
	{
		ft_strncpy(&field->str[field->len - 2], "0x", 2);
		field->len -= 2;
	}
	/** дополнить # для других флагов*/

	if ((tools->flags & M_ZERO && (!(tools->flags & M_PRECISION) && is_ddioouuxx(tools->type))) /// убрал ! после && (
			&& !(is_ddioouuxx(tools->type) && tools->precision == 0 && !field->number.i))
	{
		while (field->len)
			field->str[--field->len] = '0';
		if ((tools->flags & M_PLUS && field->number.i > 0) || field->number.i < 0 || tools->flags & M_SPACE)
			field->len++;
	}
	/** дополнить для других флагов кроме diouxX*/

	/** пишем поверх нулей "0x"*/
	if ((tools->flags & M_SHARP && (tools->flags & M_ZERO) != 0 &&
			  ((tools->type == 'x' || tools->type == 'X') && field->number.i)))
	{
		field->str[0] = '0';
		field->str[1] = tools->type;
		field->len -= 2;
	}


	if (tools->flags & M_PLUS && is_signed(tools->type) && field->number.i > 0)
		field->str[--field->len] = '+';
	else if (is_signed(tools->type) && field->number.i < 0)
		field->str[--field->len] = '-';
	else if (tools->flags & M_SPACE)
		field->str[--field->len] = ' ';

}





void	len_counting(t_prsng *tools, t_mkfld *field)
{
	field->len = 0;
	field->len_empty_field = 0;
	if ((tools->precision > 0 && (size_t)tools->precision > field->lennum)
		&& !(tools->type == 's' && !ft_strlen(field->number.cptr)))
		field->len += tools->precision - field->lennum;
	field->len += define_flaglen(field, tools);
	if (tools->field > field->lennum + field->len)
		field->len_empty_field = tools->field - field->lennum - field->len;
	field->len += field->len_empty_field;
}

void	prepare_diouxxcsp(t_prsng *tools, t_mkfld *field)
{
	if (is_ddioouuxx(tools->type))
		fill_union_diouxx(field, tools);
	else if (is_csp(tools->type))
		fill_union_csp(field, tools);

	field->base = define_base(tools);

	if (tools->type && (tools->type == 'c' || !is_flag(tools->type)))
		field->lennum = 1;
	else if (is_ddioouuxx(tools->type) && tools->precision == 0 && !field->number.i)
		field->lennum = 0;
	else if (tools->type == 's')
		{
			field->lennum = ft_strlen(field->number.cptr);
			if (field->lennum > (size_t)tools->precision && tools->precision)
				field->lennum = tools->precision;
		}
	else if (is_ddioouuxx(tools->type) || tools->type == 'p')
			field->lennum = count_lennum(field, tools);

	len_counting(tools, field);
}

void	prepare_aaeeffgg(t_prsng *tools, t_mkfld *field)
{
	///вызов PrintDouble/LDouble(tools, field->number.db(ldb));
	fill_union_aaeeffgg(field, tools);
}

void	zeroing_mkfield(t_mkfld *fld)
{
	fld->base = 0;
	fld->len = 0;
	fld->len_empty_field = 0;
	fld->lennum = 0;
	fld->number.i = 0;
	fld->str = NULL;
}

int 	organozation_by_flags_to_buff(t_prsng *tools)
{
	t_mkfld field;

	zeroing_mkfield(&field);
	if (is_aaeeffgg(tools->type))
	{
		prepare_aaeeffgg(tools, &field);
		if (tools->type && !set_buff(&field, tools))
			return (0);
	}
	else if (is_ddioouuxx(tools->type) || is_csp(tools->type) || !is_flag(tools->type))
	{
		prepare_diouxxcsp(tools, &field);
		if (tools->type && !set_buff(&field, tools))
			return (0);
	}
	return (1);
}
