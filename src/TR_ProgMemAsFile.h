#pragma once

#include <Arduino.h>
#include <SdFat.h>


struct ProgMemFileInfo {
    const char * _name;
    void * _location;
    uint32_t _size;
    const ProgMemFileInfo *_parent;
    const ProgMemFileInfo *_next;
    ProgMemFileInfo(const char* name, void* location, uint32_t size, const ProgMemFileInfo* root, const ProgMemFileInfo* next = nullptr)
    : _name(name), _location(location), _size(size), _parent(root), _next(next) {}
    ProgMemFileInfo(const ProgMemFileInfo *next)
    : _name("<root>"), _location(nullptr), _size(0), _parent(this), _next(next) {}

};

class ProgMemFile : public Stream {
    const ProgMemFileInfo *_fileInfo;
    const char * _name;
    void * _location;
    uint32_t _size;
    uint32_t _position;
public:
    ProgMemFile(const ProgMemFileInfo* fileInfo);
    ProgMemFile(void);
    virtual int read();
    virtual int peek();
    virtual int available();
    virtual size_t write(uint8_t byte);
    int read(void *buf, uint16_t nbyte);
    bool seek(uint32_t pos);
    uint32_t position() const { return _position; }
    uint32_t size() const { return _size; }
    void close();
    operator bool() const { return !!_location; };
    const char* name() const { return _name; }

    bool open(const char*name, oflag_t oflag);
    int fgets(char* str, int num, const char* delim = nullptr);
    bool seekSet(uint32_t pos) { return seek(pos); }
    bool seekCur(int pos) { return seek(_position + pos); }
    uint32_t curPosition() const { return _position; }
};


class ProgMemFs {
    static ProgMemFs* _current_fs;
    ProgMemFileInfo* _current_dir;
public:
    void begin(const ProgMemFileInfo* root);
    bool chdir(const char* dir);
    void chvol(void);
    ProgMemFile open(const char* filename, oflag_t flag);
};