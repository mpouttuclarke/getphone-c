/*
 ============================================================================
 Name        : urlrip.c
 Author      : Matt Pouttu-Clarke
 Version     :
 Copyright   : λ夫 LamdaFu, LLC
 Description : Google RE2 example
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <cre2.h>

int main(void) {
	char search[32768];
	const char* pattern =
			"(?:(?:\\+?1\\s*(?:[.-]\\s*)?)?(?:\\(\\s*([2-9]1[02-9]|[2-9][02-8]1|[2-9][02-8][02-9])\\s*\\)|([2-9]1[02-9]|[2-9][02-8]1|[2-9][02-8][02-9]))\\s*(?:[.-]\\s*)?)?([2-9]1[02-9]|[2-9][02-9]1|[2-9][02-9]{2})\\s*(?:[.-]\\s*)?([0-9]{4})(?:\\s*(?:#|x\\.?|ext\\.?|extension)\\s*(\\d+))?";
	cre2_regexp_t* rex;
	cre2_options_t* opt;

	opt = cre2_opt_new();
	if (opt) {
		cre2_opt_set_log_errors(opt, 0);
		rex = cre2_new(pattern, strlen(pattern), opt);
		if (rex) {
			if (!cre2_error_code(rex)) {
				int match_size = cre2_num_capturing_groups(rex) + 1;
				cre2_string_t match[match_size];
				while (!feof(stdin)) {
					fgets(search, 32768, stdin);
					char *newline = strpbrk(search, "\r\n");
					if (newline != NULL) {
						*newline = '\0';
					}
					size_t strlenSearch = strlen(search);
					if (cre2_match(rex, search, strlenSearch, 0, strlenSearch,
							CRE2_UNANCHORED, match, match_size) == 1) {
						printf("%s\t", search);
						int actual = 0;
						for (int x = 1; x < match_size; x++) {
							if (match[x].length < 1) {
								continue;
							}
							if (actual++ > 0) {
								printf("-");
							}
							for (int y = 0; y < match[x].length; y++) {
								printf("%c", match[x].data[y]);
							}
						}
						printf("\n");
					}
				}
				return EXIT_SUCCESS;
			} else
				puts("Invalid regex");
			cre2_delete(rex);
		} else {
			puts("Could not allocate memory for regex");
		}
		cre2_opt_delete(opt);
	} else {
		puts("Could not allocate memory for regex options");
	}
	return EXIT_FAILURE;
}
