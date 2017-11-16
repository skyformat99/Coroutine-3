#include "Coroutine.h"
#include <stdio.h>

void fo(Coroutine *c,void *t)
{
	int data = *(int *)t;
	for(int i=0;i<5;++i)
	{
		printf("data: %d\n",data+i);
		c->yield();
	}
}

class FooC
{
public:
	FooC(int t_id):id(t_id){}
	void foo(Coroutine *c,void *t)
	{
		int data = *(int *)t;
		for(int i=0;i<5;++i)
		{
			printf("id: %d data: %d\n",id,data+i);
			c->yield();
		}
	}
private:
	int id;
};

void test1()
{
	int a=1,b=10;
	Coroutine c(fo);
	Coroutine c2(fo);
	c.run(&a);
	c2.run(&b);
	while(!c.dead() || !c2.dead())
	{
		if(!c.dead())
		c.resume();
		if(!c2.dead())
		c2.resume();
	}
}

void test2()
{
	int a=2,b=11;
	FooC fc1(1);
	FooC fc2(2);

	Coroutine c1((class_func)&FooC::foo);
	Coroutine c2((class_func)&FooC::foo);
	c1.run(&fc1,&a);
	c2.run(&fc2,&b);

	while(!c1.dead() || !c2.dead())
	{
		if(!c1.dead())
		c1.resume();
		if(!c2.dead())
		c2.resume();
	}
}

int main() {
	printf("main start.\n");
	test1();
	test2();
    printf("main end.\n");
	return 0;
}
