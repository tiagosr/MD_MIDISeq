#include "TR_ProgMemAsFile.h"

static ProgMemFs* ProgMemFs::_current_fs = nullptr;

ProgMemFile::ProgMemFile(const ProgMemFileInfo* fileInfo)
{
    _fileInfo = fileInfo;
}
ProgMemFile::ProgMemFile(void)
{
    _name = "<invalid file>";
    _fileInfo = nullptr;
    _position = __UINT32_MAX__;
}

int ProgMemFile::read()
{
    if (!_fileInfo || _position >= _fileInfo->_size) {
        return -1;
    }
    return pgm_read_byte(_fileInfo->_location + _position++);
}

int ProgMemFile::peek()
{
    if (!_fileInfo || _position >= _fileInfo->_size) {
        return -1;
    }
    return pgm_read_byte(_fileInfo->_location + _position);
}

int ProgMemFile::available()
{
    return _fileInfo->_size - _position;
}

size_t ProgMemFile::write(uint8_t byte) {
    return 0;
}

int ProgMemFile::read(void *buf, uint16_t nbyte)
{
    return readBytes((char*)buf, nbyte);
}

bool ProgMemFile::seek(uint32_t pos)
{
    if (!_fileInfo || pos >= _fileInfo->_size) {
        return false;
    }
    _position = pos;
}

void ProgMemFile::close() {}

bool ProgMemFile::open(const char*name, oflag_t oflag)
{
    return _fileInfo->_name == name && (oflag & O_READ) != 0;
}

int ProgMemFile::fgets(char* str, int num, const char* delim = nullptr)
{
    int c = 0;
    int b;
    if (delim) {
        for (;c < num; c++) {
            b = read();
            if (b > 0) {
                for (int d = 0; delim[d] != 0; d++) {
                    if (b == delim[d]) {
                        str[c] = '\0';
                        return c;
                    }
                }
                str[c] = (char)b;
            } else {
                str[c] = '\0';
                return c;
            }
        }
    } else {
        for (;c < num; c++) {
            b = read();
            if (b > 0) {
                str[c] = (char)b;
            } else {
                str[c] = '\0';
                return c;
            }
        }
    }
}


void ProgMemFs::begin(const ProgMemFileInfo* root)
{
    _current_dir = root;
}

bool ProgMemFs::chdir(const char* dir)
{

    return false;
}

void ProgMemFs::chvol(void)
{
}

ProgMemFile ProgMemFs::open(const char* filename, oflag_t flag)
{
    ProgMemFileInfo* cur_dir = _current_dir;
    for (;cur_dir != nullptr; cur_dir = cur_dir->_next) {
        if (strcasecmp(filename, cur_dir->_name) == 0) {
            return ProgMemFile(cur_dir);
        }
    }
}

