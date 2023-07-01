#ifndef BITBOARD_HPP
#define BITBOARD_HPP

#include <bitset>

class BitBoard: public std::bitset<64> {
	public:
		BitBoard(): std::bitset<64>{} {}

};

#endif // BITBOARD_HPP

