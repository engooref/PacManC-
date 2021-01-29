#ifndef CAPP_H_
#define CAPP_H_


#include "CScene/CScene.h"
//#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_mixer.h>


class CApp
{
private:
	Uint32				m_uStatus;
	Uint32				m_uWindowID;

	SDL_Window		*	m_pWindow;
	SDL_Renderer	*	m_pRenderer;

	SDL_TimerID			m_nTimerID;

	CScene			*	m_pScene;

public:
	CApp();
	~CApp();

private:
	void Run();
	bool IsInitiated() const;

	static Uint32 AnimateCallBack(Uint32 interval, void* pParam);

};

#endif // !CAPP_H_