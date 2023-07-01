#include <cstdlib>
#include <cassert>
#include <algorithm> // chess::Position::setup_from_string
#include "chess.hpp"

namespace chess {
	PieceColor Position::get_piece_color(Square sq){
		if(this->byColorBB[PIECE_WHITE][sq]){
			return PIECE_WHITE;
		} 
		if(this->byColorBB[PIECE_BLACK][sq]){
			return PIECE_BLACK;
		} 
		return PIECE_COLORLESS;
	}

	PieceType Position::get_piece_type(Square sq){
		for(int i = 0; i < PIECE_N_TYPES; i++){
			if(this->byTypeBB[i][sq]){
				return PieceType(i);
			}
		}
		return PIECE_TYPELESS;
	}

	Piece Position::get_piece(Square sq){
		if(!this->occupiedBB[sq]){
			return PIECE_NULL;
		}
		return piece_new(this->get_piece_type(sq), this->get_piece_color(sq));
	}

	void Position::empty_square(Square sq){
		this->occupiedBB[sq] = false;

		int i;
		for(i = 0; i < PIECE_N_TYPES; i++){
			this->byTypeBB[i] &= this->occupiedBB;
		}
		for(i = 0; i < PIECE_N_COLORS; i++){
			this->byColorBB[i] &= this->occupiedBB;
		}
	}

	void Position::set_piece(Square sq, Piece p){
		if(p == PIECE_NULL){
			this->empty_square(sq);
		} else {
			this->occupiedBB[sq] = true;

			PieceType t = piece_type(p);
			PieceColor c = piece_color(p);

			this->byTypeBB[t][sq] = true;
			this->byColorBB[c][sq] = true;
		}
	}

	Position Position::from_string(char *str){
		Position pos;
		for(int i = 0; i < SQUARE_COUNT; i++){
			char c = str[i];
			if(!std::count(PIECE_CHAR.begin(), PIECE_CHAR.end(), c)){
				continue;
			}
			Piece p = piece_from_char(str[i]);
			pos.set_piece(Square(i), p);
		} 
		return pos;
	}
	
	bool Position::is_normal_legal(Move m){
		assert(move_type(m) == MOVE_NORMAL);

		Square src = move_src(m);
		Square dst = move_dst(m);
		PieceColor cSrc = this->get_piece_color(src);
		PieceColor cDst = this->get_piece_color(dst);
		if((src == dst) ||
		   (cSrc == cDst) ||
		   (cSrc == PIECE_COLORLESS)) { // ou seja, quadrado vazio
			return false;
		}
	}

	void Position::apply_normal(Move m){
		assert(move_type(m) == MOVE_NORMAL);

		Square src = move_src(m);
		Square dst = move_dst(m);
		Piece p1 = this->get_piece(src);
		this->empty_square(src);
		this->set_piece(dst, p1);
	}

	void Position::switch_side(void){
		this->sideToMove = ~this->sideToMove;
	}
	
	bool Position::is_legal(Move m){
		if(PieceColor(move_color(m))!=this->sideToMove){
			return false;
		}
		switch(move_type(m)){
			case MOVE_NORMAL:
				return this->is_normal_legal(m);
				break;
			default:
				return false;
		}
	}

	bool Position::make_move(Move m){
		if(!this->is_legal(m)){
			return false;
		}

		this->switch_side();
		
		MoveType t = move_type(m);
		switch(t){
			case(MOVE_NORMAL):
				this->apply_normal(m);
				break;
			default:
				std::abort();
				break;
		}

		return true;
	}

	
	bool Game::make_move(Move m){
		Position cpy { this->currPosition.copy() };

		if(!cpy.make_move(m)){
			return false;
		}

		this->currPosition = cpy;
		return true;
	}

	void test(void){
		assert(piece_from_char('p')==PIECE_WPAWN);
		assert(piece_from_char('n')==PIECE_WKNIGHT);
		assert(piece_from_char('b')==PIECE_WBISHOP);
		assert(piece_from_char('r')==PIECE_WROOK);
		assert(piece_from_char('q')==PIECE_WQUEEN);
		assert(piece_from_char('k')==PIECE_WKING);
		assert(piece_from_char('P')==PIECE_BPAWN);
		assert(piece_from_char('N')==PIECE_BKNIGHT);
		assert(piece_from_char('B')==PIECE_BBISHOP);
		assert(piece_from_char('R')==PIECE_BROOK);
		assert(piece_from_char('Q')==PIECE_BQUEEN);
		assert(piece_from_char('K')==PIECE_BKING);

		assert(piece_char(PIECE_WPAWN)=='p');
		assert(piece_char(PIECE_WKNIGHT)=='n');
		assert(piece_char(PIECE_WBISHOP)=='b');
		assert(piece_char(PIECE_WROOK)=='r');
		assert(piece_char(PIECE_WQUEEN)=='q');
		assert(piece_char(PIECE_WKING)=='k');
		assert(piece_char(PIECE_BPAWN)=='P');
		assert(piece_char(PIECE_BKNIGHT)=='N');
		assert(piece_char(PIECE_BBISHOP)=='B');
		assert(piece_char(PIECE_BROOK)=='R');
		assert(piece_char(PIECE_BQUEEN)=='Q');
		assert(piece_char(PIECE_BKING)=='K');
		
		assert(piece_index(PIECE_WPAWN)==0);
		assert(piece_index(PIECE_WKNIGHT)==1);
		assert(piece_index(PIECE_WBISHOP)==2);
		assert(piece_index(PIECE_WROOK)==3);
		assert(piece_index(PIECE_WQUEEN)==4);
		assert(piece_index(PIECE_WKING)==5);
		assert(piece_index(PIECE_BPAWN)==6);
		assert(piece_index(PIECE_BKNIGHT)==7);
		assert(piece_index(PIECE_BBISHOP)==8);
		assert(piece_index(PIECE_BROOK)==9);
		assert(piece_index(PIECE_BQUEEN)==10);
		assert(piece_index(PIECE_BKING)==11);
		assert(piece_index(PIECE_BKING)==11);

		assert(piece_type(PIECE_WPAWN) == PIECE_PAWN);
		assert(piece_type(PIECE_WKNIGHT) == PIECE_KNIGHT);
		assert(piece_type(PIECE_WBISHOP) == PIECE_BISHOP);
		assert(piece_type(PIECE_WROOK) == PIECE_ROOK);
		assert(piece_type(PIECE_WQUEEN) == PIECE_QUEEN);
		assert(piece_type(PIECE_WKING) == PIECE_KING);
		assert(piece_type(PIECE_BPAWN) == PIECE_PAWN);
		assert(piece_type(PIECE_BKNIGHT) == PIECE_KNIGHT);
		assert(piece_type(PIECE_BBISHOP) == PIECE_BISHOP);
		assert(piece_type(PIECE_BROOK) == PIECE_ROOK);
		assert(piece_type(PIECE_BQUEEN) == PIECE_QUEEN);
		assert(piece_type(PIECE_BKING) == PIECE_KING);

		assert(piece_color(PIECE_WPAWN) == PIECE_WHITE);
		assert(piece_color(PIECE_WKNIGHT) == PIECE_WHITE);
		assert(piece_color(PIECE_WBISHOP) == PIECE_WHITE);
		assert(piece_color(PIECE_WROOK) == PIECE_WHITE);
		assert(piece_color(PIECE_WQUEEN) == PIECE_WHITE);
		assert(piece_color(PIECE_WKING) == PIECE_WHITE);
		assert(piece_color(PIECE_BPAWN) == PIECE_BLACK);
		assert(piece_color(PIECE_BKNIGHT) == PIECE_BLACK);
		assert(piece_color(PIECE_BBISHOP) == PIECE_BLACK);
		assert(piece_color(PIECE_BROOK) == PIECE_BLACK);
		assert(piece_color(PIECE_BQUEEN) == PIECE_BLACK);
		assert(piece_color(PIECE_BKING) == PIECE_BLACK);

		assert(piece_new(PIECE_PAWN, PIECE_WHITE) == PIECE_WPAWN);
		assert(piece_new(PIECE_KNIGHT, PIECE_WHITE) == PIECE_WKNIGHT);
		assert(piece_new(PIECE_BISHOP, PIECE_WHITE) == PIECE_WBISHOP);
		assert(piece_new(PIECE_ROOK, PIECE_WHITE) == PIECE_WROOK);
		assert(piece_new(PIECE_QUEEN, PIECE_WHITE) == PIECE_WQUEEN);
		assert(piece_new(PIECE_KING, PIECE_WHITE) == PIECE_WKING);
		assert(piece_new(PIECE_PAWN, PIECE_BLACK) == PIECE_BPAWN);
		assert(piece_new(PIECE_KNIGHT, PIECE_BLACK) == PIECE_BKNIGHT);
		assert(piece_new(PIECE_BISHOP, PIECE_BLACK) == PIECE_BBISHOP);
		assert(piece_new(PIECE_ROOK, PIECE_BLACK) == PIECE_BROOK);
		assert(piece_new(PIECE_QUEEN, PIECE_BLACK) == PIECE_BQUEEN);
		assert(piece_new(PIECE_KING, PIECE_BLACK) == PIECE_BKING);


		#define assert1(e) do { if(!(e)) { printf("src: %d dst: %d\n", i, k); assert((e)); } } while(0) 
		for(int i = 0; i < 64; i++){
			for(int k = 0; k < 64; k++){
				Move m;
				m = move_new(MOVE_NORMAL, MOVE_WHITE, Square(i), Square(k));
				assert1(move_type(m) == MOVE_NORMAL);
				assert1(move_color(m) == MOVE_WHITE);
				assert1(move_src(m) == Square(i));
				assert1(move_dst(m) == Square(k));

				m = move_new(MOVE_NORMAL, MOVE_BLACK, Square(i), Square(k));
				assert1(move_type(m) == MOVE_NORMAL);
				assert1(move_color(m) == MOVE_BLACK);
				assert1(move_src(m) == Square(i));
				assert1(move_dst(m) == Square(k));

				m = move_new(MOVE_NONE, MOVE_WHITE, Square(i), Square(k));
				assert1(move_type(m) == MOVE_NONE);
				assert1(move_color(m) == MOVE_WHITE);
				assert1(move_src(m) == Square(i));
				assert1(move_dst(m) == Square(k));

				m = move_new(MOVE_NONE, MOVE_BLACK, Square(i), Square(k));
				assert1(move_type(m) == MOVE_NONE);
				assert1(move_color(m) == MOVE_BLACK);
				assert1(move_src(m) == Square(i));
				assert1(move_dst(m) == Square(k));
			}
		}
	}
}
