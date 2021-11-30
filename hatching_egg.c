// ライブラリを読み込むためのコード
#include <NintendoSwitchControlLibrary.h>

#include <stdio.h>
#include <math.h>

#define PI 3.141592

#define MIN  0
#define MAX  255

#define LEN  128
#define INIT_X  128
#define INIT_Y  128

double floor(double);


// ＜初期状態＞
// 場所: ズイタウン内なら、基本的にどこでも可能（ただし、自転車を乗り回して外に出てしまうとアウト）
// 状態: 自転車に乗ってBボタン
// Xボタンで開くモード: 左上（図鑑）にカーソル
// ボックス: 格納したい空のボックスを開く（カーソルはどこでも良い。閉じたら自動的に左上になるため）


void setup() {
   // Switchがマイコンを認識するまでは信号を受け付けないため、適当な処理をさせておく
   //これを行わないと、プログラムの実行が反映されるタイミングが接続するごとに変わってしまう
   pushButton(Button::Y, 500, 5);
}

int clamp(int value, int min, int max)
{
    if (value < min) {
        return min;
    }
    else if (value > max) {
        return max;
    }
    return value;
}

void runAround(int num_loop) {
   for (int i = 0; i < num_loop; i++) {
     for (int deg = 0; deg < 360; deg+=8) {
  
      double rad = deg * PI / 180.0;
      double x = INIT_X + LEN * cos( rad );
      double y = INIT_Y + LEN * sin( rad );
      x= (int)x;
      y= (int)y;

      double clip_x = clamp(x, MIN, MAX);
      double clip_y = clamp(y, MIN, MAX);
      
      SwitchControlLibrary().moveLeftStick(clip_x, clip_y);
      SwitchControlLibrary().sendReport();
      delay(0.01);
     }
     
    //1周ごとにAボタンを押して孵化画面に対応
    SwitchControlLibrary().pressButton(Button::A);
    SwitchControlLibrary().sendReport();            
    delay(10);
    SwitchControlLibrary().releaseButton(Button::A);
    SwitchControlLibrary().sendReport();    
    delay(10);
   }
  //スティックを初期位置（どの方向にも傾いていない状態）に戻す
  SwitchControlLibrary().moveLeftStick(Stick::NEUTRAL, Stick::NEUTRAL);
  SwitchControlLibrary().sendReport();
  delay(10);
}


void getEgg(int num_egg) {


  //5つの卵を受け取る
  for (int i = 0; i < num_egg; i++) {
    
    //走り回る（だいたい1000歩くらい）
    runAround(200);
    delay(3000);
    
    //空を飛ぶでポケセンの前に移動
    pushButton(Button::X, 1000,1); 
    pushHat(Hat::DOWN, 100, 1); 
    pushButton(Button::A, 1000,1); 
    pushButton(Button::A, 1000,1); 
    pushButton(Button::A, 1000,1); 
    delay(7000);
  
    //育て屋のお爺さんの前に立って、話しかける
    pushHat(Hat::LEFT, 300, 5); 
    pushHat(Hat::UP, 300, 10); 
    pushHat(Hat::LEFT, 300, 9); 
    pushButton(Button::A, 1000,10); //1.5秒に1回Aを押す。これを10回行う
  
    //話しかけている場合もあるため、Bを押して終わらせる
    pushButton(Button::B, 300,10); 

    //図鑑にカーソルを合わせる（TODO: 後で最初だけ図鑑にカーソル移動するように変更）
    pushButton(Button::X, 1000,1); 
    pushHat(Hat::UP, 100, 1); 
    pushButton(Button::B, 1000,1); 
  

    //移動して自転車に乗る
    pushHat(Hat::RIGHT, 300, 5); 
    pushHat(Hat::UP, 300, 5); 
    pushButton(Button::PLUS, 500); 
    pushHat(Hat::UP, 500, 1); 
  }
}


void keepBox(int box_number, int line_in_box) {
  //box_number番目のボックスのline_in_box列目に格納する関数

  //box画面を開く
  pushButton(Button::X, 500,1); 
  pushHat(Hat::RIGHT, 500, 1); 
  pushButton(Button::A, 1500,1); //読み込み時間を考慮して長めに
  pushButton(Button::R, 1500,1); //読み込み時間を考慮して長めに

  //任意のボックスへ移動する
  if(box_number){
    pushButton(Button::R, 500,box_number); 
  }

  //範囲選択モードにし、範囲選択をする位置にカーソルを合わせる
  pushButton(Button::Y, 500,2);
  pushHat(Hat::LEFT, 500, 1); 
  pushHat(Hat::DOWN, 500, 1); 

  //範囲選択
  pushButton(Button::A, 500,1);
  pushHat(Hat::DOWN, 500, 4);
  pushButton(Button::A, 500,1);

  ///ボックスへ預ける
  pushHat(Hat::RIGHT, 500, line_in_box+1); 
  pushHat(Hat::UP, 500, 1); 
  pushButton(Button::A, 500,1);

  //画面を閉じる
  pushButton(Button::B, 500,5);

  //図鑑にカーソルを合わせる
  pushButton(Button::X, 500,1); 
  pushHat(Hat::LEFT, 500, 1); 
  pushButton(Button::B, 500,1); 

}

void loop() {

  int num_run_loop = 1200;      //孵化のために円周上を走り回る回数
  int num_hattching_loop = 5;   //卵の受け取り→孵化→ボックスへの預けまでのセット回数
  int num_egg=5;


  for (int i = 0; i < num_hattching_loop; i++) {
    
    //卵を受け取る
    getEgg(num_egg);
    
    //卵の孵化のために走り回る
    runAround(num_run_loop);
  
    // TODO: 孵化したポケモンをボックスに預ける
    int increment_box_number = floor(i/6);
    int line_in_box = i%6;
    keepBox(increment_box_number,line_in_box);
  } 
}