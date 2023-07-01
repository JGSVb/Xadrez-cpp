#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP
#include <array>
#include <SDL2/SDL.h>
#include "chess.hpp"


namespace graphics {

	enum Piece {
		PIECE_FIRST,
		PIECE_WPAWN = PIECE_FIRST,
		PIECE_WKNIGHT,
		PIECE_WBISHOP,
		PIECE_WROOK,
		PIECE_WQUEEN,
		PIECE_WKING,
		PIECE_BPAWN,
		PIECE_BKNIGHT,
		PIECE_BBISHOP,
		PIECE_BROOK,
		PIECE_BQUEEN,
		PIECE_BKING,
		PIECE_N,
		PIECE_NULL,
	};

	constexpr std::array<char*, chess::PIECE_N> DEFAULT_PIECES = {
		"./pieces/w_pawn.svg",
		"./pieces/w_knight.svg",
		"./pieces/w_bishop.svg",
		"./pieces/w_rook.svg",
		"./pieces/w_queen.svg",
		"./pieces/w_king.svg",
		"./pieces/b_pawn.svg",
		"./pieces/b_knight.svg",
		"./pieces/b_bishop.svg",
		"./pieces/b_rook.svg",
		"./pieces/b_queen.svg",
		"./pieces/b_king.svg"
	};

	class ChessWindow {
		std::array<SDL_Texture*, PIECE_N> pieceTextures;
		SDL_Texture 				    *boardTexture;
		SDL_Window   *sdlWindow;
		SDL_Renderer *sdlRenderer;
		int windowWidth;
		int windowHeight;
		int minDim;
		int rectSize;
		SDL_Color boardBlack;
		SDL_Color boardWhite;
		chess::Game *chessGame;
		int sq1;
		int sq2;

		void prepare_sdl(void);
		void end_sdl(void);

		void load_piece(const char *filename, Piece p);
		void load_pieces(std::array<char*,PIECE_N> pieces);
		void unload_pieces(void);
		
		void create_board(void);
		void destroy_board(void);

		void mouse_click(SDL_MouseButtonEvent *ev);
		bool compute_events(void);
		void show_piece(Piece p, int sq);
		void show_position(chess::Position *pos);
		void draw_sq(void);
		void draw(void);

		int square_at(int x, int y);

		chess::Move get_move(void);
	
		SDL_Rect get_square_rect(int index);

		public:
		ChessWindow(chess::Game *game){
			pieceTextures.fill(nullptr);
			boardTexture = NULL;
			sdlWindow = NULL;
			sdlRenderer = NULL;
			windowWidth = 80*8;
			windowHeight = windowWidth;

			minDim = std::min(windowWidth, windowHeight);
			rectSize = minDim/8;
		
			boardBlack.r = 0;
			boardBlack.g = 0;
			boardBlack.b = 0;
			boardBlack.a = 255;

			boardWhite.r = 255;
			boardWhite.g = 255;
			boardWhite.b = 255;
			boardWhite.a = 255;

			chessGame = game;
			
			sq1 = -1;
			sq2 = -1;

			prepare_sdl();
			load_pieces(DEFAULT_PIECES);
			create_board();
			
		}
		~ChessWindow(void){
			unload_pieces();
			destroy_board();
			end_sdl();
		}



		void main(void);
	};

	void init(void);
	void quit(void);
}
#endif // GRAPHICS_HPP
