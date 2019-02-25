#include "sing.h"
void singleton::destory()
{
		if(m_porint != NULL)
		{
				delete m_porint;
				m_porint =NULL;
		}
}

singleton *singleton::getsingleton()
	{
		if(m_porint == NULL)
		{
			m_porint  = new singleton();
		}
		return m_porint;
	}

void  singleton::doaction()
   {
		cout<< "singleton action"<<endl;	   
   }

int main()
{
	// singleton *use = singleton::getsington();
	// use->doaction();

//	singleton *si;si->m_porint = NULL;
singleton::getsingleton()->doaction();
singleton::getsingleton()->destory();
	}


