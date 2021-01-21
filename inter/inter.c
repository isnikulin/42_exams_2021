#include <unistd.h>

int main(int argc, char *argv[])
{
	int used[255] = {0};
	int i = 2, j = 0;
	if (argc == 3)
	{
		while(i > 0)
		{
			j = 0;
			while(argv[i][j])
			{
				if(i == 2 && !used[(unsigned char)argv[i][j]])
					used[(unsigned char)argv[i][j]] = 1;
				else if (i == 1 && used[(unsigned char)argv[i][j]] == 1)
				{
					write(1, &argv[i][j], 1);
					used[(unsigned char)argv[i][j]] = 2;
				}
				j++;
			}
			i--;
		}
	}
	write(1, "\n", 1);
	return (0);
}
