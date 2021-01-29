#include "CScene/CScene.h"
#include "app.h"

SDL_Renderer* CScene::C_pRenderer{ nullptr };

CScene::CScene() :
	m_pMap(new CMap("Ressource/Sprites/level.txt"))
{
}

CScene::~CScene()
{
	if (m_pMap) delete m_pMap; m_pMap = nullptr;
}

void CScene::Draw()
{
	m_pMap->Draw(C_pRenderer);
}
