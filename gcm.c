#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

 struct PresentKey {
	unsigned long int roundKey;
	unsigned short remains;
	};

 struct FiveBit {unsigned char data:5;};

 struct PresentKey * keySheudle(struct PresentKey *xkey , struct FiveBit counter) {
	char tab[16] = {'\xc0' , '\x50' , '\x60' , '\xb0', '\x90', '\x00', '\xa0' , '\xd0' , '\x30', '\xe0', '\xf0' , '\x80', '\x40', '\x70', '\x10', '\x20'};

	struct PresentKey  *key = malloc(sizeof(struct PresentKey ));
	unsigned long int remains_copy = (unsigned long int) xkey->remains;					 
	key->roundKey = (xkey->roundKey & 7)<<61|((remains_copy & 65535) << 45) |(xkey->roundKey >> 19);
	key->remains = (xkey->roundKey >> 3) & 65535;
	
	unsigned char *c = (unsigned char*) &(key->roundKey);
	c[7]=(tab[(c[7] >> 4 ) & 255 ] & 240) | (c[7] & 15);

	unsigned short int counter_copy =  (unsigned short int)counter.data;
	unsigned long int counter_long_copy =  (unsigned long int)counter.data;
	key->remains = (((key->remains & 0x8000) ^ ( (counter_copy   & 1)<< 15 ))  & 32768) | (key->remains  & 0x7fff) ;
	key->roundKey = ((((key->roundKey & 15 ) ^ (  (counter_long_copy >> 1)   & 15) )) & 15)    | (key->roundKey  & 0xfffffffffffffff0) ;
	return key;  }

inline unsigned long PresentEncrypt(unsigned long data,struct PresentKey *key){
int k;  char *c;
unsigned long  int currentState = data  ; 

char tab[256] = {
'\xcc' , '\xc5' , '\xc6' , '\xcb', '\xc9', '\xc0', '\xca' , '\xcd' , '\xc3', '\xce', '\xcf' , '\xc8', '\xc4', '\xc7', '\xc1', '\xc2' ,
'\x5c' , '\x55' , '\x56' , '\x5b', '\x59', '\x50', '\x5a' , '\x5d' , '\x53', '\x5e', '\x5f' , '\x58', '\x54', '\x57', '\x51', '\x52' ,
'\x6c' , '\x65' , '\x66' , '\x6b', '\x69', '\x60', '\x6a' , '\x6d' , '\x63', '\x6e', '\x6f' , '\x68', '\x64', '\x67', '\x61', '\x62' ,
'\xbc' , '\xb5' , '\xb6' , '\xbb', '\xb9', '\xb0', '\xba' , '\xbd' , '\xb3', '\xbe', '\xbf' , '\xb8', '\xb4', '\xb7', '\xb1', '\xb2' ,
'\x9c' , '\x95' , '\x96' , '\x9b', '\x99', '\x90', '\x9a' , '\x9d' , '\x93', '\x9e', '\x9f' , '\x98', '\x94', '\x97', '\x91', '\x92' ,
'\x0c' , '\x05' , '\x06' , '\x0b', '\x09', '\x00', '\x0a' , '\x0d' , '\x03', '\x0e', '\x0f' , '\x08', '\x04', '\x07', '\x01', '\x02' ,
'\xac' , '\xa5' , '\xa6' , '\xab', '\xa9', '\xa0', '\xaa' , '\xad' , '\xa3', '\xae', '\xaf' , '\xa8', '\xa4', '\xa7', '\xa1', '\xa2' ,
'\xdc' , '\xd5' , '\xd6' , '\xdb', '\xd9', '\xd0', '\xda' , '\xdd' , '\xd3', '\xde', '\xdf' , '\xd8', '\xd4', '\xd7', '\xd1', '\xd2' ,
'\x3c' , '\x35' , '\x36' , '\x3b', '\x39', '\x30', '\x3a' , '\x3d' , '\x33', '\x3e', '\x3f' , '\x38', '\x34', '\x37', '\x31', '\x32' ,
'\xec' , '\xe5' , '\xe6' , '\xeb', '\xe9', '\xe0', '\xea' , '\xed' , '\xe3', '\xee', '\xef' , '\xe8', '\xe4', '\xe7', '\xe1', '\xe2' ,
'\xfc' , '\xf5' , '\xf6' , '\xfb', '\xf9', '\xf0', '\xfa' , '\xfd' , '\xf3', '\xfe', '\xff' , '\xf8', '\xf4', '\xf7', '\xf1', '\xf2' ,
'\x8c' , '\x85' , '\x86' , '\x8b', '\x89', '\x80', '\x8a' , '\x8d' , '\x83', '\x8e', '\x8f' , '\x88', '\x84', '\x87', '\x81', '\x82' ,
'\x4c' , '\x45' , '\x46' , '\x4b', '\x49', '\x40', '\x4a' , '\x4d' , '\x43', '\x4e', '\x4f' , '\x48', '\x44', '\x47', '\x41', '\x42' ,
'\x7c' , '\x75' , '\x76' , '\x7b', '\x79', '\x70', '\x7a' , '\x7d' , '\x73', '\x7e', '\x7f' , '\x78', '\x74', '\x77', '\x71', '\x72' ,
'\x1c' , '\x15' , '\x16' , '\x1b', '\x19', '\x10', '\x1a' , '\x1d' , '\x13', '\x1e', '\x1f' , '\x18', '\x14', '\x17', '\x11', '\x12' ,
'\x2c' , '\x25' , '\x26' , '\x2b', '\x29', '\x20', '\x2a' , '\x2d' , '\x23', '\x2e', '\x2f' , '\x28', '\x24', '\x27', '\x21', '\x22'  };



for(k=1;k<=31;k++){ 
currentState= currentState^(key->roundKey);
int l;
struct FiveBit counter;
c = (unsigned char*) &currentState; 
for (l=0;l<8;l++) c[l]=tab[c[l] & 255];

currentState = ((9223372036854775808U & currentState) << 0) | 
((4611686018427387904 & currentState) >> 15) | 
((2305843009213693952 & currentState) >> 30) | 
((1152921504606846976 & currentState) >> 45) | 
((576460752303423488 & currentState) << 3) | 
((288230376151711744 & currentState) >> 12) | 
((144115188075855872 & currentState) >> 27) | 
((72057594037927936 & currentState) >> 42) | 
((36028797018963968 & currentState) << 6) | 
((18014398509481984 & currentState) >> 9) | 
((9007199254740992 & currentState) >> 24) | 
((4503599627370496 & currentState) >> 39) | 
((2251799813685248 & currentState) << 9) | 
((1125899906842624 & currentState) >> 6) | 
((562949953421312 & currentState) >> 21) | 
((281474976710656 & currentState) >> 36) | 
((140737488355328 & currentState) << 12) | 
((70368744177664 & currentState) >> 3) | 
((35184372088832 & currentState) >> 18) | 
((17592186044416 & currentState) >> 33) | 
((8796093022208 & currentState) << 15) | 
((4398046511104 & currentState) << 0) | 
((2199023255552 & currentState) >> 15) | 
((1099511627776 & currentState) >> 30) | 
((549755813888 & currentState) << 18) | 
((274877906944 & currentState) << 3) | 
((137438953472 & currentState) >> 12) | 
((68719476736 & currentState) >> 27) | 
((34359738368 & currentState) << 21) | 
((17179869184 & currentState) << 6) | 
((8589934592 & currentState) >> 9) | 
((4294967296 & currentState) >> 24) | 
((2147483648 & currentState) << 24) | 
((1073741824 & currentState) << 9) | 
((536870912 & currentState) >> 6) | 
((268435456 & currentState) >> 21) | 
((134217728 & currentState) << 27) | 
((67108864 & currentState) << 12) | 
((33554432 & currentState) >> 3) | 
((16777216 & currentState) >> 18) | 
((8388608 & currentState) << 30) | 
((4194304 & currentState) << 15) | 
((2097152 & currentState) << 0) | 
((1048576 & currentState) >> 15) | 
((524288 & currentState) << 33) | 
((262144 & currentState) << 18) | 
((131072 & currentState) << 3) | 
((65536 & currentState) >> 12) | 
((32768 & currentState) << 36) | 
((16384 & currentState) << 21) | 
((8192 & currentState) << 6) | 
((4096 & currentState) >> 9) | 
((2048 & currentState) << 39) | 
((1024 & currentState) << 24) | 
((512 & currentState) << 9) | 
((256 & currentState) >> 6) | 
((128 & currentState) << 42) | 
((64 & currentState) << 27) | 
((32 & currentState) << 12) | 
((16 & currentState) >> 3) | 
((8 & currentState) << 45) | 
((4 & currentState) << 30) | 
((2 & currentState) << 15) | 
((1 & currentState) << 0) ;


counter.data=k;
key=keySheudle(key,counter); }

return currentState^key->roundKey;
}


struct GCMEncryptedData{
unsigned long int *data;
unsigned int size;
unsigned long int *ad;
unsigned int ad_size;
unsigned long int gmac;
};


inline unsigned long int GFMult(unsigned long int a, unsigned long int b){
unsigned long int result=0;
	lab_mult : 	
 	if(!a||!b) return result;
	if(b&1) result^=a;
	if(a & 0x8000000000000000)  a=(a<<1)^ 15; else a <<=1;  
	b >>= 1; 
	goto lab_mult;			 
	return result;}

struct GCMEncryptedData * GCMEncrypt(unsigned int iv, unsigned long int *data,unsigned int num,struct PresentKey *key,unsigned long int *ad,unsigned int ad_num){
	struct GCMEncryptedData *result = malloc(sizeof(struct GCMEncryptedData));
	result->data = calloc(num,sizeof(unsigned long int));
	unsigned long int iv_long = (unsigned long int) iv;
	int k;
	unsigned long int hashkey = PresentEncrypt( (unsigned long int)0 ,key); 
	unsigned long int gmac = 0x0 ;
	if(ad!=NULL && ad_num > 0)  {
				for (k=0;k<ad_num;k++)
				gmac = GFMult(ad[k]^gmac,hashkey) ;
				result->ad=ad;result->ad_size=ad_num; }	
			for (k=0;k<num;k++){ 
	 		result->data[k] =data[k]^PresentEncrypt((iv_long<< 32) |(k+1) ,key);
			gmac = GFMult( result->data[k] ^ gmac,hashkey);}
			
			unsigned long int ad_length = ((unsigned long int)(64*ad_num))<<32;
			unsigned long int data_length = (unsigned long int)(64*num);

			gmac = ad!=NULL ? GFMult(gmac^((ad_length<<32)|data_length),hashkey) : 
			GFMult(gmac^(num*64),hashkey);
			gmac = GFMult(gmac^(iv_long<<32),hashkey);
			result->gmac=gmac;
			result->size=num;
			return result;}

unsigned long int *GCMDecrypt(unsigned int iv,struct GCMEncryptedData *data,struct PresentKey *key ){
	unsigned int k;
	unsigned long int *result = malloc(sizeof(unsigned long int)*data->size);
	unsigned long int iv_long = (unsigned long int) iv;
	unsigned long int magic_zero=0;
	unsigned long int gmac = 0x0 ;
	unsigned long int hashkey = PresentEncrypt((unsigned long int) 0 ,key);
	
	if(data->ad!=NULL && data->ad_size > 0)for (k=0;k<data->ad_size;k++)gmac=GFMult(data->ad[k] ^ gmac ,hashkey) ;
	for(k=0;k<data->size;k++) { 
		result[k]=data->data[k]^PresentEncrypt((((unsigned long int)iv) << 32) |(k+1) ,key);  
		gmac=GFMult(data->data[k]^gmac,hashkey);}
			
	unsigned long int ad_length=((unsigned long int)(64*data->ad_size))<<32;
	unsigned long int data_length=(unsigned long int)(64*data->size);

	gmac = data->ad!=NULL ? GFMult(gmac^((((unsigned long int)(ad_length))<<32)|data_length),hashkey) : 
			GFMult(gmac ^(data_length),hashkey) ;
	gmac = GFMult(gmac^(iv_long << 32) ,hashkey);
	if(gmac == data->gmac) 	return result; else return NULL; }


int main(int argc,char *argv[] ) {
 struct PresentKey *key =  malloc(sizeof(struct PresentKey));
 char *user_plain_text = malloc(256);
 unsigned int current_size = 256;

 unsigned char current_c = EOF;
 unsigned int k = 0;
 unsigned int l;
 
  printf("\n********************************************************************************\n****** $$$ Welcome to Our Present GCM Ciphering/Desiphering software  $$$ ****** \n********************************************************************************\n\nPlain text : ");

 while (( current_c = getchar() ) != '\n' && current_c != EOF)	{
	user_plain_text[k++]=(char)current_c;
	if(k == current_size)
		user_plain_text = realloc(user_plain_text, (current_size=current_size*2));
	user_plain_text [k]='\0';}	

  while(k%8) user_plain_text [k++]='\0';	
  unsigned int data_size =  k / 8 ;
  
 unsigned long int * plaintext = (unsigned long int *)user_plain_text ;
 unsigned char *x = (unsigned char*)plaintext;

 printf("Plain text blocs (HEX) : ");
 for(l=0;l<data_size;l++) printf("%lx ",plaintext[l]); printf("\n");	

 unsigned char *user_ad = malloc(256);
 unsigned int current_ad_size = 256;
 k=0;

 printf("Aditonal Data : "); 	
 while (( current_c = getchar() ) != '\n' && current_c != EOF)	{
	user_ad[k++]=(unsigned char)current_c;
	if(k == current_ad_size)
		user_ad = realloc(user_ad, (current_ad_size=current_ad_size*2));
	 user_ad [k]=0;	 }
	 while(k%8) user_ad [k++]=0;
	 unsigned int ad_size =  k  / 8;		
  

 unsigned long int * ad = (unsigned long int *)user_ad ;

  printf("Aditonal Data blocs (HEX) : ");
 for(l=0;l<ad_size;l++) printf("%lx ",ad[l]);
 printf("\n");
	
 unsigned int iv;
 char iv_buffer[24];
 lab_iv :
 printf("Initialization Vector (IV) : "); 
 
 scanf("%s",iv_buffer);
 for(l=0; iv_buffer[l]!='\0';l++)  {
	if( iv_buffer[l] <= '9' &&   iv_buffer[l] >= '0') goto lab_get_iv;
	printf("IV should be a positive Integer\n");
	goto lab_iv;
	lab_get_iv: 
	iv = 10*iv + iv_buffer[l] - 48; }
printf("\n*************Result Of Encryption and Decryption**************\n\n");	
 struct GCMEncryptedData *encrypted_data = GCMEncrypt(iv,plaintext,data_size,key,ad,ad_size);

  unsigned long int *origin=GCMDecrypt(iv,encrypted_data,key);
  char *ciphertext = (char *)encrypted_data->data;	
  printf("Ciphertext : %s  \nCiphertext blocs (HEX) : ",ciphertext); 
  for(l=0;l<data_size;l++) printf("%lx ",encrypted_data->data[l]);
  printf("\n");	
  char *origin_text = (char*) origin;
  if(origin == NULL) goto gmac_error;	
  printf("Original Text (After Decryption) : %s \nOriginal Text blocs (After Decryption, HEX) : ",origin_text); 
  for(l=0;l<data_size;l++) printf("%lx ",origin[l]);
  printf("\n");	

 char *ad_text  = (char*) encrypted_data->ad;
 printf("Additonal Data : %s \nAdditonal Data (HEX) : ",ad_text); 
 for(l=0;l<ad_size;l++) printf("%lx ",encrypted_data->ad[l]);
 printf("\nGMAC : %lx\n",encrypted_data->gmac);
 goto end;  
 gmac_error : 
 printf("Wrong GMAC : deciphered text will not be displayed \n");
 end: 
 printf("\n\n********************************************************************************\n\t\t\t\tBye ;-) \n********************************************************************************\n\n"); 
return EXIT_SUCCESS;
}
