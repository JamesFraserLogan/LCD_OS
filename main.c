#include "local.h"
int main(void)
{
	struct screen *s1=makescreen(2,"s1line1","s1line2");
	struct screen *s2=makescreen(2,"s2line1","s2line2");
	struct screen *s3=makescreen(2,"s3line1","s3line2");
	struct screen *s4=makescreen(2,"s4line1","s4line2");
	return 0;
}
