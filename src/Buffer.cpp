#include "./include/Buffer.h"
#include <iostream>

Buffer::Buffer() : buf() {}
Buffer::~Buffer() {}

void Buffer::append(const char *_str, int _size) {
    for(int i = 0; i < _size; ++i) {
        if(_str[i] != '\0') {
            buf.push_back(_str[i]);
            continue;
        }
        return ;
    }
}
void Buffer::clear() {
    // swap清空法
    std::string().swap(buf);
}

void Buffer::getline() {
    clear();
    std::getline(std::cin, buf);
}

void Buffer::setBuf(const char* _buf) {
    clear();
    buf.append(_buf);
}