
//!
//! 象棋对战，双人对战，先后手，棋手，棋子，棋盘，棋子位置镜像转换
//! 落子，棋子位置矫正，是否可以落子，重置棋盘，
//!
//! 输入点位，矫正点位，判断类型，跟随操作
//! 初始化附带类型，点位附带类型，容器附带点位
//! 行棋：落子在棋盘内，符合棋子规则，棋子无阻碍（吃子：落子有敌人）
//!
//! 操作：
//! 将 士 象 马 车 炮 兵   进 退 平   上 下
//! J  S  X  M  C  P  B   J  T  P   S  X
//!
//!
#include <iostream>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <array>
#include <ctime>
#include <cstdlib>
#include <vector>
#include <map>
using namespace std;

//== 字符串类型转换 ==
template<typename T>
string to_string(const T& t){ ostringstream os; os<<t; return os.str(); }

template<typename T>
T from_string(const string& str){ T t; istringstream iss(str); iss>>t; return t; }
//== 字符串类型转换 ==

//== 绝对值 ==
template<typename T>
T  vabs(const T& value)
{ if(value < 0) return -value; else return value; }

//== 范围 ==
template<typename Tvalue,typename Trange>
bool  vrange(const Tvalue& value,const Trange& r1,const Trange& r2)
{ if(value >= r1) { if(value <= r2) { return true; } } return false; }

//== 棋盘点位 ==
struct ct_point
{
    uint32_t x;
    uint32_t y;
};

//== 方向位 ==
struct ct_dire
{
    int16_t x;
    int16_t y;
};

//== 棋子 ==
struct ct_chess
{
    bool _first;
    string _type;
    string _name;
};

//===== 操作重载 =====
//== 显示属性 ==
ostream& operator<<(ostream& out, const ct_chess& chess)
{ return out<<"{"<<chess._type<<"|"<<chess._name<<"["<<chess._first <<"]} "; }

//== 点位运算 ==
template<class Tpos>
ct_dire operator-(const Tpos& pos1, const Tpos& pos2)
{ ct_dire pos; pos.x = pos1.x - pos2.x; pos.y = pos1.y - pos2.y; return pos; }

template<class Tpos1,class Tpos2>
ct_dire operator-(const Tpos1& pos1, const Tpos2& pos2)
{ ct_dire pos; pos.x = pos1.x - pos2.x; pos.y = pos1.y - pos2.y; return pos; }

template<class Tpos1,class Tpos2>
bool operator>=(const Tpos1& pos1, const Tpos2& pos2)
{ if(pos1.x >= pos2.x && pos1.y >= pos2.y) return true; return false; }

template<class Tpos1,class Tpos2>
bool operator<=(const Tpos1& pos1, const Tpos2& pos2)
{ if(pos1.x <= pos2.x && pos1.y <= pos2.y) return true; return false; }
//===== 操作重载 =====


template<class Tboard>
class key_input
{
public:
    bool input_pos(ct_point &from,ct_dire &dire,bool &quit,bool first)
    {
        string str;
        while(true)
        {
            cin>>str;
            if(str == "quit") { quit = true; return false; } //掀棋盘操作
            return parse_pos(str,from,dire,first);
        }
    }
    void set_board(const Tboard &board) { _board = board; }

private:
    Tboard _board;

    //解析落子点
    bool parse_pos(const string &str,ct_point &from,ct_dire &dire,bool first)
    {
        //分解输入字符串
        if(str.size() < 4) return false;
        string n1 = to_string(str[0]);
        string n2 = to_string(str[2]);
        uint32_t in1 = str[1] -'0' -1;
        uint32_t in2 = str[3] -'0';

        //获取棋盘列内棋子
        vector<ct_point> vec;
        for(size_t i=0;i<_board.size();i++)
        {
            cout<<first<<_board[i][in1]._first<<"|"<<i<<in1<<endl;
            if(_board[i][in1]._name == n1
                    && _board[i][in1]._first == first)
            { vec.push_back(ct_point{(uint32_t)i,in1}); }
        }

        //分析操作
        if(n2 == "J")
        { dire.x = 1; dire.y = in2; }
        else if(n2 == "T")
        { dire.x = -1; dire.y = in2; }
        else if(n2 == "P")
        { dire.x = 0; dire.y = in2; }
        else return false;

        //同列出现并排棋子
        if(str.size() == 5 && vec.size() == 2)
        {
            if(str[0] == 'S')
            { from.x = vec[0].x; from.y = vec[0].y; }
            else if(str[0] == 'X')
            { from.x = vec[1].x; from.y = vec[1].y; }
            else return false;
        }
        else if(vec.size() == 1) { from.x = vec[0].x; from.y = vec[0].y; }

        cout<<str<<"|"<<n1<<n2<<in1<<in2<<endl;
        cout<<from.x<<"|"<<from.y<<endl;
        cout<<dire.x<<"|"<<dire.y<<endl;
        return true;
    }
};


//===== 初始化 =====
//初始化空棋盘
template<class Tboard>
void init_board_empty(Tboard &board)
{
    for(size_t i=0;i<board.size();i++)
    {
        for(size_t j=0;j<board[i].size();j++)
        {
            board[i][j]._type = "【】";
            board[i][j]._first = false;
        }
    }
}

//初始化顺序
template<class Tboard>
void init_board_rank(Tboard &board)
{
    uint32_t count = 0;
    for(size_t i=0;i<board.size();i++)
    {
        for(size_t j=0;j<board[i].size();j++)
        {
            count++;
            if(count < 10) board[i][j]._type = "0"+to_string(count);
            else board[i][j]._type = to_string(count);
            board[i][j]._first = false;
        }
    }
}

//默认棋盘初始化
template<class Tboard,class Ttemplate,class Ttable>
bool init_board_default(Tboard &board,const Ttemplate &tmp,const Ttable &table)
{
    //生成本方棋局
    if(board.size()/2 != tmp.size()) return false; //不符合镜像大小
    for(size_t i=0;i<tmp.size();i++)
    {
        if(board[i].size() != tmp[i].size()) return false; //不符合棋子量
        for(size_t j=0;j<tmp[i].size();j++)
        {
            if(tmp[i][j] != "")
            {
                board[i][j]._type = "["+tmp[i][j]+"]";
                board[i][j]._first = true;
                auto it = table.find(tmp[i][j]);
                if(it != table.end()) board[i][j]._name = it->second;
            }
            else { board[i][j]._type = "【】"; board[i][j]._name = "N"; }
        }
    }

    //生成镜像棋局
    uint32_t size = board.size()-1;
    for(size_t i=0;i<tmp.size();i++)
    {
        for(size_t j=0;j<tmp[i].size();j++)
        {
            if(tmp[i][j] != "")
            {
                board[size -i][j]._type = "("+ tmp[i][j] +")";
                board[size -i][j]._first = false;
                auto it = table.find(tmp[i][j]);
                if(it != table.end()) board[size -i][j]._name = it->second;
            }
            else { board[size -i][j]._type = "【】"; board[size -i][j]._name = "N"; }
        }
    }
    return true;
}
//===== 初始化 =====



//===== 显示 =====
//显示棋盘（默认顺序）
template<class Tboard>
void show_board(const Tboard &board)
{
    for(size_t i=0;i<board.size();i++)
    {
        for(size_t j=0;j<board[i].size();j++)
        { cout<<board[i][j]._type<<" "; }
        cout<<endl;
    }

    for(size_t i=0;i<board.size();i++)
    {
        for(size_t j=0;j<board[i].size();j++)
        { cout<<board[i][j]<<" "; }
        cout<<endl;
    }
}

//显示棋盘（玩家顺序）
template<class Tboard>
void show_board_type(const Tboard &board)
{
    for(size_t i=board.size()-1;i >= 0;i--)
    {
        for(size_t j=0;j<board[i].size();j++)
        { cout<<board[i][j]._type<<" "; }
        cout<<endl;
        if(i == 0) break; //无符号循坏退出，防止倒数
    }
}

//显示棋盘属性（玩家顺序）
template<class Tboard>
void show_board_all(const Tboard &board)
{
    for(size_t i=board.size()-1;i >= 0;i--)
    {
        for(size_t j=0;j<board[i].size();j++)
        { cout<<board[i][j]<<" "; }
        cout<<endl;
        if(i == 0) break; //无符号循坏退出，防止倒数
    }
}
//===== 显示 =====



//棋子模板
template<class Tfunc>
class TSchess
{
public:
    TSchess(ct_point pos,bool first) : _pos(pos),_first(first){}
    bool action(ct_point to) { return _func.action(_pos,to); }

private:
    Tfunc _func;
    ct_point _pos;
    bool _first;
};

//===== 行动规则 =====
//将
class Tjiang
{
public:
    /*
    5
    4     -------
    3     |     |
    2     |     |
    1 2 3 |4 5 6| 7 8 9
          '''''''
    */
    bool action(ct_point from,ct_point to)
    {
        bool ok1 = false; //限制1:落子点在宫内
        bool ok2 = false; //限制2:至少且只能移动一格距离
        bool ok3 = false; //限制3:上下或者左右移动
        if(vrange(to,ct_point{4,1},ct_point{6,3})) ok1 = true;
        ct_dire dire = to - from;
        if(vrange(vabs(dire.x),0,1) || vrange(vabs(dire.y),0,1)) ok2 = true;
        if(dire.x - dire.y == -1 || dire.x - dire.y == 1) ok3 = true;
        return ok1 && ok2 && ok3;
    }
    string type_name(bool first)
    { if(first) return "[将]"; else return "(将)"; }
};

//士
class Tshi
{
public:
    /*
    5
    4     -------
    3     |     |
    2     |     |
    1 2 3 |4 5 6| 7 8 9
          '''''''
    */
    bool action(ct_point from,ct_point to)
    {
        bool ok1 = false; //限制1:落子点在宫内
        bool ok2 = false; //限制2:至少且只能移动一格距离
        bool ok3 = false; //限制3:只能斜着走
        ct_dire dire = to - from;
        if(vrange(to,ct_point{4,1},ct_point{6,3})) ok1 = true;
        if(vrange(vabs(dire.x),0,1) || vrange(vabs(dire.y),0,1)) ok2 = true;
        if(vabs(dire.x) == 1 && vabs(dire.y) == 1) ok3 = true;
        return ok1 && ok2 && ok3;
    }
    string type_name(bool first)
    { if(first) return "[士]"; else return "(士)"; }
};

//象
class Txiang
{
public:
    /*
    |------------------
    |5                |
    |4                |
    |3                |
    |2                |
    |1 2 3 4 5 6 7 8 9|
     '''''''''''''''''
    */
    bool action(ct_point from,ct_point to)
    {
        bool ok1 = false; //限制1:落子在河内
        bool ok2 = false; //限制2:田字移动
        ct_dire dire = to - from;
        if(vrange(to,ct_point{1,1},ct_point{9,5})) ok1 = true;
        if(vabs(dire.x) == 2 && vabs(dire.y) == 2) ok2 = true;
        return ok1 && ok2;
    }
    string type_name(bool first)
    { if(first) return "[象]"; else return "(象)"; }
};

//马
class Tma
{
public:
    bool action(ct_point from,ct_point to)
    {
        bool ok1 = false; //限制1:日字移动
        ct_dire dire = to - from;
        if((vabs(dire.x) == 2 && vabs(dire.y) == 1)
                || (vabs(dire.x) == 1 && vabs(dire.y) == 2)) ok1 = true;
        return ok1;
    }
    string type_name(bool first)
    { if(first) return "[马]"; else return "(马)"; }
};

//车
class Tche
{
public:
    bool action(ct_point from,ct_point to)
    {
        bool ok1 = false; //限制1:直线移动
        ct_dire dire = to - from;
        if((dire.x == 0 && dire.y != 0)
                || (dire.y == 0 && dire.x != 0)) ok1 = true;
        return ok1;
    }
    string type_name(bool first)
    { if(first) return "[车]"; else return "(车)"; }
};

//炮
class Tpao
{
public:
    bool action(ct_point from,ct_point to)
    {
        bool ok1 = false; //限制1:直线移动
        ct_dire dire = to - from;
        if((dire.x == 0 && dire.y != 0)
                || (dire.y == 0 && dire.x != 0)) ok1 = true;
        return ok1;
    }
    string type_name(bool first)
    { if(first) return "[炮]"; else return "(炮)"; }
};

//兵
class Tbing
{
public:
    /*
    |------------------
   5|^   ^   ^   ^   ^|河内
   4|                 |    8|         ^^
   3|                 |    7|       < 兵 >
   2|                 |    6| ----------------- |河外
    |1 2 3 4 5 6 7 8 9|    5| 1 2 3 4 5 6 7 8 9
     '''''''''''''''''
    */
    bool action(ct_point from,ct_point to)
    {
        bool ok1 = false; //限制1:不许后退
        bool ok2 = false; //限制2:只能移动一格
        bool ok3 = false; //限制2:河内只能向前,河外可左右
        ct_dire dire = to - from;
        if(dire.y >= 0) ok1 = true;
        if(vabs(dire.x) + dire.y == 1) ok2 = true;
        if(from.y <= 5) { if(dire.y > 0) ok3 = true; }
        else ok3 = true;
        return ok1 && ok2 && ok3;
    }
    string type_name(bool first)
    { if(first) return "[兵]"; else return "(兵)"; }
};
//===== 行动规则 =====

//== windows中文显示 ==
#ifdef _WIN32
#include <windows.h>
#endif

int main()
{
#ifdef _WIN32
    //Windows控制台: UTF-8编码
    SetConsoleOutputCP(65001);
#endif

    cout << "===== begin =====" << endl;

#if 1
    //== 初始化经典棋盘 ==
    array<array<ct_chess,9>,10> arr_board;
    vector<vector<string>> board_layout{
        {"车","马","象","士","将","士","象","马","车"},
        {"","","","","","","","",""},
        {"","炮","","","","","","炮",""},
        {"兵","","兵","","兵","","兵","","兵"},
        {"","","","","","","","",""}
    };

    map<string,string> map_table{
        {"J","将"}, {"S","士"}, {"X","象"}, {"M","马"}, {"C","车"},
        {"P","炮"}, {"B","兵"},
    };
    map<string,string> map_table_re{
        {"将","J"}, {"士","S"}, {"象","X"}, {"马","M"}, {"车","C"},
        {"炮","P"}, {"兵","B"},
    };

    init_board_empty(arr_board);//设置空棋盘
    if(init_board_default(arr_board,board_layout,map_table_re))
    {
        cout << "===== 调试视角 =====" << endl;
        cout<<"== 默认棋盘显示 =="<<endl;
        show_board(arr_board);
        cout<<endl;

        cout<<"== 矫正棋盘显示 =="<<endl;
        show_board_all(arr_board);
        cout<<endl;

        cout << "===== 玩家视角 =====" << endl;
        show_board_type(arr_board);
    }
    //== 初始化经典棋盘 ==
#endif


//    string str = "马2平3"; JSXMCPB PJT SX


//    cin>>str;

    cout << "===== 输入位置 =====" << endl;
    ct_point from;
    ct_dire dire;
    bool quit;
    key_input<array<array<ct_chess,9>,10>> in;
    in.set_board(arr_board);
    bool ij = true;
    cout<<in.input_pos(from,dire,quit,ij)<<endl;




    //== 测试代码填充区 ==
    //...
    //== 测试代码填充区 ==



    /*
    |------------------
    |5                |
    |4                |
    |3                |
    |2                |
    |1 2 3 4 5 6 7 8 9|
     '''''''''''''''''
    */





    //== 测试代码 ==

    cout << "===== end =====" << endl;
    return 0;
}

/*
//== 测试代码 ==
//    //范围测试
//    Tjiang jiang;
//    cout<<jiang.action(ct_point{5,2},ct_point{6,2})<<endl;
//    cout<<jiang.action(ct_point{5,2},ct_point{4,2})<<endl;
//    cout<<jiang.action(ct_point{5,2},ct_point{5,1})<<endl;
//    cout<<jiang.action(ct_point{5,2},ct_point{5,3})<<endl;
//    //越界、斜线、超距测试
//    cout<<jiang.action(ct_point{5,2},ct_point{6,3})<<endl;
//    cout<<jiang.action(ct_point{5,2},ct_point{4,1})<<endl;
//    cout<<jiang.action(ct_point{6,3},ct_point{7,3})<<endl;
//    cout<<jiang.action(ct_point{4,1},ct_point{3,1})<<endl;
//    cout<<jiang.action(ct_point{6,3},ct_point{6,4})<<endl;
//    cout<<jiang.action(ct_point{4,1},ct_point{4,0})<<endl;
//    cout<<jiang.action(ct_point{5,2},ct_point{5,4})<<endl;
//    cout<<jiang.action(ct_point{5,2},ct_point{7,2})<<endl;

//    //斜线测试
//    Tshi shi;
//    cout<<shi.action(ct_point{5,2},ct_point{6,3})<<endl;
//    cout<<shi.action(ct_point{5,2},ct_point{4,1})<<endl;
//    //直线测试
//    cout<<shi.action(ct_point{5,2},ct_point{5,3})<<endl;
//    cout<<shi.action(ct_point{5,2},ct_point{5,1})<<endl;
//    cout<<shi.action(ct_point{5,2},ct_point{6,2})<<endl;
//    cout<<shi.action(ct_point{5,2},ct_point{4,2})<<endl;

//    //田字测试
//    Txiang xiang;
//    cout<<xiang.action(ct_point{3,1},ct_point{1,3})<<endl;
//    cout<<xiang.action(ct_point{3,1},ct_point{5,3})<<endl;
//    cout<<xiang.action(ct_point{5,3},ct_point{7,5})<<endl;
//    cout<<xiang.action(ct_point{5,3},ct_point{3,5})<<endl;
//    //越界、超距、直线测试
//    cout<<xiang.action(ct_point{3,5},ct_point{5,7})<<endl;
//    cout<<xiang.action(ct_point{3,5},ct_point{5,4})<<endl;
//    cout<<xiang.action(ct_point{3,5},ct_point{5,5})<<endl;

//    //日字测试
//    Tma ma;
//    cout<<ma.action(ct_point{2,1},ct_point{3,3})<<endl;
//    cout<<ma.action(ct_point{2,1},ct_point{1,3})<<endl;
//    //短距、超距、直线测试
//    cout<<ma.action(ct_point{2,1},ct_point{2,2})<<endl;
//    cout<<ma.action(ct_point{2,1},ct_point{3,2})<<endl;
//    cout<<ma.action(ct_point{2,1},ct_point{4,4})<<endl;
//    cout<<ma.action(ct_point{2,1},ct_point{5,2})<<endl;
//    cout<<ma.action(ct_point{2,1},ct_point{2,3})<<endl;

//    //直线测试
//    Tche che;
//    cout<<che.action(ct_point{1,1},ct_point{9,1})<<endl;
//    cout<<che.action(ct_point{5,1},ct_point{5,5})<<endl;
//    //斜线测试
//    cout<<che.action(ct_point{5,1},ct_point{9,5})<<endl;
//    cout<<che.action(ct_point{5,1},ct_point{1,5})<<endl;

//    //直线测试
//    Tpao pao;
//    cout<<pao.action(ct_point{1,1},ct_point{9,1})<<endl;
//    cout<<pao.action(ct_point{5,1},ct_point{5,5})<<endl;
//    //斜线测试
//    cout<<pao.action(ct_point{5,1},ct_point{9,5})<<endl;
//    cout<<pao.action(ct_point{5,1},ct_point{1,5})<<endl;

//    //河内外测试
//    Tbing bing;
//    cout<<bing.action(ct_point{5,5},ct_point{5,6})<<endl;
//    cout<<bing.action(ct_point{5,6},ct_point{6,6})<<endl;
//    cout<<bing.action(ct_point{5,6},ct_point{4,6})<<endl;
//    cout<<bing.action(ct_point{5,6},ct_point{5,7})<<endl;

//    //斜线、超距、后退测试
//    cout<<bing.action(ct_point{5,5},ct_point{5,7})<<endl;
//    cout<<bing.action(ct_point{5,5},ct_point{6,6})<<endl;
//    cout<<bing.action(ct_point{5,5},ct_point{5,4})<<endl;
//    cout<<bing.action(ct_point{5,7},ct_point{5,6})<<endl;
//== 测试代码 ==
*/
