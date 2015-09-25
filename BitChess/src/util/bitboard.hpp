/*
 * bitboard.hpp
 *
 *  Created on: 25 Sep 2015
 *      Author: miles
 */

#ifndef UTIL_BITBOARD_HPP_
#define UTIL_BITBOARD_HPP_

#include <bitset>

namespace bitchess {

class Bitboard {
public:
	/**
	 * Initialises a new empty Bitboard, equivalent to a blank board
	 */
	Bitboard() {
	}
	/**
	 * Initialises the bitboard to the specified 64-bit integer value
	 * @param initval 64-bit integer
	 */
	Bitboard( long long unsigned int initval ) :
			bits(initval) {
	}
	Bitboard( std::bitset<64> new_bs):
		bits(new_bs) {
	}
	static Bitboard with_bit_set_at(short index) {
		Bitboard b = Bitboard();
		b.bits.set(index);
		return b;
	}

	inline Bitboard nortOne();
	inline Bitboard soutOne();
	inline Bitboard westOne();
	inline Bitboard eastOne();
	inline Bitboard noEaOne();
	inline Bitboard soEaOne();
	inline Bitboard noWeOne();
	inline Bitboard soWeOne();

	std::bitset<64> bits;
};

inline Bitboard operator<<(Bitboard b,int amount) {
	return Bitboard(b.bits.to_ulong() << amount);
}
inline Bitboard operator<<(Bitboard a,Bitboard b) {
	return Bitboard(a.bits << b.bits);
}
inline Bitboard operator>>(Bitboard b,int amount) {
	return Bitboard(b.bits.to_ulong() >> amount);
}
inline Bitboard operator>>(Bitboard a,Bitboard b) {
	return Bitboard(a.bits >> b.bits);
}
inline Bitboard operator&(Bitboard b,int amount) {
	return Bitboard(b.bits.to_ulong() & amount);
}
inline Bitboard operator&(Bitboard a,Bitboard b) {
	return Bitboard(a.bits & b.bits);
}
inline Bitboard operator~(Bitboard b) {
	return Bitboard(~b.bits);
}
inline Bitboard operator|(Bitboard b,int amount) {
	return Bitboard(b.bits.to_ulong() | amount);
}
inline Bitboard operator|(Bitboard a,Bitboard b) {
	return Bitboard(a.bits | b.bits);
}
inline Bitboard operator^(Bitboard b,int amount) {
	return Bitboard(b.bits.to_ulong() ^ amount);
}
inline Bitboard operator^(Bitboard a,Bitboard b) {
	return Bitboard(a.bits ^ b.bits);
}
inline bool operator==(Bitboard a,Bitboard b) {
	return a.bits == b.bits;
}
inline bool operator!=(Bitboard a,Bitboard b) {
	return a.bits != b.bits;
}

static const Bitboard NOT_A_MASK = Bitboard(0xfefefefefefefefeULL);
static const Bitboard NOT_H_MASK = Bitboard(0x7f7f7f7f7f7f7f7fULL);

inline Bitboard Bitboard::nortOne() {
	return *this << 8;
}

inline Bitboard Bitboard::eastOne() {
	return (*this << 1) & NOT_A_MASK;
}

inline Bitboard Bitboard::soutOne() {
	return *this >> 8;
}

inline Bitboard Bitboard::westOne() {
	return (*this >> 1) & NOT_H_MASK;
}

inline Bitboard Bitboard::noEaOne() {
	return (*this << 9) & NOT_A_MASK;
}

inline Bitboard Bitboard::soEaOne() {
	return (*this >> 7) & NOT_A_MASK;
}

inline Bitboard Bitboard::noWeOne() {
	return (*this << 7) & NOT_H_MASK;
}

inline Bitboard Bitboard::soWeOne() {
	return (*this >> 9) & NOT_H_MASK;
}

}

#endif /* UTIL_BITBOARD_HPP_ */
