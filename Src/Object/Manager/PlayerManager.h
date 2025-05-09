#pragma once
#include<vector>

class Player;

class PlayerManager
{
public:

	//コンストラクタ
	PlayerManager(void);
	//デストラクタ
	~PlayerManager(void);

	//初期化
	void Init(void);
	//更新
	void Update(void);
	//描画
	void Draw(void);
	//解放
	void Release(void);

private:

	//プレイヤーのインスタンス用
	std::vector<Player*> player_;

};

