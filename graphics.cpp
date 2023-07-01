#include <array>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cstdlib>
#include "chess.hpp"
#include "graphics.hpp"

namespace graphics {
	
	static bool Ginit = false;
	static bool Gquit = true;

	inline static void switch_state(void);
	inline static void error(const char *msg);
	inline static void sdl_error(const char *msg);
	static Piece chess_piece_to_graphics_piece(chess::Piece p);
	static void draw_rect_line_width(SDL_Renderer *render, SDL_Rect *r, int width);

	inline static void switch_state(void){
		Ginit = !Ginit;
		Gquit = !Gquit;
	}

	inline static void error(const char *msg){
		fprintf(stderr, "Impossível proceder: %s\n", msg);
		std::abort();
	}

	inline static void sdl_error(const char *msg){
		fprintf(stderr, "Impossível proceder: %s: %s\n", msg, SDL_GetError());
		std::abort();
	}

	void init(void){
		if(Ginit){
			error("O sistema gráfico já se inicializou");
		}

		switch_state();
		
		if(SDL_Init(SDL_INIT_VIDEO)!=0){
			sdl_error("Impossível inicializar vídeo SDL");
		}

	}
	void quit(void){
		if(Gquit){
			error("O sistema gráfico não se encontra inicializado");
		}

		switch_state();
		SDL_Quit();
	}

	void ChessWindow::prepare_sdl(void){
		this->sdlWindow = SDL_CreateWindow("Xadrez",
		     SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		     this->windowWidth, this->windowHeight, 0);
		if(!this->sdlWindow){
			sdl_error("Impossível criar janela");
		}

		this->sdlRenderer = SDL_CreateRenderer(this->sdlWindow, -1, 0);
		if(!this->sdlRenderer){
			sdl_error("Impossível criar renderizador");
		}
	}

	void ChessWindow::end_sdl(void){
		SDL_DestroyWindow(this->sdlWindow);
		SDL_DestroyRenderer(this->sdlRenderer);
	}

	void ChessWindow::load_piece(const char *filename, Piece p){
		SDL_Surface *image = IMG_Load(filename);
		SDL_Texture *txt;
		
		if(!image){
			sdl_error("Impossível carregar textura");
		}

		txt = SDL_CreateTextureFromSurface(this->sdlRenderer, image);
		
		if(!txt){
			sdl_error("Impossível criar textura");
		}

		SDL_FreeSurface(image);

		if(this->pieceTextures[p]){
			SDL_DestroyTexture(this->pieceTextures[p]);
		}
		this->pieceTextures[p] = txt;
	}

	void ChessWindow::load_pieces(std::array<char*,chess::PIECE_N> pieces){
		for(int i = PIECE_FIRST; i < PIECE_N; i++){
			this->load_piece(pieces[i], Piece(i));
		}
	}

	void ChessWindow::unload_pieces(void){
		for(auto t: pieceTextures){
			if(t){
				SDL_DestroyTexture(t);
			}
		}

	}
	void ChessWindow::create_board(void){
		SDL_Surface *board = SDL_CreateRGBSurface(0, this->minDim, this->minDim, 32, 0,0,0,0);
		Uint32 bgCol = SDL_MapRGBA(board->format, this->boardBlack.r, this->boardBlack.g, this->boardBlack.b,
			     this->boardBlack.a);
		Uint32 fgCol = SDL_MapRGBA(board->format, this->boardWhite.r, this->boardWhite.g, this->boardWhite.b,
			     this->boardWhite.a);

		// quadrados das brancas
		SDL_Rect bg = {0,0,this->minDim,this->minDim};
		SDL_FillRect(board, &bg, fgCol);

		// quadrados das negras
		SDL_Rect rects[32];
		
		for(int i = 0; i < 32; i++){
			int row = i/4;
			int col = i%4;
			rects[i].w = this->rectSize;
			rects[i].h = this->rectSize;	
			rects[i].x = (2*col + row%2) * this->rectSize;
			rects[i].y = row*this->rectSize;
		}
		SDL_FillRects(board, rects, 32, bgCol);

		// etc ...
		SDL_Texture *txt = SDL_CreateTextureFromSurface(this->sdlRenderer, board);
		SDL_FreeSurface(board);
		this->boardTexture = txt;
	}

	void ChessWindow::destroy_board(void){
		if(this->boardTexture){
			SDL_DestroyTexture(this->boardTexture);
		}
	}

	SDL_Rect ChessWindow::get_square_rect(int index){
		SDL_Rect r;
		r.w = this->rectSize;
		r.h = this->rectSize;
		r.x = ((index % 8)) * this->rectSize;
		r.y = (7-(index / 8)) * this->rectSize;
		return r;
	}

	static Piece chess_piece_to_graphics_piece(chess::Piece p){
		return Piece(chess::piece_index(p));
	}

	void ChessWindow::show_piece(Piece p, int sq){
		SDL_Texture *txt = this->pieceTextures[p];
		SDL_Rect r = this->get_square_rect(sq);
		SDL_RenderCopy(this->sdlRenderer, txt, NULL, &r);
	}

	void ChessWindow::show_position(chess::Position *pos){
		for(int i = 0; i < chess::SQUARE_COUNT; i++){
			chess::Piece p = pos->get_piece(chess::Square(i));
			if(p != chess::PIECE_NULL){
				this->show_piece(chess_piece_to_graphics_piece(p), i);
			}
		}
	}

	static void draw_rect_line_width(SDL_Renderer *render, SDL_Rect *r, int width){
		SDL_Rect t { *r };
		for(int i = 0; i < width; i++){
			SDL_RenderDrawRect(render, &t);
			t.x++;
			t.y++;
			t.w-=2;
			t.h-=2;
		}

	}

	void ChessWindow::draw_sq(void){
		if(this->sq1 != -1){
			SDL_Rect r { this->get_square_rect(this->sq1) };
			SDL_SetRenderDrawColor(this->sdlRenderer, 255,0,0,255);
			draw_rect_line_width(this->sdlRenderer, &r, 5);
		}
		if(this->sq2 != -1){
			SDL_Rect r { this->get_square_rect(this->sq2) };
			SDL_SetRenderDrawColor(this->sdlRenderer, 50,190,50,255);
			draw_rect_line_width(this->sdlRenderer, &r, 5);
		}
	}

	void ChessWindow::draw(void){
		SDL_Rect clear = {0,0,this->minDim,this->minDim};
		SDL_RenderCopy(this->sdlRenderer, this->boardTexture, NULL, &clear);

		chess::Position p = this->chessGame->get_position();
		this->show_position(&p);

		this->draw_sq();

		SDL_RenderPresent(this->sdlRenderer);
	}

	int ChessWindow::square_at(int x, int y){
		int file = x/this->rectSize;
		int rank = (7-y/this->rectSize);
		return file + rank*8;
	}

	chess::Move get_move(void){
		if((this->sq1 == -1) || (this->sq2 == -1)){
			return chess::move_new(chess::MOVE_NONE, chess::MOVE_COLORLESS);
		}
		return chess::move_new(chess::MOVE_NORMAL, chess
	}

	void ChessWindow::mouse_click(SDL_MouseButtonEvent *ev){
		if(ev->button != SDL_BUTTON_LEFT){
			return;
		}
		int i = square_at(ev->x, ev->y);
		if(i == this->sq1){
			this->sq1 = -1;
			this->sq2 = -1;
			return;
		} else if(i == this->sq2){
			this->sq2 = -1;
			return;
		}

		if(this->sq1 == -1){
			this->sq1 = i;
			return;
		} else if(this->sq2 == -1){
			this->sq2 = i;
			return;
		}

		this->sq2 = i;
		
		chess::Move m = this->get_move();
		if(chess::move_type(m) != chess::MOVE_NONE){
			this->chessGame->make_move(m);
			this->sq1 = -1;
			this->sq2 = -1;
		}
		
	}

	bool ChessWindow::compute_events(void){
		SDL_Event ev;
		while(SDL_PollEvent(&ev)){
			switch(ev.type){
				case SDL_QUIT:
					return false;
					break;
				case SDL_MOUSEBUTTONUP:
					this->mouse_click(&ev.button);
					break;

				default:
					break;
			}
		}

		return true;
	}


	void ChessWindow::main(void){
		while(this->compute_events()){
			draw();
			SDL_Delay(20);
		}
		
	}

}
