int main()
{
   char *str;

   /* lan cap phat bo nho ban bau */
   str = (char *) malloc(15);
   strcpy(str, "QTMTeam");
   printf("Chuoi = %s,  tai Dia chi = %u\n", str, str);

   /* tai cap phat lai bo nho */
   str = (char *) realloc(str, 25);
   strcat(str, ".@gmail.com");
   printf("Chuoi = %s,  tai Dia chi = %u\n", str, str);

   /* Giai phong bo nho da cap phat */
   free(str);
   //printf("Chuoi = %s,  tai Dia chi = %u\n", str, str);
   return(0);
}
