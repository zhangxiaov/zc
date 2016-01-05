#include <stdio.h>

typedef struct
{
    int i;
    int num;
    int state;
} task;

#define crBegin(state) \
switch (state) { case 0:

#define crReturn(state, ret) \
(state) = __LINE__; return (ret); case __LINE__:

#define crEnd() \
}

int cb(task *t)
{
    crBegin(t->state);
    for (;;) {
        t->num = 1;
        for (t->i = 0; t->i < 20; t->i++) {
            crReturn(t->state, t->num);
            t->num += 1;
        }
    }
    crEnd();
    
    return 0;
}

int function(void) {
    static int i, state = 0;
    switch (state) {
        case 0: goto LABEL0;
        case 1: goto LABEL1;
    }
LABEL0: /* start of function */
    for (i = 0; i < 10; i++) {
        state = 1; /* so we will come back to LABEL1 */
        return i;
    LABEL1:; /* resume control straight after the return */
    }
    
    return 0;
}

int main_main()
{
//    task t;
//    int i;
//    
//    t.state = 0;
//    
//    for (i = 0; i < 100; i++) {
//        printf("%d ", cb(&t));
//    }
    
    printf("%d ", function());
    printf("%d ", function());
    printf("%d ", function());
    printf("%d ", function());
    printf("%d ", function());

    
    return 0;
}