#ifndef __COROUTINE_H__
#define __COROUTINE_H__

#include <ucontext.h>
#include <vector>

#define STACK_SIZE (1024*1024)
class Coroutine;
typedef void (*func)(Coroutine *,void *);
typedef void (*class_func)(void *,Coroutine *,void *);


class Coroutine
{
public:
    Coroutine(func t_f);
    Coroutine(class_func t_cf);
    ~Coroutine();
    void run(void *);
    void run(void *,void *);
    void yield();
    void resume();
    bool dead();
private:
    void saveThisCoroutineStack();
    void __main(void *);
    void __class_func_main(void *,void *);
    static std::vector<char> st_running_stack;
    static ucontext_t st_main;
    std::vector<char> m_stack;
    func m_f;
    class_func m_cf;
    ucontext_t m_uc;
    bool m_dead;
};

#endif