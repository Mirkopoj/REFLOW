#ifndef _PAD
#define _PAD
#INT_EXT2 
void isr_ext2(){
   if(input_state(C2)==0){
      if(input_state(F0)==0){      
         if(input_state(F0)==0)ultimaTecla=_TICK;
         return;
      }
      if(input_state(F1)==0){      
         if(input_state(F1)==0)ultimaTecla=9;
         return;
      }
      if(input_state(F2)==0){      
         if(input_state(F2)==0)ultimaTecla=6;
         return;
      }
      if(input_state(F3)==0){      
         if(input_state(F3)==0)ultimaTecla=3;
         return;
      }
   }
   if(input_state(C1)==0){
      if(input_state(F0)==0){
         if(input_state(F0)==0)ultimaTecla=0;
         return;
      }
      if(input_state(F1)==0){
         if(input_state(F1)==0)ultimaTecla=8;
         return;
      }
      if(input_state(F2)==0){
         if(input_state(F2)==0)ultimaTecla=5;
         return;
      }
      if(input_state(F3)==0){
         if(input_state(F3)==0)ultimaTecla=2;
         return;
      }
   }
   if(input_state(C0)==0){
      if(input_state(F0)==0){      
         if(input_state(F0)==0)ultimaTecla=_X;
         return;
      }
      if(input_state(F1)==0){
         if(input_state(F1)==0)ultimaTecla=7;
         return;
      }
      if(input_state(F2)==0){
         if(input_state(F2)==0)ultimaTecla=4;
         return;
      }
      if(input_state(F3)==0){
         if(input_state(F3)==0)ultimaTecla=1;
         return;
      }
   }
   return;
}
#INT_TIMER0
void isr_timer0(){
   #USE FAST_IO (c)
   int tecl=input_c()&7;
   #USE STANDARD_IO (c)
   if(tecl==7){
      if(input_state(F0)==0){
         OFF(F1);
         FLT(F0);
         return;
      }
      if(input_state(F1)==0){
         OFF(F2);
         FLT(F1);
         return;
      }
      if(input_state(F2)==0){
         OFF(F3);
         FLT(F2);
         return;
      }
      if(input_state(F3)==0){
         OFF(F0);
         FLT(F3);
         return;
      }
   }
   return;
}
void tecl_mode(int1 modo){
   //Modo escaneo: lee todo el teclado
   if(modo){
      SETUP_TIMER_0(T0_INTERNAL|T0_8_BIT); //Cambiar de fila cada 128uS
      enable_interrupts(INT_TIMER0);
      enable_interrupts(INT_EXT2_H2L);
      return;
   }
   //Modo estatico: solo lee X y -/
   disable_interrupts(INT_TIMER0);
   FLT(F3);
   FLT(F2);
   FLT(F1);
   OFF(F0);
   return;
}
void tecl_init(){
   SET_TRIS_B(0xF5);
   port_b_pullups(0xFF);
   tecl_mode(estatico);
   delay_us(1);
   enable_interrupts(INT_EXT2_H2L);
}
#endif
