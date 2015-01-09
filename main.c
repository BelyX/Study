/********************************************************************/
/* Copyright (C) MC2Lab-USTC, 2012                                  */
/*                                                                  */
/*  FILE NAME             :  main.c                                 */
/*  PRINCIPAL AUTHOR      :  sunmingming                              */
/*  SUBSYSTEM NAME        :  Main                                   */
/*  MODULE NAME           :  Main                                   */
/*  LANGUAGE              :  C                                      */
/*  TARGET ENVIRONMENT    :  ANY                                    */
/*  DATE OF FIRST RELEASE :  2012/6/9                              */
/*  DESCRIPTION           :  implement of NoSQL DB Test             */
/********************************************************************/

#include "common.h"
#include "api.h" /* only call this API in TestSystem */
#include "dbtime.h"
#include <string.h>
#include <time.h>

#define ID_NUM   100
#define KEYS_NUM  5000
#define N   ID_NUM*KEYS_NUM
#define MAX_RANDOM_SIZE 500000

static char tempname[100] = {'\0'};

int readnum = 1;//the number of tables that are  sequence read
int writenum = 1;//the number of tables that are  sequence write
int keynum = 100000;//the time when writed only one key, also the time when read only one key 
int number = 1000000;//the number of each cycle in the case of the rate of read and  write 
int ratenum = 1; //the total number of cycle
int casnum = 10000;


/*
 * Read the Data Randomly
 * input	: NumOfRecords
 * output	: Assessment Report
 * in/out	: None
 * return	: SUCCESS(0)/FAILURE(-1)
 */
int RandRead(int * randlist, int NumOfRecords,int maxrandsize);
int Cached(int * randlist, int NumOfRecords,int maxrandsize);
/*
 * Read the Data Sequentially
 * input	: id,basekey,NumOfRecords
 * output	: Assessment Report
 * in/out	: None
 * return	: SUCCESS(0)/FAILURE(-1)
 */
int SeqRead(tTableId id, int keyRange);


/*
 * Search only one key
 * input	: id, key,keyNum
 * output	: Assessment Report
 * in/out	: None
 * return	: SUCCESS(0)/FAILURE(-1)
 */
int SearchOnekey(tTableId id, tRowKey * key, int num);

int Cached(int * randlist, int NumOfRecords,int maxrandsize)
{
	int i,j,rc;
	tTableId id;
	tRecord *pRecord[1];
	tRowKey keys[1];
	for(i = 0;i < (ID_NUM*maxrandsize/10);i++)
	{
        id = i;//randlist[i%maxrandsize]%ID_NUM;
        for(j = 1; j <= KEYS_NUM;j++)	
        {	
    		keys[0] =  j;//randlist[i%maxrandsize]%KEYS_NUM;
    		DEBUG_PRINTF2("Random num id = %d��key = %d\n",id,keys[0]);
    		rc=GetRecords(id,keys,pRecord,1);
    		if(rc==FAILURE)
    		{
    			printf("\nRandon Read Records Error\n");
    			return FAILURE;
    		} 
    		DEBUG_PRINTF3("Table:  %d,rowkey:  %d,index[0]:    %d\n",id,(unsigned int)keys[0],pRecord[0]->index[0]); 
	    }
	}
	return SUCCESS;

}

int RandRead(int * randlist, int NumOfRecords,int maxrandsize)
{
	int i,rc;
	tTableId id;
	tRecord *pRecord[1];
	tRowKey keys[1];
	for(i = 0;i < NumOfRecords;i++)
	{
        id = rand()%(ID_NUM*maxrandsize/10);//randlist[i%maxrandsize]%ID_NUM;		
		keys[0] =  rand()%KEYS_NUM;//randlist[i%maxrandsize]%KEYS_NUM;
		DEBUG_PRINTF2("Random num id = %d��key = %d\n",id,keys[0]);
		rc=GetRecords(id,keys,pRecord,1);
		if(rc==FAILURE)
		{
			printf("\nRandon Read Records Error\n");
			return FAILURE;
		} 
		DEBUG_PRINTF3("Table:  %d,rowkey:  %d,index[0]:    %d\n",id,(unsigned int)keys[0],pRecord[0]->index[0]); 
	}
	return SUCCESS;

}

int SeqRead(tTableId id, int keyRange)
{
	int i,j,rc;
	tRecord *rcvdatapoints[1];
	tRowKey key[1];
	tRowKey keystart ;
	j = KEYS_NUM - keyRange;
	if(j == 0)
	{
		keystart = 1;
	}
	else
	{
		keystart = rand()%j;
	}
	key[0] = keystart;	
	for(i = 0; i < keyRange; i++)	
	{
		rc = GetRecords(id,key,rcvdatapoints,1);
		if(rc == FAILURE){
			DEBUG_PRINTF("\nSequence Read Records Error\n");
			return FAILURE;
		}
		key[0]++;  
	}		
	return SUCCESS;
}



int SearchOnekey(tTableId id, tRowKey * key, int num)
{
	int i,rc;		
	tRecord * record[1];	
	for(i = 0; i < num; i++)
	{
		rc=GetRecords(id,key,record,1);
		if(rc==FAILURE){
			DEBUG_PRINTF("\nRead one Records Error\n");
			return FAILURE;
		}
	}
 	return SUCCESS;  
}



int main(int argc, char **argv)
{
    srand((unsigned)time(NULL));
    tTableId id = 0;
	tRowKey key[1];
	int i;

	//for dbtime filename 
	if (argv[0][0] == '.')
        dbtime_filename = argv[0]+2;
    else
        dbtime_filename = argv[0];
    if (argc>=2)
        dbtime_filename = argv[1];
    else{
	};

	//read random data from random.data
	int *pchBuffer = NULL;
	FILE *fp = fopen("./randfile/random.data","r");
    if(fp == NULL )
    {
    	printf("Error! Not Find ./randfile/random.data\n"); 
    	return -1;
    }
	pchBuffer = (int *)malloc(sizeof(int) *N);
	for(i = 0; i< N; i++)
	{
		fscanf(fp, "%d\n", &pchBuffer[i]);
	}
	fclose(fp);

    /* test start here */
    int MaxRecords = N;    
    while(MaxRecords > 0)
    {
        char filename[128];
        sprintf(filename,"%d.hdb",MaxRecords);
    	//new database
    	strcpy (tempname, "Creating ");
        strcat (tempname, filename);
        dbtime_startTest (tempname);
        NewDatabase(filename,MaxRecords);   
        dbtime_endAndShow ();
    
    	int MaxErea = 1;
        for(i=0;i<10;i++)
        {
        	/*
        	*performance test read 1)i
        	*random read N records 
        	*/
        	DEBUG_PRINTF("\nCached all Records!%d\n");
            strcpy (tempname, "CachedRead ");
            {
                char a[100];
                sprintf (a, "CachedRead %d/10", MaxErea);
                strcat (tempname, a);
            }
            dbtime_startTest (tempname);
        	if(Cached(pchBuffer,N,MaxErea) == FAILURE)
        	{
        		printf("Random Read Records Error!\n");
        		return -1;
        	}
            dbtime_endAndShow (); 
        	DEBUG_PRINTF1("\nGet Random Records!%d\n",i);
            strcpy (tempname, "RandomRead ");
            {
                char a[100];
                sprintf (a, "%d times in %d/10", N,MaxErea);
                strcat (tempname, a);
            }            
            dbtime_startTest (tempname);
        	if(RandRead(pchBuffer,N,MaxErea) == FAILURE)
        	{
        		printf("Random Read Records Error!\n");
        		return -1;
        	}
            dbtime_endAndShow ();             
            MaxErea += 1;
    	}
#if 1    	    	
    	/*
    	*performance test read 1)ii
    	*sequence read KEYS_NUM  records
    	*run readnum times
    	*/
    
        DEBUG_PRINTF("\nGet Sequence Records!\n");
        strcpy (tempname, "SeqRead ");
        {
            char a[100];
            sprintf (a, "%d", KEYS_NUM*readnum);
            strcat (tempname, a);
        }
        dbtime_startTest (tempname);
    	for(i = 0; i < readnum; i++)
    	{
            id = pchBuffer[i]%ID_NUM;
            if(SeqRead(id,KEYS_NUM)==FAILURE)
    		{
    			DEBUG_PRINTF("Sequence Read Records Error!\n");
    		}
    	}
        dbtime_endAndShow ();	
    
    	/*
    	*performance test  4)
    	*lookup only one key (the first data in random.data)
    	*run keynum time 
    	*/
    	DEBUG_PRINTF("\n lookup only one key!\n");  
    	strcpy (tempname, "LookUpOne ");
        {
            char a[100];
            sprintf (a, "%d", keynum);
            strcat (tempname, a);
        }
        dbtime_startTest (tempname); 
    	id = pchBuffer[1]%ID_NUM;		
    	key[0] =  pchBuffer[1]%KEYS_NUM;	
    	if(SearchOnekey(id, key ,keynum) == FAILURE)
    	{
    		DEBUG_PRINTF("Lookup only one key Error!\n");
    	} 
    	   	
        dbtime_endAndShow (); 
#endif   
        DropDatabase();
        MaxRecords -= N/10;
    }
    //finalization.
    dbtime_finalize ();

    return 0;
}
