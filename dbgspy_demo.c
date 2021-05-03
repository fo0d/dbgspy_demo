/* ********************************************************* *
* By Yuriy Y. Yermilov aka (binaryONE) cyclone.yyy@gmail.com
*
* website: code.computronium.io
*
* THIS SOFTWARE IS PROVIDED BY THE OWNER ``AS IS''
* AND ANY EXPRESS OR IMPLIED WARRANTIES,
* INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR
* PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE OWNER
* PROJECT OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
* INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT LIMITED TO,
* PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
* HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER
* IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY
* WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
* ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
* ********************************************************** */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../y3libgen/list.h"
#include "../y3libgen/str.h"
#include "../y3libgen/dbgspy.h"
#include "include/compile.h"

void y3_dbgspy(struct y3_dbg *T);

/* ----------------------- TEST DRIVER START ------------------------------- */
/* -----------------------                   ------------------------------- */

/*
    This is a global definition of dbgspy.
    It is declared as an extern in dbgspy.h in libgen.
 */
struct y3_dbg *dbgspy;

/*
   The code bellow is part of the main driver, which is
   there to show how to use the dbgspy tool.
 */

struct my_data_type {
    int n1, n2;
    char *some_string;
    char m;
};

char s[] = "PAZ!";

int g1, g2, g3;

int func_to_spy_on(struct my_data_type *data, char op);

void func_to_spy_on2(int x);

/*
   This function just prints out the sum,
   product, difference and division remainder of 
   two given integers; and returns the result.
 */

int
func_to_spy_on(struct my_data_type *data, char op) {
    int error, result = 0;

    /*
        Add targets to be known to dbgspy.

        Since the id of each target needs to be unique,
        have dbgspy give out the available values
        that can be used.
     */

    if (op == '+')
        result = data->n1 + data->n2;
    else if (op == '*')
        result = data->n1 * data->n2;
    else if (op == '-')
        result = data->n1 - data->n2;
    else if (op == '%')
        result = data->n1 % data->n2;

    y3_dbgspy_target(
            dbgspy,
            "func_to_spy_on",
            &error,
            7,
            "my_data_type:data->n1", &(data->n1), "int", Y3_DBGSPY_PTR, sizeof(int),
            "my_data_type:data->n2", data->n2, "int", Y3_DBGSPY_VAL,
            "op", op, "char", Y3_DBGSPY_VAL,
            "m", &data->m, "char", Y3_DBGSPY_PTR, sizeof(char),
            "my_data_type:data->some_string", data->some_string, "string", Y3_DBGSPY_PTR, strlen(data->some_string),
            "global s", s, "string", Y3_DBGSPY_PTR, strlen(s),
            "my_data_type:data", data, "my_data_type", Y3_DBGSPY_PTR, sizeof(struct my_data_type)
    );
    return result;
}

void
func_to_spy_on2(int x) {
    static int y, z, m;
    int error;

    z = ++y * 2;

    printf("x=%d y=%d z=%d\n", x, y, z);

    y3_dbgspy_target(
            dbgspy,
            "func_to_spy_on2",
            &error,
            4,
            "x", x, "int", Y3_DBGSPY_VAL,
            "y", y, "int", Y3_DBGSPY_VAL,
            "z", z, "int", Y3_DBGSPY_VAL,
            "m", m, "int", Y3_DBGSPY_VAL
    );
}

void run_dbgspy_default_test() {
    static int error;
    int a;
//    int name_len;
    struct my_data_type *data;
    struct y3_dbg_element *dbgv;
//    struct y3_dbg_target *t;
    struct y3_dbg_context *c;

    //char _name[Y3_DBGSPY_MAX_NAME_LEN];

    data = (struct my_data_type *)malloc(sizeof(struct my_data_type));

    data->some_string = (char *)malloc(5);
    memmove(data->some_string, "ZAP!", 5);
    data->m = '$';

    /* ----------------------------------------- *
                 initialize DBGSpy
     * ----------------------------------------- */

    dbgspy = y3_dbgspy_init("dbgspy.out", &error);

    if (error != Y3_DBGSPY_SUCCESS) {
        printf("Cannot initialized DBGSPY, error id = %d...\n", error);
        return;
    }


    /* _________________________________________ */

    data->n1 = 1, data->n2 = 2;
    printf("a + b = %d\n", func_to_spy_on(data, '+'));
    data->n1 = 2, data->n2 = 4;
    printf("a - b = %d\n", func_to_spy_on(data, '-'));
    data->n1 = 9, data->n2 = 7;
    printf("a * b = %d\n", func_to_spy_on(data, '*'));
    data->n1 = 17, data->n2 = 19;
    printf("a mod b = %d\n", func_to_spy_on(data, '%'));

    func_to_spy_on2(17);
    func_to_spy_on2(19);
    func_to_spy_on2(18);
    func_to_spy_on2(19);
    func_to_spy_on2(20);
    func_to_spy_on2(21);
    func_to_spy_on2(22);

    /*
      Test contexts.
     */

    for (a = 0; a < 20; a++) {
        g1 = a + 1;
        g2 = a + 2;
        g3 = a + 3;

        c = y3_dbgspy_new_context(dbgspy, NULL, &error);

        if (error != Y3_DBGSPY_SUCCESS) {
            printf("Cannot create new context, error id = %d...\n", error);
            continue;
        }

        {
            y3_dbgspy_new_context_elements(dbgspy,
                                           c->name,
                                           &error,
                                           3,
                                           "g1", g1, "int", Y3_DBGSPY_VAL,
                                           "g2", g2, "int", Y3_DBGSPY_VAL,
                                           "g3", g3, "int", Y3_DBGSPY_VAL
            );

            if (error != Y3_DBGSPY_SUCCESS) {
                printf("Cannot create new context variables, error id = %d...\n", error);
                continue;
            }

            printf("added context [%s]\n", c->name);
        }
    }

    printf("my_data_type:data->some_string %s = %s\n", data->some_string, (char *) y3_dbgspy_get_target_element_val(
            dbgspy, "func_to_spy_on", "my_data_type:data->some_string", &error));

    data = y3_dbgspy_get_target_element_val(dbgspy, "func_to_spy_on", "my_data_type:data", &error);

    printf("setting my_data_type:data->n2 %d to %d\n", data->n2, 100);
    data->n2 = 100;
    y3_dbgspy_set_target_element_val(dbgspy, "func_to_spy_on", "my_data_type:data->n2", (int *) 100, Y3_DBGSPY_VAL,
                                     &error);
    printf("my_data_type:data->n2 %d = %d\n", data->n2, (int)y3_dbgspy_get_target_element_val(dbgspy, "func_to_spy_on", "my_data_type:data->n2", &error));

    printf("setting (PTR value)my_data_type:data->n1 %d to %d\n", data->n1, 101);
    data->n1 = 101;

    //
    // Remember data->n1 is being tracked via pointer reference, so it needs to be dereferenced
    // as a pointer of its type (in this case, int).
    //

    printf("my_data_type:data->n1 %d = %d\n", data->n1, *(int *) y3_dbgspy_get_target_element_val(dbgspy,
                                                                                                  "func_to_spy_on",
                                                                                                  "my_data_type:data->n1",
                                                                                                  &error));

    //
    // Link to a specific target and print its name...
    //

    printf("getting target instance of [%s]\n", y3_dbgspy_get_target(dbgspy, "func_to_spy_on", &error)->name);

    //
    // Link to a variable instance
    //

    dbgv = y3_dbgspy_get_target_element_inst(dbgspy, "func_to_spy_on", "my_data_type:data", &error);

    if (dbgv == NULL)
        printf("**error could not get var instance instance: var %s does not exist in target %s\n", "my_data_type:data",
               "func_to_spy_on");
    else
        printf("var%s %s exists in %s\n", dbgv->is_ptr ? "(ptr)" : "", dbgv->name, "func_to_spy_on");

    //
    // Link to a specific context and print its name...
    //

    c = y3_dbgspy_get_context(dbgspy, "19", &error);

    printf("getting context instance of [%s]\n", c->name);

    //
    // Let's see if a variable exists in a context
    // if so, get the instance of it.
    //

    dbgv = y3_dbgspy_get_context_element_inst(dbgspy, "19", "g3", &error);

    if (dbgv == NULL)
        printf("**error could not get var instance instance: var %s does not exist in context %s\n", "g3", c->name);
    else
        printf("var%s %s exists in %s\n", dbgv->is_ptr ? "(ptr)" : "", dbgv->name, "func_to_spy_on");

    /*
     * Print all the stuff out, to double check everything's gone on properly.
     */

    dbgspy->flags.show_contexts = 1;
    dbgspy->flags.show_allcontexts = 1;
    dbgspy->flags.show_all = 1;
    dbgspy->flags.show_allstates = 1;

    y3_dbgspy(dbgspy);

//    y3_echo("press enter key to exit: ");
//    while(!getc(stdin))
//        ;
}

void
run_dbgspy_fib_test() {

    // ---------------------------------------------------------
    //        initialize DBGSpy...
    // ---------------------------------------------------------

    static int error;
    dbgspy = y3_dbgspy_init("dbgspy.out", &error);

    struct y3_dbg_target *my_targets[6];

    if (error != Y3_DBGSPY_SUCCESS) {
        printf("Cannot initialized DBGSPY, error id = %d...\n", error);
        return;
    }

    int n = 13, first = 0, second = 1, next = 0, c = 0;
    char v[100];

    // ---------------------------------------------------------
    //        Setup the target and the contents to track...
    // ---------------------------------------------------------
    my_targets[0] = y3_dbgspy_target( dbgspy, "Fiboncacci (itterative solution)", &error, 0, "", "", "", 0); // just a node
    my_targets[1] = y3_dbgspy_target( dbgspy, "c = 0; n = 13", &error, 0, "", "", "", 0); // just a node

    for ( c = 0 ; c < n ; c++ ) {
        snprintf(v, 100, "%d < %d", c, n);
        my_targets[2] = y3_dbgspy_target( dbgspy, "c < n", &error, 1, "c < n", v, "boolean", Y3_DBGSPY_PTR, strlen(v));

        if ( c <= 1 ) {
            next = c;
            snprintf(v, 100, "%d < 1", c);
            my_targets[3] = y3_dbgspy_target( dbgspy, "c <= 1", &error, 2, "next", next, "int", Y3_DBGSPY_VAL, "c <= 1", v, "boolean", Y3_DBGSPY_PTR, strlen(v));
        } else {
            next = first + second;
            first = second;
            second = next;

            snprintf(v, 100, "%d > 1", c);
            my_targets[4] = y3_dbgspy_target( dbgspy, "c > 1", &error, 4, "first", first, "int", Y3_DBGSPY_VAL, "c > 1", v, "boolean", Y3_DBGSPY_PTR, strlen(v), "second", second, "int", Y3_DBGSPY_VAL, "next", next, "int", Y3_DBGSPY_VAL);
        }

        my_targets[5] = y3_dbgspy_target( dbgspy, "Fibonacci number", &error, 1, "Fibonacci number", next, "int", Y3_DBGSPY_VAL);
    }

  dbgspy->flags.show_contexts = 1;
  dbgspy->flags.show_allcontexts = 1;
  dbgspy->flags.show_all = 1;
  dbgspy->flags.show_allstates = 1;

  y3_dbgspy(dbgspy);
}

int
main(void) {
    run_dbgspy_default_test();
    run_dbgspy_fib_test();
    return 0;
}

void
y3_dbgspy(struct y3_dbg *T) {
    int j;
    struct y3_dbg_actions action = T->flags;
    struct y3_dbg_xstate *state;
    struct y3_dbg_context *context;
    struct y3_dbg_target *target;
    struct y3_dbg_element *var;
    struct y3_list *link_target, *link_state, *link_var;

    if (action.show_all) {
        for (link_target = T->target; link_target != NULL; link_target = link_target->next) {
            target = (DBGT) link_target->data;

            printf("target[%d]: %s\n", target->id, target->name);

            if (action.show_allstates) {
                j = 0;
                while ((link_state = y3_list_link_id(target->xstate, &j))) {
										j++;
                    state = (DBGS) link_state->data;

                    printf("execution state[%d]\n", state->id);
                    for (link_var = state->element; link_var != NULL; link_var = link_var->next) {
                        var = (DBGV) link_var->data;

                        if (strncmp(var->type.name, "int", 3) == 0) {
                          if(var->is_ptr)
                            printf("%s %s %d\n", var->name, var->type.name, *(int *)var->val);
                          else
                            printf("%s %s %d\n", var->name, var->type.name, (int)var->val);
                        } else if (strncmp(var->type.name, "char", 4) == 0) {
                          if(var->is_ptr)
                            printf("%s %s %d\n", var->name, var->type.name, *(char *)var->val);
                          else
                            printf("%s %s %s\n", var->name, var->type.name, (char *)var->val);
                        } else if (strncmp(var->type.name, "string", 6) == 0)
                          printf("%s %s %s\n", var->name, var->type.name, (char *)var->val);
                        else {
                          //unknown
                        }
                    }
                }
            }
        }
    }

    if (action.show_contexts) {
        if (action.show_allcontexts) {
            for (link_target = T->context; link_target != NULL; link_target = link_target->next) {
                context = (DBGC) link_target->data;
                printf("context: [%d]\n", context->id);
                for (link_var = context->element; link_var != NULL; link_var = link_var->next) {
                    var = (DBGV) link_var->data;
                    if (strncmp(var->type.name, "int", 3) == 0) {
                        if (var->is_ptr)
                          printf("%s %s %d\n", var->name, var->type.name, *(int *)var->val);
                        else
                          printf("%s %s %d\n", var->name, var->type.name, (int)var->val);
                    }
                    else if (strncmp(var->type.name, "char", 4) == 0) {
                      if (var->is_ptr)
                        printf("%s %s %d\n", var->name, var->type.name, *(char *)var->val);
                      else
                        printf("%s %s %d\n", var->name, var->type.name, (char)var->val);
                    } else if (strncmp(var->type.name, "string", 6) == 0) {
                      printf("%s %s %s\n", var->name, var->type.name, (char *)var->val);
                    }
                    else {
                      //unknown
                    }
                }
            }
        }
    }
}

/* ----------------------- TEST DRIVER END ------------------------------ */
