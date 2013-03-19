#pragma once
#include <sqrat.h>
#include <smsdk_ext.h>

using namespace Sqrat;

template<class C>
class SqEntityAlloc : public NoCopy<C>{
public:
    static SQInteger New(HSQUIRRELVM vm) {
		int index = 0;
		sq_getinteger(vm,-1,&index);
		sq_pop(vm,1);
        C* instance = new C(index);
        sq_setinstanceup(vm, 1, instance);
        sq_setreleasehook(vm, 1, &Delete);
        return 0;
	}
};

class SqEntity
{
protected:
	cell_t m_EntityRef;
	int m_iIndex;
public:
	SqEntity(void);
	SqEntity(int index);
	~SqEntity(void);

	static void RegisterInVm(Script *vm);

	int GetIndex();
	int GetValveRef();

	static int ReferenceToIndex(int index);
};

