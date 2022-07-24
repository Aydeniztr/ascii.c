#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#define BUFFER_SIZE 1000

void remove_line(FILE *src, FILE *temp, const int line);
void read_file(FILE *fptr);

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream) {

  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
  return written;
}

int urlopen(char *string) {

  printf(">>running urlopen function \x1b[42m[2/4 remaining]\x1b[0m\n");

  CURL *curl_handle;
  static const char *pagefilename = "tempfile";
  FILE *pagefile;

  curl_global_init(CURL_GLOBAL_ALL);
 
  /* init the curl session */
  curl_handle = curl_easy_init();
 
  /* set URL to get here */
  curl_easy_setopt(curl_handle, CURLOPT_URL, string);
 
  /* Switch on full protocol/debug output while testing */
  curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 0L);
 
  /* disable progress meter, set to 0L to enable it */
  curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
 
  /* send all data to this function  */
  curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
 
  /* open the file */
  pagefile = fopen(pagefilename, "wb");
  if(pagefile) {
 
    /* write the page body to this file handle */
    curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);
 
    /* get it! */
    curl_easy_perform(curl_handle);
 
    /* close the header file */
    fclose(pagefile);
  }
 
  /* cleanup curl stuff */
  curl_easy_cleanup(curl_handle);
 
  curl_global_cleanup();
  return 0;
}

void modify_print(int line) {

   FILE *src;
   FILE *temp;
   char ch;
   char path[]="tempfile";

   src = fopen(path, "r");
   temp = fopen("delete.tmp", "w");
   if (src == NULL || temp == NULL){
      printf("Unable to open file.\n");
      exit(EXIT_FAILURE);
   }

   // Delete given line from file.
   remove_line(src, temp, line);
   /* Close all open files */
   fclose(src);
   fclose(temp);
   /* Delete src file and rename temp file as src */
   remove(path);
   rename("delete.tmp", path);
}

void remove_line(FILE *src, FILE *temp, const int line){
   char buffer[BUFFER_SIZE];
   int count = 1;
   while ((fgets(buffer, BUFFER_SIZE, src)) != NULL){
      if (line != count)
         fputs(buffer, temp);
      count++;
   }
}

void read_file(FILE *fptr) {
   char ch;
   while((ch = fgetc(fptr)) != EOF)
   putchar(ch);
}

int main(void) {

	FILE *source;
	char combine[1024];
	char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char prt1[] = "https://www.degraeve.com/img2txt-yay.php?url=";
	char prt2[] = "&mode=A&size=100&carstr=";
	char prt3[] = "&order=O&invert=Y";
	char link[256];
	char c[] = "";
	char counter;
	int count;
	int i;

	printf("Enter a image url: ");
	scanf("%s", link);

	sprintf(combine, "%s%s%s%s%s", prt1, link, prt2, charset, prt3);
	sscanf(combine, "%s", &c);
	printf("\n%s\n\n",c);
	printf(">>running main function \x1b[42m[1/2 done]\x1b[0m\n");
        urlopen(c);
	printf(">>running modify function \x1b[42m[2/2 done]\x1b[0m\n");	

	source = fopen("tempfile", "r");
   	if (source == NULL){
   		printf("Unable to open file.\n");
   		exit(EXIT_FAILURE);
   	}

	for (counter = getc(source); counter != EOF; counter = getc(source)) {
        
		if (counter == '\n') {
        		count = count + 1;
		}
	}
	fclose(source);

	printf("count %d\n",count);

	for (i = 0; i < 12; ++i) {
		modify_print(1);
	}

	for (i = count-38; i < count; ++i) {
		modify_print(count-38);
	}

   	source = fopen("tempfile", "r");
   	if (source == NULL){
   		printf("Unable to open file.\n");
   		exit(EXIT_FAILURE);
   	}
   	read_file(source);
	fclose(source);

	return 0;
}

/* maded by Ahmet Yiğit AYDENIZ in 2022 */
