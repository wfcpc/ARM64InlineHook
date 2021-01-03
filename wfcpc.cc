// wfcpc.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
// g++ build_code.cc codes_arm64.cc inlinehook_arm64.cc registers_arm64.cc wfcpc.cc fix.cc -o out

#include <iostream>
#include "inlinehook_arm64.h"

#define HOOK_DEF(ret, func, ...) \
  ret (*orig_##func)(__VA_ARGS__); \
  ret new_##func(__VA_ARGS__)



HOOK_DEF(float, hook_func1,int* base1,int*  base2,int*  base3) {
    using namespace std;
    cout<<"hook func1 running~"<<endl;
    float handle = orig_hook_func1(base1,base2,base3);
    
    cout<<"got orig ret value = "<<handle<<endl;
    cout<<"arg1="<<base1<<endl;
    cout<<"arg2="<<base2<<endl;
    cout<<"arg3="<<base3<<endl;
    handle=90;
    return handle;
}
float func1( int*  base1, int*  base2, int*  base3)
{
    float a=1.1;
    float b=2.2;
    float c=3.3;
    using namespace std;
    cout<<"func1_orig running"<<endl;
    float d=a+b+c;
    return d;
}

HOOK_DEF(float, hook_func2,int* base1) {
    float handle = orig_hook_func2(base1);
    using namespace std;
    cout<<"got orig ret value = "<<handle<<endl;
    cout<<"arg1="<<base1<<endl;
    handle=666666;
    return handle;
}

float func2(int *base1)
{
    float a=11.1;
    float b=22.2;
    float c=33.3;
    using namespace std;
    cout<<"func2 orig running"<<endl;
    float d=a+b+c;
    return d;
}
int main()
{
    using namespace std;
    Inlinehook(  (void*)func1, (void *) new_hook_func1,
                        (void **) &orig_hook_func1);
    sleep(1.5);
    int a=12,b=23,c=34;
    int *aa;
    int *bb;
    int *cc;
    aa=&a;
    bb=&b;
    cc=&c;
    cout<<"------in main-------"<<endl;
    cout<<"aa addr="<<aa<<endl;
    cout<<"bb addr="<<bb<<endl;
    cout<<"cc addr="<<cc<<endl;
    float ret=func1(aa,bb,cc);
    cout<<"finally ret="<<ret<<endl;

    int d=56;
    int *dd;
    dd=&d;
    Inlinehook((void *)func2,(void *)new_hook_func2,(void **)&orig_hook_func2);
    sleep(1.5);
    cout<<"------in main-------"<<endl;
    cout<<"dd addr="<<dd<<endl;
    float ret2=func2(dd);
    cout<<"finally ret="<<ret2<<endl;
    return 0;
	
}

