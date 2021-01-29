
#ifndef CSCENE_H_
#define CSCENE_H_

#include "CMap/CMap.h"

#define NB_BLOCK_X		11
#define NB_BLOCK_Y		10


class CScene {
private:

private:
	static SDL_Renderer	* C_pRenderer;
private:
	CMap* m_pMap;

public:
	CScene();
	~CScene();

	void Draw();

public:
	static void Init(SDL_Renderer* pRenderer, const SDL_Rect*pAreaGame) {
		C_pRenderer = pRenderer;
		CMap::Init(pRenderer, pAreaGame);
	}
};

#endif // !CSCENE_H_