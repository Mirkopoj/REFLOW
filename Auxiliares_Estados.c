#ifndef _AUX_ESTADOS
#define _AUX_ESTADOS
int1 perfilVacio(int nPerfil){
   return (~read_eeprom(0x33*nPerfil-0x32)==0&&~read_eeprom(0x33*nPerfil-0x31)==0&&~read_eeprom(0x33*nPerfil-0x30)==0)? 1:0;
}
void GrafPerfil(int *nPuntos,struct Punto *Curva, int nPerfil){
   int _x=X,_y=Y;
   int1 prvPerfilVacio = TRUE;
   if(nPerfil!=1) prvPerfilVacio=perfilVacio(nPerfil-1);
   if(perfilVacio(nPerfil)||nPerfil==6){
      if(prvPerfilVacio==0)   printVacio();
   }else{
      GrafCurvaPerfil(nPuntos, Curva, nPerfil);
   }
   X=_x;
   Y=_y;
}
void printPerfilesLit(){
   switch(config&0x03){
      case 0:
         printf(tft_putc,"PERFILES");
         break;
      case 1:
         printf(tft_putc,"PROFILES");
         break;
      case 2:
         printf(tft_putc,"PROFILS");
         break;
      case 3:
         printf(tft_putc,"�PO�\\�\\");
         break;
   }
   return;
}
void printConfiguracionLit(){
   switch(config&0x03){
      case 0:
         printf(tft_putc,"CONFIGURACI�N");
         break;
      case 1:
         printf(tft_putc,"SETTINGS");
         break;
      case 2:
         printf(tft_putc,"PARAM�TRES");
         break;
      case 3:
         printf(tft_putc,"�APAMETP");
         break;
   }
   return;
}
void printTiempoLit(){
   printEspacios(3);
   switch(config&0x03){
      case 0:
         printf(tft_putc,"TIEMPO");
         break;
      case 1:
         printf(tft_putc,"TIME");
         break;
      case 2:
         printf(tft_putc,"TEMPS");
         break;
      case 3:
         printf(tft_putc,"�O�O�A");
         break;
   }
   return;
}
void printTemperaturaLit(){
   printEspacios(3);
   switch(config&0x03){
      case 0:
         printf(tft_putc,"TEMPERATURA");
         break;
      case 1:
         printf(tft_putc,"TIME");
         break;
      case 2:
         printf(tft_putc,"TEMPS");
         break;
      case 3:
         printf(tft_putc,"�O�O�A");
         break;
   }
   return;
}
void printIdiomaLit(){
   printEspacios(3);
   switch(config&0x03){
      case 0:
         printf(tft_putc,"IDIOMA");
         break;
      case 1:
         printf(tft_putc,"LANGUAGE");
         break;
      case 2:
         printf(tft_putc,"LANGAGE");
         break;
      case 3:
         printf(tft_putc,"\\�\\OMA");
         break;
   }
   return;
}
void printFormatoLit(){
   printEspacios(6);
   switch(config&0x03){
      case 0:
         printf(tft_putc,"FORMATO");
         break;
      case 1:
         printf(tft_putc,"FORMAT");
         break;
      case 2:
         printf(tft_putc,"FORMAT");
         break;
      case 3:
         printf(tft_putc,"�ORMAT");
         break;
   }
   return;
}
void printOrigenLit(){
   printEspacios(6);
   switch(config&0x03){
      case 0:
         printf(tft_putc,"ORIGEN");
         break;
      case 1:
         printf(tft_putc,"REFERENCE");
         break;
      case 2:
         printf(tft_putc,"ORIGINE");
         break;
      case 3:
         printf(tft_putc,"\\CTO�H\\K");
         break;
   }
   return;
}
void printVolver(){
   disable_interrupts(global);
   switch(config&0x03){
      case 0:
         printf(tft_putc,"VOLVER ");
         break;
      case 1:
         printf(tft_putc,"RETURN ");
         break;
      case 2:
         printf(tft_putc,"REVENIR ");
         break;
      case 3:
         printf(tft_putc,"BO[BPA�AT�C� ");
         break;
   }
   enable_interrupts(global);
   return;
}
void printInit(){
   printf(tft_putc,"\f\n ");
   return;
}
void printDobleEndl(int1 DosPuntos){
   if(DosPuntos) tft_putc(":");
   printf(tft_putc,"\n\n");
   return;
}
void printMenuPrincipal(){
   printInit();
   switch(config&0x03){
      case 0:
         printf(tft_putc,"MENU PRINCIPAL");
         break;
      case 1:
         printf(tft_putc,"MAIN MENU");
         break;
      case 2:
         printf(tft_putc,"MENU PRINCIPAL");
         break;
      case 3:
         printf(tft_putc,"��ABHOE MEH�");
         break;
   }
   
   printDobleEndl(1);
   printEspacios(3);
   printPerfilesLit();
   printDobleEndl(0);
   printEspacios(3);
   switch(config&0x03){
      case 0:
         printf(tft_putc,"CARGAR VALORES");
         break;
      case 1:
         printf(tft_putc,"LOAD VALUES");
         break;
      case 2:
         printf(tft_putc,"CHARGER VALEURS");
         break;
      case 3:
         printf(tft_putc,"[HA�EH\\� HA�PY[K\\");
         break;
   }
   printDobleEndl(0);
   printEspacios(3);
   printConfiguracionLit();
   printf(tft_putc," ");
   return;
}
void printPerfiles(){
   disable_interrupts(global);
   printf(tft_putc,"\f");
   printPerfilesLit();
   printf(tft_putc,":\n");
   printf(tft_putc,"  1  2  3  4  5  ");
   printVolver();
   enable_interrupts(global);
   return;
}
void printConfiguracion(){
   printInit();
   printConfiguracionLit();
   printDobleEndl(1);
   printTiempoLit();
   printDobleEndl(0);
   printTemperaturaLit();
   printDobleEndl(0);
   printIdiomaLit();
   printDobleEndl(0);
   printEspacios(3);
   printVolver();
   return;
}
void printTiempo(){
   printInit();
   printConfiguracionLit();
   printDobleEndl(1);
   printTiempoLit();
   printDobleEndl(1);
   printFormatoLit();
   printDobleEndl(0);
   printOrigenLit();
   printDobleEndl(0);
   printEspacios(6);
   printVolver();
   return;
}
void printFormato(){
   printInit();
   printConfiguracionLit();
   printDobleEndl(1);
   printTiempoLit();
   printDobleEndl(1);
   printFormatoLit();
   printDobleEndl(1);
   printEspacios(9);
   if(config&0x03==3) printf(tft_putc,"CCCC");
   else printf(tft_putc,"SSSS");
   printDobleEndl(0);
   printEspacios(9);
   printf(tft_putc,"MM:");
   if(config&0x03==3) printf(tft_putc,"CC");
   else printf(tft_putc,"SS");
   printDobleEndl(0);
   printEspacios(9);
   printVolver();
   return;
}
void printOrigen(){
   printInit();
   printConfiguracionLit();
   printDobleEndl(1);
   printTiempoLit();
   printDobleEndl(1);
   printOrigenLit();
   printDobleEndl(1);
   printEspacios(9);
   switch(config&0x03){
      case 0:
         printf(tft_putc,"RELATIVO");
         break;
      case 1:
         printf(tft_putc,"RELATIVE");
         break;
      case 2:
         printf(tft_putc,"RELATIF");
         break;
      case 3:
         printf(tft_putc,"PO�CTBEHH\\K");
         break;
   }
   printDobleEndl(0);
   printEspacios(9);
   switch(config&0x03){
      case 0:
         printf(tft_putc,"ABSOLUTO");
         break;
      case 1:
         printf(tft_putc,"ABSOLUT");
         break;
      case 2:
         printf(tft_putc,"ABSOLU");
         break;
      case 3:
         printf(tft_putc,"A�CO��TH�]");
         break;
   }
   printDobleEndl(0);
   printEspacios(9);
   printVolver();
   return;
}
void printTemperatura(){
   printInit();
   printConfiguracionLit();
   printDobleEndl(1);
   printTemperaturaLit();
   printDobleEndl(1);
   printEspacios(6);
   printf(tft_putc,"�C");
   printDobleEndl(0);
   printEspacios(6);
   printf(tft_putc,"�F");
   printDobleEndl(0);
   printEspacios(6);
   printVolver();
   return;
}
void printIdioma(){
   printInit();
   printConfiguracionLit();
   printDobleEndl(1);
   printIdiomaLit();
   printDobleEndl(1);
   printEspacios(6);
   printf(tft_putc,"ESPA�OL");
   printDobleEndl(0);
   printEspacios(6);
   printf(tft_putc,"ENGLISH");
   printDobleEndl(0);
   printEspacios(6);
   printf(tft_putc,"FRAN�AIS");
   printDobleEndl(0);
   printEspacios(6);
   printf(tft_putc,"PYCCK\\]");
   printDobleEndl(0);
   printEspacios(6);
   printVolver();
   return;
}
void printSoldando(){
   disable_interrupts(global);
   fillRect(X_BEGIN,Y_BEGIN-8,_width,16,BG_COLOR);
   tft_gotoxy(0,0);
   switch(config&0x03){
      case 0:
         printf(tft_putc,"SOLDANDO");
         break;
      case 1:
         printf(tft_putc,"REFLOWING");
         break;
      case 2:
         printf(tft_putc,"SOLDURE");
         break;
      case 3:
         printf(tft_putc,"�A]KA");
         break;
   }
   printf(tft_putc,"...");
   enable_interrupts(global);
   return;
}
void printCancelar(){
   disable_interrupts(global);
   fillRect(X_BEGIN,Y_BEGIN-8,_width,16,BG_COLOR);
   tft_gotoxy(0,0);
   switch(config&0x03){  
      case 0:
         printf(tft_putc,"�CANCELAR");
         break;
      case 1:
         printf(tft_putc,"CANCEL");
         break;
      case 2:
         printf(tft_putc,"ANNULER");
         break;
      case 3:
         printf(tft_putc,"OTMEH\\T�");
         break;
   } 
   tft_putc("?");
   enable_interrupts(INT_EXT2_H2L);
   enable_interrupts(global);
   return;
}
void printTermiando(){
   disable_interrupts(global);
   fillRect(X_BEGIN,Y_BEGIN-8,_width,16,BG_COLOR);
   tft_gotoxy(0,0);
   switch(config&0x03){
      case 0:
         printf(tft_putc,"TERMINADO");
         break;
      case 1:
         printf(tft_putc,"ENDED");
         break;
      case 2:
         printf(tft_putc,"FINALISE");
         break;
      case 3:
         printf(tft_putc,"[AKOH�EHH�]");
         break;
      }
   enable_interrupts(global);
   return;
}
void printIniciar(){
   switch(config&0x03){
      case 0:
         printf(tft_putc,"�INICIAR");
         break;
      case 1:
         printf(tft_putc,"START");
         break;
      case 2:
         printf(tft_putc,"COMMENCER");
         break;
      case 3:
         printf(tft_putc,"HA�\\HAT�"); 
         break;
   }
   tft_putc("?");
   return;
}
void printOtroPunto(){
   printf(tft_putc," >");
   switch(config&0x03){
      case 0:
         printf(tft_putc,"�OTRO PUNTO");
         break;
      case 1:
         printf(tft_putc,"ANOTHER POINT");
         break;
      case 2:
         printf(tft_putc,"AUTRE POINT");
         break;
      case 3:
         printf(tft_putc,"�PY�A� TO�KA");
         break;
   }
   tft_putc("?");
   return;
}
void print_temp(long ovr){
   int _x=X,_y=Y;
   tft_gotoxy(X_MAX-4,0);
   if(ovr==1000){
      printf(tft_putc,"MAX");
      X=_x;
      Y=_y;
      return;
   }
   if(ovr==-1){
      printf(tft_putc,"MIN");
      X=_x;
      Y=_y;
      return;
   }
   printf(tft_putc,"%03Lu",ovr);
   X=_x;
   Y=_y;
   return;
}
void print_tiempo(long tiempo){
   int _x=X,_y=Y;
   tft_gotoxy(X_MAX-4,1);
   if((config&0x10)==0x10){
      int min=(tiempo/60)%100,seg=tiempo%60;
      printf(tft_putc,"%02d:%02d",min,seg);
      X=_x;
      Y=_y;
      return;
   }
   if(config&0x03==3){
      printf(tft_putc,"%04LuC",tiempo);
      X=_x;
      Y=_y;
      return;
   }
   printf(tft_putc,"%04LuS",tiempo);
   X=_x;
   Y=_y;
   return;
}
int guardarPuntos(int nPerfil, int nPuntos, struct Punto *Curva){
   if((config&8)==8){
      for(int i=nPuntos;i>1;i--)Curva[i-1].Tiempo-=Curva[i-2].Tiempo;
   }
   if(nPerfil!=0){
      for(int i=0;i<nPuntos;i++){
         write_eeprom(0x33*nPerfil-0x32+3*i,~Curva[i].Temperatura);
         write_eeprom(0x33*nPerfil-0x31+3*i,~Curva[i].Tiempo>>8);
         write_eeprom(0x33*nPerfil-0x30+3*i,~Curva[i].Tiempo);
      }
      if(nPuntos<17){//Terminacion de 00 00 00 para que cargar valores sepa donde acaba la curva
         write_eeprom(0x33*nPerfil-0x32+3*(nPuntos+1),~Curva[(nPuntos+1)].Temperatura);
         write_eeprom(0x33*nPerfil-0x31+3*(nPuntos+1),~Curva[(nPuntos+1)].Tiempo>>8);
         write_eeprom(0x33*nPerfil-0x30+3*(nPuntos+1),~Curva[(nPuntos+1)].Tiempo);
      }
   }
   return _Iniciar;
}
enum{
   _CargarTemperatura =100,
   _CargarMinutos,
   _CargarSegundos,
   _CargarTiempo,
   _CheckearSiValido
};
int CargarTemperatura(int *temp, int *nPuntos){
   long ovr=*temp;
   Cursor(X_MAX-5,0);
   while(ultimaTecla!=_TICK){
      if(ultimaTecla==_X){
         ultimaTecla=_VISTO;
         if(ovr==0){
            tecl_mode(estatico);
            if((*nPuntos&0x3F)==1){
               int nPerfil=(config&0xE0)>>5;
               if(nPerfil==0) return _CargarValores;
               if(~read_eeprom(0x33*nPerfil-0x32)==0&&~read_eeprom(0x33*nPerfil-0x31)==0&&~read_eeprom(0x33*nPerfil-0x30)==0) return _Perfil_Crear;
               return _Perfil_Editar;  
            }
            (*nPuntos)--;
            return _OtroPunto;   
         }
         ovr/=10;
         print_temp(ovr);
      }
      if(ultimaTecla<10){
         ovr=(ovr*10+ultimaTecla)%1000;
         ultimaTecla=_VISTO;
         print_temp(ovr);
      }
   }
   ultimaTecla=_VISTO;
   if((config&4)==4){ 
      if(ovr<32) ovr=32;
      ovr=((ovr-32)*5)/9;
   }
   if(ovr<=MINTEMP){
      *temp=0;
      print_temp(-1);
   }else if(ovr>=255+MINTEMP){
      *temp=255;
      print_temp(1000);
   }else *temp=ovr-MINTEMP;
   if((config&16)>>4) return _CargarMinutos;
   return _CargarTiempo;
}
int CargarMinutos(long *tiempo){ 
   Cursor(X_MAX-5,1);
   while(ultimaTecla!=_TICK){
      if(ultimaTecla==_X){
         ultimaTecla=_VISTO;
         if(*tiempo==0) return _CargarTemperatura;
         *tiempo/=10;
         print_tiempo(*tiempo*60);
      }
      if(ultimaTecla<10){
         *tiempo=(*tiempo*10+ultimaTecla)%100;
         ultimaTecla=_VISTO;
         print_tiempo(*tiempo*60);
      }
   }
   ultimaTecla=_VISTO;
   *tiempo*=60;
   return _CargarSegundos;
}
int CargarSegundos(long *tiempo){
   int seg=0;
   while(ultimaTecla!=_TICK){
      if(ultimaTecla==_X){
         ultimaTecla=_VISTO;
         if(seg==0) return _CargarMinutos;
         seg/=10;
         print_tiempo((*tiempo)+seg);
      }
      if(ultimaTecla<10){
         seg=((seg%10)*10+ultimaTecla);
         ultimaTecla=_VISTO;
         print_tiempo((*tiempo+(seg%60)));
      }
   }
   ultimaTecla=_VISTO;
   *tiempo+=seg;
   return _CheckearSiValido;
}
int CargarTiempo(long *tiempo){
   Cursor(X_MAX-5,1);
   while(ultimaTecla!=_TICK){
      if(ultimaTecla==_X){
         ultimaTecla=_VISTO;
         if(*tiempo==0) return _CargarTemperatura;
         *tiempo/=10;
         print_tiempo(*tiempo);
      }
      if(ultimaTecla<10){
         *tiempo=((*tiempo%1000)*10+ultimaTecla);
         ultimaTecla=_VISTO;
         print_tiempo(*tiempo);
      }
   }
   ultimaTecla=_VISTO;
   return _CheckearSiValido;
}
int CheckearSiValido(long tiempo, int temp, int nPuntos, struct Punto *Curva){
   tecl_mode(estatico);
   int nPerfil=(config&0xE0)>>5;
   if((tiempo==0)||(((config&8)==8)&&(nPuntos>1)&&(tiempo<Curva[nPuntos-2].Tiempo))) return _PuntoNoValido;
   Curva[nPuntos-1].Temperatura=temp;
   Curva[nPuntos-1].Tiempo=tiempo;
   if(nPuntos==17) return guardarPuntos(nPerfil,nPuntos,Curva);
   struct Punto buff;
   buff = Graf(nPuntos, Curva, 1);
   GrafData(buff);
   return _OtroPunto;
}
#INT_TIMER1
void isr_timer1(){
   Setpoint+=dt;
   cont++;
   return;
}
void timer1_init(){
   setup_timer_1(T1_DIV_BY_4|T1_INTERNAL);   //-> Diferencial de tiempo, Granularidad con la que se actualiiza el setpoint. 7.62Hz
   enable_interrupts(global);            
   enable_interrupts(INT_TIMER1);
   return;
}
void adc_init(){
   setup_adc(ADC_CLOCK_INTERNAL|ADC_CLOCK_DIV_8); // FOSC/8 -> FOSCmax = 10MHz
   setup_vref(VREF_2v048); //FVR EN 2.048V
   setup_adc_ports(port_adc, VSS_FVR);
   return;
}
float MedirTemp(){
   float x=read_adc();
   float temp = (float)x*ESCALADO+OFFSET;
   delay_ms(1);
   float TempContacto;
   TempContacto = temp_tc77();
   if(TempContacto <= -99){
      print_status(-1);
      TempContacto = 0;
   }
   if(temp>TempContacto)temp-=TempContacto;
   if(temp<MINTEMP)temp=MINTEMP;
   return (temp>(255+MINTEMP))? 255:temp-MINTEMP;
}
void curva_init(struct punto *Curva){
   for(int i=0;i<17;i++){
      Curva[i].Temperatura=0;
      Curva[i].Tiempo=0;
   }
   ultimaTecla=_VISTO;
}
#endif
