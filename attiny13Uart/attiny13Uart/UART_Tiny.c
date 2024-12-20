#include "UART_Tiny.h"

int8_t BAUD_DIV,//������ ������� ��� �������
		send_flag;//���� ������ ��������

void uart_init(){
	BAUD_DIV = 256 - F_CPU/DIV/BAUD_RATE;
	TIMSK0 |= 1 << TOIE0;//���������� ���������� �� ������������
	TCCR0B |= 1 << CS01;// ������������ 8
	TCNT0 = BAUD_DIV;//������� �������
	txByte = 0xFF;//���������� ������ ����������
	DDRB |= 1 << TXPORT;//���������� ����� �� �� �����
	sei();//��������� ���������� ����������
}

void uart_send(uint8_t byte){
	PORTB &= ~(1 << TXPORT);//�������� � ���� ������� ������� �� ����� ��������. �������� ��������.
	TCNT0 = BAUD_DIV;//������� �������
	txByte = byte;//���������� � ����� ����
}

ISR(TIM0_OVF_vect)
{
	PORTB = ((txByte & 0x01) << TXPORT);//����������� ������� ������� ������ �������� ���� � ������
	txByte = txByte >> 1;//�������� ������������ ���
	txByte |= 1<< 7;//���������� � ������� ��� 1
	TCNT0 = BAUD_DIV;//������� �������
		
}