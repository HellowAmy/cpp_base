# == 函数 ==
# 递归目录并直接包含头文件函数
function(include_directories_sub root_dir)
    if (IS_DIRECTORY ${root_dir})               
        include_directories(${root_dir})
    endif()

    file(GLOB ALL_SUB RELATIVE ${root_dir} ${root_dir}/*)
    foreach(sub ${ALL_SUB})
        if (IS_DIRECTORY ${root_dir}/${sub})
        include_directories_sub(${root_dir}/${sub})
        endif()
    endforeach()
endfunction()


# 递归获取头文件子目录路径到变量
function(fn_get_include_sub_ls result root_dir)
    set(abs_dir ${CMAKE_SOURCE_DIR}/${root_dir})
    if (IS_DIRECTORY ${abs_dir})
        file(GLOB children RELATIVE ${abs_dir} ${abs_dir}/*)
        set(dirlist "")
        foreach(child ${children})
            if(IS_DIRECTORY ${abs_dir}/${child})
                list(APPEND dirlist ${root_dir}/${child})
                fn_get_include_sub_ls(subdirlist ${root_dir}/${child})
                list(APPEND dirlist ${subdirlist})
            endif()
        endforeach()
        set(${result} ${dirlist} PARENT_SCOPE)
    endif()
endfunction()

# 递归获取目录下的库文件
function(fn_get_lib_ls result root_dir)
    file(GLOB_RECURSE reslib "${root_dir}/*.so*")
    set(${result} ${reslib} PARENT_SCOPE)
endfunction()
# == 函数 ==



# ===== 编译入口 =====
# 项目名
cmake_minimum_required(VERSION 3.22.1)
project(cpp_base)


# C++ 11 版本
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# 调试模式
set(CMAKE_BUILD_TYPE "Debug")

fn_get_include_sub_ls(PRO_INCLUDE include)
# include_directories(${CMAKE_SOURCE_DIR}/include)

include_directories(${PRO_HEAD_DIR})

# == 生成目标 ==
# main_Tvlog
add_executable(main_Tvlog cpp_tools/main_Tvlog.cpp)

# main_format
add_executable(main_format cpp_tools/main_format.cpp)

