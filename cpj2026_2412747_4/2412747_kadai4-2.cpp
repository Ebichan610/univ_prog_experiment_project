#include <iostream>
using namespace std;

//クラスで使用する数値を定義
const int EXAM = 100;
const int SEAT = 150;
const int SEAT_COVID = 50;
const int PC = 150;
const int PROJECTOR = 4;

//教室クラスの宣言
class ClassRoom
{
    public:
        ClassRoom();
        void PrintMem();
    protected:
        int exam_cap;
        int seat_cap;
        int seat_cap_covid19;
        bool have_projector;
        bool have_screen;
};

//PC教室クラスの宣言(教室クラスを継承)
class PCRoom : public ClassRoom
{
    public:
        PCRoom();
        void PrintPCMem();
    private:
        int pc_num;
        int projector_num;
        bool have_mic;
        bool have_wireless_lan;
};

//教室クラスのコンストラクタの定義
ClassRoom::ClassRoom()
{
    exam_cap = EXAM;
    seat_cap = SEAT;
    seat_cap_covid19 = SEAT_COVID;
    have_projector = true;
    have_screen = true;
}

//PC教室クラスについて出力するメンバ関数
void ClassRoom::PrintMem()
{
    cout << "教室には、\n"
        << "座席が" << seat_cap << "席あり、試験時には" << exam_cap << "席だけ使用できます。"
        << "また、コロナ禍での座席定員は" << seat_cap_covid19 << "席としていました。\n"
        << "この教室にはプロジェクターが";
        if(have_projector == true)
            cout << "あり、";
        else
            cout << "なく、";
        cout << "スクリーンは";
        if(have_screen == true)
            cout << "あります。\n";
        else
            cout << "ありません。\n";
}

//PC教室クラスのコンストラクタの定義
PCRoom::PCRoom()
{
    pc_num = PC;
    if(have_projector == true)
        projector_num = PROJECTOR;
    have_mic = true;
    have_wireless_lan = true;
}

void PCRoom::PrintPCMem()
{
    cout << "PC教室には、\n"
        << "座席が" << seat_cap << "席あり、試験時には" << exam_cap << "席だけ使用できます。"
        << "また、コロナ禍での座席定員は" << seat_cap_covid19 << "席としていました。\n"
        << "この教室にはプロジェクターが";
        if(have_projector == true)
            cout << projector_num << "台あり、";
        else
            cout << "なく、";
        cout << "スクリーンは";
        if(have_screen == true)
            cout << "あります。\n";
        else
            cout << "ありません。\n";
        cout << "PCの台数は" << pc_num << "台で、"
            << "マイクは";
        if(have_mic == true)
            cout << "あります。\n";
        else
            cout << "ありません。\n";
        cout << "また、無線LANが整備されて";
        if(have_wireless_lan == true)
            cout << "います。\n";
        else
            cout << "いません。\n";
}

int main()
{

}
