#include "stdbit.h"

int __int_clz(unsigned int v);
int __int_ctz(unsigned int v);
int __int64_clz(unsigned long long v);
int __int64_ctz(unsigned long long v);

int stdc_leading_zerosuc(unsigned char value)
	{ return(__int_clz(value)-24); }
int stdc_leading_zerosus(unsigned short value)
	{ return(__int_clz(value)-16); }
int stdc_leading_zerosui(unsigned int value)
	{ return(__int_clz(value)); }
int stdc_leading_zerosul(unsigned long value)
	{ return(__int64_clz(value)); }
int stdc_leading_zerosull(unsigned long long value)
	{ return(__int64_clz(value)); }

int stdc_leading_zeros(stdbit_generic_value_type value)
{
	if(value == ((unsigned long long)value))
		return(64+stdc_leading_zerosull(value));
	return(stdc_leading_zerosull(value>>64));
}

int stdc_leading_onesuc(unsigned char value)
	{ return(__int_clz(~value)-24); }
int stdc_leading_onesus(unsigned short value)
	{ return(__int_clz(~value)-16); }
int stdc_leading_onesui(unsigned int value)
	{ return(__int_clz(~value)); }
int stdc_leading_onesul(unsigned long value)
	{ return(__int64_clz(~value)); }
int stdc_leading_onesull(unsigned long long value)
	{ return(__int64_clz(~value)); }

int stdc_leading_ones(stdbit_generic_value_type value)
	{ return(stdc_leading_zeros(~value)); }

int stdc_trailing_zerosuc(unsigned char value)
	{ return(__int_ctz(value)); }
int stdc_trailing_zerosus(unsigned short value)
	{ return(__int_ctz(value)); }
int stdc_trailing_zerosui(unsigned int value)
	{ return(__int_ctz(value)); }
int stdc_trailing_zerosul(unsigned long value)
	{ return(__int64_ctz(value)); }
int stdc_trailing_zerosull(unsigned long long value)
	{ return(__int64_ctz(value)); }

int stdc_trailing_zeros(stdbit_generic_value_type value)
{
	if(value == ((unsigned long long)value))
		return(stdc_trailing_zerosull(value));
	return(64+stdc_trailing_zerosull(value>>64));
}

int stdc_trailing_onesuc(unsigned char value)
	{ return(__int_ctz(~value)); }
int stdc_trailing_onesus(unsigned short value)
	{ return(__int_ctz(~value)); }
int stdc_trailing_onesui(unsigned int value)
	{ return(__int_ctz(~value)); }
int stdc_trailing_onesul(unsigned long value)
	{ return(__int64_ctz(~value)); }
int stdc_trailing_onesull(unsigned long long value)
	{ return(__int64_ctz(~value)); }
int stdc_trailing_ones(stdbit_generic_value_type value)
	{ return(stdc_trailing_zeros(~value)); }


int stdc_first_leading_zerouc(unsigned char value);
int stdc_first_leading_zerous(unsigned short value);
int stdc_first_leading_zeroui(unsigned int value)
	{ return(__int_clz(~value)); }
int stdc_first_leading_zeroul(unsigned long value)
	{ return(__int64_clz(~value)); }
int stdc_first_leading_zeroull(unsigned long long value)
	{ return(__int64_clz(~value)); }

int stdc_first_leading_zero(stdbit_generic_value_type value)
{
}

int stdc_first_leading_oneuc(unsigned char value);
int stdc_first_leading_oneus(unsigned short value);
int stdc_first_leading_oneui(unsigned int value);
int stdc_first_leading_oneul(unsigned long value);
int stdc_first_leading_oneull(unsigned long long value);
int stdc_first_leading_one(stdbit_generic_value_type value);

int stdc_first_trailing_zerouc(unsigned char value);
int stdc_first_trailing_zerous(unsigned short value);
int stdc_first_trailing_zeroui(unsigned int value);
int stdc_first_trailing_zeroul(unsigned long value);
int stdc_first_trailing_zeroull(unsigned long long value);
int stdc_first_trailing_zero(stdbit_generic_value_type value);

int stdc_first_trailing_oneuc(unsigned char value);
int stdc_first_trailing_oneus(unsigned short value);
int stdc_first_trailing_oneui(unsigned int value);
int stdc_first_trailing_oneul(unsigned long value);
int stdc_first_trailing_oneull(unsigned long long value);
int stdc_first_trailing_one(stdbit_generic_value_type value);

int stdc_count_onesuc(unsigned char value)
{
	const char hexnztab[16]=
		{0,1,1,2, 1,2,2,3, 1,2,2,3, 2,3,3,4};
	return(hexnztab[value&15]+hexnztab[(value>>4)&15]);
}

int stdc_count_onesus(unsigned short value)
	{ return(stdc_count_onesuc(value)+stdc_count_onesuc(value>>8)); }
int stdc_count_onesui(unsigned int value)
	{ return(stdc_count_onesus(value)+stdc_count_onesus(value>>16)); }
int stdc_count_onesul(unsigned long value)
	{ return(stdc_count_onesui(value)+stdc_count_onesui(value>>32)); }
int stdc_count_onesull(unsigned long long value)
	{ return(stdc_count_onesui(value)+stdc_count_onesui(value>>32)); }
int stdc_count_ones(stdbit_generic_value_type value)
	{ return(stdc_count_onesull(value)+stdc_count_onesull(value>>64)); }

int stdc_count_zerosuc(unsigned char value)
	{ return(stdc_count_onesuc(~value)); }
int stdc_count_zerosus(unsigned short value)
	{ return(stdc_count_onesus(~value)); }
int stdc_count_zerosui(unsigned int value)
	{ return(stdc_count_onesui(~value)); }
int stdc_count_zerosul(unsigned long value)
	{ return(stdc_count_onesul(~value)); }
int stdc_count_zerosull(unsigned long long value)
	{ return(stdc_count_onesull(~value)); }
int stdc_count_zeros(stdbit_generic_value_type value)
	{ return(stdc_count_ones(~value)); }

bool stdc_has_single_bituc(unsigned char value)
	{ return(stdc_count_onesuc(value)==1); }
bool stdc_has_single_bitus(unsigned short value)
	{ return(stdc_count_onesus(value)==1); }
bool stdc_has_single_bitui(unsigned int value)
	{ return(stdc_count_onesui(value)==1); }
bool stdc_has_single_bitul(unsigned long value)
	{ return(stdc_count_onesul(value)==1); }
bool stdc_has_single_bitull(unsigned long long value)
	{ return(stdc_count_onesull(value)==1); }
bool stdc_has_single_bit(stdbit_generic_value_type value)
	{ return(stdc_count_ones(value)==1); }

int stdc_bit_widthuc(unsigned char value)
	{ return(stdc_bit_widthull(value)); }
int stdc_bit_widthus(unsigned short value)
	{ return(stdc_bit_widthull(value)); }
int stdc_bit_widthui(unsigned int value)
	{ return(stdc_bit_widthull(value)); }
int stdc_bit_widthul(unsigned long value)
	{ return(stdc_bit_widthull(value)); }

int stdc_bit_widthull(unsigned long long value)
{
	unsigned long long tv;
	int e;
	
	if(!value)
		return(0);
	tv=value; e=0;
	while(tv>=2)
		{ tv=(tv+1)>>1; e++; }
	return(e);
}

int stdc_bit_width(stdbit_generic_value_type value)
{
	stdbit_generic_value_type tv;
	int e;
	
	if(!value)
		return(0);
	tv=value; e=0;
	while(tv>=2)
		{ tv=(tv+1)>>1; e++; }
	return(e);
}

unsigned char stdc_bit_flooruc(unsigned char value)
	{ return(stdc_bit_floorull(value)); }
unsigned short stdc_bit_floorus(unsigned short value)
	{ return(stdc_bit_floorull(value)); }
unsigned int stdc_bit_floorui(unsigned int value)
	{ return(stdc_bit_floorull(value)); }
unsigned long stdc_bit_floorul(unsigned long value)
	{ return(stdc_bit_floorull(value)); }

unsigned long long stdc_bit_floorull(unsigned long long value)
{
	unsigned long long tv;
	int e;
	
	if(!value)
		return(0);
	tv=value; e=0;
	while(tv>=2)
		{ tv=tv>>1; e++; }
	return(1ULL<<e);
}

stdbit_generic_value_type stdc_bit_floor(stdbit_generic_value_type value)
{
	stdbit_generic_value_type tv;
	int e;
	
	if(!value)
		return(0);
	tv=value; e=0;
	while(tv>=2)
		{ tv=tv>>1; e++; }
	return(1UI128<<e);
}

unsigned char stdc_bit_ceiluc(unsigned char value)
	{ return(stdc_bit_ceilull(value)); }
unsigned short stdc_bit_ceilus(unsigned short value)
	{ return(stdc_bit_ceilull(value)); }
unsigned int stdc_bit_ceilui(unsigned int value)
	{ return(stdc_bit_ceilull(value)); }
unsigned long stdc_bit_ceilul(unsigned long value)
	{ return(stdc_bit_ceilull(value)); }

unsigned long long stdc_bit_ceilull(unsigned long long value)
{
	unsigned long long tv;
	int e;
	
	if(!value)
		return(0);
	tv=value; e=0;
	while(tv>=2)
		{ tv=(tv+1)>>1; e++; }
	return(1ULL<<e);
}

stdbit_generic_value_type stdc_bit_ceil(stdbit_generic_value_type value)
{
	stdbit_generic_value_type tv;
	int e;
	
	if(!value)
		return(0);
	tv=value; e=0;
	while(tv>=2)
		{ tv=(tv+1)>>1; e++; }
	return(1UI128<<e);
}
