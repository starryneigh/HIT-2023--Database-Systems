/**
 * @author See Contributors.txt for code contributors and overview of BadgerDB.
 *
 * @section LICENSE
 * Copyright (c) 2012 Database Group, Computer Sciences Department, University of Wisconsin-Madison.
 */

#include <memory>
#include <iostream>
#include "buffer.h"
#include "exceptions/buffer_exceeded_exception.h"
#include "exceptions/page_not_pinned_exception.h"
#include "exceptions/page_pinned_exception.h"
#include "exceptions/bad_buffer_exception.h"
#include "exceptions/hash_not_found_exception.h"

namespace badgerdb { 

BufMgr::BufMgr(std::uint32_t bufs)
	: numBufs(bufs) {
	bufDescTable = new BufDesc[bufs];

  for (FrameId i = 0; i < bufs; i++) 
  {
  	bufDescTable[i].frameNo = i;
  	bufDescTable[i].valid = false;
  }

  bufPool = new Page[bufs];

	int htsize = ((((int) (bufs * 1.2))*2)/2)+1;
  hashTable = new BufHashTbl (htsize);  // allocate the buffer hash table

  clockHand = bufs - 1;
}


BufMgr::~BufMgr() {
}

void BufMgr::advanceClock()
{
	clockHand = (clockHand + 1) % numBufs;
}

void BufMgr::allocBuf(FrameId & frame) 
{
	bool flag = true;
	int cnt=0;
	while(true){
		advanceClock();
		
		if (!bufDescTable[clockHand].valid){
			frame = clockHand;
			return;
		}
		
		if (bufDescTable[clockHand].refbit){
			bufDescTable[clockHand].refbit = false;
			continue;
		}
		
		if (bufDescTable[clockHand].pinCnt){
			cnt++;
			if(cnt == numBufs){
				break;
			}
			continue;
		}
		
		if (bufDescTable[clockHand].dirty){
			bufDescTable[clockHand].file->writePage(bufPool[clockHand]);
			bufDescTable[clockHand].dirty = false;
		}
		hashTable->remove(bufDescTable[clockHand].file,bufDescTable[clockHand].pageNo);
		bufDescTable[clockHand].Clear();
		frame = clockHand;
		return;
	}
	throw BufferExceededException();
}
	
void BufMgr::readPage(File* file, const PageId pageNo, Page*& page)
{
	FrameId frame;
	try{
		hashTable->lookup(file, pageNo, frame);
		//printf("数据页请求成功！\n");
		bufDescTable[frame].refbit = true;
		bufDescTable[frame].pinCnt++;
	}
	catch(HashNotFoundException&){ 
		//printf("请求的页不在缓冲池中！\n");
		allocBuf(frame);
		bufPool[frame] = file->readPage(pageNo);
		bufDescTable[frame].Set(file, pageNo);
		hashTable->insert(file, pageNo, frame);
	}
	page = bufPool + frame;
}

void BufMgr::unPinPage(File* file, const PageId pageNo, const bool dirty) 
{
	FrameId frame;
	try{
		hashTable->lookup(file, pageNo, frame);
		if (!bufDescTable[frame].pinCnt)
			throw PageNotPinnedException(file->filename(), pageNo, frame);
			
		bufDescTable[frame].pinCnt--;
		if (dirty)	bufDescTable[frame].dirty = dirty;
	}
	catch(HashNotFoundException&){}
}

void BufMgr::flushFile(const File* file) 
{
	for (uint32_t i = 0; i < numBufs; ++i){
		if (bufDescTable[i].file == file){
			if (!bufDescTable[i].valid)
				throw BadBufferException(i, bufDescTable[i].dirty, bufDescTable[i].valid, bufDescTable[i].refbit);
			if (bufDescTable[i].pinCnt)
				throw PagePinnedException(file->filename(), bufDescTable[i].pageNo, i);
			if (bufDescTable[i].dirty){
				bufDescTable[i].file->writePage(bufPool[i]);
				bufDescTable[i].dirty = false;
			}
		}
	}
}

void BufMgr::allocPage(File* file, PageId &pageNo, Page*& page) 
{
	FrameId frame;
	allocBuf(frame);
	bufPool[frame] = file->allocatePage();
	pageNo = bufPool[frame].page_number();
	bufDescTable[frame].Set(file, pageNo);
	hashTable->insert(file, pageNo, frame);
	page = bufPool + frame;
}

void BufMgr::disposePage(File* file, const PageId PageNo)
{
	FrameId frame;
	try{
		hashTable->lookup(file, PageNo, frame);
		hashTable->remove(file, PageNo);
		bufDescTable[frame].Clear();  
	}
	catch(HashNotFoundException&){}
	file->deletePage(PageNo);
}

void BufMgr::printSelf(void) 
{
  BufDesc* tmpbuf;
	int validFrames = 0;
  
  for (std::uint32_t i = 0; i < numBufs; i++)
	{
  	tmpbuf = &(bufDescTable[i]);
		std::cout << "FrameNo:" << i << " ";
		tmpbuf->Print();

  	if (tmpbuf->valid == true)
    	validFrames++;
  }

	std::cout << "Total Number of Valid Frames:" << validFrames << "\n";
}

}
