int checkMagicNumber(FILE *filePointer, char *filename, unsigned short magic_number, int MAGIC_NUMBER_ASCII_PGM);
int checkCommentLine(FILE *filePointer, char *filename, char *commentLine, int MAX_COMMENT_LINE_LENGTH);