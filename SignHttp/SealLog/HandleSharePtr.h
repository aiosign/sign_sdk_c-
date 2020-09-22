#pragma 
#ifdef WIN32
#include <Windows.h>
class HandleWarper
{
public:
	HandleWarper(HANDLE _handle):m_uRef(0),handle(_handle)
	{
	}

	virtual ~HandleWarper()
	{
		if(handle != NULL && handle != INVALID_HANDLE_VALUE) CloseHandle(handle);
	}

public:
	virtual void IncRef(){InterlockedIncrement(&m_uRef);}
	virtual void  DecRef(){ if(InterlockedDecrement(&m_uRef) == 0) delete this;}

	volatile long m_uRef;
	HANDLE handle;
};

class HandleSharePtr
{
public:
	HandleSharePtr(HANDLE _handle):m_ptr(new HandleWarper(_handle))
	{
		if(m_ptr != 0) m_ptr->IncRef();
	}

	HandleSharePtr(const HandleSharePtr& rhs):m_ptr(rhs.m_ptr)
	{
		if(m_ptr != 0) m_ptr->IncRef();
	}

	HandleSharePtr& operator = (const HandleSharePtr& rhs)
	{
		if(m_ptr != rhs.m_ptr)
		{
			//ԭָ��decrease;
			if(m_ptr != 0) m_ptr->DecRef();

			m_ptr = rhs.m_ptr;
			//��ǰָ����������
			if(m_ptr != 0) m_ptr->IncRef();
		}
		return *this;
	}
	//���裬С��ʹ�ã�������ܱ���
	HANDLE&  operator *()
	{
		return m_ptr->handle;
	}
	//���裬С��ʹ�ã�������ܱ���
	const HANDLE&  operator *() const
	{
		return m_ptr->handle;
	}
	//���裬С��ʹ�ã�������ܱ���
	HANDLE operator ->()
	{
		return m_ptr->handle;
	}
	//���裬С��ʹ�ã�������ܱ���
	const HANDLE operator ->() const
	{
		return m_ptr->handle;
	}
	//���裬С��ʹ�ã�������ܱ���
	HANDLE Get()
	{
		return m_ptr->handle;
	}
	//���裬С��ʹ�ã�������ܱ���
	const HANDLE Get() const
	{
		return m_ptr->handle;
	}

	void IncRef()
	{
		if(m_ptr != 0) m_ptr->IncRef();
	}

	void DecRef()
	{
		if(m_ptr != 0) m_ptr->DecRef();
	}

	~HandleSharePtr()
	{
		if(m_ptr != 0) m_ptr->DecRef();
	}
public:
	HandleWarper* m_ptr;
	explicit HandleSharePtr():m_ptr(new HandleWarper(0)){if(m_ptr != 0) m_ptr->IncRef();}
};
#endif