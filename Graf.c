#ifndef _Graf
#define _Graf
//Genera un degradé desde azul hasta rojo pasando por verde
//entre dos alturas limite dadas
//y devuelve el el color correspondiente a esa altura
uint16_t HeatMap(uint8_t s, uint8_t e, uint8_t y){ 
   uint8_t i;
   if(y>(e+s)/2){
      i=e-y;
      return (((uint16_t)0x1F*(((e-s)/2)-i))/((e-s)/2))|((((uint16_t)0x3F*i)/((e-s)/2))<<5);
   }
   i=(e+s)/2-y;
   return ((((uint16_t)0x3F*(((e-s)/2)-i))/((e-s)/2))<<5)|((((uint16_t)0x1F*i)/((e-s)/2))<<11);
}
struct Punto Graf(int nPuntos, struct punto *Curva, int1 modo){
   int1 EtapaEnCurso;
   int contTotal=0, contEtapa;
   int yM=(_height-13), xM=(_width-30);
   struct Punto Max;
   Max.Tiempo = 0;
   Max.Temperatura = 0;
   for(int i=0; i<nPuntos; i++){
      if(Curva[i].Temperatura>Max.Temperatura)Max.Temperatura=Curva[i].Temperatura;
      Max.Tiempo += Curva[i].Tiempo;
   }
   float prev_y=yM;
   float K=(float)(xM-1)/Max.Tiempo; //Pixeles/segundo
   float Pincel=0, dp;
   fillRect(22,21,xM,(_height-33),BG_COLOR);
   //Bucle de dibujo
   for(int i=0; i<nPuntos; i++){
      //Determinar el delta por pixel de la etapa
      dp=((float)Curva[i].Temperatura-Pincel)/(Curva[i].Tiempo*K);
      //Resetear los flags de finalizacion
      EtapaEnCurso = TRUE;
      contEtapa = 0;
      //Bucle de la etapa
      while(EtapaEnCurso){
         //Etapa terminada
         if(contEtapa >= k*Curva[i].Tiempo-1) EtapaEnCurso = FALSE;
         Pincel+=dp;
         y = yM-((float)(_height-36)/Max.Temperatura)*Pincel;
         /* Linea a color, borde gordo
         if(prev_y-y>=0) drawVLine(cont2+22,y,abs(y-prev_y)+1,HeatMap(0,255,~(int)Pincel));
         else drawVLine(cont2+22,prev_y,abs(y-prev_y)+1,HeatMap(0,255,~(int)Pincel));*/
         if(modo){
            // Volumen a color //HeatMap(20,yM-1,y)<<Color en funsion de la altura y no la temperatura
            if(prev_y-y>0) fillRect(contTotal+22,y+1,xM-contTotal,abs(y-prev_y),HeatMap(0,255,~(int)Pincel));
            else fillRect(contTotal+22,prev_y+1,xM-contTotal,abs(y-prev_y),BG_COLOR);
         }else{
            //Linea gris, borde gordo
            if(prev_y-y>=0) drawVLine(contTotal+22,y,abs(y-prev_y)+1,0xA534);
            else drawVLine(contTotal+22,prev_y,abs(y-prev_y)+1,0xA534);
         }
         contEtapa++;
         contTotal++;
         prev_y=y;
      }
   }
   if(modo) fillRect(contTotal+23,prev_y,xM-contTotal,abs(yM-prev_y),BG_COLOR);
   return Max;
}
void GrafFrame(){
   drawHLine(        19,(_height-12), (_width-27),TX_COLOR);
   drawVLine(        21,          20,(_height-29),TX_COLOR);
   drawHLine(        19,          20,           5,TX_COLOR);
   drawVLine((_width-8),(_height-14),           5,TX_COLOR);
   return;
}
void GrafData(struct Punto Vals){
   long Tiempo = Vals.Tiempo;
   long Temp = (long)Vals.Temperatura+MINTEMP;
   int x=X, y=Y;
   char u='C';
   if((config&4)==4){ 
      Temp=((Temp*9)/5)+32;
      u='F';
   }
   tft_gotoxy(0, 2);
   printf(tft_putc,"°%c\n%03Lu", u, Temp);
   tft_gotoxy(X_MAX-5, Y_MAX);
   if((config&16)>>4){
      printf(tft_putc,"%02Lu:%02Lu", Tiempo/60, Tiempo%60);
   }
   else if((config&0x03)==3) printf(tft_putc,"%04LuC",Tiempo);
   else printf(tft_putc,"%04LuS",Tiempo);
   tft_gotoxy(x, y);
   return;
}
void printVacio(){
   int _x=X,_y=Y;
   disable_interrupts(global);
   fillRect(22,21,(_width-30),(_height-33),BG_COLOR);
   tft_gotoxy(0, 2);
   printf(tft_putc,"  \n   ");
   tft_gotoxy(X_MAX-5, Y_MAX);
   printf(tft_putc,"     ");
   tft_gotoxy(11,8);
   switch(config&0x03){
      case 0:
         printf(tft_putc,"VACIO");
         break;
      case 1:
         printf(tft_putc,"EMPTY");
         break;
      case 2:
         printf(tft_putc,"VIDE");
         break;
      case 3:
         printf(tft_putc,"ÏYCTO]"); 
         break;
   }
   enable_interrupts(global);
   X=_x;
   Y=_y;
   return;
}
void GrafCurvaPerfil(int *nPuntos,struct Punto *Curva, int nPerfil){
   *nPuntos=17;
      for(int i=0;i<(*nPuntos);i++){
         Curva[i].Temperatura=~read_eeprom(0x33*nPerfil-0x32+3*i);
         Curva[i].Tiempo=~read_eeprom(0x33*nPerfil-0x31+3*i);
         Curva[i].Tiempo<<=8;
         Curva[i].Tiempo|=~read_eeprom(0x33*nPerfil-0x30+3*i);
         if(Curva[i].Temperatura==0&&Curva[i].Tiempo==0){
            *nPuntos=i;
            break;
         }
      }
      struct Punto buff;
      buff = Graf(*nPuntos, Curva, 1);
      GrafData(buff);
}
void Cursor(int _x,int _y){
   x-=6;
   printf(tft_putc," ");
   tft_gotoxy(_x,_y);
   printf(tft_putc,">");
}
void printEspacios(int n){
   for(int i=0;i<n;i++)
      tft_putc(" ");
   return;
}
#endif
