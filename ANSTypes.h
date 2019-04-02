#pragma once
#ifndef ANSTYPES_H
#define ANSTYPES_H

#include <cogra/types.h>

//! We call a Symbol an element of the uncompressed source alphabet.
typedef cogra::uint8 Symbol;

//! A CodewordUANS is an element of the compressed destination alphabet.
typedef cogra::uint32 CodewordUANS;

typedef cogra::uint16 RansByte;

//! A CodewordRANS is an element of the compressed destination alphabet.
typedef cogra::uint64 CodewordRANS;

//! pixel values
constexpr cogra::uint32 COUNT_OF_SYMBOLS = 256;

//! the basis for rans coding (=2^16)
constexpr cogra::uint64 BASIS_RANS = (1ul << 16ul);//65536;16

//! the sum of frequencies to scale to for rans coding (=2^11)
constexpr cogra::uint32 M_RANS = (1ul << 11ul);

//! the k 
constexpr cogra::uint32 K_RANS = (1ul << 2ul);

//! the default parameter L (= lower bound of normalized interval)for rans coding
constexpr cogra::uint32 LOWER_BOUND_RANS = (1ul << 16ul);//K_RANS * M_RANS;

//! the default parameter R (= upper bound of normalized interval)for rans coding
constexpr cogra::uint64 UPPER_BOUND_RANS = 4294967296;//(1ul << 32ul) gives zero..

//! the basis for uans coding
constexpr cogra::uint32 BASIS_UANS = 2;

//! the default parameter L for uans coding
constexpr cogra::uint32 LOWER_BOUND_UANS = 16;

#endif