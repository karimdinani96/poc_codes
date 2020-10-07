#include <stdio.h>
#include <openssl/evp.h>
#include <string.h>
/*
 * Compile using gcc md5_evp.c -lcrypto -o md5sum
 */

int main(int argc, char *argv[])
{
		FILE *fp;
		size_t bytes;
		unsigned char buf[256];
		unsigned char *md5_result = NULL;
		EVP_MD_CTX *mdctx;
		const EVP_MD *md;
		unsigned char md_value[EVP_MAX_MD_SIZE];
		int md_len, i;

		if (argc < 2)
		{
				printf("Usage: md5_test <filename>\n");
				return 1;
		}

		fp = fopen(argv[1], "r");
		if (fp == NULL)
		{
				printf("Couldn't open file");
				return -1;
		}

		//get filesize
		//fseek(fp, 0L, SEEK_END);
		//long int filesize = ftell(fp);
		//fseek(fp, 0L, SEEK_SET);
		//buf=malloc(filesize)
		//fread(buf,1,filesize,fp)
		//fclose(fp);

		//md = EVP_get_digestbyname("md5");
		md = EVP_md5();
		if(!md) {
				printf("Unknown message digest %s\n", argv[1]);
				exit(1);
		}

		//allocates, initializes and returns a digest context.
		mdctx = EVP_MD_CTX_new();

		//sets up digest context ctx to use a digest type
		EVP_DigestInit_ex(mdctx, md, NULL);


		//for large files update in chunks
		//else copy update in one shot using filesize

		bytes=fread(buf, 1,256,fp); // reads 1 byte x 256
		while(bytes > 0)
		{
				//hashes "bytes" bytes of data at "buf" into the digest context "mdctx". 
				//This function can be called several times on the same ctx to hash additional data. 
				EVP_DigestUpdate(mdctx, buf, bytes);
				bytes=fread(buf, 1,256,fp); // reads 1 byte x 256
		}

		// retrieves the digest value from ctx and places it in md
		EVP_DigestFinal_ex(mdctx, md_value, &md_len);

		// cleans up digest context ctx and frees up the space allocated to it.
		EVP_MD_CTX_free(mdctx);

		printf("Digest is: ");
		for (i = 0; i < md_len; i++)
				printf("%02x", md_value[i]);

		printf("\n");

		return 0;
}
