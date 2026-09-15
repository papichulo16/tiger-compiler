extern bool EM_anyErrors;

void EM_newline(void);

extern int EM_tokPos;
extern int EM_err_count;

void EM_error(int, string,...);
void EM_impossible(string,...);
void EM_reset(string filename);

void EM_new_srcline (int pos, char* line);
void EM_semantic_error(int pos, char* msg, ...);

