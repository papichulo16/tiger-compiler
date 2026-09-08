extern bool EM_anyErrors;

void EM_newline(void);

extern int EM_tokPos;
extern int EM_err_count;

void EM_error(int, string,...);
void EM_impossible(string,...);
void EM_reset(string filename);
