# Best Practices for C Programming

## 第一章：提高代码可读性

### 作业

    参照 Linux 内核编码风格整理 BPCP 仓库中的 bin2c.c 源文件

## 第二章：用好写好头文件

### 作业

    重构 OpenWRT libubox 函数库的头文件

    重构后的文件架构如下所示

    source
    ├── include                 # 外部头文件所在位置
    │   ├── private                 # 全局内部头文件所在位置
    │   │   └── assert.h
    │   ├── avl-cmp.h
    │   ├── avl.h
    │   ├── blob.h
    │   ├── blobmsg.h
    │   ├── blobmsg_json.h
    │   ├── json_script.h
    │   ├── kvlist.h
    │   ├── list.h
    │   ├── md5.h
    │   ├── runqueue.h
    │   ├── safe_list.h
    │   ├── udebug.h
    │   ├── udebug-proto.h
    │   ├── ulog.h
    │   ├── uloop.h
    │   ├── usock.h
    │   ├── ustream.h
    │   ├── utils.h
    │   └── vlist.h
    ├── uavl                    
    │   ├── avl.c
    │   └── avl-cmp.c
    ├── ublob
    │   ├── blob.c
    │   ├── blobmsg.c
    │   └── blobmsg_json.c
    ├── ucrypto
    │   ├── base64.c
    │   └── md5.c
    ├── udebug                  # 模块源文件及模块内部头文件所在位置
    │   ├── udebug.c
    │   ├── udebug-priv.h
    │   ├── udebug-remote.c
    │   └── ulog.c
    ├── ujson
    │   ├── jshn.c
    │   └── json_script.c
    ├── ulist
    │   ├── kvlist.c
    │   ├── safe_list.c
    │   └── vlist.c
    ├── uloop
    │   ├── uloop.c
    │   ├── uloop-epoll.c
    │   └── uloop-kqueue.c
    ├── urunqueue
    │   └── runqueue.c
    ├── usock
    │   └── usock.c
    ├── ustream
    │   ├── ustream.c
    │   └── ustream-fd.c
    └── uutils
        └── utils.c