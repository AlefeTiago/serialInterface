#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

unsigned char *p_ubrr0h; // Ponteiro que será utilizado para definir o Baud Rate.
unsigned char *p_ubrr01; // Ponteiro que será utilizado também para definir o Baud Rate.
unsigned char *p_ucsr0a; // Ponteiro que será utilizado para verificar se os dados foram transferidos, configurar a velocidade de transmissão dos dados e também a função de multiprocessamento.
unsigned char *p_ucsr0b; // Ponteiro que será utilziado para definir as interrupções; configurações de transmissão e recepção de dados; numero de bits.
unsigned char *p_ucsr0c; // Ponteiro para definir o modo de operaçaõ da USART; paridade;bits de parada.
unsigned char *p_udr0; // Ponteiro para o registrador de dados

//Ponteiros para a porta b do Atmega328P 
unsigned char *p_ddrb;
unsigned char *p_mcucr; 
unsigned char *p_portb;
unsigned char *p_pinb; 


char msg_np[]= "Transmissao serial utilizando a USART: verificacao de termino da transmissao por varredura. O botao nao esta pressionado. \n\n";
char msg_p[]= "Transmissao serial utilizando a USART: verificacao de termino da transmissao por varredura. O botao esta pressionado. \n\n";


void setup()
{
 
  p_ubrr0h = (unsigned char *) 0xC5;
  p_ubrr01 = (unsigned char *) 0xC4;
  p_ucsr0a = (unsigned char *) 0xC0;
  p_ucsr0b = (unsigned char *) 0xC1;
  p_ucsr0c = (unsigned char *) 0xC2;
  p_udr0 = (unsigned char *) 0xC6;
  p_ddrb = (unsigned char *) 0x24;
  p_portb = (unsigned char *) 0x25;
  p_mcucr = (unsigned char *) 0x55;
  p_pinb = (unsigned char *) 0x23;

  // Enderecei cada ponteiro em seu respectivo registrador. 
  
  *p_ubrr0h = 0; // Para Baud Rate de 115200 o valor em ubrr0('l' e 'h') deve ser 8. Então no High o valor deve ser 0x00 (High é o registrador dos bits mais significativos)
  *p_ubrr01 = 8; // Em LOW (Bits menos significativos), o registrador deve ser setado no valor 8 (0000 0100 ou 0x08)
 *p_ucsr0a &= ~3; // Aqui eu desabilito a taxa dobrada de transmissão de dados e também a função de multiprocessamento.
 *p_ucsr0b &= ~0xF4;
 *p_ucsr0b |= 0x08; // As duas operações em ucsr0b foram para: Desabilitar as interrupções, desabilitar a recepção de dados, habilitar a transmissão de dados, definir a quantidade de bits (8).
  *p_ucsr0c &= 0x2E; // Configuração para utilizar a USART em modo assincrono.
  *p_ddrb &= 0xFD;  // Define a porta PB1 como entrada.
  *p_mcucr &= 0xEF; // Habilita o resistor de pull-up nas entradas.
  *p_portb |= 0x02;  //Habilita o resistor de pull-up na porta PB1.
}


int main(void){
setup(); // // Inicializo os periféricos 
  

while (1){
     if (((*p_pinb & 0x02) == 0x00)){ // Avaliação para caso o botão seja pressionado. 
      int i = 0; // Defino uma váriavel de contagem para varrer o vetor de caracteres.
  while(msg_p[i] != '\0') // Enquanto a mensagem não chega no seu caracter final, 
  { 
    if((*p_ucsr0a & 0x20) == 0x20)// verificamos se o registrador UDR0 está vázio (UDRE0 = 1)
    {
      *p_udr0 = msg_p[i]; // Se o registrador estiver vazio, então é passado um caractere para o registrador de dados
      i++; // o incremento aqui serve para o registrador de dados receber o proximo caractere do vetor.
    }
  }
    _delay_ms(500);//Após toda a mensagem ser enviada, espera-se 500ms, para evitar conflitos de sobrecarga.
     }

     else if((*p_pinb & 0x02) == 0x02){
      int i = 0; // Váriavel de contagem para varrer o vetor de caracteres.
  while(msg_np[i] != '\0') // Enquanto a mensagem não chega no seu caracter final, 
  { 
    if((*p_ucsr0a & 0x20) == 0x20)// verifica-se se o registrador UDR0 está vázio (UDRE0 = 1) e pronto para receber outra letra (conjunto de bits).
    {
      *p_udr0 = msg_np[i]; // Se o registrador estiver vazio, então é passado um caractere para o registrador de dados
      i++; // o incremento aqui serve para o registrador de dados receber o proximo caractere do vetor.
    }
  }
    _delay_ms(500); //Após toda a mensagem ser enviada, espera-se 500ms, para evitar conflitos de sobrecarga.
  }
}
  return 0;
  }