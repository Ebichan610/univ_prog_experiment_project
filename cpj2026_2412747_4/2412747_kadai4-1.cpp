#include <iostream>
using namespace std;

typedef struct s_quad
{
    double a;
    double b;
    double c;
}   t_quad;

static bool check_arg(char* arg)
{

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
    for(int i = 0; i < argc; i++)
    {
        if(check_arg(argv[i]) == false)
        {
            cerr << i + 1 <<"番目の引数の値が不正です。数値を入力してください\n";
            return(1);
        }
    }

    //引数の値を構造体の各メンバに代入
}
