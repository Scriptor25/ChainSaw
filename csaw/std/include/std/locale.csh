# LOCALE #

LC_ALL:      [const] int32 = 0;
LC_COLLATE:  [const] int32 = 1;
LC_CTYPE:    [const] int32 = 2;
LC_MONETARY: [const] int32 = 3;
LC_NUMERIC:  [const] int32 = 4;
LC_TIME:     [const] int32 = 5;

def lconv {
	decimal_point: int8*,
	thousands_sep: int8*,
	grouping: int8*,
	int_curr_symbol: int8*,
	currency_symbol: int8*,
	mon_decimal_point: int8*,
	mon_thousands_sep: int8*,
	mon_grouping: int8*,
	positive_sign: int8*,
	negative_sign: int8*,
	frac_digits: int8,
	p_cs_precedes: int8,
	n_cs_precedes: int8,
	p_sep_by_space: int8,
	n_sep_by_space: int8,
	p_sign_posn: int8,
	n_sign_posn: int8,
	int_frac_digits: int8,
	int_p_cs_precedes: int8,
	int_n_cs_precedes: int8,
	int_p_sep_by_space: int8,
	int_n_sep_by_space: int8,
	int_p_sign_posn: int8,
	int_n_sign_posn: int8
}

@[c]setlocale(category: int32, locale: int8*): int8*;
@[c]localeconv::lconv*;
