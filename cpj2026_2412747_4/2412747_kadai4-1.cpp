#include <iostream>
#include <cstdio>
#include <cctype>
using namespace std;

typedef struct s_comp_num
{
    double real;
    double img;
}   t_comp_num;

//二次方程式の各定数をもつ構造体
typedef struct s_quad
{
    double a;
    double b;
    double c;
    t_comp_num ans1;
    t_comp_num ans2;
}   t_quad;

//引数チェック用関数
static bool check_arg(char* arg)
{
    while(isspace(*arg))
        arg++;
    while(*arg == '+' || *arg == '-')
        arg++;
    if(*arg < '0' || *arg > '9')
        return(false);
    while(*arg >= '0' && *arg <= '9')
        arg++;
    if(*arg != '\0')
        return(false);
    return(true);
}

//引数の数値をquad構造体のメンバに代入して初期化する関数
static t_quad initialize_quad(char* argv[])
{
    t_quad quad;

    quad.a = atoi(argv[1]);
    quad.b = atoi(argv[2]);
    quad.c = atoi(argv[3]);
    return(quad);
}

static void calc_quad(t_quad& quad)
{
    //計算結果を保存する変数
    t_comp_num res1, res2;
    //複素数解とならない場合
    if(quad.b * quad.b - 4 * quad.a * quad.c >= 0)
    {
        //虚数部は存在せず
        res1.img = 0;
        res2.img = 0;
        //実数部の計算
        res1.real = (-quad.b - sqrt(quad.b * quad.b - 4 * quad.a * quad.c)) / 2.0 * quad.a;
        res2.real = (-quad.b + sqrt(quad.b * quad.b - 4 * quad.a * quad.c)) / 2.0 * quad.a;
    }
    //複素数解となる場合
    else
    {
        //虚数部単体の計算
        res1.img = -sqrt(4 * quad.a * quad.c - quad.b * quad.b) / 2.0 * quad.a;
        res2.img = sqrt(4 * quad.a * quad.c - quad.b * quad.b) / 2.0 * quad.a;
        //実数部単体の計算
        res1.real = -quad.b;
        res2.real = -quad.b;
    }
    //アドレス引数のquadにそれぞれの解を代入
    quad.ans1 = res1;
    quad.ans2 = res2;
}

int main(int argc, char *argv[])
{
    //引数エラー処理
    if(argc != 4)
    {
        cerr << "引数の数が違います。a、b、cの値を入力して実行してください。\n";
        return(1);
    }

    //引数が数値なのかのチェック
    for(int i = 1; i < argc; i++)
    {
        if(check_arg(argv[i]) == false)
        {
            cerr << i + 1 <<"番目の引数の値が不正です。数値を入力してください\n";
            return(1);
        }
    }

    //引数の値を構造体の各メンバに代入
    t_quad quad = initialize_quad(argv);
    //a = 0は不正
    if(quad.a == 0)
    {
        cerr << "aが0であるのは不正です。0以外の数値を入力してください。\n";
        return(1);
    }

    //二次方程式の計算
    calc_quad(quad);

    //出力をそれぞれの場合で分ける
    cout << quad.a << "x\u00B2 + " << quad.b << "x + " << quad.c <<" = 0の解は、\n";
    if(quad.ans1.img == 0)
        cout << "x = " << quad.ans1.real << "と、x = " << quad.ans2.real << "です。\n";
    else
    {
        cout << "x = " << quad.ans1.real << " + " << quad.ans1.img << "iと、"
            << "x = " << quad.ans2.real << " + " << quad.ans2.img << "iです。\n";
    }
    return(0);
}
