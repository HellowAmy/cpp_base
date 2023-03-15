//!
//! C++基础：模板化编程-附带象棋代码实例
//!
//!
//! ===== 基本概念 =====
//! 模板化编程基本概念:
//!     1、数据模板化 : 泛化数据类型，使用同一操作对不同数据结构进行处理
//!     2、操作模板化 : 泛化操作流程，使回调接口控制流程并忽视具体调用的实现细节
//!
//!     数据类型的模板化在STL容器上得到广泛的运用，并取得很好的效果，一种类型的容器只需一份实现，
//!         便能得到所有类型的存放。而操作模板化在STL上的使用并不算太多，
//!         如容器算法内的伪函数回调或传入的Lambda表达式控制操作流程。
//!     模板是C++的经典编程方式之一，在C++的多泛式编程下，面向对象与模板都各占半壁江山，
//!         模板由于可以在编译期完成绑定，所以可以将很多运行时确定的操作转移到编译期，
//!         从而用编译时间换取运行时的速度，是追求高性能手段之一，
//!         所以模板化编程在网络服务器、游戏引擎等要求高性能的框架中频繁出现。
//!     模板化编程由于数据与操作的高度抽象与不确定，编程难度相比于面向对象略高一些，
//!         使用难度的提高使得模板化编程常常被初学者的忽视，本篇文章将简单展示模板化的实例代码，
//!         帮助初学者快速入门模板化编程的世界。
//! ===== 基本概念 =====
//!
//!
//!
//! ===== 任务介绍 =====
//! 使用模板化编程的方式写一个象棋游戏:
//!     场景描述:
//!         一般的象棋都为两个玩家对战，分先后手进行下棋，棋盘上棋子位置固定，
//!             下棋时棋子由一个点移动到另一个点位吃子，或者移动位置。
//!     游戏过程:
//!         摆棋盘、先手下棋、拿去棋子、吃子获取移动、吃掉将帅或者对方认输则胜利
//! ===== 任务介绍 =====
//!
//!
//!
//! ===== 具体规划 =====
//! 游戏过程
//! ==================================
//! 》行棋 <--------                 |
//!     》落子     ^                |
//!         》移动 ^               |
//!         》吃子 ^ 吃将 -> 结束  |
//! ==============================
//!
//! 简单实现
//! =========================
//! 》获取双点位            |
//!     》判断是否可行动   |
//!         》交换行动点  |
//! =====================
//!
//! 实现过程
//! ===========================================================================
//! 》输入指令（固定的命令）                                                    |
//!     》识别选中棋子与行动（分析棋子点位与行动方向）                           |
//!         》让棋子是识别行动具体（棋子根据棋子方向分析出两个坐标）             |
//!             》根据坐标分析行动类型（移动或者吃子）                        |
//!                 》验证行动的可行性（根据棋盘与棋子类型验证这坐标可行性）   |
//!                     》执行动作（回到主流程执行结果）                    |
//! =====================================================================
//!
//! 可用指令
//! ==========================================
//! 将 士 象 马 车 炮 兵   进 退 平   上 下   /
//! J  S  X  M  C  P  B   J  T  P   S  X  /
//! ======================================
//! ===== 具体规划 =====
//!
//!
//! 结束语:
//!     不得不承认在具体的业务实现上，使用面向对象可以降低很多难度，
//!         模板始终在具体实现上缺少简单可用的设计模型。
//!     本次实现为了达到棋子的动态增加采用了面向对象里设计模式的职责链模式，
//!         不过用模板重新进行了实现，与经典的职责链有所出入。
//!     模板化职责链的具体代码解析将在另一篇文章中说明，如果感兴趣可去观看。
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
#include <tuple>

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
//== 棋子输出 ==


//===== 棋盘点位解析 =====
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
            _cmd = str;
            if(str == "quit") { quit = true; return false; } //掀棋盘操作
            if(_pares_f.parse_pos(str,from,first)
                    && _pares_t.parse_pos(str,from,to)) return true;
            else return false;
        }
    }
    string get_cmd() { return _cmd; }

    void set_pares(Tparse_f parse_f,Tparse_t parse_t)
    { _pares_f = parse_f; _pares_t = parse_t; }

private:
    Tparse_f _pares_f;
    Tparse_t _pares_t;
    string _cmd;
};

//解析落子点
template<class Tboard,class Tchoose>
class Tparse_f
{
public:
    bool parse_pos(const string &cmd,ct_point &from,bool first)
    {
        //分解输入字符串
        if(cmd.size() < 4) { cout<<"指令长度太短"<<endl; return false; }
        string n1 = to_string(cmd[0]);
        int32_t in1 = cmd[1] -'0' -1;

        //获取棋盘列内棋子
        vector<ct_point> vec;
        for(size_t i=0;i<_board->size();i++)
        {
            if((*_board)[i][in1]._name == n1
                    && (*_board)[i][in1]._first == first)
            { vec.push_back(ct_point{in1,(int32_t)i}); }
        }
        return _choose.choose_vec(from,cmd,vec);
    }
    void set_board(Tboard &board) { _board = &board; }
    void set_choose(const Tchoose &choose) { _choose = choose; }

private:
    Tboard *_board;
    Tchoose _choose;
};

//解析并排棋子
class Tchoose
{
public:
    //同列出现并排棋子
    bool choose_vec(ct_point &from,const string &cmd,vector<ct_point> vec)
    {
        if(vec.size() == 2)
        {
            //出现并列情况
            if(cmd.size() < 5) { cout<<"未指定并排棋子"<<endl; return false; }
            if(cmd[4] == 'X')
            { from.x = vec[0].x; from.y = vec[0].y; }
            else if(cmd[4] == 'S')
            { from.x = vec[1].x; from.y = vec[1].y; }
            else { cout<<"命令不符[S or X]"<<endl; return false; }
        }
        else if(vec.size() == 1) { from.x = vec[0].x; from.y = vec[0].y; }
        else { cout<<"不存在选中棋子"<<endl; return false; }
        return true;
    }
};

//== 职责链模板 ==
//任务调用模板
template<class Ttuple,size_t size>
class Twork
{
public:
    static bool action(Ttuple tuple,const string &cmd,const ct_point &from,ct_point &to)
    {
        auto it = std::get<size -1>(tuple);
        if(it->is_ok(cmd)) { return it->parse_pos(cmd,from,to); }//return true;
        return Twork<Ttuple,size -1>::action(tuple,cmd,from,to);
    }
};

//任务终止模板
template<class Ttuple>
class Twork<Ttuple,0>
{
public:
    static bool action(Ttuple,const string &,const ct_point &,ct_point &) { return false; }
};

//启动任务模板
template<class ...Tarr>
class Tparse_t
{
public:
    typedef Twork<tuple<Tarr...>,tuple_size<tuple<Tarr...>>::value> def_work;
    void add_work(Tarr ...arg)
    { _tuple = make_tuple(arg...); }

    bool parse_pos(const string &cmd,const ct_point &from,ct_point &to)
    { return def_work::action(_tuple,cmd,from,to); }

private:
    tuple<Tarr...> _tuple;
};
//== 职责链模板 ==
//===== 棋盘点位解析 =====



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



//===== 行动规则 =====
//== 判断先手阵营相等 ==
template<class Tboard>
bool equal_first(ct_point from,ct_point to,const Tboard &board)
{ return board[to.y][to.x]._first == board[from.y][from.x]._first; }

//== 判断棋子是否存在于棋盘 ==
template<class Tboard>
bool is_exist_chess(ct_point to,const Tboard &board)
{ return board[to.y][to.x]._name != "N"; }

//== 判断棋子是否存在于棋盘 ==
template<class Tboard>
ct_chess get_chess(ct_point pos,const Tboard &board)
{ return board[pos.y][pos.x]; }

//== 查看两点之间存在多少个棋子 ==
template<class Tboard>
size_t count_line_chess(ct_point from,ct_point to,const Tboard &board)
{
    size_t count = 0; int32_t dx = 0; int32_t dy = 0;
    if((to.x - from.x) > 0) dx = 1; if((to.x - from.x) < 0) dx = -1;
    if((to.y - from.y) > 0) dy = 1; if((to.y - from.y) < 0) dy = -1;

    size_t size = vabs((to.x - from.x) - (to.y - from.y));
    for(size_t i=0;i<size -1;i++)
    {
        if(board[from.y + dy][from.x + dx]._name != "N") { count++; }
        if(dy > 0) dy += 1; if(dy < 0) dy -= 1;
        if(dx > 0) dx += 1; if(dx < 0) dx -= 1;
    }
    return count;
}

//将
template<class Tboard>
class Tjiang
{
public:
    //行动函数,每个棋子有自己的行动限制
    bool action(ct_point from,ct_point to)
    {
        bool ok1 = false; //限制1:落子点在宫内
        bool ok2 = false; //限制2:至少且只能移动一格距离
        bool ok3 = false; //限制3:上下或者左右移动
        if(vrange(to,ct_point{4,1},ct_point{6,3})) ok1 = true;
        ct_point dire = to - from;
        if(vrange(vabs(dire.x),0,1) || vrange(vabs(dire.y),0,1)) ok2 = true;
        if(dire.x - dire.y == -1 || dire.x - dire.y == 1) ok3 = true;
        return ok1 && ok2 && ok3;
    }

    //障碍函数,棋子遇到障碍则无法行动
    //  返回成功则表示有障碍不执行，返回失败为无障碍，继续下一步处理
    bool barrier(ct_point from,ct_point to)
    {
        //阵营相同则退出
        if(is_exist_chess(to,*_board)) //判断棋子存在
            if(equal_first(from,to,*_board)) //判断阵营相同
                { cout<<"阵营相同"<<endl; return true; }
        return false;
    }

    //解析函数调用：
    //  Tkey_input类调用Tparse_t类的parse_pos,
    //  Tparse_t调用每个独立棋子注册的parse_pos函数,最终调用到这个棋子函数
    bool parse_pos(const string &cmd,const ct_point &from,ct_point &to)
    {
        //获取指令
        if(cmd.size() < 4) return false;
        int32_t in1 = cmd[1] -'0';
        int32_t in2 = cmd[3] -'0';
        string n2 = to_string(cmd[2]);
        to = from;

        //指令解析
        if(n2 == "J")
        { to.y += in2; }
        else if(n2 == "T")
        { to.y -= in2; }
        else if(n2 == "P")
        { to.x += (in2 - in1); }
        else { cout<<"不存在指令:将[J.T.P]"<<endl; return false; }

        //遇到障碍等导致无法行动（相同阵营）
        if(barrier(from,to)) return false;

        //矫正并行动:将容器下标矫正为棋盘点位并行动
        ct_point in_from = from + ct_point{1,1};
        ct_point in_to = to + ct_point{1,1};
        return action(in_from,in_to);
    }

    //确定职责函数：
    //  该函数用于职责链判断时，与任务标记对比，判断本次任务是否由自己处理
    //  如果返回失败则不处理，返回成功则表示自己处理当前任务
    bool is_ok(const string &cmd)
    { if(to_string(cmd[0]) == _name) return true; else return false; }

    //构造函数，用于设置棋盘和当前类的任务标记
    Tjiang(const Tboard &board,const string &name)
        : _board(&board),_name(name){}
    const Tboard *_board;
    string _name;
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

    bool barrier(ct_point from,ct_point to)
    {
        if(is_exist_chess(to,*_board))
            if(equal_first(from,to,*_board))
                { cout<<"阵营相同"<<endl; return true; }
        return false;
    }

    bool parse_pos(const string &cmd,const ct_point &from,ct_point &to)
    {
        if(cmd.size() < 4) return false;
        int32_t in1 = cmd[1] -'0';
        int32_t in2 = cmd[3] -'0';
        string n2 = to_string(cmd[2]);
        to = from;

        if(n2 == "J" || n2 == "Y") //扬士
        { to.y += 1; to.x += (in2 - in1); }
        else if(n2 == "T" || n2 == "L") //落士
        { to.y -= 1; to.x -= (in1 - in2);  }
        else { cout<<"不存在指令:士[J.T.Y.L]"<<endl; return false; }

        if(barrier(from,to)) return false;
        ct_point in_from = from + ct_point{1,1};
        ct_point in_to = to + ct_point{1,1};
        return action(in_from,in_to);
    }

    bool is_ok(const string &cmd)
    { if(to_string(cmd[0]) == _name) return true; else return false; }

    Tshi(const Tboard &board,const string &name)
        : _board(&board),_name(name){}
    const Tboard *_board;
    string _name;
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

    bool barrier(ct_point from,ct_point to)
    {
        if(is_exist_chess(to,*_board))
            if(equal_first(from,to,*_board))
                { cout<<"阵营相同"<<endl; return true; }
        //卡象眼（两点求中点得象眼）
        ct_point tm;
        tm.x = (to.x + from.x) /2; tm.y = (to.y + from.y) /2;
        if((*_board)[tm.y][tm.x]._name != "N")
        { cout<<"卡象眼"<<endl; return true; }
        return false;
    }

    bool parse_pos(const string &cmd,const ct_point &from,ct_point &to)
    {
        if(cmd.size() < 4) return false;
        int32_t in1 = cmd[1] -'0';
        int32_t in2 = cmd[3] -'0';
        string n2 = to_string(cmd[2]);
        to = from;

        if(n2 == "F" || n2 == "J") //飞象
        { to.y += 2; to.x += (in2 - in1); }
        else if(n2 == "L" || n2 == "T") //落象
        { to.y -= 2; to.x -= (in1 - in2);  }
        else { cout<<"不存在指令:象[J.T.F.L]"<<endl; return false; }

        if(barrier(from,to)) return false;
        ct_point in_from = from + ct_point{1,1};
        ct_point in_to = to + ct_point{1,1};
        return action(in_from,in_to);
    }

    bool is_ok(const string &cmd)
    { if(to_string(cmd[0]) == _name) return true; else return false; }

    Txiang(const Tboard &board,const string &name)
        : _board(&board),_name(name){}
    const Tboard *_board;
    string _name;
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

    bool barrier(ct_point from,ct_point to)
    {
        if(is_exist_chess(to,*_board))
            if(equal_first(from,to,*_board))
                { cout<<"阵营相同"<<endl; return true; }
        //憋马脚（一步偏移点不变，两步偏移求中点得马脚）
        ct_point tm;
        if(vabs(from.x - to.x) == 1) tm.x = from.x;
        if(vabs(from.y - to.y) == 1) tm.y = from.y;
        if(vabs(from.x - to.x) == 2) tm.x = (from.x + to.x)/2;
        if(vabs(from.y - to.y) == 2) tm.y = (from.y + to.y)/2;
        if((*_board)[tm.y][tm.x]._name != "N")
        { cout<<"憋马脚"<<endl; return true; }
        return false;
    }

    bool parse_pos(const string &cmd,const ct_point &from,ct_point &to)
    {
        if(cmd.size() < 4) return false;
        int32_t in1 = cmd[1] -'0';
        int32_t in2 = cmd[3] -'0';
        string n2 = to_string(cmd[2]);
        to = from;

        int32_t sub = in1 - in2;
        int32_t abs_s = vabs(sub);
        if(n2 == "J")
        { to.x -= sub; if(abs_s == 1){to.y += 2;} if(abs_s == 2 ){to.y += 1;} }
        else if(n2 == "T")
        { to.x -= sub; if(abs_s == 1){to.y -= 2;} if(abs_s == 2 ){to.y -= 1;} }
        else { cout<<"不存在指令:马[J.T]"<<endl; return false; }

        if(barrier(from,to)) return false;
        ct_point in_from = from + ct_point{1,1};
        ct_point in_to = to + ct_point{1,1};
        return action(in_from,in_to);
    }

    bool is_ok(const string &cmd)
    { if(to_string(cmd[0]) == _name) return true; else return false; }

    Tma(const Tboard &board,const string &name)
        : _board(&board),_name(name){}
    const Tboard *_board;
    string _name;
};

//车
template<class Tboard>
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

    bool barrier(ct_point from,ct_point to)
    {
        if(is_exist_chess(to,*_board))
            if(equal_first(from,to,*_board))
                { cout<<"阵营相同"<<endl; return true; }

        //拦路虎（中途遇到阻碍则为拦路）
        if(count_line_chess(from,to,*_board) > 0)
        { cout<<"拦路虎"<<endl; return true; }
        return false;
    }

    bool parse_pos(const string &cmd,const ct_point &from,ct_point &to)
    {
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
        else { cout<<"不存在指令:车[J.T.P]"<<endl; return false; }
        cout<<from<<to<<endl;

        if(barrier(from,to)) return false;
        ct_point in_from = from + ct_point{1,1};
        ct_point in_to = to + ct_point{1,1};
        return action(in_from,in_to);
    }

    bool is_ok(const string &cmd)
    { if(to_string(cmd[0]) == _name) return true; else return false; }

    Tche(const Tboard &board,const string &name)
        : _board(&board),_name(name){}
    const Tboard *_board;
    string _name;
};

//炮
template<class Tboard>
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

    bool barrier(ct_point from,ct_point to)
    {
        if(is_exist_chess(to,*_board))
            if(equal_first(from,to,*_board))
                { cout<<"阵营相同"<<endl; return true; }

        //落点有棋子--跨棋吃子
        if(is_exist_chess(to,*_board))
        {
            if(count_line_chess(from,to,*_board) != 1)
            { cout<<"无跨棋或多跨棋"<<endl; return true; }
        }
        else //落点无棋子--移动
        {
            //拦路虎（中途遇到阻碍则为拦路）
            if(count_line_chess(from,to,*_board) > 0)
            { cout<<"拦路虎"<<endl; return true; }
        }
        return false;
    }

    bool parse_pos(const string &cmd,const ct_point &from,ct_point &to)
    {
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
        else { cout<<"不存在指令:炮[J.T.P]"<<endl; return false; }
        cout<<from<<to<<endl;

        if(barrier(from,to)) return false;
        ct_point in_from = from + ct_point{1,1};
        ct_point in_to = to + ct_point{1,1};
        return action(in_from,in_to);
    }

    bool is_ok(const string &cmd)
    { if(to_string(cmd[0]) == _name) return true; else return false; }

    Tpao(const Tboard &board,const string &name)
        : _board(&board),_name(name){}
    const Tboard *_board;
    string _name;
};

//兵
template<class Tboard>
class Tbing
{
public:
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

    bool barrier(ct_point from,ct_point to)
    {
        if(is_exist_chess(to,*_board))
            if(equal_first(from,to,*_board))
                { cout<<"阵营相同"<<endl; return true; }
        return false;
    }

    bool parse_pos(const string &cmd,const ct_point &from,ct_point &to)
    {
        if(cmd.size() < 4) return false;
        int32_t in1 = cmd[1] -'0';
        int32_t in2 = cmd[3] -'0';
        string n2 = to_string(cmd[2]);
        to = from;

        if(n2 == "J")
        { to.y += in2; }
        else if(n2 == "P")
        { to.x += (in2 - in1); }
        else { cout<<"不存在指令:兵[J.P]"<<endl; return false; }
        cout<<from<<to<<endl;

        if(barrier(from,to)) return false;
        ct_point in_from = from + ct_point{1,1};
        ct_point in_to = to + ct_point{1,1};
        return action(in_from,in_to);
    }

    bool is_ok(const string &cmd)
    { if(to_string(cmd[0]) == _name) return true; else return false; }

    Tbing(const Tboard &board,const string &name)
        : _board(&board),_name(name){}
    const Tboard *_board;
    string _name;
};
//===== 行动规则 =====

//== 复制棋盘 ==
template<class Tboard>
Tboard copy_borad(const Tboard &board)
{
    Tboard tm;
    for(size_t y=0;y<board.size();y++)
        for(size_t x=0;x<board[x].size();x++)
        { tm[y][x] = board[y][x]; }
    return tm;
}

//== 镜像棋盘 ==
template<class Tboard>
void rotate_borad(Tboard &board)
{
    Tboard tm = copy_borad(board);
    for(size_t y=0;y<board.size();y++)
        for(size_t x=0;x<board[y].size();x++)
        { board[y][x] = tm[board.size() -y -1][board[y].size() -x -1]; }
}

//!
//!
//! 0 1 2
//!
//!
//! 9 8 7
//!
//!
//!
//0 1 2 3
//1 1 2 3
//2 2 2 3
//3 3 3 3


//== windows中文显示 ==
#ifdef _WIN32
#include <windows.h>
#endif

int main()
{

    //== Windows控制台: UTF-8编码 ==
#ifdef _WIN32
    SetConsoleOutputCP(65001);
#endif
    //== Windows控制台: UTF-8编码 ==


    //== 初始化经典棋盘 ==
    typedef array<array<ct_chess,9>,10> def_arr_b;
    def_arr_b arr_board; //棋盘点位容器
    vector<vector<string>> board_layout{
        {"车","马","象","士","将","士","象","马","车"},
        {"","","","","","","","",""},
        {"","炮","","","","","","炮",""},
        {"兵","","兵","","兵","","兵","","兵"},
        {"","","","","","","","",""}
    };

    //显示与名称映射
    map<string,string> map_table{
        {"将","J"}, {"士","S"}, {"象","X"}, {"马","M"}, {"车","C"},
        {"炮","P"}, {"兵","B"},
    };

    //初始化经典棋盘布局
    init_board_empty(arr_board);//设置空棋盘
    if(init_board_default(arr_board,board_layout,map_table) == false)
    { cout<<"err: init board"<<endl; return -1; }

    //显示棋盘
    //show_board(arr_board);//默认棋盘显示
    //show_board_all(arr_board);//矫正棋盘显示
    //show_board_type(arr_board);//玩家视角显示
    //== 初始化经典棋盘 ==


    //===== 设置游戏机制 =====
    //声明棋子类型
    Tjiang<def_arr_b> jiang(arr_board,"J");
    Tshi<def_arr_b> shi(arr_board,"S");
    Txiang<def_arr_b> xiang(arr_board,"X");
    Tma<def_arr_b> ma(arr_board,"M");
    Tche<def_arr_b> che(arr_board,"C");
    Tpao<def_arr_b> pao(arr_board,"P");
    Tbing<def_arr_b> bing(arr_board,"B");

    //棋子选择点位解析--解析出命令选中棋子的具体坐标
    typedef  Tparse_f<def_arr_b,Tchoose> def_parse_f;
    def_parse_f parse_f;
    parse_f.set_board(arr_board);
    parse_f.set_choose(Tchoose());//设置冲突选择--解决并列棋子选择冲突问题

    //棋子目标落点解析--解析出落子坐标
    //      1.采用模板化职责链模式（模仿职责链将任务函数串成任务列）
    //      2.将解析类加入任务列表
    //      3.依次寻找符合条件的棋子类解析--倒序
    typedef Tparse_t<
                Tjiang<def_arr_b>*,
                Tshi<def_arr_b>*,
                Txiang<def_arr_b>*,
                Tma<def_arr_b>*,
                Tche<def_arr_b>*,
                Tpao<def_arr_b>*,
                Tbing<def_arr_b>*
            > def_parse_t;
    def_parse_t parse_t;
    parse_t.add_work(&jiang,&shi,&xiang,&ma,&che,&pao,&bing);

    //键盘输入--分析选棋与落子的点位
    Tkey_input<def_parse_f,def_parse_t> input;
    input.set_pares(parse_f,parse_t); //输入解析方式设置
    //===== 设置游戏机制 =====

    //== 测试代码填充区 ==
    //... 测试代码段复制到这里执行，测试各个棋子的移动是否符合象棋规范
    //== 测试代码填充区 ==

    //===== 进入游戏 =====
    //待解析的棋子点位与状态
    ct_point from;              //选中棋子点位
    ct_point to;                //即将移动的点位
    bool quit = false;          //玩家主动退出
    bool first = true;          //先手标记
    bool end = false;           //游戏结束标记
    vector<string> vec_his;     //历史容器

    //游戏显示棋盘与双方棋子，接收玩家落点之后移动棋子并切换落子选手
    //      1.会记录行棋记录，结束时显示
    //      2.会翻转棋盘显示，如果不想镜像显示可以在显示之前翻转回来
    //          不翻转棋盘，解析棋子无法处理落点结果
    //          因为当前棋子只解析单方位置
    while(true)
    {
        //显示信息
        cout << "\n===== 玩家视角 =====\n" << endl;
        cout << "===== 先手:[将] | 后手:(将) =====\n" <<endl;
        show_board_type(arr_board);
        if(first) cout << "\n先手玩家:" << endl;
        else cout << "\n后手玩家:" << endl;

        //获取输入，并判断是否胜利
        //  获取输入点位之后，会执行一系列的行棋规范判定
        //  符合棋规则返回成功
        if(input.input_pos(from,to,quit,first))
        {
            //吃将
            ct_chess chess = get_chess(to,arr_board);
            if(chess._name == "J")
            {
                end = true;
                if(first) cout<<"先手玩家成功斩杀"<<endl;
                else cout<<"后手玩家成功斩杀"<<endl;
            }

            first = !first;//选手切换
            move_chess(from,to,arr_board);//移动棋子
            rotate_borad(arr_board);//翻转棋盘显示
            vec_his.push_back(input.get_cmd());//加入历史
        }

        //玩家退出
        if(quit)
        {
            if(first) cout<<"先手玩家掀棋盘,后手玩家胜利"<<endl;
            else cout<<"后手玩家掀棋盘,先手玩家胜利"<<endl;
            end = true;
        }

        //游戏结束
        if(end) break;
    }
    //===== 进入游戏 =====

    //显示历史记录
    cout<<"\n== 历史记录 =="<<endl;
    for(size_t i=0;i<vec_his.size();i++)
    { cout<<"num "<<i<<": "<<vec_his[i]<<endl; }
    cout<<"== 历史记录 ==\n"<<endl;

    cout << "===== end =====" << endl;
    return 0;
}

/*
== 历史记录(移动参考)==
    B5J1
    C1J1
    P2P5
    M2J3
    P8J4
    X3F1
    S4J5
    C1P5
    P5J4
    P2J7
    P5J3
== 历史记录(移动参考) ==
*/


/*
    //== 测试代码 ==
    //范围测试
    cout<<jiang.action(ct_point{5,2},ct_point{6,2})<<endl;
    cout<<jiang.action(ct_point{5,2},ct_point{4,2})<<endl;
    cout<<jiang.action(ct_point{5,2},ct_point{5,1})<<endl;
    cout<<jiang.action(ct_point{5,2},ct_point{5,3})<<endl;
    //越界、斜线、超距测试
    cout<<jiang.action(ct_point{5,2},ct_point{6,3})<<endl;
    cout<<jiang.action(ct_point{5,2},ct_point{4,1})<<endl;
    cout<<jiang.action(ct_point{6,3},ct_point{7,3})<<endl;
    cout<<jiang.action(ct_point{4,1},ct_point{3,1})<<endl;
    cout<<jiang.action(ct_point{6,3},ct_point{6,4})<<endl;
    cout<<jiang.action(ct_point{4,1},ct_point{4,0})<<endl;
    cout<<jiang.action(ct_point{5,2},ct_point{5,4})<<endl;
    cout<<jiang.action(ct_point{5,2},ct_point{7,2})<<endl;

    //斜线测试
    cout<<shi.action(ct_point{5,2},ct_point{6,3})<<endl;
    cout<<shi.action(ct_point{5,2},ct_point{4,1})<<endl;
    //直线测试
    cout<<shi.action(ct_point{5,2},ct_point{5,3})<<endl;
    cout<<shi.action(ct_point{5,2},ct_point{5,1})<<endl;
    cout<<shi.action(ct_point{5,2},ct_point{6,2})<<endl;
    cout<<shi.action(ct_point{5,2},ct_point{4,2})<<endl;

    //田字测试
    cout<<xiang.action(ct_point{3,1},ct_point{1,3})<<endl;
    cout<<xiang.action(ct_point{3,1},ct_point{5,3})<<endl;
    cout<<xiang.action(ct_point{5,3},ct_point{7,5})<<endl;
    cout<<xiang.action(ct_point{5,3},ct_point{3,5})<<endl;
    //越界、超距、直线测试
    cout<<xiang.action(ct_point{3,5},ct_point{5,7})<<endl;
    cout<<xiang.action(ct_point{3,5},ct_point{5,4})<<endl;
    cout<<xiang.action(ct_point{3,5},ct_point{5,5})<<endl;

    //日字测试
    cout<<ma.action(ct_point{2,1},ct_point{3,3})<<endl;
    cout<<ma.action(ct_point{2,1},ct_point{1,3})<<endl;
    //短距、超距、直线测试
    cout<<ma.action(ct_point{2,1},ct_point{2,2})<<endl;
    cout<<ma.action(ct_point{2,1},ct_point{3,2})<<endl;
    cout<<ma.action(ct_point{2,1},ct_point{4,4})<<endl;
    cout<<ma.action(ct_point{2,1},ct_point{5,2})<<endl;
    cout<<ma.action(ct_point{2,1},ct_point{2,3})<<endl;

    //直线测试
    cout<<che.action(ct_point{1,1},ct_point{9,1})<<endl;
    cout<<che.action(ct_point{5,1},ct_point{5,5})<<endl;
    //斜线测试
    cout<<che.action(ct_point{5,1},ct_point{9,5})<<endl;
    cout<<che.action(ct_point{5,1},ct_point{1,5})<<endl;

    //直线测试
    cout<<pao.action(ct_point{1,1},ct_point{9,1})<<endl;
    cout<<pao.action(ct_point{5,1},ct_point{5,5})<<endl;
    //斜线测试
    cout<<pao.action(ct_point{5,1},ct_point{9,5})<<endl;
    cout<<pao.action(ct_point{5,1},ct_point{1,5})<<endl;

    //河内外测试
    cout<<bing.action(ct_point{5,5},ct_point{5,6})<<endl;
    cout<<bing.action(ct_point{5,6},ct_point{6,6})<<endl;
    cout<<bing.action(ct_point{5,6},ct_point{4,6})<<endl;
    cout<<bing.action(ct_point{5,6},ct_point{5,7})<<endl;

    //斜线、超距、后退测试
    cout<<bing.action(ct_point{5,5},ct_point{5,7})<<endl;
    cout<<bing.action(ct_point{5,5},ct_point{6,6})<<endl;
    cout<<bing.action(ct_point{5,5},ct_point{5,4})<<endl;
    cout<<bing.action(ct_point{5,7},ct_point{5,6})<<endl;
    //== 测试代码 ==
*/
