#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <time.h>
#include <locale.h>
#define PREFIX 0 
#define STRING 1

int dec = 0, cnt = 256;

struct Data
{
	char *string;
	char *prefix;
	int prefix_code;
	int code;
};
typedef struct dictionary
{
	struct Data data;
	struct dictionary *next;
	struct dictionary *prev;
}dcr;

dcr *head = NULL;
dcr *tail = NULL;


void printf_d()
{
	dcr *tmp = head;
	while (tmp)
	{
		printf("PREFIX: %s| STRING: %s| PR_CODE = %ld| CODE = %ld\n", tmp->data.prefix, tmp->data.string, tmp->data.prefix_code, tmp->data.code);
		tmp = tmp->next;
	}
}
dcr* search_in_dictionary(char *string)
{
	dcr *start = head->next;
	dcr *end = tail;
	
	if (start == NULL) return NULL;

	while (1)
	{
		if (strcmp(string, start->data.string) == 0) return start;	
		if ((end != head) && (strcmp(string, end->data.string) == 0)) return end;
	
		if (start == NULL || end == head->next) return NULL;

		start = start->next;
		end = end->prev;
	}
	return NULL;
}
dcr* search_code(int code)
{
	dcr *start = head->next;
	dcr *end = tail;

	if (start == NULL) return NULL;
	
	while (1)
	{

	   if (start->data.code == code) return start;
	   if ((end != head) && (end->data.code == code)) return end;

		if (start == NULL || end == head->next) return NULL;

		start = start->next;
		end = end->prev;
	}
	return NULL;
}

void push_dcr(char *string, char *prefix)
{
	dcr *temp = (dcr*)malloc(sizeof(dcr));
	dcr *search_ptr = NULL;
	temp->next = NULL;

	if (head == NULL) temp->data.code = -1;
	else if (head->next == NULL) temp->data.code = 256;
	else if (dec) temp->data.code = cnt;
	else if (dec == 0) temp->data.code = tail->data.code + 1;
	
	if ((prefix != NULL) && (strlen(prefix) == 1)) temp->data.prefix_code = prefix[0];
	else if ((prefix != NULL)&&(dec==0))
	{
		search_ptr = search_in_dictionary(prefix);
		temp->data.prefix_code = search_ptr->data.code;
	}
	else temp->data.prefix_code = temp->data.code;

	if (prefix)
	{
		temp->data.prefix = (char*)malloc(sizeof(char)*(strlen(prefix) + 1));
		memcpy(temp->data.prefix, prefix, sizeof(char)*(strlen(prefix) + 1));
	}
	else temp->data.prefix = NULL;

	if (string)
	{
		temp->data.string = (char*)malloc(sizeof(char)*(strlen(string) + 1));
		memcpy(temp->data.string, string, sizeof(char)*(strlen(string) + 1));
	}
	else temp->data.string = NULL;

	if (head != NULL)
	{
		temp->prev = tail;
		tail->next = temp;
		tail = temp;
	}
	else
	{
		temp->prev = NULL;
		head = tail = temp;
	}
}
void destruct_dictionary()
{
	if (head == NULL) return;

	while (head)
	{
		tail = head->next;

		free(head->data.string);
		head->data.string = NULL;

		free(head->data.prefix);
		head->data.prefix = NULL;

		free(head);
		head = NULL;
		head = tail;
	}
}

char* push_string(char *string, int symbol, int *count_bytes)
{
	string = (char*)realloc(string, sizeof(char)*((*count_bytes) + 2));
	string[(*count_bytes)++] = symbol;
	string[(*count_bytes)] = '\0';
	return string;
}
char* my_strcat(char *dst, char *source)
{
	int first_length = strlen(dst);
	int second_length = strlen(source);
	int i = 0, j = 0;
	char *result = NULL;
		
	result = (char*)realloc(result, sizeof(char)*(first_length + second_length + 1));
	while (i < (first_length + second_length))
	{
		if (i > first_length - 1)	result[i++] = source[j++];	
		else result[i] = dst[i++];
	}
	result[i] = '\0';
	return result;
}
int get_code(FILE *input)
{
	int a;
	fread((int*)&a, sizeof(int), 1, input);
	return a;
}
long get_file_size(char *filename)
{
	FILE *input = fopen(filename, "rb");
	long size;

	fseek(input, 0, SEEK_END);
	size = ftell(input);
	rewind(input);

	return (size / sizeof(int));
}


void Encrypt(char *input_string, char *output_string)
{
	FILE *input, *output;
	dcr *search_ptr = NULL;
	char *current_string = NULL;
	char *prefix = NULL;
	int c = 0, i = 0, j = 0;
	dec = 0;
	input = fopen(input_string, "r");
	output = fopen(output_string, "wb");
	prefix = push_string(prefix, (c=fgetc(input)), &j);
	push_dcr(NULL, prefix);

	while (1)
	{
		current_string = push_string(current_string, (c=fgetc(input)), &i);

		if ((search_ptr = search_in_dictionary(my_strcat(prefix, current_string))) != NULL)
		{
			prefix = my_strcat(prefix, current_string);
			free(current_string);
			i = 0;
			current_string = NULL;
		}
		else
		{
            push_dcr(my_strcat(prefix, current_string), prefix);
			fwrite((int*)&tail->data.prefix_code, 1, sizeof(int), output);
			free(prefix);
			j = 0;
			prefix = NULL;

			prefix = (char*)realloc(prefix, sizeof(current_string)*(strlen(current_string) + 1));
			memcpy(prefix, current_string, strlen(current_string) + 1);

			free(current_string);
			i = 0;
			current_string = NULL;
		}

		if (c == EOF) break;
	}

	free(prefix);
	free(current_string);
	prefix = NULL;
	current_string = NULL;

	fclose(output);
	fclose(input);
}
void Decrypt(char *input_string, char *output_string)
{
	FILE *input, *output;
	dcr* search_ptr = NULL;
	char *prefix = NULL;
	char *postfix = NULL;
	char *string = NULL;
	char *current_string = NULL;

	int cure_code = 0, i = 0, j = 0, cat = 0;
	int postfix_code = 0;
	int prefix_code = 0;
	int not_search = 0;
	int symbol = 0;

	long cnt_bytes=0;
	long file_size = get_file_size(input_string);
	dec = 1;
	input = fopen(input_string, "rb");
	output = fopen(output_string, "w");

	cure_code = get_code(input);
	fprintf(output, "%c", cure_code);

	prefix = push_string(prefix, cure_code, &j);
	push_dcr(NULL, prefix);
	prefix_code = cure_code;

	while (cnt_bytes < file_size - 1)
	{
		cure_code = get_code(input);
		cnt_bytes++;

		if ((search_ptr = search_code(cure_code)) != NULL)
		{
			free(postfix);
			postfix = NULL;
			current_string = (char*)malloc(sizeof(char)*(strlen(search_ptr->data.string) + 1));
		    memcpy(current_string, search_ptr->data.string, sizeof(char)*(strlen(search_ptr->data.string) + 1));


			postfix = (char*)malloc(sizeof(char) * 2);
			postfix[0] = current_string[0];
			postfix[1] = '\0';
		}
		else if (cure_code >= 0 && cure_code <= 255)
		{
			free(postfix);
			postfix = NULL;
			current_string = push_string(current_string, cure_code, &i);

			postfix = (char*)malloc(sizeof(char)*2);
			postfix[0] = current_string[0];
			postfix[1] = '\0';
			not_search = 1;
		}
		else
		{
			not_search = 1;
			cat = 1;
			current_string = my_strcat(prefix, postfix);
		}

		fprintf(output, "%s", current_string);

			if (cat)
			{
				push_dcr(current_string, prefix);
				cat = 0;
			}
			else
			{
				string = (char*)realloc(string, sizeof(char)*(strlen(prefix) + 2));
				memcpy(string, prefix, sizeof(char)*(strlen(prefix) + 1));
				string[strlen(prefix)] = current_string[0];
				string[strlen(prefix) + 1] = '\0';

				push_dcr(string, prefix);
			}
			not_search = 0;

		 cnt++;

		free(prefix);
		prefix = NULL;
		j = 0;

		if (cure_code >= 0 && cure_code <= 255) prefix = push_string(prefix, cure_code, &j);
		else
		{
			search_ptr = search_code(cure_code);
			prefix = (char*)malloc(sizeof(char)*(strlen(search_ptr->data.string) + 1));
			memcpy(prefix, search_ptr->data.string, sizeof(char)*(strlen(search_ptr->data.string) + 1));
		}
		
		free(string);
		string = NULL;

		free(current_string);
		current_string = NULL;
		i = 0;

	}
	free(current_string);
	current_string = NULL;

	free(prefix);
	prefix = NULL;

	free(string);
	string = NULL;

	free(postfix);
	postfix = NULL;

	fclose(input);
	fclose(output);
}

int main()
{
	setlocale(LC_ALL, "RUS");
	char EorD = 0;
	time_t start_clock, end_clock;
	char input_string[500];
	char encr_string[1000];
	char decr_string[1000];
	double compression = 0;
	setlocale(LC_ALL, "RUS");

	printf("Encrypt/Decrypt (E/D): ");

	scanf("%c", &EorD);

	printf("Укажите путь к входному файлу: ");
	scanf("%s", input_string);

	if (EorD == 'E')
	{
		printf("Укажите путь к зашифрованному файлу: ");
		scanf("%s", encr_string);

		start_clock = clock();
		Encrypt(input_string, encr_string);
		end_clock = clock();

		compression = (double)get_file_size(encr_string) / (double)get_file_size(input_string);
		printf("\nВремя шифрования: %0.6f sec\n", ((double)end_clock - (double)start_clock) / CLOCKS_PER_SEC);
		printf("Коэффициент сжатия: %0.4f\n", compression);
	}
	else if (EorD == 'D')
	{
		printf("Укажите путь к расшифрованному файлу: ");
		scanf("%s", decr_string);

		start_clock = clock();
		Decrypt(input_string, decr_string);
		end_clock = clock();
		printf("\nВремя расшифрования: %0.6f sec\n", ((double)end_clock - (double)start_clock) / CLOCKS_PER_SEC);
	}
	destruct_dictionary();
}

