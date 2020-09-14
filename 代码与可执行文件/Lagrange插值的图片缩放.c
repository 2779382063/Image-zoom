#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

typedef  unsigned char  U8;
typedef  unsigned short U16;
typedef  unsigned int   U32;

#pragma  pack(1)
typedef struct
{
    //bmp header
    U16 bfType;         // B  M
    U32 bfSize;         //�ļ���С
    U16 bfReserved1;
    U16 bfReserved2;
    U32 bfOffBits;      //�ļ�ͷƫ����

    //DIB header
    U32 biSize;         //DIBͷ��С
    U32 biWidth;        //�ļ����
    U32 biHeight;       //�ļ��߶�
    U16 biPlanes;
    U16 biBitCount;     //ÿ�����ص��λ��
    U32 biCompression;
    U32 biSizeImage;    //ͼ�ļ���С
    U32 biXPelsPerMeter;
    U32 biYPelsPerMeter;
    U32 biClrUsed;
    U32 biClrImportant;
} BMP_HEADER;

#pragma  pack()

int main(int argc, char **argv)
{
    int fd;
    int i,j,k;
    int width,height,compression,bpp,sizeImage;
    int stride;
    char fName0[256],fName[256];
    BMP_HEADER  header;
    U8 data[512*512*3];
    U8 clrPal[256*4];
    
   
    int width0,height0,n;
    U8 data0[512*512*3];

    if(argc==1)// ����ͼƬ������� 
    {
        printf("File name of the image to be processed: ");
        scanf("%s", fName0);
        strcpy(fName,"");
        strncat(fName,fName0,strlen(fName0)-4);
        strcat(fName,"_eq.bmp");
    }


    fd = open(fName0, O_RDONLY | O_BINARY);
    if(-1 == fd)
        perror("open bmp file fail");

    read(fd, &header, sizeof(BMP_HEADER));

	printf("ԭͼƬ����Ϣ������ʾ��\n") ;
    printf(" bfType          : %c%c\n", header.bfType%256,header.bfType/256 );
    printf(" bfSize          : %d  \n", header.bfSize          );
    printf(" bfReserved1     : %d  \n", header.bfReserved1     );
    printf(" bfReserved2     : %d  \n", header.bfReserved2     );
    printf(" bfOffBits       : %d\n\n", header.bfOffBits       );
    printf(" biSize          : %d  \n", header.biSize          );
    printf(" biWidth         : %d  \n", header.biWidth         );
    printf(" biHeight        : %d  \n", header.biHeight        );
    printf(" biPlanes        : %d  \n", header.biPlanes        );
    printf(" biBitCount      : %d  \n", header.biBitCount      );
    printf(" biCompression   : %d  \n", header.biCompression   );
    printf(" biSizeImage     : %d  \n", header.biSizeImage     );
    printf(" biXPelsPerMeter : %d  \n", header.biXPelsPerMeter );
    printf(" biYPelsPerMeter : %d  \n", header.biYPelsPerMeter );
    printf(" biClrUsed       : %d  \n", header.biClrUsed       );
    printf(" biClrImportant  : %d  \n", header.biClrImportant  );

    lseek(fd, header.bfOffBits, SEEK_SET);
    read(fd, data, header.biSizeImage);

    close(fd);

	printf("��ȴ�С��4�ı�������");
	scanf("%d",&width0);
	printf("�߶ȴ�С��4�ı�������");
	scanf("%d",&height0);
	printf("��ֵ����������");
	scanf("%d",&n);

    fd = open(fName, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY);
    if(fd < 0 )
        perror("open x.bmp fail");

    width = header.biWidth;
    height = header.biHeight;
    bpp = header.biBitCount;
    compression = header.biCompression;
    sizeImage = header.biSizeImage;
    stride = sizeImage/height;
    
    memset(&header, 0, sizeof(BMP_HEADER));
    header.bfType = 'M'*256 + 'B';
    header.bfOffBits = sizeof(BMP_HEADER) + ((bpp==8)?1024:0);
    header.bfSize = header.bfOffBits + height0*(width0/4)*4*((bpp==8)?1:3) ;//���ź�ͼƬ�ܴ�С 
    header.biSize = 40;
    header.biWidth = width0;//���ź�Ŀ�� 
    header.biHeight = height0;//���ź�ĸ߶ȶ� 
    header.biPlanes = 1;
    header.biBitCount = bpp;
    header.biCompression = compression;
    header.biSizeImage =header.biHeight*(header.biWidth/4)*4*((bpp==8)?1:3) ;//���ź�ͼƬ���ݴ�С
    header.biClrUsed = (bpp==8)?256:0;
    write(fd, &header, sizeof(BMP_HEADER));


    if (bpp==8||bpp==24)
    {
    	if(bpp==8) //�Ҷ�ͼƬ��ɫ�� 
    	{
      		for(i=0; i<256; i++)
      		{
           	 	clrPal[4*i] = clrPal[4*i+1] = clrPal[4*i+2] = i;
            	clrPal[4*i+3] = 0;
        	}
       		write(fd, clrPal, 256*4);
   		}
        int k,n1,n2,a[512*3][20],b[512*3][20];
        double temp,wid,hei,g[512*4];
        g[0]=0;
        temp=(width-1);
    	wid=temp/(width0-1);
    	//printf("%lf",wid);
    	for(i=0;i<width0;i++)
    	{
    		temp=i*wid;
    		g[i]=g[0]+temp;
    		//printf("%lf\n",g[i]);
		}
		
		if(n%2==0)	// ���ҵĸõ�ĸ��������Ҹ��м��� 
		{
			n1=n*0.5;
			n2=n1;
		}
		else
		{
			n1=n*0.5;
			n2=n1+1;
		}
		for(i=0;i<width0;i++)//��ÿ���㸽���Ĳ�ֵ�㣨��������ŵó��ĵ㣩 
		{
			
			k=(int)(g[i]);
			if((g[i]-k)>=0.5)
			{
				for(j=0;j<n2;j++)
				{
					b[i][j]=k+1+j;
					if((k+1+j)>(width-1))	a[i][j]=k+1+j-(1+j)*2+1;
					else	a[i][j]=k+1+j; 
				}
				for(j=n2;j<n;j++)
				{
					b[i][j]=k-(j-n2);
					if(k-(j-n2)<0)	a[i][j]=0-(k-(j-n2));
					else	a[i][j]=k-(j-n2); 
				}
			}
			else
			{
				for(j=0;j<n1;j++)
				{
					b[i][j]=k+1+j;
					if((k+1+j)>(width-1))	a[i][j]=k+1+j-(1+j)*2;
					else	a[i][j]=k+1+j; 
				}
				for(j=n1;j<n;j++)
				{
					b[i][j]=k-(j-n1);
					if(k-(j-n1)<0)	a[i][j]=0-(k-(j-n1));
					else	a[i][j]=k-(j-n1); 
				}
			}
		}
		
		if(bpp==8)//�Ҷ�ͼ�����ſ�� 
		{
			for(i=0;i<height;i++)
			{
				for(j=0;j<width0;j++)
				{
					int t=-1,m1,m2;
					double temp2=0.0;
					for(m1=0;m1<n;m1++)//��������Lagrange��ֵ 
					{
						t++;
						double temp1=1.0;
						for(m2=0;m2<n;m2++)
						{
						
							if(m2!=t)		temp1=(g[j]-b[j][m2])/(b[j][t]-b[j][m2])*temp1;
					
						}
						temp2=temp2+data[i*stride+a[j][t]]*temp1; 
					}
					//temp2=temp2+0.5; 
					data0[i*width0+j]=(U8)temp2;
				
				}
			}
		}
		
		else//��ɫͼƬ�����ſ��
		{
			
			for(i=0;i<height;i++)
			{
				for(j=0;j<width0;j++)
				{
					for(k=0;k<3;k++)
					{
						int t=-1,m1,m2;
						double temp2=0.0;
						for(m1=0;m1<n;m1++)
						{
							t++;
							double temp1=1.0;
							for(m2=0;m2<n;m2++)
							{
						
								if(m2!=t)		temp1=(g[j]-b[j][m2])/(b[j][t]-b[j][m2])*temp1;
					
							}
							temp2=temp2+data[i*stride+a[j][t]*3+k]*temp1;
						}
						//temp2=temp2+0.5; 
						data0[i*width0*3+3*j+k]=(U8)temp2;
				
					}
				
				}
			}
		}
		/*ͬ�����߶�����*/ 
		temp=height-1;
    	hei=temp/(height0-1);
		for(i=0;i<height0;i++)
    	{
    		temp=i*hei;
    		g[i]=g[0]+temp;
    		//printf("%lf\n",g[i]);
		}
		
		if(n%2==0)	
		{
			n1=n*0.5;
			n2=n1;
		}
		else
		{
			n1=n*0.5;
			n2=n1+1;
		}
		for(i=0;i<height0;i++)
		{
			
			k=(int)(g[i]);
			if((g[i]-k)>=0.5)
			{
				for(j=0;j<n2;j++)
				{
					b[i][j]=k+1+j;
					if((k+1+j)>(height-1))	a[i][j]=k+1+j-(1+j)*2+1;
					else	a[i][j]=k+1+j; 
				}
				for(j=n2;j<n;j++)
				{
					b[i][j]=k-(j-n2);
					if(k-(j-n2)<0)	a[i][j]=0-(k-(j-n2));
					else	a[i][j]=k-(j-n2); 
				}
			}
			else
			{
				for(j=0;j<n1;j++)
				{
					b[i][j]=k+1+j;
					if((k+1+j)>(height-1))	a[i][j]=k+1+j-(1+j)*2;
					else	a[i][j]=k+1+j; 
				}
				for(j=n1;j<n;j++)
				{
					b[i][j]=k-(j-n1);
					if(k-(j-n1)<0)	a[i][j]=0-(k-(j-n1));
					else	a[i][j]=k-(j-n1); 
				}
			}
		}
		
		if(bpp==8)//��ɫͼƬ�����Ÿ߶�
		{
			for(i=0;i<width0;i++)
			{
				for(j=0;j<height0;j++)
				{
					int t=-1,m1,m2;
					double temp2=0.0;
					for(m1=0;m1<n;m1++)
					{
						t++;
						double temp1=1.0;
						for(m2=0;m2<n;m2++)
						{
							
							if(m2!=t)		temp1=(g[j]-b[j][m2])/(b[j][t]-b[j][m2])*temp1;
					
						}
						temp2=temp2+data0[i+a[j][t]*width0]*temp1;
					}
					//temp2=temp2+0.5; 
					data[i+j*width0]=(U8)temp2;
				
				}
			}
		}
	
		else//��ɫͼƬ�����Ÿ߶�
		{
			for(i=0;i<width0;i++)
			{
				for(j=0;j<height0;j++)
				{
					for(k=0;k<3;k++) 
					{
						int t=-1,m1,m2;
						double temp2=0.0;
						for(m1=0;m1<n;m1++)
						{
							t++;
							double temp1=1.0;
							for(m2=0;m2<n;m2++)
							{
								
								if(m2!=t)		temp1=(g[j]-b[j][m2])/(b[j][t]-b[j][m2])*temp1;
					
							}
							temp2=temp2+data0[3*i+a[j][t]*width0*3+k]*temp1;
						}
						//temp2=temp2+0.5; 
						data[3*i+j*width0*3+k]=(U8)temp2;
					}
				}
			}
			
		}
		
   }

    else
        perror("Wrong type of image.");

    
	write(fd,data, header.biSizeImage);
    close(fd);
}

