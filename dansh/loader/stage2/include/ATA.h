
typedef struct 
{
	int (*read) (unsigned int, unsigned int);
}ata_form;

extern ata_form ata;