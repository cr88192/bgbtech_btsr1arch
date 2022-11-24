#ifndef __STDBIT_INCLUDED
#define __STDBIT_INCLUDED

#include <stdint.h>

#define		__STDC_ENDIAN_LITTLE__	1
#define		__STDC_ENDIAN_BIG__		2

#ifdef __BJX2__
#define __STDC_ENDIAN_NATIVE__	__STDC_ENDIAN_LITTLE__
#endif



#ifndef __STDC_ENDIAN_NATIVE__
#define __STDC_ENDIAN_NATIVE__	__STDC_ENDIAN_LITTLE__
#endif

#define stdbit_generic_value_type	uint128_t


int stdc_leading_zerosuc(unsigned char value);
int stdc_leading_zerosus(unsigned short value);
int stdc_leading_zerosui(unsigned int value);
int stdc_leading_zerosul(unsigned long value);
int stdc_leading_zerosull(unsigned long long value);
int stdc_leading_zeros(stdbit_generic_value_type value);

int stdc_leading_onesuc(unsigned char value);
int stdc_leading_onesus(unsigned short value);
int stdc_leading_onesui(unsigned int value);
int stdc_leading_onesul(unsigned long value);
int stdc_leading_onesull(unsigned long long value);
int stdc_leading_ones(stdbit_generic_value_type value);

int stdc_trailing_zerosuc(unsigned char value);
int stdc_trailing_zerosus(unsigned short value);
int stdc_trailing_zerosui(unsigned int value);
int stdc_trailing_zerosul(unsigned long value);
int stdc_trailing_zerosull(unsigned long long value);
int stdc_trailing_zeros(stdbit_generic_value_type value);

int stdc_trailing_onesuc(unsigned char value);
int stdc_trailing_onesus(unsigned short value);
int stdc_trailing_onesui(unsigned int value);
int stdc_trailing_onesul(unsigned long value);
int stdc_trailing_onesull(unsigned long long value);
int stdc_trailing_ones(stdbit_generic_value_type value);


int stdc_first_leading_zerouc(unsigned char value);
int stdc_first_leading_zerous(unsigned short value);
int stdc_first_leading_zeroui(unsigned int value);
int stdc_first_leading_zeroul(unsigned long value);
int stdc_first_leading_zeroull(unsigned long long value);
int stdc_first_leading_zero(stdbit_generic_value_type value);

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

int stdc_count_onesuc(unsigned char value);
int stdc_count_onesus(unsigned short value);
int stdc_count_onesui(unsigned int value);
int stdc_count_onesul(unsigned long value);
int stdc_count_onesull(unsigned long long value);
int stdc_count_ones(stdbit_generic_value_type value);

int stdc_count_zerosuc(unsigned char value);
int stdc_count_zerosus(unsigned short value);
int stdc_count_zerosui(unsigned int value);
int stdc_count_zerosul(unsigned long value);
int stdc_count_zerosull(unsigned long long value);
int stdc_count_zeros(stdbit_generic_value_type value);

bool stdc_has_single_bituc(unsigned char value);
bool stdc_has_single_bitus(unsigned short value);
bool stdc_has_single_bitui(unsigned int value);
bool stdc_has_single_bitul(unsigned long value);
bool stdc_has_single_bitull(unsigned long long value);
bool stdc_has_single_bit(stdbit_generic_value_type value);

int stdc_bit_widthuc(unsigned char value);
int stdc_bit_widthus(unsigned short value);
int stdc_bit_widthui(unsigned int value);
int stdc_bit_widthul(unsigned long value);
int stdc_bit_widthull(unsigned long long value);
int stdc_bit_width(stdbit_generic_value_type value);

unsigned char stdc_bit_flooruc(unsigned char value);
unsigned short stdc_bit_floorus(unsigned short value);
unsigned int stdc_bit_floorui(unsigned int value);
unsigned long stdc_bit_floorul(unsigned long value);
unsigned long long stdc_bit_floorull(unsigned long long value);
stdbit_generic_value_type stdc_bit_floor(stdbit_generic_value_type value);

unsigned char stdc_bit_ceiluc(unsigned char value);
unsigned short stdc_bit_ceilus(unsigned short value);
unsigned int stdc_bit_ceilui(unsigned int value);
unsigned long stdc_bit_ceilul(unsigned long value);
unsigned long long stdc_bit_ceilull(unsigned long long value);
stdbit_generic_value_type stdc_bit_ceil(stdbit_generic_value_type value);

#endif
