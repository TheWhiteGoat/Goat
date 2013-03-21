#include "SqEntity.h"


SqEntity::SqEntity(int index)
{
	m_iIndex = index;
	m_EntityRef = gamehelpers->IndexToReference(index);
}

SqEntity::~SqEntity(void)
{
}

int SqEntity::GetIndex()
{
	return m_iIndex;
}

int SqEntity::GetValveRef()
{
	return m_EntityRef;
}

int SqEntity::ReferenceToIndex(int index)
{
	return gamehelpers->ReferenceToIndex(index);
}

void SqEntity::RegisterInVm(Script *vm)
{
	DefaultVM::Set(vm->GetVM());
	RootTable().Bind(_SC("Entity"),
		Class<SqEntity, SqEntityAlloc<SqEntity>>()
		.Func(_SC("GetIndex"),&SqEntity::GetIndex)
		.Func(_SC("GetValveRef"),&SqEntity::GetValveRef)
		.StaticFunc(_SC("RefToIndex"),&SqEntity::ReferenceToIndex)
		);

	RootTable().Bind(_SC("Vector"), //commented functions got a linking problem 
		Class<Vector>()
		.Var("x",&Vector::x)
		.Var("y",&Vector::y)
		.Var("z",&Vector::z)
		.Func("Init",&Vector::Init)
		.Func("IsValid",&Vector::IsValid)
		.Func("Invalidate",&Vector::Invalidate)
		.Func("Random",&Vector::Random)
		.Func("Zero",&Vector::Zero)
		.Func("Negate",&Vector::Negate)
		.Func("Length",&Vector::Length)
		.Func("LengthSqr",&Vector::LengthSqr)
		.Func("IsZero",&Vector::IsZero)
		.Func("NormalizeInPlace",&Vector::NormalizeInPlace)
		.Func("IsLengthGreaterThan",&Vector::IsLengthGreaterThan)
		.Func("IsLengthLessThan",&Vector::IsLengthLessThan)
		.Func("WithinAABox",&Vector::WithinAABox)
		.Func("DistTo",&Vector::DistTo)
		.Func("DistToSqr",&Vector::DistToSqr)
		.Func("Dot",&Vector::Dot)
		.Func("Length2D",&Vector::Length2D)
		.Func("Length2DSqr",&Vector::Length2DSqr)
		.Func("Cross",&Vector::Cross)
		.Func("Min",&Vector::Min)
		.Func("Max",&Vector::Max)

		//.Func<Vector (Vector::*)(const Vector&) const>("_cmp",&Vector::operator==) //needs a wrapper
#ifndef VECTOR_NO_SLOW_OPERATIONS
		.Func<Vector (Vector::*)(const Vector&) const>("_sub",&Vector::operator-)
		.Func<Vector (Vector::*)(const Vector&) const>("_add",&Vector::operator+)
		.Overload<Vector (Vector::*)(const Vector&) const>("_mul",&Vector::operator*)
		.Overload<Vector (Vector::*)(const float) const>("_mul",&Vector::operator*)
		.Overload<Vector (Vector::*)(const Vector&) const>("_div",&Vector::operator/)
		.Overload<Vector (Vector::*)(const float) const>("_div",&Vector::operator/)
#endif
		);
}