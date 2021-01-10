#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "Module.h"
#include "Animation.h"
#include "SDL/include/SDL.h"

#include "Point.h"
#include "GuiButton.h"
#include "GuiSlider.h"
#include "GuiCheckBox.h"

#include "Entity.h"

struct Collider;

class Player : public Module
{

public:
	enum PlayerStates
	{
		NORMAL,
		SETTINGS,
		OPTIONS,
		EXIT
	};

	Player();

	// Destructor
	virtual ~Player();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool StopMovementY(Collider* c1, Collider* c2) override;
	bool StopMovement(Collider* c1, Collider* c2) override;

	bool Fall(Collider* c1, Collider* c2) override;

	bool Die(Collider* c1, Collider* c2) override;
	bool CameraScroll(Collider* c1, Collider* c2) override;
	bool CheckPoint(Collider* c1, Collider* c2) override;
	bool CollectApple(Collider* c1, Collider* c2) override;
	bool CollectPineapple(Collider* c1, Collider* c2) override;

	bool LoadPlayerCamera();
	bool LoadState(pugi::xml_node&);
	bool SaveState(pugi::xml_node&) const;
	bool OnGuiMouseClickEvent(GuiControl* control);

	// Called before quitting
	bool CleanUp();
	SDL_Texture* currentTex = nullptr;

	SDL_Texture* idleTex = nullptr;
	SDL_Texture* doubleJumpTex = nullptr;
	SDL_Texture* fallTex = nullptr;
	SDL_Texture* runTex = nullptr;
	SDL_Texture* jumpTex = nullptr;
	SDL_Texture* deathTex = nullptr;
	SDL_Texture* lifesTex = nullptr;
	SDL_Texture* appeTex = nullptr;
	SDL_Texture* desAppeTex = nullptr;
	SDL_Texture* appleTex = nullptr;
	SDL_Texture* dronTex = nullptr;
	SDL_Texture* chickenFlyTex = nullptr;

	SDL_Texture* boxTex;
	SDL_Texture* resumeButtTex;
	SDL_Texture* optionButtTex;
	SDL_Texture* returnButtTex;
	SDL_Texture* exitButtTex;

	PlayerStates playerState;

	GuiButton* btnResume;
	GuiButton* btnSettings;
	GuiButton* btnBack;
	GuiButton* btnBack2;
	GuiButton* btnExit;
	GuiCheckBox* playerCheckFullScreen;
	GuiSlider* playerSliderMusic;
	GuiSlider* playerSliderFx;

	bool flip;
	
	int skillCoolDown = 0;

	bool skillRest = 0;

	int lifes = 3;

	int score;
	int highScore = 10000;

	bool checkpointLvl1 = false;
	bool checkpointLvl2 = false;
	bool dead = false;

	unsigned int hitFx;
	unsigned int abilityFx;
	unsigned int jumpFx;
	unsigned int pointFx;
	unsigned int leafFx;
	

	Animation* currentAnimation = nullptr;

	Animation idleAnim;
	Animation fallAnim;
	Animation runAnim;
	Animation jumpAnim;
	Animation deathAnim;
	Animation doubleJumpAnim;
	Animation appeAnim;
	Animation desAppeAnim;
	Animation dronAnim;
	Animation chickenAnim;
	Animation appleGUIAnim;

	iPoint position;

	float speedX;
	float speedY;
	float speedXLastFrame;

	Collider* playerColl = nullptr;
	Collider* cameraColl = nullptr;
	iPoint auxc;
	iPoint auxp;

	bool stopMovementX;
	bool stopMovementY;

	bool god = false;
	bool death = false;
	bool appear = false;
	bool appleGUICollected = false;

	int appleCounter = 0;
	int whiteFont = -1;
	int blackFont = -1;
	char scoreText[10] = { "\0" };
	char secondsText[10] = { "\0" };
	char minutesText[10] = { "\0" };
	char msText[10] = { "\0" };
	int secondsLvl;
	int minutesLvl;
	int msLvl;
	int startTime;

	bool yDownCollision;
	bool yUpCollision;
	bool xLeftCollision;
	bool xRightCollision;

	bool isJumping;
	bool secondJump;

	int lvl = 1;

	unsigned int currentLvl;
	unsigned int menuHoverFx;
	unsigned int menuPressedFx;
	bool saveGame = false;
	bool playerSave = false;
	bool playerLoadF6 = false;
	bool changeLevel1 = false;
	bool changeLevel2 = false;
	bool nothingSaved = true;
	bool started = false;
	bool setScore = false;
	bool newHighScore = false;
	bool startLevel = false;
	bool newGame = false;

	bool cont = false;
};

#endif // __PLAYER_H__