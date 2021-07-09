//
// SceneFactory.h
//

#include"TitleScene.h"
#include"PlayMethod.h"
#include "MainScene.h"
#include"GameClearScene.h"
#include"GameOverScene.h"

class SceneFactory final {
public:
	static std::unique_ptr<Scene> CreateScene(const NextScene nextScene)
	{
		std::unique_ptr<Scene> scene;
		switch (nextScene) {
		case NextScene::TitleScene: scene = std::make_unique<TitleScene>(); break;
		case NextScene::PlayMethodScene: scene = std::make_unique<PlayMethodScene>(); break;
		case NextScene::MainScene:	scene = std::make_unique<MainScene>();	break;
		case NextScene::GameClearScene: scene = std::make_unique<GameClearScene>(); break;
		case NextScene::GameOverScene: scene = std::make_unique<GameOverScene>(); break;
		}
		return scene;
	}
};