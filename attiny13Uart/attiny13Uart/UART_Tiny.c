#include "UART_Tiny.h"

int8_t BAUD_DIV,//отступ отсчета для таймера
		send_flag;//флаг начала передачи

void uart_init(){
	BAUD_DIV = 256 - F_CPU/DIV/BAUD_RATE;
	TIMSK0 |= 1 << TOIE0;//разрешение прерываний по переполнению
	TCCR0B |= 1 << CS01;// предделитель 8
	TCNT0 = BAUD_DIV;//двигаем счетчик
	txByte = 0xFF;//заполнение буфера единичками
	DDRB |= 1 << TXPORT;//выставляем линию ТХ на выход
	sei();//разрешаем глобальные прерывания
}

void uart_send(uint8_t byte){
	PORTB &= ~(1 << TXPORT);//опускаем в ноль уровень сигнала на линии передачи. Начинаем отправку.
	TCNT0 = BAUD_DIV;//двигаем счетчик
	txByte = byte;//отправляем в буфер байт
}

ISR(TIM0_OVF_vect)
{
	PORTB = ((txByte & 0x01) << TXPORT);//присваиваем уровень сигнала равным младшему биту в буфере
	txByte = txByte >> 1;//затираем отправленный бит
	txByte |= 1<< 7;//записываем в старший бит 1
	TCNT0 = BAUD_DIV;//двигаем счетчик
		
}