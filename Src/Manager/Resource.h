#pragma once
#include <string>
#include <vector>

class Resource
{

public:
	
	// リソースタイプ
	enum class TYPE
	{
		NONE,			
		IMG,			//画像
		IMGS,			//分割画像
		MASK,			//マスク画像
		MODEL,			//モデル
		EFFEKSEER,		//エフェクト
		SOUND			//サウンド
	};

	// コンストラクタ
	Resource(void);
	// コンストラクタ
	Resource(TYPE type, const std::string& path);
	// コンストラクタ(IMGS用)
	Resource(TYPE type, const std::string& path, int numX, int numY, int sizeX, int sizeY);

	// デストラクタ
	~Resource(void);

	// 読み込み
	void Load(void);

	// 解放
	void Release(void);

	// 複数画像ハンドルを別配列にコピー
	void CopyHandle(int* imgs);

	// リソースタイプ
	TYPE resType_;

	// リソースの読み込み先
	std::string path_;

	// 画像とモデルのハンドルID
	int handleId_;

	// IMGS::LoadDivGraph用
	int* handleIds_;	//ハンドルの先頭アドレス
	int numX_;			//横分割数
	int numY_;			//縦分割数
	int sizeX_;			//画像一つの横の大きさ
	int sizeY_;			//画像一つの縦の大きさ

	// モデル複製用
	std::vector<int> duplicateModelIds_;

};

