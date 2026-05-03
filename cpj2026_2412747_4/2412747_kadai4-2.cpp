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
        //出力関数
        virtual void PrintMem();
    //継承するのでprotected
    protected:
        //プロジェクターの有無
        virtual void PrintProjectorInfo();
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
        void PrintMem();
    //親クラスと揃える
    protected:
        //プロジェクターの有無だけでなく台数も
        void PrintProjectorInfo();
    private:
        int pc_num;
        int projector_num;
        bool have_mic;
        bool have_wireless_lan;
};

//教室クラスのコンストラクタの定義
ClassRoom::ClassRoom()
    :exam_cap(EXAM),
    seat_cap(SEAT),
    seat_cap_covid19(SEAT_COVID),
    have_projector(true),
    have_screen(true)
{};

//プロジェクターの有無を出力する関数
void ClassRoom::PrintProjectorInfo()
{
    if(have_projector == true)
        cout << "あり、";
    else
        cout << "なく、";
}

//PC教室クラスについて出力するメンバ関数
void ClassRoom::PrintMem()
{
    cout << "この教室には、\n"
        << "座席が" << seat_cap << "席あり、試験時には" << exam_cap << "席だけ使用できます。"
        << "また、コロナ禍での座席定員は" << seat_cap_covid19 << "席としていました。\n"
        << "この教室にはプロジェクターが";
        PrintProjectorInfo();
        cout << "スクリーンは";
        if(have_screen == true)
            cout << "あります。\n";
        else
            cout << "ありません。\n";
}

//PC教室クラスのコンストラクタの定義
PCRoom::PCRoom()
    :ClassRoom(),
    pc_num(PC),
    projector_num(PROJECTOR),
    have_mic(true),
    have_wireless_lan(true)
{
    if(have_projector == false)
        projector_num = 0;
}

//プロジェクターの有無だけでなく台数も出力する関数
void PCRoom::PrintProjectorInfo()
{
    if(have_projector == true)
        cout << projector_num << "台あり、";
    else
        cout << "なく、";
}

//PC教室の情報を出力する関数
void PCRoom::PrintMem()
{
    ClassRoom::PrintMem();

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
    ClassRoom room1;
    PCRoom room2;

    room1.PrintMem();
    room2.PrintMem();
    return(0);
}
