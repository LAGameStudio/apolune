#include "MemoryPool.h"

#include "ListItem.h"
#include "LinkedList.h"
#include "Recyclable.h"

MemoryPool::MemoryPool(void) {
 recycling.Instantiate();
 recycling->resize(MP_GRANULARITY,MP_GRANULARITY);
}


MemoryPool::~MemoryPool(void) {
}

void MemoryPool::Recycle( ListItem *L ) {

}


MemoryManager::MemoryManager() {
}

MemoryManager::~MemoryManager() {
}

MemoryManager mm;
Zp<MemoryManager> memory=&mm;