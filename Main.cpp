#include <iostream>
#include <string>
using namespace std;


void main(int argc, char **argv)
{
    if (argc != 2)
    {
        cout<<"Usage:"<<endl;
        cout<<"DumpBin.exe [filename]"<<endl;
        return;;
    }

    char szInfile[256];
    strcpy_s(szInfile, argv[1]);

    FILE* fin = fopen(szInfile, "rb");

    fseek(fin, 0, SEEK_END);
    int nSize = ftell(fin);
    int nOff = 0;
    fseek(fin, 0, SEEK_SET);
    printf("Input file size %d bytes\n", nSize);
    
    unsigned char pbyBuff[10240];
    if (fread(pbyBuff, 1, nSize, fin) != nSize)
    {
        printf("Failed on read data\n");
        fclose(fin);
        return;
    }
    fclose(fin);

    printf("Read done!\n");

    int nDumpFormat = 0;
    printf("Choose dump data format:\n");
    printf("\t0 - 00 01 02 03 ...\n");
    printf("\t1 - 00, 01, 02, 03, ...\n");
    printf("\t2 - 0x00, 0x01, 0x02, 0x03, ...\n");
    printf("\t3 - 0x00, 0x02, 0x03, 0x03, ... // 0x10 \n");
    printf("\t4 - 0x00000010: 00 01 02 03 ...; abc...\n");
    printf("\t5 - 0x03020100,...\n");
    scanf("%d", &nDumpFormat);
    printf("\n");

    while (nSize > 0)
    {
        int cur = (nSize > 16) ? 16 : nSize;
        int i = 0;
        switch (nDumpFormat)
        {
            case 0:
            {
                for (; i < cur; i++)
                {
                    printf("%02X ", pbyBuff[nOff + i]);
                }
                break;
            }
            case 1:
            {
                for (; i < cur; i++)
                {
                    printf("%02X, ", pbyBuff[nOff + i]);
                }
                break;
            }
            case 2:
            {
                for (; i < cur; i++)
                {
                    printf("0x%02X, ", pbyBuff[nOff + i]);
                }
                break;
            }
            case 3:
            {
                for (; i < cur; i++)
                {
                    printf("0x%02X, ", pbyBuff[nOff + i]);
                }
                for (; i < 16; i++)
                {
                    printf("      ");
                }
                printf("// %04X", nOff);
                break;
            }
            case 4:
            {
                printf("0x%08X: ", nOff);
                for (; i < cur; i++)
                {
                    printf("%02X ", pbyBuff[nOff + i]);
                }
                for (; i < 16; i++)
                {
                    printf("   ");
                }
                printf(" ; ");
                for (i = 0; i < cur; i++)
                {
                    char chTemp = (char)pbyBuff[nOff + i];
                    if ( (chTemp < ' ') || (chTemp > 'z') )
                    {
                        chTemp = '.';
                    }
                    printf("%c", chTemp);
                }
                break;
            }
            case 5 :
            {
                for (; i < cur; i+=4)
                {
                    printf("0x%08X,\n", *(unsigned long*)(pbyBuff + nOff + i));
                }
                break;
            }
        }
        if (nDumpFormat != 5)
            printf("\n");
        nSize -= cur;
        nOff += cur;
    }

    printf("\nDUMP DONE!\n");

    system("pause");
}
