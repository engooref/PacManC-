#ifndef CMAP_H_
#define CMAP_H_


#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_rotozoom.h>

#include <iostream>

class CMap
{
private:
	typedef unsigned char tileindex;

	//Structure contenant informations tiles
	struct s_tileProp {
		SDL_Rect R;
		bool full;
	};

	static SDL_Rect C_pRectArea;
	static SDL_Renderer* C_pRenderer;

private:
	
	//Largueur et Hauteur d'une tile
	int m_largeurTile;
	int m_hauteurTile;
	
	//Nb tiles en X et Y
	int m_nbTilesX;
	int m_nbTilesY;

	//lien vers la tileset chargée
	SDL_Surface* m_tileset;

	//Propriété tile
	s_tileProp* m_props;
	
	//tableau a deux dimensions contiendra les chiffre de la partie Level
	tileindex** m_schema;

	//nombre de tiles en largeur et hauteur dans le monde (dimension utilise pour le tableau schema)
	int m_nbTilesLargeur, m_nbTilesHauteur;

public:
	CMap(const char* pFileName);
	~CMap();

	void Draw(SDL_Renderer* pRenderer) const;

public:
	static void Init(SDL_Renderer* pRenderer,const SDL_Rect* Area) {
		C_pRenderer = pRenderer;
		memcpy(&C_pRectArea, Area, sizeof(SDL_Rect));
	};
};

#endif CMAP_H_