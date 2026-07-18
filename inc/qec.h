// Copyright (C) 2007 Joe Linhoff, All Rights Reserved
// qec.h
// this is the API to the engine
#ifndef QEC_H
#define QEC_H

#ifndef QE_H
#include "qe.h"
#endif // QE_H

/// Engine 'super-object'.  Provides engine-consistent new & delete.
/// Must inherit publicly so the new & delete are usable.  All 'new' operations
/// zero allocated memory.
class QE_API qe {
public:
   void* operator new(size_t size); ///< alloc & zero memory for object
   void* operator new(size_t size,void *adr); ///< placement new (see note)
   void* operator new[](size_t size); ///< alloc & zero memory
   void operator delete(void *p); ///< free memory for object
   void operator delete[](void *p); ///< free memory
}; // class qe

/// Note: The placement new operator doesn't allocate memory, you supply the 
/// address of the valid memory to use for that object.  The memory is not 
/// zerod.  Do not delete these objects. These objects are useful when you 
/// have a block of memory you would like to use or reuse for an object or 
/// for memory mapped IO.  The constructor will be called.

/// Special update base class.
/// Do not 'delete' objects derived from this class -- use objRemove()
/// note: this was renamed from qeUpdateBase 1/15/2008
// JFL 24 Nov 07
// JFL 15 Jan 08
class QE_API qeUpdateBase : public qe {
public:
   qeObj *_oShared; ///< the engine object
private:
   virtual int control(void) {return -1;} ///< called during shared control
   virtual int update(void) {return -1;} ///< called during shared update
   virtual int draw(void) {return -1;} ///< called during shared draw
   virtual void final(void) {delete this;} ///< called before object destruction
public:
   qeUpdateBase(chr *name,int8 sort,int appId); ///< sort bigger later, appId unique
   int objRemove(void); ///< remove, call from update() or draw()
   int objOp(uns op,...); ///< additional operations
}; // class qeUpdateBase

#endif // ndef QEC_H
