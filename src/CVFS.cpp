///////////////////////////////////////////////////////////////////////
//
//  File Name   : CVFS.cpp
//
//  Description  : This project emulates file system functionality to create a virtual 
//                 environment where users can perform file operations without 
//                 interacting with a real disk
//
//  Author      : Rekha Shankarlal Kumawat
//
//  Date        : 13/01/2026
//
///////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Header File Inclusion
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<string.h>

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  User Defined Macros
//
///////////////////////////////////////////////////////////////////////////////////////////////////

// maximum file size that we allow in the project
#define MAX_FILE_SIZE 50

// maximum Number of file we can opened once
#define MAX_OPEN_FILES 20

//Maximum number of file can be created once
#define MAX_INODE 5

// Permissionof file
#define READ 1
#define WRITE 2
#define EXECUTE 4

// Like Lseek
#define START 0
#define CURRENT 1
#define END 2

// on Success return value
#define EXECUTE_SUCCESS 0

// type of file 
#define REGULAR_FILE 1
#define SPECIAL_FILE 2

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  User Defined Macros for error handling
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#define ERR_INVALID_PARAMETER -1

#define ERR_NO_INODES -2

#define ERR_FILE_ALREADY_EXIST -3
#define ERR_FILE_NOT_EXIST -4

#define ERR_PERMISSION_DENIED -5

#define ERR_INSUFFICIENT_SPACE -6
#define ERR_INSUFFICIENT_DATA -7

#define ERR_MAX_FILES_OPEN -8

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  User Defined Structures
//
///////////////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name :    BootBlock
//  Description :       Holds the information to boot the OS
//
///////////////////////////////////////////////////////////////////////////////////////////////////

struct BootBlock
{
    char Information[100];
    
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name :    SuperBlock
//  Description :       Holds the information about the file system
//
///////////////////////////////////////////////////////////////////////////////////////////////////

struct SuperBlock
{
    int TotalInodes;
    int FreeInodes;
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name :    Inode
//  Description :       Holds the information about file
//
///////////////////////////////////////////////////////////////////////////////////////////////////

#pragma pack(1)
struct Inode
{
    char FileName[20];
    int InodeNumber;
    int FileSize;
    int ActualFileSize;
    int FileType;
    int ReferenceCount;
    int Permission;
    char *Buffer;
    struct Inode *next;
};

typedef struct Inode INODE;
typedef struct Inode * PINODE;
typedef struct Inode ** PPINODE;

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name :    FileTable
//  Description :       Holds the information about opened file
//
///////////////////////////////////////////////////////////////////////////////////////////////////

struct FileTable                            // opened files
{
    int ReadOffset;
    int WriteOffset;
    int Mode;
    PINODE ptrinode;
};

typedef FileTable FILETABLE;
typedef FileTable * PFILETABLE;

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Structure Name :    UAREA
//  Description :       Holds the information about process file
//
///////////////////////////////////////////////////////////////////////////////////////////////////

struct UAREA
{
    char ProcessName[20];
    PFILETABLE UFDT[MAX_OPEN_FILES];
};

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Global variables or objects used in the project
//
///////////////////////////////////////////////////////////////////////////////////////////////////

BootBlock bootobj;
SuperBlock superobj;
UAREA uareaobj;

PINODE head = NULL;

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     InitialiseUAREA
//  Description :       It is used to initialise UAREA members
//  Author :            Rekha Shankarlal Kumawat
//  Date :              13/01/2026
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void InitialiseUAREA()
{
   strcpy(uareaobj.ProcessName,"Myexe");
   
   int i = 0;

   for(i = 0; i < MAX_OPEN_FILES; i++)
   {
        uareaobj.UFDT[i] = NULL;
   }
    printf("Marvellous CVFS : UAREA gets initialised succesfully\n");

}// End of InitialiseUAREA function

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     InitialiseSuperBlock
//  Description :       It is used to initialise Super block members
//  Author :            Rekha Shankarlal Kumawat
//  Date :              13/01/2026
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void InitialiseSuperBlock()
{
    superobj.TotalInodes = MAX_INODE;
    superobj.FreeInodes = MAX_INODE;

    printf("Marvellous CVFS : Super block gets initialised succesfully\n");

}// End of InitialiseSuperBlock function

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     CreateDILB
//  Description :       It is used to create Linkedlist of inodes
//  Author :            Rekha Shankarlal Kumawat
//  Date :              13/01/2026
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void CreateDILB()
{
    int i = 1;
    PINODE newn = NULL;
    PINODE temp = head;

    for(i = 1; i <= MAX_INODE; i++)
    {
        newn = (PINODE)malloc(sizeof(INODE));

        strcpy(newn->FileName,"\0");
        newn->InodeNumber = i;
        newn->FileSize = 0;
        newn->ActualFileSize = 0;
        newn->FileType = 0;
        newn->ReferenceCount = 0;
        newn->Permission = 0;
        newn->Buffer = NULL;
        newn->next = NULL;

        if(temp == NULL)    // LL is empty
        {
            head = newn;
            temp = head;
        }
        else                // LL contains atleast 1 node
        {
            temp->next = newn;
            temp = temp->next;
        }
    }

    printf("Marvellous CVFS : DILB created succesfully\n");

}// End of CreateDILB function 

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     StartAuxillaryDataInitilisation
//  Description :       It is used to call all such functions which are
//                      used to initialise auxillary data
//  Author :            Rekha Shankarlal Kumawat
//  Date :              13/01/2026
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void StartAuxillaryDataInitilisation()
{
    strcpy(bootobj.Information,"Booting process of Marvellous CVFS is done");

    printf("%s\n",bootobj.Information);

    InitialiseSuperBlock();

    CreateDILB();

    InitialiseUAREA();

    printf("Marvellous CVFS : Auxillary data initialised succesfully\n");

}// End of StartAuxillaryDataInitilisation function

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     DisplayHelp
//  Description :       It is used to display the help page
//  Author :            Rekha Shankarlal Kumawat
//  Date :              14/01/2026
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void DisplayHelp()
{
    printf("-----------------------------------------------\n");
    printf("---------- Marvellous CVFS Help Page ----------\n");
    printf("-----------------------------------------------\n");

    printf("man    : It is used to display the specific manual page of command\n");
    printf("clear  : It is used to clear the console of Marvellous CVFS\n");
    printf("creat  : It is used to create new regular file\n");
    printf("write  : It is used to write the data into the file\n");
    printf("read   : It is used to read the data from the file\n");
    printf("stat   : It is used to display statistical information about file\n");
    printf("unlink : It is used to delete existing the file\n");
    printf("exit   : It is used to terminate the shell of Marvellous CVFS\n");
    printf("ls     : It is used to list out all files fromm the directory\n");


    printf("-----------------------------------------------\n");

} // end of DisplayHelp function

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     ManPageDisplay
//  Description :       It is used to display man page
//  Author :            Rekha Shankarlal Kumawat
//  Date :              14/01/2026
//
///////////////////////////////////////////////////////////////////////////////////////////////////

void ManPageDisplay(
                    char Name[]                // Name of Command
                    )
{
    FILE * file  ; 
    char Buffer[1024];
    int Bytesread = 0;

    if(strcmp("ls",Name) == 0)
    {
        file = fopen("ls.txt" , "r");

        while ((Bytesread = fread(Buffer , 1 ,sizeof(Buffer) , file)) > 0)
        {
            fwrite(Buffer , 1 , Bytesread , stdout);
        }
    }
    else if(strcmp("man",Name) == 0)
    {
        file = fopen("man.txt" , "r");

        while ((Bytesread = fread(Buffer , 1 ,sizeof(Buffer) , file)) > 0)
        {
            fwrite(Buffer , 1 , Bytesread , stdout);
        }       
    }
    else if(strcmp("exit",Name) == 0)
    {
        file = fopen("exit.txt" , "r");

        while ((Bytesread = fread(Buffer , 1 ,sizeof(Buffer) , file)) > 0)
        {
            fwrite(Buffer , 1 , Bytesread , stdout);
        }     
    }
    else if(strcmp("clear",Name) == 0)
    {
        file = fopen("clear.txt" , "r");

        while ((Bytesread = fread(Buffer , 1 ,sizeof(Buffer) , file)) > 0)
        {
            fwrite(Buffer , 1 , Bytesread , stdout);
        }       
    }
    else if(strcmp("stat",Name) == 0)
    {
        file = fopen("stat.txt" , "r");

        while ((Bytesread = fread(Buffer , 1 ,sizeof(Buffer) , file)) > 0)
        {
            fwrite(Buffer , 1 , Bytesread , stdout);
        }       
    }
    else if(strcmp("creat",Name) == 0)
    {
        file = fopen("creat.txt" , "r");

        while ((Bytesread = fread(Buffer , 1 ,sizeof(Buffer) , file)) > 0)
        {
            fwrite(Buffer , 1 , Bytesread , stdout);
        }       
    }
    else if(strcmp("write",Name) == 0)
    {
        file = fopen("write.txt" , "r");

        while ((Bytesread = fread(Buffer , 1 ,sizeof(Buffer) , file)) > 0)
        {
            fwrite(Buffer , 1 , Bytesread , stdout);
        }       
    }
    else if(strcmp("unlink",Name) == 0)
    {
        file = fopen("unlink.txt" , "r");

        while ((Bytesread = fread(Buffer , 1 ,sizeof(Buffer) , file)) > 0)
        {
            fwrite(Buffer , 1 , Bytesread , stdout);
        }       
    }
    else
    {
        printf("No manual entry for %s\n",Name);
    }

}// End of ManPageDisplay function

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     IsFileExist
//  Description :       It is used to check whether file is already exist or not
//  Input :             It accepts file name
//  Output :            It returns the true or false
//  Author :            Rekha Shankarlal Kumawat
//  Date :              16/01/2026
//
///////////////////////////////////////////////////////////////////////////////////////////////////

bool IsFileExist(
                    char *name      // File name
                )
{
    PINODE temp = head;
    bool bFlag = false;

    while(temp != NULL)
    {
        if((strcmp(name,temp->FileName) == 0) && (temp->FileType == REGULAR_FILE))
        {
            bFlag = true;
            break;
        }
        temp = temp->next;
    }
    
    return bFlag;

}// end of IsFileExit function

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     CreateFile
//  Description :       It is used to create new regular file
//  Input :             It accepts file name and permissions
//  Output :            It returns the file descriptor
//  Author :            Rekha Shankarlal Kumawat
//  Date :              16/01/2026
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int CreateFile(
                    char *name,         // Name of new file
                    int permission      // Permission for that file
                )
{
    PINODE temp = head;
    int i = 0;

    printf("Total number of Inodes remaining : %d\n",superobj.FreeInodes);

    // If name is missing
    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    // If the permission value is wrong
    // permission -> 1 -> READ
    // permission -> 2 -> WRITE
    // permission -> 3 -> READ + WRITE
    if(permission < 1 || permission > 3)
    {
        return ERR_INVALID_PARAMETER;
    }

    // If the inodes are full
    if(superobj.FreeInodes == 0)
    {
        return ERR_NO_INODES;
    }

    // If file is already present
    if(IsFileExist(name) == true)
    {
        return ERR_FILE_ALREADY_EXIST;
    }

    // Search empty Inode
    while(temp != NULL)
    {
        if(temp -> FileType == 0)
        {
            break;    
        }
        temp = temp -> next;
    }
    
    if(temp == NULL)
    {
        printf("There is no inode\n");
        return ERR_NO_INODES;
    }

    // Search for empty UFDT entry
    // Note : 0,1,2 are reserved
    for(i = 3; i < MAX_OPEN_FILES; i++)
    {
        if(uareaobj.UFDT[i] == NULL)
        {
            break;
        }
    }

    // UFDT is full
    if(i == MAX_OPEN_FILES)
    {
        return ERR_MAX_FILES_OPEN;
    }

    // Allocate ememory for file table
    uareaobj.UFDT[i] = (PFILETABLE)malloc(sizeof(FILETABLE));

    // Initialise File table
    uareaobj.UFDT[i]->ReadOffset = 0;
    uareaobj.UFDT[i]->WriteOffset = 0;
    uareaobj.UFDT[i]->Mode = permission;
    
    // Connect File table with Inode
    uareaobj.UFDT[i]->ptrinode = temp;

    // Initialise elements of Inode
    strcpy(uareaobj.UFDT[i]->ptrinode->FileName,name);
    uareaobj.UFDT[i]->ptrinode->FileSize = MAX_FILE_SIZE;
    uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;
    uareaobj.UFDT[i]->ptrinode->FileType = REGULAR_FILE;
    uareaobj.UFDT[i]->ptrinode->ReferenceCount = 1;
    uareaobj.UFDT[i]->ptrinode->Permission = permission;

    // Allocate ememory for files data
    uareaobj.UFDT[i]->ptrinode->Buffer = (char *)malloc(MAX_FILE_SIZE);

    superobj.FreeInodes--;

    return i;   // File descriptor

}// end of CreateFile function

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     LsFile()
//  Description :       It is used to list all files
//  Input :             Nothing
//  Output :            Nothing
//  Author :            Rekha Shankarlal Kumawat
//  Date :              16/01/2026
//
///////////////////////////////////////////////////////////////////////////////////////////////////

// ls -l
void LsFile()
{
    PINODE temp = head;

    printf("-----------------------------------------------\n");
    printf("------ Marvellous CVFS Files Information ------\n");
    printf("-----------------------------------------------\n");

    while(temp != NULL)
    {
        if(temp -> FileType != 0)
        {
            printf("%d\t%s\t%d\n",temp->InodeNumber,temp->FileName,temp->ActualFileSize);
        }
        
        temp = temp -> next;
    }
    
    printf("-----------------------------------------------\n");

}// end of LsFile function

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     StatFile()
//  Description :       It is used to Display the information abou the given files
//  Input :             filename
//  Output :            Nothing
//  Author :            Rekha Shankarlal Kumawat
//  Date :              16/01/2026
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int StatFile(
                char *name                  // Name of file
            )
{
    PINODE temp = head ;

    // Filters

    // Name not provided of the file
    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    // File does not Exit
    if(IsFileExist(name) == false)
    {
        return ERR_FILE_NOT_EXIST;
    }

    while(temp != NULL)
    {
        if((strcmp(name , temp->FileName) == 0) && (temp->FileType != 0))
        {
            printf("-------------Statistical Information of File-------------------\n");
            printf("File Name :- %s\n",temp->FileName);
            printf("File Size on Disk :- %d\n",temp->FileSize);
            printf("Actual File Name :- %d\n",temp->ActualFileSize);
            printf("Reference Count :- %d\n",temp->ReferenceCount);

            printf("File permission :- ");

            if(temp->Permission == READ)
            {
                printf("READ\n");
            }
            else if (temp->Permission == WRITE)
            {
                printf("WRITE\n");
            }
            else if (temp->Permission == READ + WRITE)
            {
                printf("READ + WRITE\n");
            }
            
            printf("File Type :- ");
            if (temp->FileType == REGULAR_FILE)
            {
                printf("Regular File\n");
            }
            else if (temp->FileType == SPECIAL_FILE)
            {
                printf("Special File\n");
            }

            printf("-------------------------------------------------------------------\n");
        }

        temp = temp->next ;
    }

    return EXECUTE_SUCCESS;
}
///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     UnlinkFile()
//  Description :       It is used to delete the file
//  Input :             File name
//  Output :            Nothing
//  Author :            Rekha Shankarlal Kumawat
//  Date :              22/01/2026
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int UnlinkFile(
                    char *name
              )
{
    int i = 0;

    if(name == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(IsFileExist(name) == false)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // Travel the UFDT
    for(i = 0; i < MAX_OPEN_FILES; i++)
    {
        if(uareaobj.UFDT[i] != NULL)
        {
            if(strcmp(uareaobj.UFDT[i]->ptrinode->FileName, name) == 0)
            {
                // Deallocate memory of Buffer
                free(uareaobj.UFDT[i]->ptrinode->Buffer);
                uareaobj.UFDT[i]->ptrinode->Buffer = NULL;

                // Reset all values of inode
                // Dont deallocate memmory of inode
                uareaobj.UFDT[i]->ptrinode->FileSize = 0;
                uareaobj.UFDT[i]->ptrinode->ActualFileSize = 0;
                uareaobj.UFDT[i]->ptrinode->FileType = 0;
                uareaobj.UFDT[i]->ptrinode->ReferenceCount = 0;
                uareaobj.UFDT[i]->ptrinode->Permission = 0;

                memset(uareaobj.UFDT[i]->ptrinode->FileName, '\0', sizeof(uareaobj.UFDT[i]->ptrinode->FileName));

                // Dealloacte memory of file table
                free(uareaobj.UFDT[i]);

                // Set NULL to UFDT
                uareaobj.UFDT[i] = NULL;

                // // Increment free inodes count
                superobj.FreeInodes++;

                break;  // IMP
            }   // End of if
        }       // End of if
    }           // End of for

    return EXECUTE_SUCCESS;

} // End of UnLinkFile function

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Function Name :     WriteFile()
//  Description :       It is used to write the data into the file
//  Input :             File descriptor
//                      Address of buffer which contains data
//                      Size of data that we want to write
//  Output :            Number of bytes succesfully written
//  Author :            Rekha Shankarlal Kumawat
//  Date :              22/01/2026
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int WriteFile(
                    int fd,
                    char *data,
                    int size
            )
{
    printf("File descriptor : %d\n",fd);
    printf("Data that we want to write : %s\n",data);
    printf("Number of bytes that we want to write : %d\n",size);

    // Invalid FD
    if(fd < 0 || fd > MAX_OPEN_FILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    // FD points to NULL
    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // There is no permission to write
    if(uareaobj.UFDT[fd]->ptrinode->Permission < WRITE)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Insufficient space
    if((MAX_FILE_SIZE - uareaobj.UFDT[fd]->WriteOffset) < size)
    {
        return ERR_INSUFFICIENT_SPACE;
    }

    // Write the data into the file
    strncpy(uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]->WriteOffset, data, size);

    // Update the write offset
    uareaobj.UFDT[fd]->WriteOffset = uareaobj.UFDT[fd]->WriteOffset + size;

    // Update the actual file size
    uareaobj.UFDT[fd]->ptrinode->ActualFileSize = uareaobj.UFDT[fd]->ptrinode->ActualFileSize + size;

    return size;

}// end of WriteFile function

//////////////////////////////////////////////////////////
//
//  Function Name :     ReadFile()
//  Description :       It is used to read the data from the file
//  Input :             File descriptor
//                      Address of empty buffer
//                      Size of data that we want to read
//  Output :            Number of bytes succesfully read
//  Author :            Piyush Manohar Khairnar
//  Date :              22/01/2026
//
//////////////////////////////////////////////////////////

int ReadFile(
                int fd,
                char *data,
                int size
            )
{

    //  Invaid FD
    if(fd < 0 || fd > MAX_OPEN_FILES)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(data == NULL)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(size <= 0)
    {
        return ERR_INVALID_PARAMETER;
    }

    if(uareaobj.UFDT[fd] == NULL)
    {
        return ERR_FILE_NOT_EXIST;
    }

    // Filter for permission
    if(uareaobj.UFDT[fd]->ptrinode->Permission < READ)
    {
        return ERR_PERMISSION_DENIED;
    }

    // Insuuficeint data
    if((MAX_FILE_SIZE - uareaobj.UFDT[fd]->ReadOffset) < size)
    {
        return ERR_INSUFFICIENT_DATA;
    }

    // Read the data
    strncpy(data,uareaobj.UFDT[fd]->ptrinode->Buffer + uareaobj.UFDT[fd]->ReadOffset, size);

    // Update the read offset
    uareaobj.UFDT[fd]->ReadOffset = uareaobj.UFDT[fd]->ReadOffset + size;

    return size;
}// End of ReadFile function

///////////////////////////////////////////////////////////////////////////////////////////////////
//
//  Entry Point function of the project
//
///////////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    char str[80] = {'\0'};
    char Command[5][20] = {{'\0'}};
    char InputBuffer[MAX_FILE_SIZE] = {'\0'};

    char *EmptyBuffer = NULL;

    int iCount = 0;
    int iRet = 0;

    StartAuxillaryDataInitilisation();

    printf("-----------------------------------------------\n");
    printf("----- Marvellous CVFS started succesfully -----\n");
    printf("-----------------------------------------------\n");
    
    // Infinite Listening Shell
    while(1)
    {
        fflush(stdin);

        strcpy(str,"");

        printf("\nMarvellous CVFS : > ");
        fgets(str,sizeof(str),stdin);
        
        iCount = sscanf(str,"%s %s %s %s %s",Command[0],Command[1],Command[2],Command[3], Command[4]);

        fflush(stdin);

        if(iCount == 1)
        {
            // Marvellous CVFS : > exit
            if(strcmp("exit",Command[0]) == 0)
            {
                printf("Thank you for using Marvellous CVFS\n");
                printf("Deallocating all the allocated resources\n");

                break;
            }
            // Marvellous CVFS : > ls
            else if(strcmp("ls",Command[0]) == 0)
            {
                LsFile();
            }
            // Marvellous CVFS : > help
            else if(strcmp("help",Command[0]) == 0)
            {
                DisplayHelp();
            }
            // Marvellous CVFS : > clear
            else if(strcmp("clear",Command[0]) == 0)
            {
                #ifdef _WIN32
                    system("cls");
                #else
                    system("clear");
                #endif
            }
        } // End of else if 1

        else if(iCount == 2)
        {
            // Marvellous CVFS : > man ls
            if(strcmp("man",Command[0]) == 0)
            {
                ManPageDisplay(Command[1]);
            }
            // Marvellous CVFS : > unlink Demo.txt
            else if(strcmp("unlink",Command[0]) == 0)
            {
                iRet = UnlinkFile(Command[1]);
            
                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameter\n");
                }

                if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : Unable to delete as there is no such file");
                }

                if(iRet == EXECUTE_SUCCESS)
                {
                    printf("File gets succesfully deleted\n");
                }
            }
            // Marvellous CVFS > stat Demo.txt

            else if(strcmp(Command[0], "stat") == 0)
            {
                iRet = StatFile(Command[1]);

                if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : Unable to display statistics as file is not present\n");
                }
                else if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameters for the function\n");
                    printf("Please check Man page for more details\n");
                }
            }
            // Marvellous CVFS : > write 2
            else if(strcmp("write",Command[0]) == 0)
            {
                printf("Enter the data that you want to write : \n");
                fgets(InputBuffer,MAX_FILE_SIZE,stdin);

                iRet = WriteFile(atoi(Command[1]), InputBuffer, strlen(InputBuffer)-1);
            
                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameters \n");
                }
                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : There is no such file\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : Unable to write as there is no permission\n");
                }
                else if(iRet == ERR_INSUFFICIENT_SPACE)
                {
                    printf("Error : Unable to write as there is no space\n");
                }
                else
                {
                    printf("%d bytes gets succesfully written\n",iRet);
                }
            }
            else
            {
                printf("There is no such command\n");
                printf("Please refer HELP option or use man command\n");

            }
        } // End of else if 2
        else if(iCount == 3)
        {
            // Marvellous CVFS : > creat Ganesh.txt 3
            if(strcmp("creat",Command[0]) == 0)
            {
                iRet = CreateFile(Command[1],atoi(Command[2]));

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Unable to create the file as parameters are invalid\n");
                    printf("Please refer man page\n");
                }

                if(iRet == ERR_NO_INODES)
                {
                    printf("Error : Unable to create file as there is no inode\n");
                }

                if(iRet == ERR_FILE_ALREADY_EXIST)
                {
                    printf("Error : Unable to create file because the file is already present\n");
                }

                if(iRet == ERR_MAX_FILES_OPEN)
                {
                    printf("Error : Unable to create file\n");
                    printf("Max opened files limit reached\n");
                }

                printf("File gets succesfully created with FD %d\n",iRet);
            } 

            // Marvellous CVFS : > read 3 10
            else if(strcmp("read",Command[0]) == 0)
            {
                EmptyBuffer = (char *)malloc(sizeof(atoi(Command[2])));

                iRet = ReadFile(atoi(Command[1]), EmptyBuffer, atoi(Command[2]));

                if(iRet == ERR_INVALID_PARAMETER)
                {
                    printf("Error : Invalid parameter\n");
                }
                else if(iRet == ERR_FILE_NOT_EXIST)
                {
                    printf("Error : File not exist\n");
                }
                else if(iRet == ERR_PERMISSION_DENIED)
                {
                    printf("Error : Permission denied\n");
                }
                else if(iRet == ERR_INSUFFICIENT_DATA)
                {
                    printf("Error : Insufficient data\n");
                }
                else
                {
                    printf("Read operation is succesful\n");
                    printf("Data from file is : %s\n",EmptyBuffer);

                    free(EmptyBuffer);
                }
            }
            else
            {
                printf("There is no such command\n");
                printf("Please refer HELP option or use man command\n");
            }
            
        } // End of else if 3
        else if(iCount == 4)
        {

        } // End of else if 4
        else
        {
            printf("Command not found\n");
            printf("Please refer help option to get more information\n");
        } // End of else
    } // End of while

    return 0;
} // End of main
