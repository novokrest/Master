#include <cstdlib>
#include <cstdio>

static void check_attr_constructor(void) __attribute__((constructor));
static void
check_attr_constructor(void)
{
    fprintf(stdout, "%s", "constructor attribute works\n");
}

int main()
{
    fprintf(stdout, "%s", "inside main\n");

    return 0;
}
