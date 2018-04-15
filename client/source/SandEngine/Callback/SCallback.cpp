#include "SandEnginePCH.h"
#include "SCallback.h"

void SCallback::operator +=( SCallbackHandle& handle )
{
	handles.PushBack( &handle );
}

void SCallback::operator-=( SCallbackHandle& handle )
{
	handles.RemoveValue( &handle );
}

void SCallback::Trigger( SCallbackUserData* data )
{
	SCallbackUserData userData;
	if( !data )	data = &userData;
	data->pSender = this;
	
	for( int i = 0 , count = handles.GetSize(); i < count; i++ )
	{
		handles[i]->OnHandle( *data );
	}
}