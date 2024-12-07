/** dung led 7 segment kathode chung nen la noi dat dieu khien 
   tuc la 1 2 = 0 thi 2 led moi hoat dong = 1  thi khong hoat dong 
	 kathode chung 1 sang 0 tat 
**/
#include <REGX51.H>
#define hienthi P2
#define ON  0;
#define OFF  1;
sbit do1 = P1^0;
sbit do2 = P1^1;
sbit vang1 = P1^2;
sbit vang2 = P1^3;
sbit xanh1 = P1^4;
sbit xanh2 = P1^5;
sbit chedo = P1^6;
sbit huong = P1^7;
sbit dvi_doc = P0^0;
sbit chuc_doc = P0^1;
sbit dvi_ngang = P0^2;
sbit chuc_ngang = P0^3;
char so[] ={0x3F,0x6,0x5B,0x4F,0x66,0x6D,0x7D,0x7,0x7F,0x6F}; //{0,1,2,3,4,5,6,7,8,9}
int dem,laplai;


void delay_ms(unsigned int time)
{
	unsigned int x;
	for(x=1;x<=time;x++){
		TMOD=0x11;
		TH0=55536/256;
		TL0=55536%256;
		TR0=1;
		while(!TF0);
		TF0=TR0=0;
	}
}	

void dem_tu_dong()
{
	// ngang duoc di 
		do1 = 1;   	do2 = 0;
		vang1 = 0; 	vang2 = 0;
		xanh1 = 0;	xanh2 = 1; 
	//cho delay 10s tuc la xanh 7s do 10s
		for(dem=10; dem >=3 ;dem--){
			for(laplai=0;laplai<30;laplai++){ // so khung hinh tren giay
					int dem2 = dem-3 ;
					int chuc = dem2/10;
					int dvi = dem2%10;
					dvi_doc = ON; // doc dem tu 10 
					hienthi = so [dem%10]; // luc nay doc la RED dem tu 10
							delay_ms(1);
					dvi_doc = OFF;
					dvi_ngang = ON; // ngang dem tu 7 
				  hienthi = so[dvi];// luc nay ngang la green dem tu 7
							delay_ms(1); 
				  dvi_ngang =OFF;
				  chuc_doc=ON; 
					hienthi=so[dem/10];
							delay_ms(1);
				  chuc_doc=OFF;
				  chuc_ngang=ON;
				  hienthi=so[chuc];
							delay_ms(1);
				  chuc_ngang= OFF;
			
			}	
		}
	
	// ngang chuyen sang vang 
		do1 = 1;		do2 = 0;
		vang1 = 0;	vang2 = 1;
		xanh1 = 0;	xanh2 = 0; 
	// cho delay 3s 
		for(dem=2; dem >=0 ;dem--){
			for(laplai=0;laplai<30;laplai++){ //so khung hinh tren giay
					dvi_doc = ON; 
					hienthi = so [dem%10];
							delay_ms(1);
					dvi_doc = OFF;
					dvi_ngang = ON; 
				  hienthi = so[dem%10];
							delay_ms(1);
				  dvi_ngang =OFF;
				  chuc_doc=ON; 
					hienthi=so[dem/10];
							delay_ms(1);
				  chuc_doc=OFF;
				  chuc_ngang=ON;
				  hienthi=so[dem/10];
							delay_ms(1);
				  chuc_ngang= OFF;
			
			}	
		}
	  
	
	// doc duoc di
		do1 = 0;		do2 = 1;
		vang1 = 0;	vang2 = 0;
		xanh1 = 1;	xanh2 = 0; 
	// delay 10s
	for(dem=10; dem >=3 ;dem--){
			for(laplai=0;laplai<30;laplai++){
					int dem2 = dem-3 ;
					int chuc = dem2/10;
					int dvi = dem2%10;
					dvi_ngang = ON; // doc dem tu 10 
					hienthi = so [dem%10]; // luc nay doc la RED dem tu 10
							delay_ms(1);
					dvi_ngang = OFF;
					dvi_doc = ON; // ngang dem tu 7 
				  hienthi = so[dvi];// luc nay ngang la green dem tu 7
							delay_ms(1);
				  dvi_doc =OFF;
				  chuc_ngang=ON; 
					hienthi=so[dem/10];
							delay_ms(1);
				  chuc_ngang=OFF;
				  chuc_doc=ON;
				  hienthi=so[chuc];
							delay_ms(1);
				  chuc_doc= OFF;
			
			}	
		}
		
	// doc chuyen sang vang
		do1 = 0;		do2 = 1;
		vang1 = 1;	vang2 = 0;
		xanh1 = 0;	xanh2 = 0; 
	 // delay 3s
		for(dem=2; dem >=0 ;dem--){
			for(laplai=0;laplai<30;laplai++){
					dvi_doc = ON; 
					hienthi = so [dem%10];
							delay_ms(1);
					dvi_doc = OFF;
					dvi_ngang = ON; 
				  hienthi = so[dem%10];
							delay_ms(1);
				  dvi_ngang =OFF;
				  chuc_doc=ON; 
					hienthi=so[dem/10];
							delay_ms(1);
				  chuc_doc=OFF;
				  chuc_ngang=ON;
				  hienthi=so[dem/10];
							delay_ms(1);
				  chuc_ngang= OFF;
			
			}	
		}
	  
}


void main()
{
  while(chedo==1){
	 if(huong==1){ // ngang duoc chay 
			if(do2==1){
			  do1=0;		do2=1;
				vang1=1;	vang2=0;
				xanh1=0;	xanh2=0;
				delay_ms(400);
			}
			do1=1;		do2=0;
			vang1=0;	vang2=0;
			xanh1=0;	xanh2=1;
			for(laplai=0;laplai<30;laplai++){
					dvi_doc = ON;
						hienthi = 0x7F;
				  dvi_doc= OFF;
					dvi_ngang= ON;
						hienthi = 0x7F;
					dvi_ngang= OFF;
					chuc_doc= ON;
						hienthi = 0x7F;
					chuc_doc=OFF;
				  chuc_ngang= ON;
						hienthi = 0x7F;
				  chuc_ngang = OFF;
			}
	 }
	 if(huong==0){ // doc duoc chay
	   if(do1==1){
		   do1=0;		do2=1;
			 xanh1=0;	xanh2=0;
			 vang1=1;	vang2=0;
		   delay_ms(400);
		 }
	     do1=0;			do2=1;
			 vang1=0;		vang2=0;
			 xanh1=1;		xanh2=0;
		 		for(laplai=0;laplai<30;laplai++){
					dvi_doc = ON;
						hienthi = 0x7F;
				  dvi_doc= OFF;
					dvi_ngang= ON;
						hienthi = 0x7F;
					dvi_ngang= OFF;
					chuc_doc= ON;
						hienthi = 0x7F;
					chuc_doc=OFF;
				  chuc_ngang= ON;
						hienthi = 0x7F;
				  chuc_ngang = OFF;
			}
	 }
	}
	while(chedo==0){
	  dem_tu_dong();
	}
}
	