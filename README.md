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