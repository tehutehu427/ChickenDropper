//#pragma once

//クラスの前方宣言
class Camera;
class Fader;
class SceneBase;

class SceneManager
{
public:
	//列挙型
	//-----------------
	//シーン管理用
	enum class SCENE_ID
	{
		NONE
		,TITLE		//タイトル
		,SELECT		//セレクト
		,GAME		//ゲーム
		,RESULT		//リザルト
	};

	//定数
	//-------------------

	//最初のシーン
	static constexpr SCENE_ID FIRST_SCENE = SCENE_ID::TITLE;	

	//メンバー関数
	//-------------------
	
	//初期化処理
	bool Init(void);		
	//3Dに関する初期設定
	void Init3D(void);		

	//更新処理
	void Update(void);		
	//描画処理
	void Draw(void);		
	//解放処理
	bool Release(void);		

	//シーン切り替えを依頼する関数
	void ChangeScene(SCENE_ID nextID, bool isToFade);

	//シングルトン化
	//----------------------------
	
	//外部から静的にインスタンスの生成をする
	static void CreateInstance(void);

	//外部でインスタンスを利用可能とするために
	//インスタンスを返す関数を作る(参照型でも、ポインタ型でもよい)
	static SceneManager& GetInstance(void);

private:
	//メンバー変数
	//------------------------
	//シーンID
	SCENE_ID sceneID_;				//現在のシーンID
	SCENE_ID waitSceneID_;			//次に遷移するID

	//シーン遷移
	bool isSceneChanging_;			//シーン遷移のフラグ(true:遷移中)

	//カメラ
	Camera* camera_;				//カメラのインスタンス用

	//フェード
	Fader* fader_;					//フェードのインスタンス用

	//シーン管理
	SceneBase* scene_;				//シーンのインスタンス管理用

	//静的なインスタンス格納領域
	static SceneManager* instance_;

	//メンバー関数
	//--------------------------
	//シーン切り替える
	void DoChangeScene(void);				
	//フェード実施用関数
	void Fade(void);						
	//指定したシーンの解放用
	void ReleaseScene(SCENE_ID iSceneID);	

	//シングルトン化
	//----------------------------
	//デフォルトコンストラクタ
	SceneManager(void);

	// デストラクタ
	~SceneManager(void);

	//コピーコンストラクタを利用できないようにする
	SceneManager(const SceneManager& ins);

	//インスタンスの破棄
	void Destroy(void);


};