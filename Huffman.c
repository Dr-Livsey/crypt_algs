#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <locale.h>
#include <time.h>
#include <string.h>
#define PRIORITY 0
#define CHAR 1
#define TREE 2

FILE *input;
char zero_one[500];
int zo_size = 0;

typedef struct My_Tree
{
	int priority;
	int id;
	int Char;
	int binary;
	struct My_Tree *left;
	struct My_Tree *right;
	struct My_Tree *parent;
}tree;
typedef struct symbols
{
	tree *cur_root;
	struct symbols *next;
	struct symbols *prev;
} symbols;

symbols *head = NULL;
symbols *tail = NULL;

typedef struct sym_sypher
{
	int Char;
	char cypher[500];
	struct sym_sypher *next;
	struct sym_sypher *prev;
}cyphers;

cyphers *Chead = NULL;
cyphers *Ctail = NULL;

tree* destruct_tree(tree *cur)
{
	if (cur != NULL)
	{
		destruct_tree(cur->left);
		destruct_tree(cur->right);
		free(cur);
		cur = NULL;
	}
	else return NULL;
	return NULL;
}
void push_symbols(int symbol)
{
	symbols *temp = (symbols*)malloc(sizeof(symbols));
	temp->cur_root = (tree*)malloc(sizeof(tree));

	temp->cur_root->Char = symbol;
	temp->cur_root->left = NULL;
	temp->cur_root->right = NULL;
	temp->cur_root->parent = NULL;
	temp->cur_root->id = CHAR;
	temp->cur_root->priority = 1;
	temp->cur_root->binary = 0;

	temp->next = NULL;

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
void insert_symbols(symbols *sym_ptr, tree *tree_ptr)
{
	symbols *temp = (symbols*)malloc(sizeof(symbols));
	symbols *cur = head;

	temp->cur_root = tree_ptr;

	if (sym_ptr == head && head)
	{
		temp->prev = NULL;
		temp->next = head;
		head->prev = temp;
		head = temp;
	}
	else if (head == NULL)
	{
		temp->prev = NULL;
		head = tail = temp;
	}
	else
	{
		while (cur != sym_ptr) cur = cur->next;

		if (cur==NULL)
		{
			temp->prev = tail;
			tail->next = temp;
			temp->next = NULL;
			tail = temp;
		}
		else
		{
			temp->next = sym_ptr;
			temp->prev = sym_ptr->prev;
			sym_ptr->prev->next = temp;
			sym_ptr->prev = temp;
		}
	}
}
void pop_symbols(symbols *ptr)
{

	if ((ptr==head)&&(head->next)) { 
		symbols *temp = head;	
		head = head->next;	
		head->prev = NULL;	
		free(temp);
		temp = NULL;

		return;		
	}
	else if ((ptr==head) && (head == tail)) {

		head->next = NULL;	
		free(head);
		head = NULL;

		return;			
	}
	else if (ptr==tail) {

		symbols *temp = tail;	
		tail = tail->prev;	
		tail->next = NULL;	
		free(temp);
		temp = NULL;

		return;	
	}	
	symbols *temp = head, *temp2; 
								 
	while(temp!=ptr) temp = temp->next;  

	temp2 = temp;	
	temp2->prev->next = temp->next;	
	temp2->next->prev = temp->prev;
	free(temp);
	temp = NULL;

	return;
}

void push_cyphers(int Char)
{	
	cyphers *temp = (cyphers*)malloc(sizeof(cyphers));

	temp->Char = Char;
	memcpy(temp->cypher, zero_one, sizeof(char) * (strlen(zero_one)+1));

	zero_one[strlen(zero_one) - 1] = '\0';
	zo_size = strlen(zero_one);
	temp->next = NULL;
	if (Chead != NULL)
	{
		temp->prev = Ctail;
		Ctail->next = temp;
		Ctail = temp;
	}
	else
	{
		temp->prev = NULL;
		Chead = Ctail = temp;
	}
}
void destruct_cyphers()
{
	if (Chead == NULL) return;

	while (Chead)
	{
		Ctail = Chead->next;
		free(Chead);
		Chead = NULL;
		Chead = Ctail;
	}
}
void destruct_symbols()
{
	if (head == NULL) return;

	while (head)
	{
		tail = head->next;
		free(head);
		head = NULL;
		head = tail;
	}
}
int search_and_push(int symbol)
{
	symbols *tmp_head = head;
	symbols *tmp_tail = tail;

	while (tmp_head && tmp_tail)
	{
		if (tmp_head->cur_root->Char == symbol || tmp_head->cur_root->Char == symbol)
		{
			if (tmp_head->cur_root->Char == symbol)
			{
				tmp_head->cur_root->priority++;
				return 1;
			}
			else if (tmp_head->cur_root->Char == symbol)
			{
				tmp_head->cur_root->priority++;
				return 1;
			}
		}
		tmp_head = tmp_head->next;
		tmp_tail = tmp_tail->prev;
	}
	push_symbols(symbol);
	return 0;
}

void swap(symbols *a, symbols *b)
{
	symbols *x = (symbols*)malloc(sizeof(symbols));

	x->cur_root = a->cur_root;
	a->cur_root = b->cur_root;
	b->cur_root = x->cur_root;

	free(x);
	x = NULL;
}
void qsorting(symbols *left, symbols *right)
{
	symbols *pL = left;
	symbols *pR = right;
	symbols *x = left;
	
	if (!head->next) return;

	do 
	{
		while (pL->cur_root->priority < x->cur_root->priority) pL = pL->next;
		while (pR->cur_root->priority > x->cur_root->priority) pR = pR->prev;

		if (pL <= pR)
		{
		   swap(pL, pR);
		   pL = pL->next;
		   pR = pR->prev;
		}

	} while (pL <= pR);

	if (left < pR) qsorting(left, pR);
	if (right > pL) qsorting(pL, right);
}
tree* unite(symbols *a, symbols *b)
{
	tree* root = (tree*)malloc(sizeof(tree));

	root->id = PRIORITY;

    if (b!=NULL) root->priority = a->cur_root->priority + b->cur_root->priority;
    else root->priority = a->cur_root->priority;

	root->parent = NULL;
	root->left = a->cur_root;
	a->cur_root->parent = root; 

	if (b != NULL) {
		root->right = b->cur_root;
		b->cur_root->parent = root;
	}
	else root->right = NULL;

	pop_symbols(head);
    if (b!=NULL) pop_symbols(head);

	return root;
}
symbols *search_the_same_priority(tree *ptr)
{
	symbols *tmp = head;
	while (tmp)
	{
		if (tmp->cur_root->priority < ptr->priority) tmp = tmp->next;
		else return tmp;
	}
	return NULL;
}

tree* load_tree()
{
	symbols *current = head;
	symbols *insert_tree = NULL;
	tree *current_tree = NULL;

	while (current)
	{
		current_tree = unite(current, current->next);
		if (head == NULL) break;
		insert_tree = search_the_same_priority(current_tree);
		insert_symbols(insert_tree, current_tree);
		current = head;
	}
	return current_tree;
}

void pstfx(tree* cur,FILE* alph_dec)
{
	if (cur != NULL)
	{
		if (cur->left) fprintf(alph_dec, "0");
		pstfx(cur->left,alph_dec);

		if (!cur->left && !cur->right)	fprintf(alph_dec, "\"%c\"", cur->Char);
		
		if (cur->right)	fprintf(alph_dec, "1");
		pstfx(cur->right,alph_dec);
	}
}
void get_codes(tree* cur)
{
	if (cur != NULL)
	{
		if (cur->left)	zero_one[zo_size++] = '0';
	    get_codes(cur->left);

		if (!cur->left && !cur->right)
		{
			cur->binary = 1;
			zero_one[zo_size] = '\0';			
			push_cyphers(cur->Char);
			return;
		}

		if (cur->right)	zero_one[zo_size++] = '1';
		get_codes(cur->right);

		if ((cur->parent) && (cur->left && cur->right))
		{
			zero_one[strlen(zero_one) - 1] = '\0';
			zo_size = strlen(zero_one);
		}
	}
}

tree* load_pst(int *bits)
{
	FILE *alph_dec = fopen("abc.txt", "r");
	tree *tree_cur = NULL;
	tree *root=NULL;
	int c = fgetc(alph_dec);
	int prev_c = 0;

	while (c != EOF)
	{
		if (c == '|' && prev_c == '"') break;

		if (root == NULL)
		{
			root = (tree*)malloc(sizeof(tree));
			root->left = NULL;
			root->right = NULL;
			root->parent = NULL;
			tree_cur = root;
        }
		if ((c == '0')&&(tree_cur->left==NULL))
		{
			tree_cur->left = (tree*)malloc(sizeof(tree));
			tree_cur->left->parent = tree_cur;
			tree_cur = tree_cur->left;
			tree_cur->right = NULL;
			tree_cur->left = NULL;
			prev_c = c;
			c = fgetc(alph_dec);
		}
		else if ((c == '1')&&(tree_cur->right==NULL))
		{
			tree_cur->right = (tree*)malloc(sizeof(tree));
			tree_cur->right->parent = tree_cur;
			tree_cur = tree_cur->right;
			tree_cur->right = NULL;
			tree_cur->left = NULL;
			prev_c = c;
			c = fgetc(alph_dec);
		}
		else if (tree_cur->left == NULL && tree_cur->right == NULL)
		{
			c = fgetc(alph_dec);
			tree_cur->Char = c;
			tree_cur = tree_cur->parent;
			c = fgetc(alph_dec);
			prev_c = c;
			c = fgetc(alph_dec);
		}
		else tree_cur = tree_cur->parent;
	
	}
	fscanf(alph_dec, "%d", bits);

	fclose(alph_dec);
	return root;
}
void get_binary(tree *tree)
{
	FILE *alph_dec = fopen("abc.txt", "w");	
	pstfx(tree, alph_dec);
	fclose(alph_dec);
}

char* search_code(int symbol)
{
	cyphers *tmp = Chead;
	if (symbol == EOF) return NULL;
	while (tmp)
	{
		if (tmp->Char == symbol) return tmp->cypher;
		tmp = tmp->next;
	}
	return NULL;
}
char* save_file(char *file_name)
{
	int c, i = 0;
	char *file_string = NULL;
	FILE* file = fopen(file_name, "r");


	while ((c = fgetc(file)) != EOF)
	{
		file_string = (char*)realloc(file_string, sizeof(char)*(i + 1));
		file_string[i++] = c;
	}
	file_string = (char*)realloc(file_string, sizeof(char)*(i + 1));
	file_string[i] = '\0';

	fclose(file);
	return file_string;
}
char push_byte(char byte, char *code_string, int *bits, FILE* encr_file)
{
	int i = 0;
	while (code_string[i] != '\0')
	{
		byte = byte << 1;
		(*bits)++;
		byte = byte + (code_string[i] - '0');

		if ((*bits) == 8)
		{
			fwrite((char*)&byte, 1, sizeof(char), encr_file);
			byte = 0;
			(*bits) = 0;
		}
		i++;
	}
	return byte;
}

void decrypt_bytes(char *binary_file, int cnt_bytes, tree* root, int end_byte_bits, FILE* decr_file)
{
	int bits = 1;
	int shift = 7, bytes = 0, i = 0;
	tree* tree_cur = root;

	while (bytes <= cnt_bytes)
	{
		if (shift == -1)
		{
			i++;
			bytes++;
			shift = 7;
		}

		if (bytes == cnt_bytes - 1)
		{
			bytes++;
			shift = end_byte_bits - 1;
		}

		if ((binary_file[i] & (bits << shift)))	tree_cur = tree_cur->right;
		else if ((binary_file[i] & (bits << shift)) == 0)	 tree_cur = tree_cur->left;

		if (!tree_cur->left && !tree_cur->right)
		{
			fprintf(decr_file, "%c", tree_cur->Char);
			tree_cur = root;
		}

		shift--;
		if (bytes > cnt_bytes) break;
	}
}

void Encrypt(char *filename, char *encr_name)
{
	FILE *encr_file;
	FILE *abc;
	tree* tree = NULL;
	char *code_string = NULL;
	char *file_string = NULL;
	char byte = 0;
	int i = 0, bits = 0, c = 0;

	encr_file = fopen(filename, "r");
	while ((c = fgetc(encr_file)) != EOF)	search_and_push(c);
	fclose(encr_file);

	qsorting(head, tail);
	tree = load_tree();
	get_binary(tree);
	get_codes(tree);

	file_string = save_file(filename);
	encr_file = fopen(encr_name, "wb");

	while (file_string[i] != '\0')
	{
		code_string = search_code(file_string[i]);
		byte = push_byte(byte, code_string, &bits, encr_file);
		i++;
	}

	abc = fopen("abc.txt", "a");	
	fprintf(abc, "|%d", bits);
	fclose(abc);

	fwrite((char*)&byte, 1, sizeof(char), encr_file);
	fclose(encr_file);

	free(file_string);
	file_string = NULL;

	destruct_symbols();
	destruct_cyphers();
	destruct_tree(tree);
}
void Decrypt(char *filename, char *decr_name)
{
	FILE *encr_file, *decr_file;
	tree *tree = NULL;
	char *binary_file = NULL;
	long cnt_bytes = 0;
	int end_byte_bits = 0;

	tree = load_pst(&end_byte_bits);

	encr_file = fopen(filename, "rb");

	fseek(encr_file, 0, SEEK_END);
    cnt_bytes = ftell(encr_file);
	rewind(encr_file);
	binary_file = (char*)malloc(sizeof(char)*cnt_bytes);
	fread(binary_file, sizeof(char), cnt_bytes, encr_file);

	fclose(encr_file);

	decr_file = fopen(decr_name, "w");
    decrypt_bytes(binary_file, cnt_bytes, tree, end_byte_bits, decr_file);	
	fclose(decr_file);
}

long get_file_size(char *name)
{
	FILE *bin = fopen(name, "rb");
	long bytes = 0;

	fseek(bin, 0, SEEK_END);
	bytes = ftell(bin);
	rewind(bin);

	fclose(bin);
	return bytes;
}

int main()
{
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
}