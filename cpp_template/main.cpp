﻿
//!
//! 象棋对战，双人对战，先后手，棋手，棋子，棋盘，棋子位置镜像转换
//! 落子，棋子位置矫正，是否可以落子，重置棋盘，
//!
//! 输入点位，矫正点位，判断类型，跟随操作
//! 初始化附带类型，点位附带类型，容器附带点位
//! 行棋：落子在棋盘内，符合棋子规则，棋子无阻碍（吃子：落子有敌人）
//!
//! 》输入指令（固定的命令）
//!     》识别选中棋子与行动（分析棋子点位与行动方向）
//!         》让棋子是识别行动具体（棋子根据棋子方向分析出两个坐标）
//!             》根据坐标分析行动类型（移动或者吃子）
//!                 》验证行动的可行性（根据棋盘与棋子类型验证这坐标可行性）
//!                     》执行动作（回到主流程执行结果）
//! 》获取双点位
//!     》判断是否可行动
//!         》交换行动点
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
#include <functional>

using namespace std;
using namespace  std::placeholders;

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
bool  vrange(const Tvalue &value,const Trange &r1,const Trange &r2)
{ if(value >= r1) { if(value <= r2) { return true; } } return false; }

//== 交换值 ==
template<typename T>
void vswap(T &s1,T &s2)
{ T tm; tm = s1; s1 = s2; s2 = tm; }

//== 操作重载:点位运算 ==
template<class Tpos>
Tpos operator+(const Tpos& pos1, const Tpos& pos2)
{ Tpos pos; pos.x = pos1.x + pos2.x; pos.y = pos1.y + pos2.y; return pos; }

template<class Tpos>
Tpos operator-(const Tpos& pos1, const Tpos& pos2)
{ Tpos pos; pos.x = pos1.x - pos2.x; pos.y = pos1.y - pos2.y; return pos; }

template<class Tpos1,class Tpos2>
bool operator>=(const Tpos1& pos1, const Tpos2& pos2)
{ if(pos1.x >= pos2.x && pos1.y >= pos2.y) return true; return false; }

template<class Tpos1,class Tpos2>
bool operator<=(const Tpos1& pos1, const Tpos2& pos2)
{ if(pos1.x <= pos2.x && pos1.y <= pos2.y) return true; return false; }
//== 操作重载:点位运算 ==


//== 交换值 ==
template<typename Tpos,typename Tboard>
void vswap_board(const Tpos &pos1,const Tpos &pos2,Tboard &board)
{
    auto tm = board[pos1.y][pos1.x];
    board[pos1.y][pos1.x] = board[pos2.y][pos2.x];
    board[pos2.y][pos2.x] = tm;
}


//== 棋盘方位 ==
struct ct_point
{
    int32_t x;
    int32_t y;
};

//== 棋子 ==
struct ct_chess
{
    bool _first;
    string _type;
    string _name;
};

//空棋子
template<class Tboard,class Tdire>
void emply_chess(Tboard &board,Tdire x,Tdire y)
{
    board[y][x]._first = false;
    board[y][x]._type = "【】";
    board[y][x]._name = "N";
}

//移动棋子
template<class Tboard,class Tpos>
void move_chess(Tpos from,Tpos to,Tboard &board)
{
    board[to.y][to.x] = board[from.y][from.x];
    emply_chess(board,from.x,from.y);
}

//== 棋子输出 ==
ostream& operator<<(ostream& out, const ct_point& point)
{ return out<<"["<<point.x<<"."<<point.y<<"] "; }

ostream& operator<<(ostream& out, const ct_chess& chess)
{ return out<<"{"<<chess._type<<"|"<<chess._name<<"["<<chess._first <<"]} "; }



//解析落子点
template<class Tboard,class Tchoose>
class Tparse_f
{
public:
    bool parse_pos(const string &cmd,ct_point &from,bool first)
    {
        //分解输入字符串
        if(cmd.size() < 4) return false;
        string n1 = to_string(cmd[0]);
        int32_t in1 = cmd[1] -'0' -1;
        if(in1 >= (int32_t)_board.size()) return false;

        //获取棋盘列内棋子
        vector<ct_point> vec;
        for(size_t i=0;i<_board.size();i++)
        {
            if(_board[i][in1]._name == n1
                    && _board[i][in1]._first == first)
            { vec.push_back(ct_point{in1,(int32_t)i}); }
        }
        return _choose.choose_vec(from,cmd,vec);
    }
    void set_board(const Tboard &board) { _board = board; }
    void set_choose(const Tchoose &choose) { _choose = choose; }

private:
    Tboard _board;
    Tchoose _choose;
};

//解析目标位置--转发到棋子位置
template<class Targv>
class Tparse_t
{
public:
    bool parse_pos(const string &cmd,const ct_point &from,ct_point &to)
    {
        if(cmd.size() < 1) return false;
        auto it = _map.find(to_string(cmd[0])); //解析出对应棋子
        if(it != _map.end())
        { return (it->second)(cmd,from,to); }
        return false;
    }
    void set_func(const map<string,Targv> &map) { _map = map; };
private:
    map<string,Targv> _map;
};

//解析落子点
class Tchoose
{
public:
    //同列出现并排棋子
    bool choose_vec(ct_point &from,const string &cmd,vector<ct_point> vec)
    {
        if(vec.size() == 2)
        {
            //出现并列情况
            if(cmd.size() < 5) return false;
            if(cmd[4] == 'S')
            { from.x = vec[0].x; from.y = vec[0].y; }
            else if(cmd[4] == 'X')
            { from.x = vec[1].x; from.y = vec[1].y; }
            else return false;
        }
        else if(vec.size() == 1) { from.x = vec[0].x; from.y = vec[0].y; }
        else return false;
        return true;
    }
};


//键盘输入
template<class Tparse_f,class Tparse_t>
class Tkey_input
{
public:
    bool input_pos(ct_point &from,ct_point &to,bool &quit,bool first)
    {
        string str;
        while(true)
        {
            cin>>str;
            if(str == "quit") { quit = true; return false; }    //掀棋盘操作
            bool ok1 = _pares_f.parse_pos(str,from,first);      //获取选择棋子点
            bool ok2 = _pares_t.parse_pos(str,from,to);
            return ok1 && ok2;
        }
    }
    void set_pares(Tparse_f parse_f,Tparse_t parse_t)
    { _pares_f = parse_f; _pares_t = parse_t; }

private:
    Tparse_f _pares_f;
    Tparse_t _pares_t;
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
            else { emply_chess(board,j,i); }
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
            else { emply_chess(board,j,size -i); }
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



////棋子模板
//template<class Tfunc>
//class TSchess
//{
//public:
//    TSchess(ct_point pos,bool first) : _pos(pos),_first(first){}
//    bool action(ct_point to) { return _func.action(_pos,to); }

//private:
//    Tfunc _func;
//    ct_point _pos;
//    bool _first;
//};

template<class Tboard>
bool equal_first(ct_point from,ct_point to,Tboard board)
{ return board[to.x][to.y]._first == board[from.x][from.y]._first; }

//===== 行动规则 =====
//将
template<class Tboard>
class Tjiang
{
public:
    bool action(ct_point from,ct_point to)
    {
        cout<<"action:Tjiang"<<endl;
        bool ok1 = false; //限制1:落子点在宫内
        bool ok2 = false; //限制2:至少且只能移动一格距离
        bool ok3 = false; //限制3:上下或者左右移动
        if(vrange(to,ct_point{4,1},ct_point{6,3})) ok1 = true;
        ct_point dire = to - from;
        if(vrange(vabs(dire.x),0,1) || vrange(vabs(dire.y),0,1)) ok2 = true;
        if(dire.x - dire.y == -1 || dire.x - dire.y == 1) ok3 = true;
        return ok1 && ok2 && ok3;
    }
    bool move(ct_point from,ct_point to)
    { return action(from,to); }
    bool attack(ct_point from,ct_point to)
    {
        if(equal_first(from,to,_board)) return false;
        return action(from,to);
    }

    bool parse_pos(const string &cmd,const ct_point &from,ct_point &to)
    {
        cout<<"pos from: "<<from<<endl;
        if(cmd.size() < 4) return false;
        int32_t in1 = cmd[1] -'0';
        int32_t in2 = cmd[3] -'0';
        string n2 = to_string(cmd[2]);
        to = from;

        if(n2 == "J")
        { to.y += in2; }
        else if(n2 == "T")
        { to.y -= in2; }
        else if(n2 == "P")
        { to.x += (in2 - in1); }
        else return false;

        cout<<to<<endl;
        if(_board[to.x][to.y]._name == "N")
        { cout<<"move"<<endl; return move(from,to); }
        else
        { cout<<"attack"<<endl; return attack(from,to); }
        return false;
    }

    Tjiang(const Tboard &board) : _board(board){}
    Tboard _board;
};

//士
template<class Tboard>
class Tshi
{
public:
    bool action(ct_point from,ct_point to)
    {
        bool ok1 = false; //限制1:落子点在宫内
        bool ok2 = false; //限制2:至少且只能移动一格距离
        bool ok3 = false; //限制3:只能斜着走
        ct_point dire = to - from;
        if(vrange(to,ct_point{4,1},ct_point{6,3})) ok1 = true;
        if(vrange(vabs(dire.x),0,1) || vrange(vabs(dire.y),0,1)) ok2 = true;
        if(vabs(dire.x) == 1 && vabs(dire.y) == 1) ok3 = true;
        return ok1 && ok2 && ok3;
    }
    bool move(ct_point from,ct_point to)
    {
        if(equal_first(from,to,_board)) return false;
        return action(from,to);
    }
    bool attack(ct_point from,ct_point to)
    { return move(from,to); }

    bool parse_pos(const string &cmd,const ct_point &from,ct_point &to)
    {

//        if(cmd != _chess_parse._name) return _next_func->parse_pos(cmd,to);
//        else _chess_parse.parse(cmd,to);
        return false;
    }

    Tshi(const Tboard &board) : _board(board){}
    Tboard _board;
};

//象
template<class Tboard>
class Txiang
{
public:
    bool action(ct_point from,ct_point to)
    {
        bool ok1 = false; //限制1:落子在河内
        bool ok2 = false; //限制2:田字移动
        ct_point dire = to - from;
        if(vrange(to,ct_point{1,1},ct_point{9,5})) ok1 = true;
        if(vabs(dire.x) == 2 && vabs(dire.y) == 2) ok2 = true;
        return ok1 && ok2;
    }
    bool move(ct_point from,ct_point to)
    {
        if(equal_first(from,to,_board)) return false;
        if(action(from,to)) //卡象眼
        {
            ct_point tm;
            tm.x = (to.x - from.x)/2; tm.y = (to.y - from.y)/2;
            if(_board[from.x + to.x][from.y + to.y]._type == "") return true;
        }
        return false;
    }
    bool attack(ct_point from,ct_point to)
    { return move(from,to); }

    Txiang(Tboard board) : _board(board){}
    Tboard _board;
};

//马
template<class Tboard>
class Tma
{
public:
    bool action(ct_point from,ct_point to)
    {
        bool ok1 = false; //限制1:日字移动
        ct_point dire = to - from;
        if((vabs(dire.x) == 2 && vabs(dire.y) == 1)
                || (vabs(dire.x) == 1 && vabs(dire.y) == 2)) ok1 = true;
        return ok1;
    }
    bool move(ct_point from,ct_point to)
    {
        if(equal_first(from,to,_board)) return false;
        if(action(from,to)) //别马脚
        {
            ct_point tm;
            tm.x = (to.x - from.x)/2; tm.y = (to.y - from.y)/2;
            if(_board[from.x + to.x][from.y + to.y]._type == "") return true;
        }
        return false;
    }
    bool attack(ct_point from,ct_point to)
    { return move(from,to); }

    Tma(Tboard board) : _board(board){}
    Tboard _board;
};

//车
class Tche
{
public:
    bool action(ct_point from,ct_point to)
    {
        bool ok1 = false; //限制1:直线移动
        ct_point dire = to - from;
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
        ct_point dire = to - from;
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
        ct_point dire = to - from;
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

//template< class Tinput>
//void start_play(Tinput input)
//{
//    ct_point from;
//    ct_point dire;
//    bool quit;//bool first
//    while(true)
//    {
//        if(input.input_pos(from,dire,quit,false))
//        {
//            if(quit == true)
//            { break; }


//        }
//        else
//        {}
//    }
//}




int main()
{
#ifdef _WIN32
    //Windows控制台: UTF-8编码
    SetConsoleOutputCP(65001);
#endif

    cout << "===== begin =====" << endl;

#if 1

    //== 初始化经典棋盘 ==
    array<array<ct_chess,9>,10> arr_board; //棋盘点位容器
    vector<vector<string>> board_layout{
        {"车","马","象","士","将","士","象","马","车"},
        {"","","","","","","","",""},
        {"","炮","","","","","","炮",""},
        {"兵","","兵","","兵","","兵","","兵"},
        {"","","","","","","","",""}
    };
    typedef array<array<ct_chess,9>,10> arr_b;

//    map<string,string> map_table{
//        {"J","将"}, {"S","士"}, {"X","象"}, {"M","马"}, {"C","车"},
//        {"P","炮"}, {"B","兵"},
//    };
    map<string,string> map_table{
        {"将","J"}, {"士","S"}, {"象","X"}, {"马","M"}, {"车","C"},
        {"炮","P"}, {"兵","B"},
    };

    //初始化经典棋盘布局
    init_board_empty(arr_board);//设置空棋盘
    if(init_board_default(arr_board,board_layout,map_table))
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

    //声明棋子类型
    Tjiang<arr_b> jiang(arr_board);
    Tshi<arr_b> shi(arr_board);

    //声明函数容器
    map<string,function
            <bool(const string &cmd,
                  const ct_point &from,
                  ct_point &to)>> map_func;

    //简化声明（容器声明不使用新类型，提升透明度）
    typedef function
            <bool(const string &cmd,
                  const ct_point &from,
                  ct_point &to)> func_m;

    //插入解析函数
    map_func.insert(pair<string,func_m>
        ("J",bind(&Tjiang<arr_b>::parse_pos,jiang,_1,_2,_3)));
    map_func.insert(pair<string,func_m>
        ("S",bind(&Tshi<arr_b>::parse_pos,shi,_1,_2,_3)));


    cout << "===== 输入位置 =====" << endl;
    //解决并列棋子选择冲突问题
    Tchoose choose;

    //棋子选择点位解析
    Tparse_f<arr_b,Tchoose> parse_f;
    parse_f.set_board(arr_board);
    parse_f.set_choose(choose);

    //棋子目标落点解析
    Tparse_t<func_m> parse_t;
    parse_t.set_func(map_func);

    //键盘输入分析落子
    Tkey_input<Tparse_f<arr_b,Tchoose>,Tparse_t<func_m>> input;
    input.set_pares(parse_f,parse_t); //输入解析方式设置



    while(true)
    {
        ct_point from;
        ct_point to;
        bool quit = false;
        bool first = true;
        if(input.input_pos(from,to,quit,first))
        {
            cout<<from<<" | "<<to<<endl;
            move_chess(from,to,arr_board);
//            vswap_board(from,to,arr_board);
            cout<<from<<" | "<<to<<endl;


            cout<<"== 矫正棋盘显示 =="<<endl;
            show_board_all(arr_board);
            cout<<endl;


            cout << "===== 玩家视角 =====" << endl;
            show_board_type(arr_board);
            cout<<endl;
            parse_f.set_board(arr_board);

        }
        else cout<<"not input_pos"<<endl;
    }


//    cout<<input.input_pos(from,dire,quit,first)<<endl;

//    template<class Tnext>
//    class Tparse_t




//    Tparse_t<> mao;


//    Tparse_t<Tnext<Tjiang<array<array<ct_chess,9>,10>>>> ji(net1);

//    start_play<key_input<array<array<ct_chess,9>,10>>>(in);




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
