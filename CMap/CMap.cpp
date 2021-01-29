#include "CMap.h"

#include <cstring>
#include <cstdio>

using namespace std;

SDL_Rect CMap::C_pRectArea{0};
SDL_Renderer* CMap::C_pRenderer{nullptr};

#define CACHE_SIZE		5000

CMap::CMap(const char* pFileName)
{

	srand((unsigned int)time(NULL));

	//Ouverture Fichier
	FILE* file;
	file = fopen(pFileName, "r");
	if (!file) {
		string except = "Fichier ";
		except += pFileName;
		except += " introuvable !!";

		throw except;
	}

	//Creation Buffer
	char buf[CACHE_SIZE];  
	char buf2[CACHE_SIZE]; 

	fseek(file, strlen("#tileset"), SEEK_SET); // Se deplace a la prochaine ligne du fichier
	fscanf(file, "%s", &buf); // Nom du fichier
	
	m_tileset = IMG_Load(buf);	// Charge le tileset en RAM
	if (m_tileset == NULL)
	{
		string except = "Image ";
		except += buf;
		except += " introuvable !!";

		throw except;
	}

	fscanf(file, "%d %d", &m_nbTilesX, &m_nbTilesY);
	fscanf(file, "%d %d", &m_nbTilesLargeur, &m_nbTilesHauteur); //Lecture de la longueur et largueur total du monde en tiles

	double ratioY = ((C_pRectArea.h + C_pRectArea.y) / m_nbTilesHauteur) * m_nbTilesY / (double)m_tileset->h;
	double ratioX = ((C_pRectArea.w + C_pRectArea.x)/ m_nbTilesLargeur) * m_nbTilesX / (double)m_tileset->w;

	//Lecture du nombre de Tiles dans le monde dans le tilset

	m_tileset = rotozoomSurfaceXY(m_tileset, 0.0, ratioX, ratioY, 1);

	//Calcul de la longueur et la largueur de chaque Tiles puis allocation memoire

	m_largeurTile = m_tileset->w / m_nbTilesX;
	m_hauteurTile = m_tileset->h / m_nbTilesY;
	m_props = reinterpret_cast<s_tileProp*>(malloc(m_nbTilesX * m_nbTilesY * sizeof(s_tileProp)));
	
	int numTiles = 0;
	fscanf(file, "%s %s", buf, buf2);

	for (int j = 0; j < m_nbTilesY; j++)
	{
		for (int i = 0; i < m_nbTilesX; i++, numTiles++)
		{
			//Definition de la position et de la taille de chaque tiles
			m_props[numTiles].R.w = m_largeurTile;
			m_props[numTiles].R.h = m_hauteurTile;
			m_props[numTiles].R.x = i * m_largeurTile;
			m_props[numTiles].R.y = j * m_hauteurTile;

			//Gestion de la transparence pour le personnage avec chaque tiles
			if (atoi(&buf[strlen("tile")]) == numTiles) {
				m_props[numTiles].full = strcmp(buf2, "vide");
				fscanf(file, "%s", buf);
				if (strcmp(buf, "#level")) {
					fscanf(file, "%s", buf2);
				}
			}
			else {
				m_props[numTiles].full = m_props[0].full;
			}
			
			
		}
	}
	
	
	m_schema = reinterpret_cast<tileindex**>(malloc(m_nbTilesLargeur * sizeof(tileindex*))); //Allocation memoire du monde

	for (int i = 0; i < m_nbTilesLargeur; i++)
		m_schema[i] = reinterpret_cast<tileindex*>(malloc(m_nbTilesHauteur * sizeof(tileindex))); //Allocation memoire de chaque tiles

	int tmp;
	
	for (int j = 0; j < m_nbTilesHauteur; j++)
	{
		for (int i = 0; i < m_nbTilesLargeur; i++)
		{
			//Lecture du nombre pour chaque tiles		
			fscanf(file, "%d", &tmp);
			if (tmp >= (m_nbTilesX * m_nbTilesY))
			{
				throw string("Level Tile hors limite");
			}
			//Attribution pour chaque membre du monde
			
			if (tmp==-1) {

			}

			m_schema[i][j] = tmp;
		}
	}

	fclose(file);
}

CMap::~CMap()
{
	
	SDL_FreeSurface(m_tileset);
	for (int i = 0; i < m_nbTilesLargeur; i++)
		free(m_schema[i]);
	free(m_schema); 
	free(m_props);
}

void CMap::Draw(SDL_Renderer* pRenderera) const
{
	
	SDL_Rect Rect_dest;
	int numero_tile;
	SDL_Surface* pSurf = nullptr;
	SDL_Texture* pTexture = nullptr;

	for (int i = 0; i < m_nbTilesLargeur; i++)
	{
		for (int j = 0; j < m_nbTilesHauteur; j++)
		{
			//Preparation du rectangle de destination
			Rect_dest.x = i * m_largeurTile + C_pRectArea.x;
			Rect_dest.y = j * m_hauteurTile + C_pRectArea.y;
			Rect_dest.w = m_largeurTile;
			Rect_dest.h = m_hauteurTile;

			numero_tile = m_schema[i][j];
			
			//Creation d'une surface de recuperation
			pSurf = SDL_CreateRGBSurface(m_tileset->flags, m_largeurTile, m_hauteurTile, m_tileset->format->BytesPerPixel*8, 0, 0, 0, 255);

			//Recuperation de la tiles
			SDL_BlitSurface(m_tileset, &(m_props[numero_tile].R), pSurf, NULL);
			//Creation texture et collage sur le Renderer
			pTexture = SDL_CreateTextureFromSurface(C_pRenderer, pSurf);
			SDL_RenderCopy(C_pRenderer, pTexture, NULL, &Rect_dest);
	
			
			//Retour memoire de la surface et de la texture
			SDL_FreeSurface(pSurf);
			SDL_DestroyTexture(pTexture);
		}
	}
}