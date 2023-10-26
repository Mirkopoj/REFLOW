#ifndef _PAD
#define _PAD
#INT_EXT2 
void isr_ext2(){
   delay_ms(1);
   if(input_state(C0)==0){
      if(input_state(F0)==0){      
         ultimaTecla=_X;
         return;
      }
      if(input_state(F1)==0){
         ultimaTecla=7;
         return;
      }
      if(input_state(F2)==0){
         ultimaTecla=4;
         return;
      }
      if(input_state(F3)==0){
         ultimaTecla=1;
         return;
      }
   }
   return;
}
int1 flancoDes(int1 pata, int bmap){
   if(pata)
      //rb5 (PATA 1)
      return ((_ultimo&0x20) == 0x20) && ((bmap&0x20) == 0x00)? 1:0;
      //rb4 (PATA 0)
      return ((_ultimo&0x10) == 0x10) && ((bmap&0x10) == 0x00)? 1:0;
}


#INT_RB
void isr_rb(){
   delay_ms(3);
   #USE FAST_IO (b)
   int bmap = input_b() & 0x30;
   #USE STANDARD_IO (b)
   int1 pata0 = flancoDes(0,bmap);
   int1 pata1 = flancoDes(1,bmap);
   _ultimo = bmap;
   //rb4 (PATA 0)
   if(pata0){
      if(input_state(F0)==0){
         ultimaTecla=0;
         return;
      }
      if(input_state(F1)==0){
         ultimaTecla=8;
         return;
      }
      if(input_state(F2)==0){ 
         ultimaTecla=5;
         return;
      }
      if(input_state(F3)==0){
         ultimaTecla=2;
         return;
      }
   }
   //rb5 (PATA 1)
   if(pata1){
      if(input_state(F0)==0){
         ultimaTecla=_TICK;
         return;
      }
      if(input_state(F1)==0){
         ultimaTecla=9;
         return;
      }
      if(input_state(F2)==0){
         ultimaTecla=6;
         return;
      }
      if(input_state(F3)==0){  
         ultimaTecla=3;
         return;
      }
   }
   return;   
}
#INT_TIMER0
void isr_timer0(){
   #USE FAST_IO (b)
   #USE FAST_IO (c)
   int cmap = input_c();
   int bmap = input_b();
   #USE STANDARD_IO (b)
   #USE STANDARD_IO (c)
   
   int rowL = (cmap&0x03);
   int rowH = (bmap&0xC0)>>4;
   int colH = (bmap&0x30)>>3;
   int colL = (bmap&0x04)>>2;
   
   int col = colL | colH;
   int row = rowL | rowH;
   if(col==7){
      switch(row){
         case 0x0E:
            OFF(F1);
            FLT(F0);
            break;
         case 0x0D:
            OFF(F2);
            FLT(F1);
            break;
         case 0x0B:
            OFF(F3);
            FLT(F2);
            break;
         case 0x07:
            OFF(F0);
            FLT(F3);
            break;
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
      enable_interrupts(INT_RB4);
      enable_interrupts(INT_RB5);
      enable_interrupts(INT_RB);
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
   port_b_pullups(0xF5);
   SET_TRIS_C(0x03);
   tecl_mode(Estatico);
   delay_us(1);
   enable_interrupts(INT_RB4);
   enable_interrupts(INT_RB5);
   enable_interrupts(INT_RB);
   enable_interrupts(INT_EXT2_H2L);
   ultimaTecla = _VISTO;
}
#endif
