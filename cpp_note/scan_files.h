//!
//! scan_files.h
//!
#ifndef SCAN_FILES_H
#define SCAN_FILES_H

#include <string.h>
#include <sys/stat.h>
#include <dirent.h>

#include <string>
#include <vector>

using namespace std;

//!
//! 功能:获取目录下文件和目录
//! 注意：Linux系统
//!
class scan_files
{
public:
    //带层级的树形结构
    struct tree
    {
        int deep;           //目录递归深度
        string type;        //目录类型（REG/DIR）
        string name;        //文件或者目录名称
        string path_abs;    //绝对路径
    };

public:
    //递归的将路径下所有普通文件和目录放入容器
    //参数1：扫描目录（绝对路径）
    //参数2：存储文件的容器
    //参数3：存储目录的容器
    //返回值：成功或者失败
    static bool get_files(const string &path,vector<string> &vec_files,vector<string> &vec_dir)
    {
        DIR *pdir_all = opendir(path.c_str());
        if(pdir_all == nullptr) return false;

        dirent *info_dir;
        vector<string> vec_save_dir;
        struct stat ct_stat;
        while ((info_dir = readdir(pdir_all)) != nullptr)
        {
            //获取绝对路径后判断文件类型
            string st_path = path +"/"+ info_dir->d_name;
            stat(st_path.c_str(),&ct_stat);

            //将绝对路径添加到容器后返回
            if(S_ISDIR(ct_stat.st_mode) == true
                    && (strcmp(info_dir->d_name,".") != 0)
                    && (strcmp(info_dir->d_name,"..") != 0))
            {
                string in_path = path +"/"+ info_dir->d_name;
                vec_save_dir.push_back(in_path);
            }
            else if(S_ISREG(ct_stat.st_mode))
            {
                string in_files = path +"/"+ info_dir->d_name;
                vec_files.push_back(in_files);
            }
        }
        vec_dir.insert(vec_dir.end(),vec_save_dir.begin(),vec_save_dir.end());

        //递归搜索
        for(size_t i=0;i<vec_save_dir.size();i++)
        { get_files(vec_save_dir[i],vec_files,vec_dir); }
        closedir(pdir_all);
        return true;
    }

    //get_tree()的简单调用
    //参数1：扫描目录（绝对路径）
    //参数2：文件与目录结构容器
    static bool get_tree_dir(const string &path_abs,vector<tree> &vec_tree)
    { return get_tree(path_abs,".",vec_tree,0); }

    //递归获取带层级的文件目录，深度优先（获取当前层文件，然后进入子文件夹获取所有内容，再获取第二个子文件夹）
    //参数1：扫描目录（绝对路径）
    //参数2：文件或目录名称
    //参数3：文件与目录结构容器
    //参数4：递归深度
    static bool get_tree(const string &abs,const string &rel,vector<tree> &vec_tree,int deep)
    {
        DIR *pdir_all = opendir(abs.c_str());
        if(pdir_all == nullptr) return false;
        tree tm;
        tm.deep = deep;
        tm.type = "DIR";
        tm.path_abs = abs;
        tm.name = rel;
        vec_tree.push_back(tm);
        deep++;

        dirent *info_dir;
        vector<string> vec_save_dir;
        struct stat ct_stat;
        while ((info_dir = readdir(pdir_all)) != nullptr)
        {
            //获取绝对路径后判断文件类型
            string st_path = abs +"/"+ info_dir->d_name;
            stat(st_path.c_str(),&ct_stat);

            //将绝对路径添加到容器后返回
            if(S_ISDIR(ct_stat.st_mode) == true
                    && (strcmp(info_dir->d_name,".") != 0)
                    && (strcmp(info_dir->d_name,"..") != 0))
            { vec_save_dir.push_back(info_dir->d_name); }

            else if(S_ISREG(ct_stat.st_mode))
            {
                tree tm;
                tm.deep = deep;
                tm.type = "REG";
                tm.path_abs = abs +"/"+ info_dir->d_name;
                tm.name = info_dir->d_name;
                vec_tree.push_back(tm);
            }
        }

        //递归搜索
        for(size_t i=0;i<vec_save_dir.size();i++)
        { get_tree(abs+"/"+vec_save_dir[i],vec_save_dir[i],vec_tree,deep); }
        closedir(pdir_all);

        return true;
    }

    //判断目录
    static bool is_dir(string path)
    {
        struct stat ct_stat;
        stat(path.c_str(),&ct_stat);
        return S_ISDIR(ct_stat.st_mode);
    }

    //判断普通文件
    static bool is_file(string path)
    {
        struct stat ct_stat;
        stat(path.c_str(),&ct_stat);
        return S_ISREG(ct_stat.st_mode);
    }
};

#endif // SCAN_FILES_H
