#include "pager.h"
#include <fcntl.h>
#include <unistd.h>
#include <cassert>
#include <cstring>
#include <stdexcept>

using namespace std;

Pager::Pager(const string& filename) {
    fd_ = open(filename.c_str(), O_RDWR | O_CREAT, 0644);

    if (fd_ == -1) {
        throw runtime_error("Failed to open database file");
    }

    off_t fileSize = lseek(fd_, 0, SEEK_END);

    if (fileSize % PAGE_SIZE != 0) {
        throw runtime_error("Database file is not a whole number of pages");
    }

    numPages_ = static_cast<PageNum>(fileSize / PAGE_SIZE);
}

Pager::~Pager() {
    flushAll();
    close(fd_);
}

uint8_t* Pager::getPage(PageNum pageNum) {
    assert(pageNum < numPages_ &&
           "getPage called on an unallocated page");

    auto it = pageCache_.find(pageNum);

    if (it != pageCache_.end()) {
        return it->second;
    }

    uint8_t* buffer = new uint8_t[PAGE_SIZE];

    readPageFromDisk(pageNum, buffer);
    pageCache_[pageNum] = buffer;

    return buffer;
}

PageNum Pager::allocatePage() {
    PageNum newPage = numPages_;
    numPages_++;

    uint8_t* buffer = new uint8_t[PAGE_SIZE];

    memset(buffer, 0, PAGE_SIZE);

    pageCache_[newPage] = buffer;
    dirtyPages_.insert(newPage);

    return newPage;
}

void Pager::flushAll() {
    for (PageNum pageNum : dirtyPages_) {
        writePageToDisk(pageNum, pageCache_[pageNum]);
    }

    dirtyPages_.clear();
}

void Pager::readPageFromDisk(PageNum pageNum, uint8_t* buffer) {
    off_t offset = static_cast<off_t>(pageNum) * PAGE_SIZE;

    lseek(fd_, offset, SEEK_SET);

    ssize_t bytesRead = read(fd_, buffer, PAGE_SIZE);

    if (bytesRead < 0) {
        throw runtime_error("Failed to read page from disk");
    }

    if (static_cast<size_t>(bytesRead) < PAGE_SIZE) {
        memset(buffer + bytesRead, 0, PAGE_SIZE - bytesRead);
    }
}

void Pager::writePageToDisk(PageNum pageNum, uint8_t* buffer) {
    off_t offset = static_cast<off_t>(pageNum) * PAGE_SIZE;

    lseek(fd_, offset, SEEK_SET);

    write(fd_, buffer, PAGE_SIZE);
}