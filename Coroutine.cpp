#include "Coroutine.h"

std::vector<char> Coroutine::st_running_stack(STACK_SIZE,0);
ucontext_t Coroutine::st_main;

Coroutine::Coroutine(func t_f):m_f(t_f),m_cf(NULL),m_dead(false)
{
}

Coroutine::Coroutine(class_func t_cf):m_f(NULL),m_cf(t_cf),m_dead(false)
{
}

void Coroutine::__main(void *t_args)
{
    m_dead = false;
    m_f(this,t_args);
    m_dead = true;
}

void Coroutine::__class_func_main(void *t_this,void *t_args)
{
    m_dead = false;
    m_cf(t_this,this,t_args);
    m_dead = true;
}

void Coroutine::run(void *t_args)
{
    getcontext(&m_uc);
    m_uc.uc_stack.ss_sp = &st_running_stack[0];
    m_uc.uc_stack.ss_size = st_running_stack.size();
    m_uc.uc_link = &st_main;
    makecontext(&m_uc,(void (*)(void))&Coroutine::__main,2,this,t_args);
    swapcontext(&st_main,&m_uc);
}


void Coroutine::run(void *t_this,void *t_args)
{
    getcontext(&m_uc);
    m_uc.uc_stack.ss_sp = &st_running_stack[0];
    m_uc.uc_stack.ss_size = st_running_stack.size();
    m_uc.uc_link = &st_main;
    makecontext(&m_uc,(void (*)(void))&Coroutine::__class_func_main,3,this,t_this,t_args);
    swapcontext(&st_main,&m_uc);
}

void Coroutine::yield()
{
    saveThisCoroutineStack();
	swapcontext(&m_uc,&st_main);
}

void Coroutine::resume()
{
    std::copy(m_stack.begin(),m_stack.end(),st_running_stack.end()-m_stack.size());
    swapcontext(&st_main,&m_uc);
}


void Coroutine::saveThisCoroutineStack()
{
    char top_in_st_running_stack = 0;
    int current_stack_size = &st_running_stack[0]+STACK_SIZE-&top_in_st_running_stack;
    m_stack.resize(current_stack_size);//这一步待优化
    std::copy(st_running_stack.end()-current_stack_size,st_running_stack.end(),m_stack.begin());
}

bool Coroutine::dead()
{
    return m_dead;
}


Coroutine::~Coroutine()
{

}