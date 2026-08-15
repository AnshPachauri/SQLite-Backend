#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>

constexpr size_t PAGE_SIZE = 4096;
using PageNum = uint32_t;

class Pager {
public:
    explicit Pager(const std::string& filename);
    ~Pager();

    uint8_t* getPage(PageNum pageNum);
    PageNum allocatePage();
    void flushAll();

private:
    int fd_;
    PageNum numPages_;
    std::unordered_map<PageNum, uint8_t*> pageCache_;
    std::unordered_set<PageNum> dirtyPages_;

    void readPageFromDisk(PageNum pageNum, uint8_t* buffer);
    void writePageToDisk(PageNum pageNum, uint8_t* buffer);
};