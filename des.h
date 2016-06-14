#include <memory.h>

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned long ulong;

//==========================================================================
// Описание 64-х битного блока
typedef struct _ulong64
{
	ulong l;
	ulong h;
} ulong64;

//==========================================================================
// Получение 32-х битного числа из потока байт по позиции (формат BIG ENDIAN)
// BIG ENDIAN - ПЕРВЫМ БАЙТ ПОТОКА БУДЕТ СТАРШИМ БАЙТОВ В ЧИСЛЕ
// Параметры:
//   ulong n  - 32-х битное число, в которое записываются данные
//   uchar* b - входной поток байт
//   ulong i  - позиция, с которой начинается чтение данных из входного потока
#ifndef GET_ULONG_BE
#define GET_ULONG_BE(n,b,i)                     \
{                                               \
    (n) = ( (ulong) (b)[(i)    ] << 24 )        \
        | ( (ulong) (b)[(i) + 1] << 16 )        \
        | ( (ulong) (b)[(i) + 2] <<  8 )        \
        | ( (ulong) (b)[(i) + 3]       );       \
}
#endif

//==========================================================================
// Запись 32-х битного числа в поток байт в нужной позиции (формат BIG ENDIAN)
// Параметры:
//   ulong n  - 32-х битное число, из которго читаются данные
//   uchar* b - выходной поток байт
//   ulong i  - позиция, с которой начинается запись данных в выходной поток
#ifndef PUT_ULONG_BE
#define PUT_ULONG_BE(n,b,i)                     \
{                                               \
    (b)[(i)    ] = (uchar) ( (n) >> 24 );       \
    (b)[(i) + 1] = (uchar) ( (n) >> 16 );       \
    (b)[(i) + 2] = (uchar) ( (n) >>  8 );       \
    (b)[(i) + 3] = (uchar) ( (n)       );       \
}
#endif

//==========================================================================
// Функция формирования 16-ти раундовых ключей шифрования DES
// Параметры:
//   - src - 64-х битный ключ алгоритма DES
//   - keys - результирующие 16 64-х битных ключей циклов шифрования
void des_create_keys(ulong64 src, ulong64 keys[16]);
// Функция шифрования DES одного 64-х битного блока
// Параметры:
//   - src - 64-х битный входной блок открытого теста 
//   - keys - 16 64-х битных ключей циклов шифрования
// Результат:
//   - 64-х битный входной блок криптограммы
ulong64 des_encrypt_block(ulong64 src, ulong64 keys[16]);
// Функция расшифрования DES одного 64-х битного блока
// Параметры:
//   - src - 64-х битный входной блок криптограммы
//   - keys - 16 64-х битных ключей циклов шифрования
// Результат:
//   - 64-х битный входной блок открытого теста 
ulong64 des_decrypt_block(ulong64 src, ulong64 keys[16]);

// ECB
ulong des_encrypt_ecb(uchar *src, ulong srclen, uchar *dst, ulong64 key);
ulong des_decrypt_ecb(uchar *src, ulong srclen, uchar *dst, ulong64 key);

// CBC
ulong des_encrypt_cbc(uchar *src, ulong srclen, uchar *dst, ulong64 key, ulong64 iv);
ulong des_decrypt_cbc(uchar *src, ulong srclen, uchar *dst, ulong64 key, ulong64 iv);

// CFB
ulong des_encrypt_cfb(uchar *src, ulong srclen, uchar *dst, ulong64 key, ulong64 iv);
ulong des_decrypt_cfb(uchar *src, ulong srclen, uchar *dst, ulong64 key, ulong64 iv);

// OFB
ulong des_encrypt_ofb(uchar *src, ulong srclen, uchar *dst, ulong64 key, ulong64 iv);
ulong des_decrypt_ofb(uchar *src, ulong srclen, uchar *dst, ulong64 key, ulong64 iv);
