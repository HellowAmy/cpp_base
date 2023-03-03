//!
//! ===== 基本概念 =====
//! 面向对象的基本概念:
//!     1、封装 : 将数据与操作封装在类内，对外只暴露最小接口
//!     2、继承 : 子类继承父类多有的数据与操作，实现代码复用
//!     3、多态 : 由父类定义接口，子类重新实现具体操作，可以利用父类指针指向子类操作
//!
//!     在面向对象中，最关键的点就在于多态的使用，使用多态的好处可以通过接口简化具体操作，
//!         由父类定义抽象接口实现具体的任务流，通过子类重新实现时可以做到在固定流程程中，
//!         具体实现的重定义
//!     封装是将数据结构与具体操作私有化，不让外部随便调用，放入任务出结果，实现模块化，
//!         继承则只是简单的复制父类的所有内容，这两个特性并不神奇，不过仍然需要妥善处理，
//!         不然返回会出现需要反复利用的代码被隐藏，冗余的数据被子类大量继承这种奇怪的场景。
//!     掌握理论知识之后，接下来进入实际的编程环节。
//! ===== 基本概念 =====
//!
//!
//!
//! ===== 任务介绍 =====
//! 使用面向对象的方式写一个五子棋游戏:
//!     场景描述:
//!         一般的五子棋都为两个选手对着一个棋盘下棋，一个裁判判断胜利谁会胜出。
//!             总结为:两个人下棋，一个裁判，一个棋盘，一个规则，谁会胜出？
//!     高级抽象:
//!         棋手类、裁判类、棋盘类、规则类、棋子类
//! ===== 任务介绍 =====
//!
//!
//!
//! ===== 具体规划 =====
//! 棋手:在棋盘下落子，会有先后手
//!     抽象棋手:
//!         行为[设置先手权、下棋、掀棋盘、获取棋盘规则、获取胜利通知]
//!         属性[落子权、累计落子]
//!     五子棋手:
//!         行为[庆祝语]
//!
//! 裁判:负责规划整个游戏流程，设置骑手先后手，
//!     行为[设置棋手、设置棋盘、设置棋局规则、判定先手权、判定获胜、开始游戏]
//!
//! 棋盘:显示棋盘，棋手在棋盘上落子的信息展示与记录
//!     抽向棋盘:
//!         行为[允许落子]
//!         属性[累计回合]
//!     五子棋盘:
//!         属性[五子棋盘]
//!
//! 规则:判断落子之后是否胜利
//!     抽象规则:
//!         行为[获取棋盘、获取棋盘落点、是否获胜、规则描述]
//!     五子规则:
//!         行为[五子棋胜利判定规则]
//!
//! 棋子:在棋盘上的标记
//!     属性[坐标、先手]
//! ===== 具体规划 =====
//!
#include <cstdint>
#include <iostream>
#include <sstream>
#include <vector>
#include <ctime>

using namespace std;

//== 字符串类型转换 ==
template<typename T>
string to_string(const T& t){ ostringstream os; os<<t; return os.str(); }

template<typename T>
T from_string(const string& str){ T t; istringstream iss(str); iss>>t; return t; }
//== 字符串类型转换 ==



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

//===== 输入方式 =====
//== 输入抽象接口 ==
class abs_input
{
public:
    virtual ct_point input_pos() = 0;
    bool get_quit(){ return _quit; };

protected:
    bool _quit = false; //掀棋盘
};

//== 键盘输入 ==
class key_input : public abs_input
{
public:
    ct_point input_pos() override
    {
        string str; ct_point pos;
        while(true)
        {
            cin>>str;
            if(str == "quit") { _quit = true; return pos; } //掀棋盘操作
            if(parse_pos(str,pos)) { return pos; }
        }
    }

private:
    //解析落子点
    string _flg = "-";
    bool parse_pos(const string &str,ct_point &pos)
    {
        size_t index = str.find(_flg);
        if(index != str.npos)
        {
            string first(str.begin(),str.begin() + index);
            string second(str.begin() + _flg.size() + index,str.end());
            pos.x = from_string<uint32_t>(first);
            pos.y = from_string<uint32_t>(second);
            return true;
        }
        return false;
    }
};
//===== 输入方式 =====



//===== 棋手 =====
//== 抽象棋手 ==
class abs_player
{
public:
    void set_input(abs_input *input) { _input = input; };       //设置输入方式
    void set_first(bool first) { _first = first; };             //设置先手
    void set_victory(bool victory) { _victory = victory; };     //设置胜利
    bool get_is_first() { return _first; };                     //查看是否为先手
    bool get_is_quit() { return _input->get_quit(); };          //掀棋盘
    void set_name(const string &name) { _name = name; }         //设置选手名称
    string get_name() { return _name; }                         //获取选手名称

    virtual ct_point down_piece() = 0;  //落子
    virtual void down_succeed() = 0;    //落子成功反馈
    virtual void show_victory() = 0;    //胜利通知

protected:
    abs_input *_input = nullptr;
    bool _first = false;
    bool _victory = false;
    string _name;
};

//== 五子棋手 ==
class five_player : public abs_player
{
public:
    //设置胜利庆祝语
    void set_victory_celebratory(const string &str) { _celebratory = str; }
    uint32_t get_count_down() { return _count; } //累计落子数

    void down_succeed() override { _count++ ; };
    void show_victory() override { cout<<_celebratory<<endl; };
    ct_point down_piece() override { cout<<_name<<": "; return _input->input_pos(); };

private:
    uint32_t _count = 0;
    string _celebratory;
};
//===== 棋手 =====



//===== 棋子 =====
//== 抽象棋子 ==
class abs_chess
{
public:
    abs_chess(){}
    abs_chess(ct_point pos) : _pos(pos){}
    void add_point(ct_dire dire) { _pos.x += dire.x; _pos.y += dire.y; }
    ct_point _pos;
    bool _first;
};

//== 五子棋 ==
class five_chess : public abs_chess
{
public:
    bool _black;
    bool _exist;
};
//===== 棋子 =====



//===== 棋盘 =====
//== 抽象棋盘 ==
class abs_chessboard
{
protected:
    //== 棋盘范围 ==
    class ct_range
    {
    public:
        ct_point begin;
        ct_point end;
        bool has_range(ct_point pos) //判断落点是否在棋盘上
        {
            if(pos.x >= end.x || pos.y >= end.y ||
                    pos.x < begin.x || pos.y < begin.y) return false;
            else return true;
        }
    };

public:
    abs_chessboard(uint32_t x,uint32_t y)
    {
        _range.begin.x = 0; _range.begin.y = 0;
        _range.end.x = x; _range.end.y = y;
    }
    bool range(abs_chess chess) { return _range.has_range(chess._pos); };
    void show_history() //游戏结束时显示记录
    { for(const auto &a : _history) { cout<<a.x<<"|"<<a.y<<endl; } }

    virtual void show_chessboard() = 0; //显示棋盘
    virtual bool down_piece(abs_chess chess) = 0; //棋盘落子

protected:
    ct_range _range;
    vector<ct_point> _history;
};

//== 五子棋盘 ==
class five_chessboard : public abs_chessboard
{
public:
    five_chessboard(uint32_t x,uint32_t y) : abs_chessboard(x,y)
    { init_board(x,y); }

    //在显示棋盘内容上，本打算在创建一个抽象显示类，
    //      可惜实在是想不出怎么样处理各种不同游戏不同类型的数据结构容器，不懂该怎么去定义接口，
    //      因为本人对面向对象编程方式的理解并不深入，而是一个模板爱好者，
    //      如果使用模板则可以轻松解决这个问题，但这一份代码就是为了学习面向对象的处理方式，万不可引用模板
    //      所以除非想到更好的用多态的方式解决，否则就让代码更死板一点吧
    //
    //显示棋盘内容 :
    void show_chessboard() override
    {
        for(uint32_t y=0;y<_board.size();y++)
        {
            for(uint32_t x=0;x<_board[y].size();x++)
            {
                five_chess ch = _board[x][y];
                if(ch._exist)
                {
                    if(ch._black) cout<<"黑"<<" ";
                    else cout<<"白"<<" ";
                }
                else cout<<"[]"<<" ";
            }
            cout<<endl;
        }
    }

    //成功落子
    bool down_piece(abs_chess chess) override
    {
        if(range(chess._pos))
        {
            five_chess *ch = &_board[chess._pos.x][chess._pos.y];
            if(ch->_exist == false)
            {
                ch->_pos.x = chess._pos.x; ch->_pos.y = chess._pos.y;
                ch->_exist = true; ch->_black = chess._first;
                _history.push_back(chess._pos);
                return true;
            }
        }
        return false;
    }

    //获取棋子
    five_chess get_chess(abs_chess chess)
    { return _board[chess._pos.x][chess._pos.y]; }
    vector<vector<five_chess>> * get_chessboard() { return &_board; } //获取棋盘

    //检查棋盘
    void show_board()
    {
        for(uint32_t y=0;y<_board.size();y++)
        {
            for(uint32_t x=0;x<_board[y].size();x++)
            {
                five_chess ch = _board[x][y];
                cout<<ch._pos.x<<"|"<<ch._pos.y<<"|"<<ch._black<<"|"<<ch._exist<<" ";
            }
            cout<<endl;
        }
    }

private:
    vector<vector<five_chess>> _board;

    //初始化棋盘大小
    void init_board(uint32_t x,uint32_t y)
    {
        for(uint32_t i=0;i<x;i++)
        {
            vector<five_chess> vec;
            five_chess ch;
            for(uint32_t j=0;j<y;j++)
            { ch._pos.x = 0; ch._pos.y = 0; ch._exist = false; vec.push_back(ch); }
            _board.push_back(vec);
        }
    }
};
//===== 棋盘 =====



//===== 规则 =====
//== 抽象规则 ==
class abs_rule
{
public:
    virtual bool has_victory(abs_chess pos) = 0;
};

//== 五子规则 ==
class five_rule : public abs_rule
{
public:
    //判断胜利标记
    bool has_victory(abs_chess pos) override
    {
        uint32_t hori = 1;//东+西
        hori += count_chess(pos,ct_dire{-1,0});
        hori += count_chess(pos,ct_dire{1,0});
        uint32_t vert = 1;//南+北
        vert += count_chess(pos,ct_dire{0,1});
        vert += count_chess(pos,ct_dire{0,-1});
        uint32_t right_up = 1;//东南+西北
        right_up += count_chess(pos,ct_dire{-1,1});
        right_up += count_chess(pos,ct_dire{1,-1});
        uint32_t left_down = 1;//西南+东北
        left_down += count_chess(pos,ct_dire{1,1});
        left_down += count_chess(pos,ct_dire{-1,-1});

        cout<<hori<<"|"<<vert<<"|"<<right_up<<"|"<<left_down<<endl;
        if(hori >= 5 || vert >= 5 || right_up >= 5 || left_down >= 5) return true;
        else return false;
    }
    void set_chessboard(five_chessboard *board) { _board = board; }

private:
    five_chessboard *_board = nullptr;

    //递归统计一个方向内相同颜色的棋子
    uint32_t count_chess(abs_chess pos,ct_dire dire)
    {
        pos.add_point(dire);
        if(_board->range(pos))
        {
            five_chess chess = _board->get_chess(pos);
            if(chess._exist == true
                    && chess._black == pos._first)
                return count_chess(pos,dire) + 1;
        }
        return 0;
    }
};
//===== 规则 =====



//===== 裁判 =====
//== 抽象裁判 ==
class abs_praetor
{
public:
    abs_praetor() { srand((uint32_t)time(NULL)); }

    //游戏流程（需要两个玩家，一个棋盘，一个游戏规则）
    virtual void start_play(abs_player *p1,abs_player *p2,
        abs_chessboard *board,abs_rule *rule) = 0;

protected:
    bool _first = true;
};

//== 五子棋裁判 ==
class five_praetor : public abs_praetor
{
public:
    void start_play(abs_player *p1,abs_player *p2,
        abs_chessboard *board,abs_rule *rule) override
    {
        bool victory_first = false; //胜利方标记（先手或者后手方）
        if((rand() % 2) == 0) { p1->set_first(_first); p2->set_first(!_first); }
        else { p2->set_first(_first); p1->set_first(!_first); }

        //先手提示
        {
            string name;
            if(p1->get_is_first()) name = p1->get_name();
            else name = p2->get_name();
            cout<<endl<<"first player:"<<name<<endl;
        }

        abs_player *down_player;
        while (true)
        {
            //判断落子选手
            if(_first == p1->get_is_first()) down_player = p1;
            else down_player = p2;

            //如果选手掀棋盘，对方胜利
            abs_chess pos = down_player->down_piece(); //选手落子
            pos._first = down_player->get_is_first();
            if(down_player->get_is_quit()) //掀棋盘处理
            { victory_first = !down_player->get_is_first(); break; }

            //查看棋盘是否允许落子，不允许则重新落子
            if(board->down_piece(pos))
            {
                board->show_chessboard();
                down_player->down_succeed(); _first = !_first; //落子状态取非，下回合到对手

                //规则胜利
                if(rule->has_victory(pos))
                { victory_first = down_player->get_is_first(); break; }
            }
            else cout<<"Invalid input"<<endl;
        }

        //显示历史
        cout<<"===== history ====="<<endl;
        board->show_history();
        cout<<"----- history -----"<<endl;

        //胜利反馈
        if(p1->get_is_first() == victory_first) p1->show_victory();
        else p2->show_victory();
    }
};
//===== 裁判 =====

#ifdef _WIN32
#include <windows.h>
#endif

int main()
{

#ifdef _WIN32
    //Windows控制台: UTF-8编码
    SetConsoleOutputCP(65001);
#endif

    cout<<"===== 中文测试 ====="<<endl;
    cout<<"===== begin ====="<<endl;
    key_input input; //输入方式

    //玩家P1
    five_player p1;
    p1.set_name("p1[Jerry]");
    p1.set_victory_celebratory("Jerry:What are you doing? Tom");
    p1.set_input(&input);

    //玩家P2
    five_player p2;
    p2.set_name("P2[Tom]");
    p2.set_victory_celebratory("Tom:Nice Tom cat,You are the best");
    p2.set_input(&input);

    //设置棋盘
    five_chessboard board(15,15);
    board.show_chessboard();

    //设置规则
    five_rule rule;
    rule.set_chessboard(&board); //获取棋盘的旗子容器

    //生成裁判，游戏开始
    five_praetor praetor;
    praetor.start_play(&p1,&p2,&board,&rule);
    cout<<"===== end ====="<<endl;

    return 0;
}
