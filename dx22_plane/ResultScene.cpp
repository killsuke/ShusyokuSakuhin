#include "ResultScene.h"
#include "Game.h"

// コンストラクタ
ResultScene::ResultScene()
{
}

// デストラクタ
ResultScene::~ResultScene()
{
}

// 更新
void ResultScene::Update()
{
	
	// エンターキーを押してタイトルへ
	if (Input::GetKeyTrigger(VK_RETURN) || Input::GetButtonTrigger(XINPUT_A))
	{
		//Game::GetInstance()->ChangeScene(TITLE);
	}
}
