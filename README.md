# Best Practices for C Programming

## 第一章：提高代码可读性

### 作业

    参照 Linux 内核编码风格整理 BPCP 仓库中的 bin2c.c 源文件

## 第二章：用好写好头文件

### 作业

    重构 OpenWRT libubox 函数库的头文件

    重构后的文件架构如下所示

    source
    ├── avl
    │   ├── avl.c
    │   └── avl-cmp.c
    ├── blob
    │   ├── blob.c
    │   ├── blobmsg.c
    │   └── blobmsg_json.c
    ├── crypto
    │   ├── base64.c
    │   └── md5.c
    ├── debug                       # 模块源文件及模块内部头文件所在位置
    │   ├── udebug.c
    │   ├── udebug-priv.h
    │   ├── udebug-remote.c
    │   └── ulog.c
    ├── include                     # 外部头文件所在位置
    │   ├── private                     # 全局内部头文件所在位置
    │   │   └── assert.h
    │   ├── ubox-avl-cmp.h
    │   ├── ubox-avl.h
    │   ├── ubox-blob.h
    │   ├── ubox-blobmsg.h
    │   ├── ubox-blobmsg_json.h
    │   ├── ubox-json_script.h
    │   ├── ubox-kvlist.h
    │   ├── ubox-list.h
    │   ├── ubox-md5.h
    │   ├── ubox-runqueue.h
    │   ├── ubox-safe_list.h
    │   ├── ubox-udebug.h
    │   ├── ubox-udebug-proto.h
    │   ├── ubox-ulog.h
    │   ├── ubox-uloop.h
    │   ├── ubox-usock.h
    │   ├── ubox-ustream.h
    │   ├── ubox-utils.h
    │   └── ubox-vlist.h
    ├── json
    │   ├── jshn.c
    │   └── json_script.c
    ├── list
    │   ├── kvlist.c
    │   ├── safe_list.c
    │   └── vlist.c
    ├── loop
    │   ├── uloop.c
    │   ├── uloop-epoll.c
    │   └── uloop-kqueue.c
    ├── runqueue
    │   └── runqueue.c
    ├── sock
    │   └── usock.c
    ├── stream
    │   ├── ustream.c
    │   └── ustream-fd.c
    └── utils
        └── utils.c

## 第三章：消除编译警告

### 作业

    使用 -Wall -Wextra 选项编译 bin2.c 和 libubox，并消除或者压制其中的警告

    本章作业在文件夹 lecture1 及 lecture2 中完成

## 第四章：常量的定义和使用

### 作业

    编写一个程序，使用 Glib 或 PurC 的原子字符串接口判断用户输入的单词是否为一个 C 语言关键词

    使用 Glib 库，在 lecture4/ckw.c 中实现了此功能

## 第五章：善用构建系统生成器

### 作业

    使用本章介绍的 CMake 构建体系模板管理 libubox 项目

    本章作业在文件夹 lecture2 中完成