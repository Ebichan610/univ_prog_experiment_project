#include <iostream>
using namespace std;

//経路数を計算するプログラム
static int calc_route(int x, int y)
{
    //例外処理
    if(x == 4 && y == 2)
        return(0);
    //終了条件
    if(x == 0 || y == 0)
        return(1);
    //再帰的に計算
    return(calc_route(x - 1, y) + calc_route(x, y - 1));
}

int main(int argc, char *argv[])
{
    //引数の数のエラー処理
    if(argc != 3)
    {
        cerr << "引数の数が2つではありません。\n";
        return(1);
    }
    //引数が数字かどうかの処理
    for(int i = 0; argv[1][i] != '\0'; i++)
    {
        if(argv[1][i] < '0' || argv[1][i] > '9')
        {
            cerr << "引数に0~9の数字以外の文字が入っています。正の整数を入力してください。\n";
            return(1);
        }
    }
    for(int j = 0; argv[2][j] != '\0'; j++)
    {
        if(argv[2][j] < '0' || argv[2][j] > '9')
        {
            cerr << "引数に0~9の数字以外の文字が入っています。正の整数を入力してください。\n";
            return(1);
        }
    }
    //引数の数字を変数に格納
    int x = atoi(argv[1]);
    int y = atoi(argv[2]);
    //入力が(4,2)のときをエラー扱いに
    if(x == 4 && y == 2)
    {
        cerr << "(4,2)は工事中なため使用できません。\n";
        return(1);
    }
    //経路数を計算
    int route = calc_route(x, y);
    cout << "終点座標が(" << x << "," << y <<")のときの経路数は" << route << "です。\n"; 
    return (0);
}
