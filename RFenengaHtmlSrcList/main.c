//
//  main.c
//  RFenengaHtmlSrcList
//
//  Created by Russ Fenenga on 10/22/14.
//  Copyright (c) 2014 school. All rights reserved.
//

#include <stdio.h>
#include <stdbool.h>
#include <curl/curl.h>

void getHtmlData();
void getCount();
void getTags();
void getUrls();
void getFreq();



char htmlData[256*1024];
char URL[BUFSIZ+1];

char tagsArrays[BUFSIZ][BUFSIZ];
char countArray[BUFSIZ]={0};

char *pSrc;
FILE *fp;
int totalTagCount=0;
int main() {
    char inputChar;
    bool quit = false;
    setvbuf(stdout, NULL, _IONBF, 0);
    printf("URL:\n");
    gets(URL);
    
    getHtmlData(URL);
    while(!quit){
        printf("Ready\n");
        inputChar = getchar();
        getchar();  //consumes extra newline left over by getchar.
        switch (inputChar) {
            case 'c':
                getCount();
                break;
            case 't':
                getTags();
                break;
            case 'u':
                getUrls();
                break;
            case 'f':
                getFreq();
                break;
            case 'q':
                quit = true;
                printf("Complete");
                break;
            default:
                printf("Invalid character input entering self destruct sequence.");
                break;
        }
        
    }
    return 0;
}

void getHtmlData(){
    char temp[BUFSIZ];
    char string1[BUFSIZ] = "curl -s ";
    strcat(string1, URL);
    fp = popen(string1, "r");
    while(fgets(temp,BUFSIZ, fp)!= NULL){
        strcat(htmlData, temp);
    }
    pclose(fp);
}

void getCount(){
    char *pTemp = htmlData;
    int count;
    while(strstr(pTemp, "src=\"")){
        pTemp = strstr(pTemp, "src=\"");
        if(isspace(*(pTemp-1))){
            count++;
        }
        pTemp+=5;
    }
    printf("%d\n",count);
}
void getUrls(){
    char *pTemp = htmlData;
    char tempURL[BUFSIZ];
    while(strstr(pTemp, "src=\"")){
        pTemp = strstr(pTemp, "src=\"");
        char *pQ1 = pTemp;
        char *pQ2;
        if(isspace(*(pTemp-1))){                //check if it is a valid tag as that is all we want to compute urls for.
            while(*pQ1!='\"'){
                pQ1++;
            }
            pQ2 = pQ1+1;
            while(*pQ2!='\"'){
                pQ2++;
            }
            long len = pQ2 - (pQ1+1);
            strncpy(tempURL,pQ1+1,len);
            tempURL[len]= '\0';
            printf("%s\n",tempURL);
        }
        pTemp+=5;
    }
}
void getTags(){
    char *pTemp = htmlData;
    char tempType[BUFSIZ];
    while(strstr(pTemp, "src=\"")){
        pTemp = strstr(pTemp, "src=\"");
        char *pQ1 = pTemp;
        char *pQ2;
        if(isspace(*(pTemp-1))){
            
            while(*pQ1!='<'){
                pQ1--;
            }
            pQ2 = pQ1;
            while(!isspace(*pQ2)){
                pQ2++;
            }
            
            long len = (pQ2-1)-pQ1;
            strncpy(tempType, pQ1+1, len);
            tempType[len] = '\0';
            printf("%s\n",tempType);
        }
        pTemp+=5;
    }
}
void getFreq(){
    char *pTemp = htmlData;
    char tempType[BUFSIZ];
    while(strstr(pTemp, "src=\"")){
        pTemp = strstr(pTemp, "src=\"");
        char *pQ1 = pTemp;
        char *pQ2;
        if(isspace(*(pTemp-1))){
            
            while(*pQ1!='<'){
                pQ1--;
            }
            pQ2 = pQ1;
            while(!isspace(*pQ2)){
                pQ2++;
            }
            
            long len = (pQ2-1)-pQ1;
            strncpy(tempType, pQ1+1, len);
            tempType[len] = '\0';
            int i = 0;
            totalTagCount++;
            while(i<totalTagCount){
                printf("the value of tag count is %d\n",totalTagCount);
                printf("the value of our tag is %s\n",tempType);
                if(strcmp(tagsArrays[i],tempType)!=0){               //if the strings are not equal the if gets executed
                    i++;
                }else{                                              //else they have to be equal.
                    int temp = countArray[i];
                    countArray[i] = ++temp;
                    break;
                }
                
            }
            if(i==totalTagCount){
                strcpy(tagsArrays[i-1],tempType);
                countArray[i-1]=1;
                printf("The value in tagsArray of %d is %s\n",i-1,tagsArrays[i-1]);
                printf("The value in count Array of %d is %d\n",i-1,countArray[i-1]);
            }
        }
        pTemp+=5;
    }
    int i =0;
    while(i<totalTagCount){
        printf("%s %d\n",tagsArrays[i],countArray[i]);
        i++;
    }
}