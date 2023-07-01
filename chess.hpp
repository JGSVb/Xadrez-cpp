#ifndef CHESS_HPP
#define CHESS_HPP

#include <array>
#include <algorithm> // piece_index, piece from index ...; std::find
#include <iterator> // piece_index, ...... 		 ; std::distance
#include <cstdint>
#include "bitboard.hpp"

namespace chess {

	constexpr int SQUARE_COUNT { 64 };

	enum Square : int {
		A1, B1, C1, D1, E1, F1, G1, H1,
		A2, B2, C2, D2, E2, F2, G2, H2,
		A3, B3, C3, D3, E3, F3, G3, H3,
		A4, B4, C4, D4, E4, F4, G4, H4,
		A5, B5, C5, D5, E5, F5, G5, H5,
		A6, B6, C6, D6, E6, F6, G6, H6,
		A7, B7, C7, D7, E7, F7, G7, H7,
		A8, B8, C8, D8, E8, F8, G8, H8,
	};

	constexpr std::array<char[3], SQUARE_COUNT> SQUARE_NAME {
		"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
		"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
		"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
		"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
		"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
		"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
		"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
		"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
	};

	inline Square square_new(int file, int rank){
		return Square(rank*8 + file);
	}
	inline int square_rank(Square sq){
		return sq/8;
	}
	inline int square_file(Square sq){
		return sq%8;
	}


	// PEÇA: 0x00000000
	// 1. Byte: cor e tipo
	// 0x000000AB: A: cor, B: tipo
	enum PieceType : int {
		PIECE_PAWN,
		PIECE_KNIGHT,
		PIECE_BISHOP,
		PIECE_ROOK,
		PIECE_QUEEN,
		PIECE_KING,
		PIECE_N_TYPES,
		PIECE_TYPELESS,
	};

	enum PieceColor : int {
		PIECE_WHITE,
		PIECE_BLACK,
		PIECE_N_COLORS,
		PIECE_COLORLESS,
	};

	enum Piece : int {
		PIECE_WPAWN = PIECE_PAWN | PIECE_WHITE<<4,
		PIECE_WKNIGHT = PIECE_KNIGHT | PIECE_WHITE<<4,
		PIECE_WBISHOP = PIECE_BISHOP | PIECE_WHITE<<4,
		PIECE_WROOK = PIECE_ROOK | PIECE_WHITE<<4,
		PIECE_WQUEEN = PIECE_QUEEN | PIECE_WHITE<<4,
		PIECE_WKING = PIECE_KING | PIECE_WHITE<<4,
		
		PIECE_BPAWN = PIECE_PAWN | PIECE_BLACK<<4,
		PIECE_BKNIGHT = PIECE_KNIGHT | PIECE_BLACK<<4,
		PIECE_BBISHOP = PIECE_BISHOP | PIECE_BLACK<<4,
		PIECE_BROOK = PIECE_ROOK | PIECE_BLACK<<4,
		PIECE_BQUEEN = PIECE_QUEEN | PIECE_BLACK<<4,
		PIECE_BKING = PIECE_KING | PIECE_BLACK<<4,
		PIECE_NULL,
		PIECE_N = 12,
	};

	constexpr std::array<char, PIECE_N> PIECE_CHAR {'p', 'n', 'b', 'r', 'q', 'k',
		'P', 'N', 'B', 'R', 'Q', 'K'};
	constexpr std::array<Piece, PIECE_N> PIECE_LIST {
		PIECE_WPAWN, PIECE_WKNIGHT, PIECE_WBISHOP, PIECE_WROOK, PIECE_WQUEEN, PIECE_WKING,
		PIECE_BPAWN, PIECE_BKNIGHT, PIECE_BBISHOP, PIECE_BROOK, PIECE_BQUEEN, PIECE_BKING,
	};
	
	inline PieceColor operator~(PieceColor c){
		switch(c){
			case PIECE_WHITE:
				return PIECE_BLACK;
				break;
			case PIECE_BLACK:
				return PIECE_WHITE;
				break;
			default:
				return PIECE_COLORLESS;
				break;
		}
	}

	inline PieceType piece_type(Piece p){
		return PieceType(p & 0x0F);
	}
	inline PieceColor piece_color(Piece p){
		return PieceColor(p>>4);
	}
	inline Piece piece_new(PieceType t, PieceColor c){
		return Piece(t | (c<<4));
	}
	inline int piece_index(Piece p){
		auto it = std::find(PIECE_LIST.begin(), PIECE_LIST.end(), p);
		return std::distance(PIECE_LIST.begin(), it);
	}
	inline Piece piece_from_index(int i){
		return PIECE_LIST[i];
	}
	inline char piece_char(Piece p){
		return PIECE_CHAR[piece_index(p)];
	}
	inline Piece piece_from_char(char c){
		auto it = std::find(PIECE_CHAR.begin(), PIECE_CHAR.end(), c);
		int i = std::distance(PIECE_CHAR.begin(), it);
		return PIECE_LIST[i];
	}

	enum CastleRight : int {
		CASTLE_WHITE = 0x01,
		CASTLE_BLACK = 0x10,
		CASTLE_BOTH  = 0x11,
	};

	enum MoveType : int {
		MOVE_NONE,
		MOVE_NORMAL,
	};
	enum MoveColor : int {
		MOVE_WHITE = PIECE_WHITE,
		MOVE_BLACK = PIECE_BLACK,
		MOVE_COLORLESS,
	};

	typedef uint32_t Move;

	inline Move move_new(MoveType t, MoveColor c, Square src, Square dst){
		return t | (c<<8) | (src<<16) | (dst<<22);
	}
	inline Move move_new(MoveType t, MoveColor c){
		return t | (c<<8);
	}
	inline MoveType move_type(Move m){
		return MoveType(m&0xFF);
	}
	inline MoveColor move_color(Move m){
		return MoveColor((m>>8)&0xFF);
	}
	inline Square move_src(Move m){
		return Square((m>>16)&63);
	}
	inline Square move_dst(Move m){
		return Square((m>>22)&63);
	}

	constexpr char* DEFAULT_POSITION {
		"rnbqkbnr"
		"pppppppp"
		"        "
		"        "
		"        "
		"        "
		"PPPPPPPP"
		"RNBQKBNR"
	};

	class Position {
		std::array<BitBoard, PIECE_N_TYPES> byTypeBB;
		std::array<BitBoard, PIECE_N_COLORS> byColorBB;
		BitBoard occupiedBB;

		CastleRight castleRights;
		PieceColor sideToMove;

		PieceColor get_piece_color(Square sq);
		PieceType get_piece_type(Square sq);
		void set_piece(Square sq, Piece p);
		void empty_square(Square sq);

		bool is_normal_legal(Move m);
		void apply_normal(Move m);

		void switch_side(void);

		public:
		Position(void) : castleRights { CASTLE_BOTH }, sideToMove { PIECE_WHITE }{}
		Position copy(void){
			return *this;
		}
		
		static Position from_string(char *str);
		Piece get_piece(Square sq);
		Piece get_piece(int file, int rank){ return get_piece(square_new(file, rank)); };
		bool is_legal(Move m);
		bool make_move(Move m);
	};

	constexpr int BUFSIZE { 2048 };

	class Game {
		Position currPosition;

		public:
		Game(void){
			currPosition = Position::from_string(DEFAULT_POSITION);
		}

		bool make_move(Move m);
		Position get_position(void){
			return currPosition;
		}

	};

	void test(void);
}


#endif // CHESS_HPP
