//
// Created by da.filiptsev on 13.11.2019.
//

#ifndef MYFLOAT_H
# define MYFLOAT_H

# include <stdint.h>
# define OFFSET_DBL 1023
# define OFFSET_LDBL 16383

typedef union	u_double
{
	uint64_t	intg;
	double		num;
}				t_double;

typedef struct	s_dbl_comp
{
	_Bool		sign;
	int32_t		exp_val;
	uint64_t	mant_val;
	uint32_t	mant_High_Bits;
	uint32_t	mant_Low_Bits;
}				t_dbl_comp;

_Bool			get_sign(t_double *dbl);
int32_t			get_exp(t_double *dbl);
uint64_t		get_mantissa(t_double *dbl);
_Bool			get_lsign(uint64_t *ldbl);
int32_t			get_lexp(uint64_t *ldbl);
uint64_t		get_lmantissa(uint64_t *ldbl);
char			*print_nan(_Bool sign);
char			*print_inf(_Bool sign);
char			*print_zero(_Bool sign);

#endif