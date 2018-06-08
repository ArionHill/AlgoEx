#include <cstdlib>
#include <cstdio>
#include <set>
using namespace std;

void back_tracking(int *a, int *flags, int t, int size, int sum, int target) {
	if ( t == size) {
		if (sum == target) {		
			printf("RESULT:");
			for (int i = 0; i < size; i ++) {
				printf(" %d", flags[i]);
			}
			printf("\n");
			return;
		}
	}
	else {
		flags[t] = 1;
		sum += a[t];
		if (sum <= target)
		    back_tracking(a, flags, t + 1, size, sum, target);
		flags[t] = 0;
		sum -= a[t];
		back_tracking(a, flags, t + 1, size, sum, target);
	}
}

int main()
{
	int set_size;
	printf("PLEASE INPUT THE SIZE OF SET: ");
	while(scanf("%d", &set_size) != EOF) {	
		printf("PLEASE INPUT THE ITEMS OF SET: ");
		int *a = new int[set_size];
		int *flags = new int[set_size];
		int sum = 0;
		for (int i = 0; i < set_size; ++i) {
			scanf("%d", &a[i]);
			flags[i] = 0;
			sum += a[i];
		}
		if (sum % 2 != 0) {
			printf("NO SUCH A SUBSET!\n");
		}
		else {
		    back_tracking(a, flags, 0, set_size, 0, sum/2);
		}
		printf("\nPLEASE INPUT THE SIZE OF SET: ");
	}

    return 0;
}

