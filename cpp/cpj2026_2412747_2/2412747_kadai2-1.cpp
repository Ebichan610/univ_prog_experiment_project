#include <iostream>
#include <cstdio>
using namespace std;

//最大公約数を求める関数
static int calc_gcd(int m, int n)
{
    //もし片方が0であるならば、もう片方を返す
    if(n == 0)
        return(m);
    int r = m % n;
    while(r != 0)
    {
        m = n;
        n = r;
        r = m % n;
    }
    return(n);
}

int main(int argc, char *argv[])
{
    //引数の数のエラー処理
    if(argc != 3)
    {
        cerr << "引数の数が2つではありません。入力し直してください。\n";
        return(1);
    }
    //引数が数字でない
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
    //引数の数字を格納
    int x = atoi(argv[1]), y = atoi(argv[2]);
    int m, n;
    //数の大小によってmとnに代入する数字を変える
    if(x >= y)
    {
        m = x;
        n = y;
    }
    else
    {
        m = y;
        n = x;
    }
    //両方0は弾く
    if(m == 0 && n == 0)
    {
        cerr << "両方とも0の場合、不定となるので計算できません。\n";
        return(1);
    }
    //最大公約数を求める処理
    int gcd = calc_gcd(m, n);
    cout << argv[1] << "と" << argv[2] << "の最大公約数は" << gcd << "です。\n";
    return(0);
}
