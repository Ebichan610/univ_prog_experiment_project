#include <iostream>
#include <fstream>
#include <cerrno>
#include <string>
#include <vector>
#include <dirent.h>
#include <sys/stat.h>
using namespace std;

const unsigned char SIGNATURE[] = {
    0x31, 0xC9, 0xE8, 0x8C, 0x32, 0x00, 0x00, 0x45, 0x31, 0xC0, 0x31
};
const int SIG_LEN = sizeof(SIGNATURE) / sizeof(SIGNATURE[0]);

static vector<int> build_failure_table()
{
    vector<int> table(SIG_LEN, 0);
    int j = 0;
    for(int i = 1; i < SIG_LEN; i++)
    {
        while(j > 0 && SIGNATURE[i] != SIGNATURE[j])
            j = table[j - 1];
        if(SIGNATURE[i] == SIGNATURE[j])
            j++;
        table[i] = j;
    }
    return(table);
}

static int malware_search(vector<unsigned char> &data, vector<int> &table)
{
    int j = 0;
    for(int i = 0; i < (int)data.size(); i++)
    {
        while(j > 0 && data[i] != SIGNATURE[j])
            j = table[j - 1];
        if(data[i] == SIGNATURE[j])
            j++;
        if(j == SIG_LEN)
            return(i - SIG_LEN + 1);
    }
    return(-1);
}

static bool read_binary(string &path, vector<unsigned char> &buf)
{
    ifstream fin(path, ios::binary);
    if(!fin)
        return(false);
    buf.assign(istreambuf_iterator<char>(fin), istreambuf_iterator<char>());
    return(true);
}

static bool is_regular_file(string &path)
{
    struct stat st;
    if(stat(path.c_str(), &st) != 0)
        return(false);
    return(S_ISREG(st.st_mode));
}

static bool natural_num_comp(string &a, string &b)
{
    int i = 0, j = 0;

    while(i < (int)a.size() && j < (int)b.size())
    {
        if(isdigit(a[i]) && isdigit(b[j]))
        {
            int num_a = 0, num_b = 0;
            while(i < (int)a.size() && isdigit(a[i]))
            {
                num_a = num_a * 10 + ((a[i]) - '0');
                i++;
            }
            while(j < (int)b.size() && isdigit(b[j]))
            {
                num_b = num_b * 10 + ((b[j]) - '0');
                j++;
            }
            if(num_a != num_b)
                return(num_a < num_b);
        }
        else
        {
            if(a[i] != b[j])
                return(a[i] < b[j]);
            i++;
            j++;
        }
    }
    return(a.size() < b.size());
}

int main(int argc, char *argv[])
{
    //引数の数のエラー処理
    if(argc != 2)
    {
        cerr << "引数が2つではありません。\n";
        return(1);
    }
    //ディレクトリの読み込みとエラー処理
    string dir_path = argv[1];
    DIR *dir = opendir(dir_path.c_str());
    if(dir == NULL)
    {
        perror("ディレクトリが開けませんでした。");
        return(errno);
    }
    //
    vector<int> table = build_failure_table();
    //
    vector<string> filenames;
    struct dirent *entry;
    while((entry = readdir(dir)) != nullptr)
    {
        string name = entry->d_name;
        if(name == "." || name == "..")
            continue;
        string full_path = dir_path + "/" + name;
        if(is_regular_file(full_path) == false)
            continue;
        filenames.push_back(name);
    }
    //ディレクトリを閉じる
    closedir(dir);
    //ファイルを名前順にソート
    sort(filenames.begin(), filenames.end(), natural_num_comp);
    //マルウェア検出の変数
    bool is_find = false;
    for(string &name : filenames)
    {
        string full_path = dir_path + "/" + name;
        
        vector<unsigned char> buf;
        if(read_binary(full_path, buf) == false)
        {
            cerr << "読み込みが失敗しました\n";
            continue;
        }

        int offset = malware_search(buf, table);
        if(offset != -1)
        {
            cout << "[検出]:" << name
                 << "  オフセット: 0x"
                 << hex << offset << dec << "\n";
            is_find = true;
        }

    }
    //
    if(is_find == true)
        cout << "以上のファイルよりマルウェアAファミリーが検出されました。\n";
    else
        cout << "マルウェアAファミリーは検出されませんでした。\n";
    return(0);

}