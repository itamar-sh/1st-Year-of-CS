#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_WORDS_IN_SENTENCE_GENERATION 20
#define MAX_WORD_LENGTH 100
#define MAX_SENTENCE_LENGTH 1000


typedef struct WordStruct {
    char *word;
    struct WordProbability *prob_list;
    int num_of_occurs;  //num of times the word shown in the all file
    //... Add your own fields here
} WordStruct;

typedef struct WordProbability {
    struct WordStruct **word_struct_ptr;
    int size;
    //... Add your own fields here
} WordProbability;

/************ LINKED LIST ************/
typedef struct Node {
    WordStruct *data;
    struct Node *next;
} Node;

typedef struct LinkList {
    Node *first;
    Node *last;
    int size;
    int size_of_open_words;
} LinkList;

// functions declaration
int check_input(char *argv[], FILE** fsource);
LinkList* read_input(int argc, char *argv[], FILE** fsource);
// read input calls -> fill_dictionary
WordStruct *get_i_word_from_dict(LinkList *dictionary, int place);
// fill_dictionary calls -> init_words
int init_words(char line[MAX_SENTENCE_LENGTH], int readen_words, int words_to_read, LinkList *dictionary);
// init_words calls -> 1 search_word_in_dict.
// -> 2 init_word_struct
// -> 3 add
// -> 4 has_a_dot
// -> 5 add_word_to_probability_list
int has_a_dot(char *word);
WordStruct *search_word_in_dict(LinkList *dictionary, char word[MAX_WORD_LENGTH]);
WordStruct *init_word_struct(char *word);
// output process
void control_output(LinkList* p_dict, char* argv[]);




//void print_dict(LinkList* dict){
//    if (dict->first == NULL)
//        {
//            printf("nothing to print");
//        } else{
//            Node* our_node = dict->first;
//            for (int i = 0; i< dict->size-1;i++){
//                    fprintf(stdout, "word number: %d  is: %s\n", i, our_node->data->word);
//                    our_node = our_node->next;
//                }
//        }
//}




/**
 * Add data to new node at the end of the given link list.
 * @param link_list Link list to add data to
 * @param data pointer to dynamically allocated data
 * @return 0 on success, 1 otherwise
 */
int add(LinkList *link_list, WordStruct *data)
{
    Node *new_node = malloc(sizeof(Node));
    if (new_node == NULL)
        {
            return 1;
        }
    *new_node = (Node){data, NULL};

    if (link_list->first == NULL)
        {
            link_list->first = new_node;
            link_list->last = new_node;
        }
    else
        {
            link_list->last->next = new_node;
            link_list->last = new_node;
        }

    link_list->size++;
    if (has_a_dot(data->word) == 0){
            link_list->size_of_open_words++;
        }
    return 0;
}
/*************************************/

/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number
 * @return Random number
 */
int get_random_number(int max_number)
{
    return rand() % (max_number) + 1;
}

/**
 * Choose randomly the next word from the given dictionary, drawn uniformly.
 * The function won't return a word that end's in full stop '.' (Nekuda).
 * @param dictionary Dictionary to choose a word from
 * @return WordStruct of the chosen word
 */
WordStruct *get_first_random_word(LinkList *dictionary)
{
    int rand_num = get_random_number(dictionary->size);
    WordStruct *p_word = get_i_word_from_dict(dictionary, rand_num);
    while (has_a_dot(p_word->word) == 1 || p_word->prob_list == NULL){
        rand_num = get_random_number(dictionary->size);
        p_word = get_i_word_from_dict(dictionary, rand_num);
    }
    return p_word;
}

/**
 * Choose randomly the next word. Depend on it's occurrence frequency
 * in word_struct_ptr->WordProbability.
 * @param word_struct_ptr WordStruct to choose from
 * @return WordStruct of the chosen word
 */
WordStruct *get_next_random_word(WordStruct *word_struct_ptr)
{
    int rand_num = get_random_number(word_struct_ptr->prob_list->size);
    return word_struct_ptr->prob_list->word_struct_ptr[rand_num-1];
}
/**
 * Receive dictionary, generate and print to stdout random sentence out of it.
 * The sentence most have at least 2 words in it.
 * @param dictionary Dictionary to use
 * @return Amount of words in printed sentence
 */
int generate_sentence(LinkList *dictionary)
{
    int words_counter = 1;
    WordStruct *cur_word = get_first_random_word(dictionary);
    while (has_a_dot(cur_word->word) != 1 && words_counter<MAX_WORDS_IN_SENTENCE_GENERATION && cur_word->prob_list != NULL){
            fprintf(stdout, "%s ", cur_word->word);
            cur_word = get_next_random_word(cur_word);
            words_counter++;
        }
    fprintf(stdout, "%s\n", cur_word->word);
    return 0;
}
int has_a_dot(char *word){
    char *p_char = strchr(word, '.');
    if (p_char == NULL){
            return 0;  // no dot
        }
    else{
            return 1; // has dot
        }
}
/**
 * Gets 2 WordStructs. If second_word in first_word's prob_list,
 * update the existing probability value.
 * Otherwise, add the second word to the prob_list of the first word.
 * @param first_word
 * @param second_word
 * @return 0 if already in list, 1 otherwise.
 */
int add_word_to_probability_list(WordStruct *first_word,
                                 WordStruct *second_word)
{
    int answer = 1;
    if (first_word->prob_list == NULL){
            first_word->prob_list = malloc(sizeof (WordProbability));  // WordProbability contains a list of pointers tp WordStructs inside him.
            first_word->prob_list->size = 0;
            first_word->prob_list->word_struct_ptr = malloc(sizeof (WordStruct*));
        }
    int length_of_list = first_word->prob_list->size;
    for (int i = 0; i<length_of_list ; i++){
            if (strcmp(first_word->prob_list->word_struct_ptr[i]->word, second_word->word)==0){
                    answer = 0;
                    break;
                }
        }
    first_word->prob_list->size++;
    first_word->prob_list->word_struct_ptr = realloc(first_word->prob_list->word_struct_ptr, sizeof(WordStruct*)*first_word->prob_list->size);
    if (first_word->prob_list->word_struct_ptr == NULL){
            fprintf(stdout, "Allocation failure: realloc in add_word");
            return -1; // -1 in case of failure
        }
    first_word->prob_list->word_struct_ptr[first_word->prob_list->size-1] = second_word; // i move the wordstruct himself and not the pointer!
    return answer;
}

/**
 * Read word from the given file. Add every unique word to the dictionary.
 * Also, at every iteration, update the prob_list of the previous word with
 * the value of the current word.
 * @param fp File pointer
 * @param words_to_read Number of words to read from file.
 *                      If value is bigger than the file's word count,
 *                      or if words_to_read == -1 than read entire file.
 * @param dictionary Empty dictionary to fill
 */
void fill_dictionary(FILE *fp, int words_to_read, LinkList *dictionary)
{
    int counter_readen_words = 0;
    while (words_to_read == -1 || counter_readen_words < words_to_read){
            char cur_line[MAX_SENTENCE_LENGTH];
            if (fgets(cur_line, MAX_SENTENCE_LENGTH, fp) != NULL){
                    counter_readen_words += init_words(cur_line, counter_readen_words, words_to_read, dictionary);
            } else{
                //fprintf(stdout, "we had a problem with fgets\n");
                break;
            }
    }
}

/**
 * Free the given dictionary and all of it's content from memory.
 * @param dictionary Dictionary to free
 */
void free_dictionary(LinkList *dictionary)
{
    Node *p_cur_node = dictionary->first;
    Node *p_next_node;
    for (int i = 0; i < dictionary->size; i++){
        if (p_cur_node->data->prob_list != NULL){
            free(p_cur_node->data->prob_list->word_struct_ptr);
            free(p_cur_node->data->prob_list);
        }
        free(p_cur_node->data->word);
        free(p_cur_node->data);
        p_next_node = p_cur_node->next;
        p_cur_node = p_next_node;
    }
    p_cur_node = dictionary->first;
    for (int i = 0; i<dictionary->size; i++){
        p_next_node = p_cur_node->next;
        free(p_cur_node);
        p_cur_node = p_next_node;
    }
    //p_cur_node = NULL;
    //p_next_node = NULL;
    free(dictionary);
}

/**
 * @param argc
 * @param argv 1) Seed
 *             2) Number of sentences to generate
 *             3) Path to file
 *             4) Optional - Number of words to read
 */
WordStruct *get_i_word_from_dict(LinkList *dictionary, int place){
    Node cur_node = *(dictionary->first);
    for (int i = 1; i < place; i++){
            cur_node = *(cur_node.next);
        }
    return cur_node.data;
}
int check_input(char *argv[], FILE** p_fsource)
{
    *p_fsource = fopen(argv[3], "r");
    if (*p_fsource == NULL){
            fprintf(stdout, "Error: the path for the file is not valid\n");
            return 1; // 1 its fail
        }
    else {
            return 0; // 0 its success
        }

}
LinkList* read_input(int argc, char *argv[], FILE** p_fsource)
{
    int words_to_read;
    if (argc == 4){
            words_to_read = -1;
        }
    else {
            words_to_read = strtol(argv[4], NULL, 0);
        }
    LinkList* dict = (LinkList*)malloc(sizeof(LinkList));
    dict->first = NULL;
    dict->last = NULL;
    dict->size = 0;
    dict->size_of_open_words = 0;
    fill_dictionary(*p_fsource, words_to_read, dict);
    return dict;
}
// read words from line and update the counter
// init Wordstruct and inside init a wordprobabilty and inside init a problist
// stop when readen words equal to words_to_read
int init_words(char line[MAX_SENTENCE_LENGTH], int readen_words, int words_to_read, LinkList *dictionary)
{
    char *word = strtok(line, " \n");
    int counter_readen_words = 0;
    WordStruct *temp_word;
    while (readen_words + counter_readen_words < words_to_read || words_to_read == -1){
            if (counter_readen_words != 0){
                    word = strtok(NULL, " \n");
            }
            if (word == NULL){
                return counter_readen_words;
            } else {
                counter_readen_words++;
                WordStruct *p_word = search_word_in_dict(dictionary, word);
                if (p_word == NULL){
                    // add new word to dict
                    p_word = init_word_struct(word);
                    add(dictionary, p_word);
                } else {
                    p_word->num_of_occurs++;
                }
                // check if its start of a sentence if not add the prob of the last word
                if (counter_readen_words != 1 && has_a_dot(temp_word->word) == 0){
                    add_word_to_probability_list(temp_word, p_word);
                }
                temp_word = p_word;  // keep the value of word in temp_word
            }
        }
    return counter_readen_words;
}
WordStruct *search_word_in_dict(LinkList *dictionary, char word[MAX_WORD_LENGTH])
{
    if (dictionary->size == 0){
            return NULL;
    }
    Node cur_node = *(dictionary->first);
    for (int i = 1; i <= dictionary->size; i++){
        if (strcmp(cur_node.data->word, word) == 0){
            return cur_node.data;
        }
        if (cur_node.next != NULL){
            cur_node = *(cur_node.next);
        }
    }
    return NULL;
}
WordStruct *init_word_struct(char *word){
    WordStruct *p_word = (WordStruct*)malloc(sizeof (WordStruct));
    p_word->prob_list = NULL;
    p_word->num_of_occurs = 1;
    p_word->word = NULL;
    unsigned int num = (strlen(word));
    p_word->word = (char*)malloc((num+1)* sizeof (char));
    int i = 0;
    for (; i<strlen(word); i++){
            *(p_word->word+i)=word[i];
    }
    *(p_word->word+i)='\0';
    return p_word;
}
void control_output(LinkList* p_dict, char* argv[]){
    int twits_to_generate = strtol(argv[2], NULL, 0);
    for (int i = 1; i<=twits_to_generate; i++){
            fprintf(stdout, "Tweet %d: ", i);
            generate_sentence(p_dict);
        }
}
int main(int argc, char *argv[])
{
    if (argc != 4 && argc != 5){
            fprintf(stdout, "Usage: you need to bring 4 or 5 args\n");
            return 1; // 1 its fail
        }
    srand(strtol(argv[1], NULL, 10));
    FILE *fsource = NULL;
    FILE **p_fsource = &fsource;
    if (check_input(argv, p_fsource) == 1){
            return EXIT_FAILURE;
        }
    LinkList* p_dict = read_input(argc, argv, p_fsource);
    if (p_dict == NULL){
            fclose(fsource);
            return EXIT_FAILURE;
        }
    //print_dict(p_dict);
    control_output(p_dict, argv);
    free_dictionary(p_dict);
    p_dict = NULL;
    fclose(fsource);
    return 0;
}