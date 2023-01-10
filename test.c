#include <assert.h>
#include <string.h>
#include <stdbool.h>

bool str_eq(const char* a, const char* b)
{
	return strcmp(a, b) == 0;
}

char* reverse_str(char* const s, const unsigned len)
{
	char* l = s;
	char* r = s + len - 1;

	while (l < r)
	{
		char tmp = *l;
		*l = *r;
		*r = tmp;

		l++;
		r--;
	}

	return s;
}

int main(void)
{
	struct
	{
		char input[32];
		unsigned len;
		const char* expect;
	} tests[] = {
		{ "", 0, "" },
		{ "a", 1, "a" },
		{ "ab", 2, "ba" },
		{ "abc", 3, "cba" },
		{ "abcd", 4, "dcba" },
		{ "abcd", 3, "cbad" },
	};

	for (unsigned i = 0; i < sizeof tests / sizeof *tests; ++i)
	{
		assert(str_eq(reverse_str(tests[i].input, tests[i].len), tests[i].expect));
	}
}