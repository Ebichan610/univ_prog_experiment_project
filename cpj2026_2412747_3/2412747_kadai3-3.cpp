#include <iostream>
#include <cstdint>
#include <cstdio>
using namespace std;

//uint8内の数字なのかを判定する関数
static bool is_in_uint8(char *str)
{
    //空白はスキップ
    while(isspace(*str))
        str++;
    //+符号はスキップ
    while(*str == '+')
        str++;
    //-符号およびその他は弾く
    if(*str < '0' || *str > '9')
        return(false);
    //uint8内なのかを計算しつつ判定する
    uint16_t result = 0;
    while(*str >= '0' && *str <= '9')
    {
        //uint8の最大値の255を10で割った25より大きかったらその時点で不正
        if(result > UINT8_MAX / 10)
            return(false);
        //uint8の最大値の255を10で割った25と等しいとき
        if(result == UINT8_MAX / 10)
        {
            //次の数字が255の1の位より大きいなら不正
            if(*str >= '6')
                return(false);
        }
        //数字を実際に変換して計算
        result = result * 10 + (*str - '0');
        //uint8の最大値の255を超えたら不正
        if(result > UINT8_MAX)
            return(false);
        str++;
    }
    //余分な文字が入っていたら不正
    if(*str != '\0')
        return(false);
    return(true);
}

static bool check_little_endian(int num)
{
    uint8_t *check_num;
    *check_num = num;
    
    return(true);
}

int main(int argc, char* argv[])
{
    //引数エラー処理
    if(argc < 2)
    {
        cerr << "引数を入力してください\n";
        return(1);
    }
    //引数に入力されたものがuint8に収まっているものなのかを判定
    for(int i = 1; i < argc; i++)
    {
        if(is_in_uint8(argv[i]) == false)
        {
            cerr << "有効な数字を入力してください\n";
            return(1);
        }
    }
    //little endianかどうかを順番に検索
    int num;
    for(int j = 1; j < argc; j++)
    {
        num = atoi(argv[j]);
        if(check_little_endian(num) == true)
        {

        }
    }
    return(0);
}