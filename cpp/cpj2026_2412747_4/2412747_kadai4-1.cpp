#include <iostream>
#include <cmath>
#include <cstdio>
#include <cctype>
using namespace std;

//複素数の実部と虚部をもつ構造体
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
    // 解と係数の関係の利用
    //和
    double sum = -quad.b / quad.a;
    //積
    double pro =  quad.c / quad.a;
    //判別式
    double disc = quad.b * quad.b - 4.0 * quad.a * quad.c;

    //実数解ならば
    if (disc >= 0)
    {

        double x1 = (-quad.b - sqrt(disc)) / (2.0 * quad.a);
        double x2;
        //0除算回避
        if (x1 != 0.0)
            x2 = pro / x1;
        else
            x2 = sum - x1;
        //それぞれに格納
        quad.ans1.real = x1;
        quad.ans1.img  = 0.0;
        quad.ans2.real = x2;
        quad.ans2.img  = 0.0;
    }
    //虚数部もあるならば
    else
    {
        double real = sum / 2.0;
        double img = sqrt(-disc) / (2.0 * quad.a);
        //求めたものを代入
        quad.ans2.real = real;
        quad.ans2.img  = img;
        //解と係数の関係から計算
        double denom = real * real + img * img;
        quad.ans1.real =  pro * real / denom;
        quad.ans1.img  = -pro * img / denom;
    }
}

//「+」や「-」の出力を工夫して出すための関数
//「-」の場合のみ出力したい場合を考えて第2引数を追加
static string plus_or_minus(double num, bool minus_only)
{
    if(num > 0 && minus_only == false)
        return("+");
    else if(num < 0)
        return("-");
    else
        return("");
}

//方程式とその解を出力する関数
static void print_quad(t_quad quad)
{
    //-符号
    if(quad.a < 0)
        cout << plus_or_minus(quad.a, true);
    //先頭に1はつけない
    if(abs(quad.a) != 1)
        cout << abs(quad.a);
    //2乗の出力
    cout << "x\u00B2 ";
    //b = 0の場合は出力しない
    if(quad.b != 0)
        cout << plus_or_minus(quad.b, false) << " " << abs(quad.b)<< "x ";
    //c = 0の場合は出力しない
    if(quad.c != 0)
        cout << plus_or_minus(quad.c, false) << " " << abs(quad.c);
    cout <<" = 0の解は、\n";
    //実数の場合
    if(quad.ans1.img == 0)
    {
        //重解
        if(quad.ans1.real == quad.ans2.real)
            cout <<"x = " << plus_or_minus(quad.ans1.real, true)
                << abs(quad.ans1.real) << " (重解)です。\n";
        //重解でない
        else
            cout << "x = " << plus_or_minus(quad.ans1.real, true) << abs(quad.ans1.real)
                << "と、x = " << plus_or_minus(quad.ans2.real, true) << abs(quad.ans2.real)
                << "です。\n";
    }
    //虚数部がある場合
    else
    {
        //重解
        if(quad.ans1.real == quad.ans2.real)
        {
            cout << "x = ";
            //実部が0なら出力しない
            if(quad.ans1.real != 0)
                cout << plus_or_minus(quad.ans1.real,true) << abs(quad.ans1.real) << " ";
            cout << plus_or_minus(quad.ans1.img,false);
            //実部が0のときは虚部が先頭に来るので負符号と数値の間に空白はいらない
            if(quad.ans1.real != 0)
                cout << " ";
            //虚部に1はつけない
            if(abs(quad.ans1.img) != 1)
                cout << abs(quad.ans1.img);
            cout << "i (重解)です。\n";

        }
        //重解でない
        else
        {
            cout << "x = ";
            //実部が0なら出力しない
            if(quad.ans1.real != 0)
                cout << plus_or_minus(quad.ans1.real,true) << abs(quad.ans1.real) << " ";
            cout << plus_or_minus(quad.ans1.img,false);
            //実部が0のときは虚部が先頭に来るので負符号と数値の間に空白はいらない
            if(quad.ans1.real != 0)
                cout << " ";
            //虚部に1はつけない
            if(abs(quad.ans1.img) != 1)
                cout << abs(quad.ans1.img);
            cout << "iと、"<< "x = ";
            //実部が0なら出力しない
            if(quad.ans2.real != 0)
                cout << plus_or_minus(quad.ans2.real,true) << abs(quad.ans2.real) << " ";
            cout << plus_or_minus(quad.ans2.img, false);
            //実部が0のときは虚部が先頭に来るので負符号と数値の間に空白はいらない
            if(quad.ans2.real != 0)
                cout << " ";
            //虚部に1はつけない
            if(abs(quad.ans2.img) != 1)
                cout << abs(quad.ans2.img);
            cout << "iです。\n";
        }
    }
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
    print_quad(quad);
    return(0);
}
